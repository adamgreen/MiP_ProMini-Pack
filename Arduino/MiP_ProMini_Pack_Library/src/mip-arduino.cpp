/* Copyright (C) 2018  Adam Green (https://github.com/adamgreen)

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/
/* Implementation of MiP BLE transport for Arduino. */
#include <Arduino.h>
#include <string.h>
#include "mip-transport.h"
#include "mip.h"

// Size of out of band response queue.  The queue will overwrite the oldest item once this size is hit.
#define MIP_OOB_RESPONSE_QUEUE_SIZE 10

// Timeout if expected response doesn't arrive back in this amount of time (in milliseconds).
#define MIP_RESPONSE_TIMEOUT 1000

// Delay between requests sent to MiP (in milliseconds).
#define MIP_REQUEST_DELAY 4

// Boolean values returned as integers.
#define FALSE   0
#define TRUE    1

// MiP Protocol Commands.
// These command codes are placed in the first byte of requests sent to the MiP and responses sent back from the MiP.
// See https://github.com/WowWeeLabs/MiP-BLE-Protocol/blob/master/MiP-Protocol.md for more information.
#define MIP_CMD_PLAY_SOUND              0x06
#define MIP_CMD_SET_POSITION            0x08
#define MIP_CMD_GET_GESTURE_RESPONSE    0x0A
#define MIP_CMD_SET_GESTURE_RADAR_MODE  0x0C
#define MIP_CMD_GET_RADAR_RESPONSE      0x0C
#define MIP_CMD_GET_GESTURE_RADAR_MODE  0x0D
#define MIP_CMD_GET_SOFTWARE_VERSION    0x14
#define MIP_CMD_SET_VOLUME              0x15
#define MIP_CMD_GET_VOLUME              0x16
#define MIP_CMD_GET_HARDWARE_INFO       0x19
#define MIP_CMD_SHAKE_RESPONSE          0x1A
#define MIP_CMD_CLAP_RESPONSE           0x1D
#define MIP_CMD_ENABLE_CLAP             0x1E
#define MIP_CMD_GET_CLAP_SETTINGS       0x1F
#define MIP_CMD_SET_CLAP_DELAY          0x20
#define MIP_CMD_GET_UP                  0x23
#define MIP_CMD_DISTANCE_DRIVE          0x70
#define MIP_CMD_DRIVE_FORWARD           0x71
#define MIP_CMD_DRIVE_BACKWARD          0x72
#define MIP_CMD_TURN_LEFT               0x73
#define MIP_CMD_TURN_RIGHT              0x74
#define MIP_CMD_STOP                    0x77
#define MIP_CMD_CONTINUOUS_DRIVE        0x78
#define MIP_CMD_GET_STATUS              0x79
#define MIP_CMD_GET_WEIGHT              0x81
#define MIP_CMD_GET_CHEST_LED           0x83
#define MIP_CMD_SET_CHEST_LED           0x84
#define MIP_CMD_READ_ODOMETER           0x85
#define MIP_CMD_RESET_ODOMETER          0x86
#define MIP_CMD_FLASH_CHEST_LED         0x89
#define MIP_CMD_SET_HEAD_LEDS           0x8A
#define MIP_CMD_GET_HEAD_LEDS           0x8B


struct OobResponse
{
    uint8_t buffer[MIP_RESPONSE_MAX_LEN];
    uint8_t length;
};

struct MiPTransport
{
    uint8_t      responseBuffer[MIP_RESPONSE_MAX_LEN];
    uint8_t      expectedResponseCommand;
    uint8_t      expectedResponseSize;
    uint8_t      oobQueueRead;
    uint8_t      oobQueueWrite;
    uint8_t      oobQueueCount;
    OobResponse  oobQueue[MIP_OOB_RESPONSE_QUEUE_SIZE];
};


static MiPTransport g_transportSingleton;


static int processAllResponseData(MiPTransport* pTransport);
static void copyHexTextToBinary(uint8_t* pDest, uint8_t* pSrc, uint8_t length);
static uint8_t parseHexDigit(uint8_t digit);
static void processOobResponseData(MiPTransport* pTransport, uint8_t commandByte);
static void discardUnexpectedSerialData();
static void advanceOobQueueWriteIndex(MiPTransport* pTransport);
static void advanceOobQueueReadIndex(MiPTransport* pTransport);


MiPTransport* mipTransportInit(const char* pInitOptions)
{
    memset(&g_transportSingleton, 0, sizeof(g_transportSingleton));
    return &g_transportSingleton;
}

void mipTransportUninit(MiPTransport* pTransport)
{
    if (!pTransport)
        return;
}

int mipTransportConnectToRobot(MiPTransport* pTransport, const char* pRobotName)
{
    // Configure the pin used to select the UART destination between the MiP and PC.
    pinMode(MIP_UART_SELECT_PIN, OUTPUT);
    digitalWrite(MIP_UART_SELECT_PIN, LOW);

    // The MiP requires the UART to communicate at 115200-N-8-1.
    Serial.begin(115200);

    for (int retry = 0 ; retry < 2 ; retry++)
    {
        // Send 0xFF to the MiP via UART to enable the UART communication channel in the MiP.
        uint8_t initMipCommand[] = { 0xFF };
        int result = mipTransportSendRequest(pTransport, initMipCommand, sizeof(initMipCommand), MIP_EXPECT_NO_RESPONSE);
        if (result != MIP_ERROR_NONE)
        {
            // Retry on error.
            continue;
        }

        // Issue GetStatus command to see if we have successfully connected or not.
        const uint8_t getStatusCommand[1] = { MIP_CMD_GET_STATUS };
    
        result = mipTransportSendRequest(pTransport, getStatusCommand, sizeof(getStatusCommand), MIP_EXPECT_RESPONSE);
        if (result != MIP_ERROR_NONE)
        {
            // Retry on error.
            continue;
        }

        uint8_t       response[1+2];
        size_t        responseLength;
        result = mipTransportGetResponse(pTransport, response, sizeof(response), &responseLength);
        if (result == MIP_ERROR_NONE)
        {
            // Connection must be successful since this request was successful.
            break;
        }
    }

    // The MiP UART documentation indicates that this delay is required after sending 0xFF.
    delay(30);

    return MIP_ERROR_NONE;
}

int mipTransportDisconnectFromRobot(MiPTransport* pTransport)
{
    // Send 0xFA to the MiP via UART to put the MiP to sleep.
    uint8_t sleepMipCommand[] = { 0xFA };
    mipTransportSendRequest(pTransport, sleepMipCommand, sizeof(sleepMipCommand), MIP_EXPECT_NO_RESPONSE);

    return MIP_ERROR_NONE;
}

int mipTransportSendRequest(MiPTransport* pTransport, const uint8_t* pRequest, size_t requestLength, int expectResponse)
{
    // Wait for any queued data destined to the PC to be transmitted and then switch the TS3USB221A switch 
    // to connect the AVR UART pins to the MiP.
    Serial.flush();
    digitalWrite(MIP_UART_SELECT_PIN, HIGH);

    // Remember the command byte (first byte) if expecting a response to this request since the response should start
    // with the same byte.
    if (expectResponse)
    {
        pTransport->expectedResponseCommand = pRequest[0];
    }
    else
    {
        pTransport->expectedResponseCommand = 0;
    }
    pTransport->expectedResponseSize = 0;
    pTransport->responseBuffer[0] = 0;

    // Send the specified bytes to the MiP via the UART.
    while (requestLength-- > 0)
    {
        Serial.write(*pRequest++);
    }

    // UNDONE:
    if (!expectResponse)
    {
        // Wait for that data to be transmitted and then set the TS3USB221A switch to connect the AVR UART pins back to 
        // the PC connection.
        Serial.flush();
        digitalWrite(MIP_UART_SELECT_PIN, LOW);
    }

    // UNDONE: Might be better to record time here and spin at top if calling again too soon.
    //         That frees up CPU to do other things while waiting.
    // Let the MiP process the request before letting another request be issued.
    delay(MIP_REQUEST_DELAY);
    
    return MIP_ERROR_NONE;
}

int mipTransportGetResponse(MiPTransport* pTransport, uint8_t* pResponseBuffer, size_t responseBufferSize, size_t* pResponseLength)
{
    int result = MIP_ERROR_NONE;
    
    if (pTransport->expectedResponseCommand == 0)
        return MIP_ERROR_NO_REQUEST;
    if (responseBufferSize > MIP_RESPONSE_MAX_LEN)
        return MIP_ERROR_PARAM;
    
    // Wait for any queued data destined to the PC to be transmitted and then switch the TS3USB221A switch 
    // to connect the AVR UART pins to the MiP.
    Serial.flush();
    digitalWrite(MIP_UART_SELECT_PIN, HIGH);

    // Process all received bytes (which might include out of band notifications) until we find the response to the
    // last request made. Will timeout after a second.
    pTransport->expectedResponseSize = (uint8_t)responseBufferSize;
    uint32_t startTime = millis();
    int responseFound = FALSE;
    do
    {
        responseFound = processAllResponseData(pTransport);
    } while (!responseFound && (uint32_t)millis() - startTime < MIP_RESPONSE_TIMEOUT);

    if (!responseFound)
    {
        // Never received the expected response within the timeout window.
        result = MIP_ERROR_TIMEOUT;
        goto Cleanup;
    }

    // Copy reponse data into caller provided buffer and clear state in pTransport about the expected response.
    memcpy(pResponseBuffer, pTransport->responseBuffer, pTransport->expectedResponseSize);
    *pResponseLength = pTransport->expectedResponseSize;
    pTransport->expectedResponseCommand = 0;
    pTransport->expectedResponseSize = 0;
    pTransport->responseBuffer[0] = 0;

#ifdef UNDONE
    // UNDONE:
    for (int i = 0 ; i < responseBufferSize ; i++)
    {
        PRINT(pResponseBuffer[i], 16); PRINT(',');
    }
    PRINTLN("good3");
#endif // UNDONE

Cleanup:
    // Set the TS3USB221A switch to connect the AVR UART pins back to the PC connection.
    Serial.flush();
    digitalWrite(MIP_UART_SELECT_PIN, LOW);

    return result;
}

static int processAllResponseData(MiPTransport* pTransport)
{
    int     result = FALSE;
    uint8_t buffer[(MIP_RESPONSE_MAX_LEN - 1) * 2];
    size_t  bytesToRead;
    size_t  bytesRead;

    // First check to see if a previous scan for out of band notifications was halted when the response for the last
    // request was encountered.
    if (pTransport->expectedResponseCommand != 0 && pTransport->responseBuffer[0] == pTransport->expectedResponseCommand)
    {
        if (pTransport->expectedResponseSize != 0)
        {
            // Already read the command byte into element 0 of the response buffer earlier so just need to read in the 
            // rest of the expected response bytes now.
            bytesToRead = pTransport->expectedResponseSize - 1;
            bytesRead = Serial.readBytes(buffer, bytesToRead * 2);
            if (bytesRead == bytesToRead * 2)
            {
                copyHexTextToBinary(&pTransport->responseBuffer[1], buffer, bytesToRead);
                result = TRUE;
                PRINTLN("good1");
                goto Cleanup;
            }
        }

        // Just return now since we are still scanning for out of band notifications but we are waiting to be
        // called to retrieve the outstanding response for the last request.
        goto Cleanup;
    }
    
    while (Serial.available() >= 2)
    {
//        PRINTLN("Yay!");
        
        uint8_t highNibble = Serial.read();
        uint8_t lowNibble = Serial.read();
        uint8_t commandByte = (parseHexDigit(highNibble) << 4) | parseHexDigit(lowNibble);

        if (pTransport->expectedResponseCommand != 0 && commandByte == pTransport->expectedResponseCommand)
        {
            // Store away the command byte that we just read into response buffer so that it isn't lost.
            pTransport->responseBuffer[0] = commandByte;

            if (pTransport->expectedResponseSize == 0)
            {
                // Stop processing when we hit the response for the last request while the caller is looking for
                // out of band notifications. 
                PRINTLN("expectedResponseSize=0");
                break;
            }

            // Already read the command byte into element 0 of the response buffer earlier so just need to read in the 
            // rest of the expected response bytes now.
            bytesToRead = pTransport->expectedResponseSize - 1;
            bytesRead = Serial.readBytes(buffer, bytesToRead * 2);
            if (bytesRead == bytesToRead * 2)
            {
                copyHexTextToBinary(&pTransport->responseBuffer[1], buffer, bytesToRead);
                result = TRUE;
#ifdef UNDONE
                // UNDONE:
                for (int i = 0 ; i < pTransport->expectedResponseSize ; i++)
                {
                    PRINT(pTransport->responseBuffer[i], 16); PRINT(',');
                }
                PRINTLN("good2");
#endif // UNDONE
                break;
            }
            else
            {
                // Timed out waiting for all of the response data.
                pTransport->expectedResponseCommand = 0;
                pTransport->expectedResponseSize = 0;
                pTransport->responseBuffer[0] = 0;
                PRINT(bytesToRead * 2); PRINT(','); PRINTLN(bytesRead);
                PRINTLN("timedOut");
                break;
            }
        }
        else
        {
            processOobResponseData(pTransport, commandByte);
        }
    }

Cleanup:
    return result;
}

static void copyHexTextToBinary(uint8_t* pDest, uint8_t* pSrc, uint8_t length)
{
    while (length-- > 0)
    {
        *pDest = (parseHexDigit(pSrc[0]) << 4) | parseHexDigit(pSrc[1]);
        pDest++;
        pSrc+=2;
    }
}

static uint8_t parseHexDigit(uint8_t digit)
{
    if (digit >= '0' && digit <= '9')
    {
        return digit - '0';
    }
    else if (digit >= 'a' && digit <= 'f')
    {
        return digit - 'a' + 10;
    }
    else if (digit >= 'A' && digit <= 'F')
    {
        return digit - 'A' + 10;
    }
    else
    {
        return 0;
    }
}

static void processOobResponseData(MiPTransport* pTransport, uint8_t commandByte)
{
    size_t length = 0;

    // The number of additional bytes to read depends on which notification has been found in serial buffer.
    switch (commandByte)
    {
    case MIP_CMD_GET_RADAR_RESPONSE:
    case MIP_CMD_GET_GESTURE_RESPONSE:
    case MIP_CMD_CLAP_RESPONSE:
    case MIP_CMD_GET_WEIGHT:
        length = 1;
        break;
    case MIP_CMD_SHAKE_RESPONSE:
        length = 0;
        break;
    case MIP_CMD_GET_STATUS:
        length = 2;
        break;
    default:
        // UNDONE:
        PRINT(commandByte, 16);
            PRINTLN(" - BadOOB");
        discardUnexpectedSerialData();
        return;
    }

    uint8_t buffer[2 * 2];
    size_t  bytesRead = Serial.readBytes(buffer, length * 2);
    if (bytesRead != length * 2)
    {
        PRINTLN("ShortOOB");
        discardUnexpectedSerialData();
        return;
    }

    // Add this response to the Out Of Band circular queue. It's okay to overwrite oldest item in queue if full.
    OobResponse* pResponse = &pTransport->oobQueue[pTransport->oobQueueWrite];
    pResponse->buffer[0] = commandByte;
    copyHexTextToBinary(&pResponse->buffer[1], buffer, length);
    pResponse->length = (uint8_t)length + 1;

    advanceOobQueueWriteIndex(pTransport);
    if (pTransport->oobQueueCount < MIP_OOB_RESPONSE_QUEUE_SIZE)
    {
        pTransport->oobQueueCount++;
    }
    else
    {
        // Queue was full so oldest response was overwritten. Increment read index to discard oldest.
        advanceOobQueueReadIndex(pTransport);
    }
}

static void discardUnexpectedSerialData()
{
    // Unexpected response data encountered. Throw away all data in serial buffer since it is hard to tell
    // where next response begins.
    while (Serial.available() > 0)
    {
        Serial.read();
        // Delay long enough for next serial byte to be received if MiP is still actively sending at 115200 baud.
        delayMicroseconds(100);
    }
    return;
}

static void advanceOobQueueWriteIndex(MiPTransport* pTransport)
{
    if (pTransport->oobQueueWrite == MIP_OOB_RESPONSE_QUEUE_SIZE - 1)
    {
        // Wrap around to beginning of circular queue.
        pTransport->oobQueueWrite = 0;
    }
    else
    {
        pTransport->oobQueueWrite++;
    }
}

static void advanceOobQueueReadIndex(MiPTransport* pTransport)
{
    if (pTransport->oobQueueRead == MIP_OOB_RESPONSE_QUEUE_SIZE - 1)
    {
        // Wrap around to beginning of circular queue.
        pTransport->oobQueueRead = 0;
    }
    else
    {
        pTransport->oobQueueRead++;
    }
}

int mipTransportIsResponseAvailable(MiPTransport* pTransport)
{
    if (pTransport->expectedResponseCommand != 0 && Serial.available() > 0)
        return TRUE;
    return FALSE;
}

int mipTransportGetOutOfBandResponse(MiPTransport* pTransport, uint8_t* pResponseBuffer, size_t responseBufferSize, size_t* pResponseLength)
{
    // Wait for any queued data destined to the PC to be transmitted and then switch the TS3USB221A switch 
    // to connect the AVR UART pins to the MiP.
    // UNDONE: I am not re-enabling it since it tends to get called over and over again looking for notifications and
    //         don't want to keep switching back and forth during this waiting.
    Serial.flush();
    digitalWrite(MIP_UART_SELECT_PIN, HIGH);

    processAllResponseData(pTransport);

    if (pTransport->oobQueueCount == 0)
        return MIP_ERROR_EMPTY;

    // Pop the oldest out of band response from the circular queue.
    OobResponse* pResponse = &pTransport->oobQueue[pTransport->oobQueueRead];
    uint8_t length = pResponse->length;
    if (length > responseBufferSize)
        length = responseBufferSize;
    memcpy(pResponseBuffer, pResponse->buffer, length);
    *pResponseLength = length;
    pResponse->length = 0;
    advanceOobQueueReadIndex(pTransport);
    pTransport->oobQueueCount--;

    return MIP_ERROR_NONE;
}
