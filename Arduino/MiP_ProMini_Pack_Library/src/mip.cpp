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
/* Implementation of MiP C API. */
#include "mip.h"


// Make sure that the MiP module itself always uses the actual Serial object and not the redirection to MiPStream.
#undef Serial


// Number of times that begin() method should try to initialize the MiP.
#define MIP_MAX_BEGIN_RETRIES 5

// Number of milliseconds to wait between retries in begin().
#define MIP_BEGIN_RETRY_WAIT 500

// Number of times to retry methods other than begin().
#define MIP_MAX_RETRIES 2

// Number of milliseconds to wait between retries in methods other than begin().
#define MIP_RETRY_WAIT 50

// Should timeout if expected response doesn't arrive back in this amount of time (in milliseconds).
#define MIP_RESPONSE_TIMEOUT 100

// Delay between requests sent to MiP (in milliseconds). If the user attempts to send requests to the MiP faster than
// this, the library will busy wait and only continue with the request after this amount of time has passed. The MiP 
// will sometimes ignore requests sent faster than this.
#define MIP_REQUEST_DELAY 8

// Delay between continuousDrive requests sent to MiP (in milliseconds). continuousDrive() will just ignore faster 
// requests.
#define MIP_CONTINUOUS_DRIVE_DELAY 50


// MiP Protocol Commands.
// These command codes are placed in the first byte of requests sent to the MiP and responses sent back from the MiP.
// See https://github.com/WowWeeLabs/MiP-BLE-Protocol/blob/master/MiP-Protocol.md for more information.
#define MIP_CMD_PLAY_SOUND              0x06
#define MIP_CMD_SET_POSITION            0x08
#define MIP_CMD_GET_GESTURE_RESPONSE    0x0A
#define MIP_CMD_SET_GESTURE_RADAR_MODE  0x0C
#define MIP_CMD_GET_RADAR_RESPONSE      0x0C
#define MIP_CMD_GET_GESTURE_RADAR_MODE  0x0D
#define MIP_CMD_SET_USER_DATA           0x12
#define MIP_CMD_GET_USER_DATA           0x13
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
#define MIP_CMD_SET_GAME_MODE           0x76
#define MIP_CMD_STOP                    0x77
#define MIP_CMD_CONTINUOUS_DRIVE        0x78
#define MIP_CMD_GET_STATUS              0x79
#define MIP_CMD_GET_WEIGHT              0x81
#define MIP_CMD_GET_GAME_MODE           0x82
#define MIP_CMD_GET_CHEST_LED           0x83
#define MIP_CMD_SET_CHEST_LED           0x84
#define MIP_CMD_READ_ODOMETER           0x85
#define MIP_CMD_RESET_ODOMETER          0x86
#define MIP_CMD_FLASH_CHEST_LED         0x89
#define MIP_CMD_SET_HEAD_LEDS           0x8A
#define MIP_CMD_GET_HEAD_LEDS           0x8B
#define MIP_CMD_SLEEP                   0xFA
#define MIP_CMD_DISCONNECT_APP          0xFE


// expectResponse parameter values for transportSendRequest() parameter.
#define MIP_EXPECT_NO_RESPONSE 0
#define MIP_EXPECT_RESPONSE    1



// It is expected that the user will only instantiate a single MiP object (mostly likely a global object). This
// pointer is set from within that singleton's constructor and later used by the global MiPStream when it needs to
// call upon the MiP object to switch the UART between MiP and PC.
MiP* MiP::s_pInstance = NULL;



// Define an assert mechanism that can be used to log and halt when the user is found to be calling the API incorrectly.
#define MIP_ASSERT(EXPRESSION) if (!(EXPRESSION)) mipAssert(__LINE__);

static void mipAssert(uint32_t lineNumber)
{
    MiPStream.print(F("MiP Assert: mip.cpp:"));
        MiPStream.println(lineNumber);

    while (1)
    {
    }
}



MiP::MiP(int8_t serialSelectPin /* = MIP_UART_SELECT_PIN */)
{
    m_serialSelectPin = serialSelectPin;

    clear();

    // Track this instance in class specific global so that MiPStream can find it.
    s_pInstance = this;
}

MiP::~MiP()
{
    end();
    s_pInstance = NULL;
}

void MiP::clear()
{
    m_lastRequestTime = millis();
    m_lastContinuousDriveTime = millis();
    m_flags = 0;
    memset(m_responseBuffer, 0, sizeof(m_responseBuffer));
    m_expectedResponseCommand = 0;
    m_expectedResponseSize = 0;
    m_lastError = MIP_ERROR_NONE;
    m_soundIndex = -1;
    m_lastRadar = MIP_RADAR_INVALID;
    m_lastStatus.clear();
    m_lastWeight = 0;
    m_clapEvents.clear();
    m_gestureEvents.clear();
}

bool MiP::begin()
{
    // Configure the pin used to select the UART destination between the MiP and PC.
    pinMode(m_serialSelectPin, OUTPUT);
    digitalWrite(m_serialSelectPin, LOW);

    // The MiP requires the UART to communicate at 115200-N-8-1.
    // Call MiPStream.begin() instead of Serial.begin() directly so that it can track the begin/end state. This allows
    // it to know that it should automatically initialize the Serial stream to 115200 if the user attempts to write to 
    // it before calling this MiP::begin method.
    MiPStream.begin(115200);
    Serial.setTimeout(MIP_RESPONSE_TIMEOUT);

    // Initialize the class members.
    clear();
    // Roll the timers back so that the first calls can occur immediately.
    m_lastRequestTime = millis() - MIP_REQUEST_DELAY;
    m_lastContinuousDriveTime = millis() - MIP_CONTINUOUS_DRIVE_DELAY;

    // Assume that the connection to the MiP will be successfully initialized. Will clear the flag if a connection
    // error is detected. If this wasn't done then the calls to rawSend() & rawGetStatus() below would fail.
    m_flags |= MRI_FLAG_INITIALIZED;

    // Sometimes the init fails. It seems to happen when the MiP is busy at power-up doing other things like
    // attempting to balance.
    int8_t retry;
    for (retry = 0 ; retry < MIP_MAX_BEGIN_RETRIES ; retry++)
    {
        // Send 0xFF to the MiP via UART to enable the UART communication channel in the MiP.
        const uint8_t initMipCommand[] = { 0xFF };
        rawSend(initMipCommand, sizeof(initMipCommand));

        // The MiP UART documentation indicates that this delay is required after sending 0xFF.
        delay(30);
        // Flush any outstanding junk data in receive buffer.
        discardUnexpectedSerialData();

        // Attempt to get the MiP's latest status to see if the connection was successful or not.
        int result = rawGetStatus(m_lastStatus);
        if (result == MIP_ERROR_NONE)
        {
            // Connection must be successful since this request was successful.
            break;
        }

        // Sleep a bit before making the next attempt.
        delay(MIP_BEGIN_RETRY_WAIT);
    }
    if (retry == MIP_MAX_BEGIN_RETRIES)
    {
        m_flags &= ~MRI_FLAG_INITIALIZED;
        end();
        return false;
    }

    return true;
}

void MiP::end()
{
    if (isInitialized())
    {
        // Send the disconnect command.  If it is successful the app will be disconnected, indicated by a 
        // blue chest LED.
        const uint8_t command[] = { MIP_CMD_DISCONNECT_APP };
        rawSend(command, sizeof(command));
    }

    clear();

    MiPStream.end();
    pinMode(m_serialSelectPin, INPUT);
}

void MiP::sleep()
{
    // Put the MiP to sleep. 
    // The MiP will need to be reset before another begin() will succeed.
    const uint8_t command[] = { MIP_CMD_SLEEP };
    rawSend(command, sizeof(command));
}



void MiP::printLastCallResult()
{
    if (m_lastError != MIP_ERROR_NONE)
    {
        MiPStream.print(F("mip: MiP API returned "));
        switch (m_lastError) 
        {
        case MIP_ERROR_TIMEOUT: 
            MiPStream.println(F("MIP_ERROR_TIMEOUT (Timed out waiting for response)"));
            break;
        case MIP_ERROR_NO_EVENT: 
            MiPStream.println(F("MIP_ERROR_NO_EVENT (No event has arrived from MiP yet)"));
            break;
        case MIP_ERROR_BAD_RESPONSE: 
            MiPStream.println(F("MIP_ERROR_BAD_RESPONSE (Unexpected response from MiP)")); 
            break;
        case MIP_ERROR_MAX_RETRIES:
            MiPStream.println(F("MIP_ERROR_MAX_RETRIES (Exceeded maximum number of retries to get this operation to succeed)"));
            break;
        default: 
            MiPStream.println(F("unknown error")); 
            break;
        }
    }
}


void MiP::enableRadarMode()
{
    verifiedSetGestureRadarMode(MIP_RADAR);
}

void MiP::enableGestureMode()
{
    verifiedSetGestureRadarMode(MIP_GESTURE);
}

void MiP::disableRadarMode()
{
    verifiedSetGestureRadarMode(MIP_GESTURE_RADAR_DISABLED);
}

void MiP::disableGestureMode()
{
    verifiedSetGestureRadarMode(MIP_GESTURE_RADAR_DISABLED);
}

// This internal protected method sends the command to change the radar/gesture mode and then sends a request to get
// the new state. If this request fails or the new state isn't as expected, it will retry the command.
void MiP::verifiedSetGestureRadarMode(MiPGestureRadarMode desiredMode)
{
    int8_t result;

    // Always mark cached RADAR data as invalid when changing modes.
    m_flags &= ~MIP_FLAG_RADAR_VALID;

    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        rawSetGestureRadarMode(desiredMode);

        // Read back and make sure that it was set as expected.
        MiPGestureRadarMode actualMode = MIP_GESTURE_RADAR_DISABLED;
        result = rawGetGestureRadarMode(actualMode);
        if (result == MIP_ERROR_NONE && actualMode == desiredMode)
        {
            // The set was successful so return immediately.
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    if (result != MIP_ERROR_NONE)
    {
        // Kept getting an error back from rawGetGestureRadarMode().
        m_lastError = result;
    }
    else
    {
        // rawGetGestureRadarMode() was successful but didn't match mode to which we were attempting to change.
        m_lastError = MIP_ERROR_MAX_RETRIES;
    }
}

bool MiP::isRadarModeEnabled()
{
    return checkGestureRadarMode(MIP_RADAR);
}

bool MiP::isGestureModeEnabled()
{
    return checkGestureRadarMode(MIP_GESTURE);
}

bool MiP::areGestureAndRadarModesDisabled()
{
    return checkGestureRadarMode(MIP_GESTURE_RADAR_DISABLED);
}

// This internal protected method requests the current radar/gesture mode and then returns whether it matches the
// passed in value or not. It includes retry code incase the request should fail.
bool MiP::checkGestureRadarMode(MiPGestureRadarMode expectedMode)
{
    int8_t result;
    
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        MiPGestureRadarMode currentMode;
        result = rawGetGestureRadarMode(currentMode);
        if (result == MIP_ERROR_NONE)
        {
            return currentMode == expectedMode;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    m_lastError = result;
    return false;
}

MiPRadar MiP::readRadar()
{
    // Fetch bytes from the Serial receive buffer and process any event data found within.
    processAllResponseData();

    if ((m_flags & MIP_FLAG_RADAR_VALID) == 0)
    {
        // Haven't received a radar event yet.
        m_lastError = MIP_ERROR_NO_EVENT;
        return MIP_RADAR_INVALID;
    }

    m_lastError = MIP_ERROR_NONE;
    return m_lastRadar;
}

uint8_t MiP::availableGestureEvents()
{
    // Fetch bytes from the Serial receive buffer and process any event data found within.
    processAllResponseData();
    m_lastError = MIP_ERROR_NONE;
    return m_gestureEvents.available();
}

MiPGesture MiP::readGestureEvent()
{
    // Fetch bytes from the Serial receive buffer and process any event data found within.
    processAllResponseData();

    MiPGesture gestureEvent = MIP_GESTURE_INVALID;
    if (!m_gestureEvents.pop(gestureEvent))
    {
        m_lastError = MIP_ERROR_NO_EVENT;
        return MIP_GESTURE_INVALID;
    }
    m_lastError = MIP_ERROR_NONE;
    return gestureEvent;
}

// This internal protected method sends the set gesture/radar mode command with no error checking. The error handling /
// recovery happens at a higher level of the driver.
void MiP::rawSetGestureRadarMode(MiPGestureRadarMode mode)
{
    uint8_t command[1+1];

    command[0] = MIP_CMD_SET_GESTURE_RADAR_MODE;
    command[1] = mode;
    rawSend(command, sizeof(command));
}

// This internal protected method sends the get gesture/radar mode command with minimal error handling. The error 
// recovery happens at a higher level of the driver.
int8_t MiP::rawGetGestureRadarMode(MiPGestureRadarMode& mode)
{
    const uint8_t getGestureRadarMode[1] = { MIP_CMD_GET_GESTURE_RADAR_MODE };
    uint8_t       response[1+1];
    size_t        responseLength;
    int8_t        result;

    result = rawReceive(getGestureRadarMode, sizeof(getGestureRadarMode), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != 2 ||
        response[0] != MIP_CMD_GET_GESTURE_RADAR_MODE ||
        (response[1] != MIP_GESTURE_RADAR_DISABLED &&
         response[1] != MIP_GESTURE &&
         response[1] != MIP_RADAR))
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    mode = (MiPGestureRadarMode)response[1];
    return MIP_ERROR_NONE;
}



void MiP::writeChestLED(uint8_t red, uint8_t green, uint8_t blue)
{
    int8_t result;

    // The blue channel is actually only 6-bit and not a full 8-bit so zero out lower 2 bits (the MiP does this too).
    blue &= ~3;

    // Send the set command and then issue the corresponding get command. Retry if the get fails or doesn't return the
    // expected new setting.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        rawSetChestLED(red, green, blue);

        // Read back and make sure that it was set as expected.
        MiPChestLED actualChestLED;
        result = rawGetChestLED(actualChestLED);
        if (result == MIP_ERROR_NONE && 
            actualChestLED.red == red &&
            actualChestLED.green == green &&
            actualChestLED.blue == blue)
        {
            // The set was successful so return immediately.
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    if (result != MIP_ERROR_NONE)
    {
        // Kept getting an error back from read attempt.
        m_lastError = result;
    }
    else
    {
        // Read was successful but didn't match setting to which we were attempting to change.
        m_lastError = MIP_ERROR_MAX_RETRIES;
    }
}

void MiP::writeChestLED(uint8_t red, uint8_t green, uint8_t blue, uint16_t onTime, uint16_t offTime)
{
    int8_t result;

    // on/off time are in units of 20 msecs.
    MIP_ASSERT( onTime / 20 <= 255 && offTime / 20 <= 255 );
    onTime = (onTime + 10) / 20;
    offTime = (offTime + 10) / 20;

    // The blue channel is actually only 6-bit and not a full 8-bit so zero out lower 2 bits (the MiP does this too).
    blue &= ~3;
    
    // Send the set command and then issue the corresponding get command. Retry if the get fails or doesn't return the
    // expected new setting.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        rawFlashChestLED(red, green, blue, onTime, offTime);

        // Read back and make sure that it was set as expected.
        MiPChestLED actualChestLED;
        result = rawGetChestLED(actualChestLED);
        if (result == MIP_ERROR_NONE && 
            actualChestLED.red == red &&
            actualChestLED.green == green &&
            actualChestLED.blue == blue &&
            actualChestLED.onTime / 20 == onTime &&
            actualChestLED.offTime / 20 == offTime)
        {
            // The set was successful so return immediately.
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    if (result != MIP_ERROR_NONE)
    {
        // Kept getting an error back from read attempt.
        m_lastError = result;
    }
    else
    {
        // Read was successful but didn't match setting to which we were attempting to change.
        m_lastError = MIP_ERROR_MAX_RETRIES;
    }
}

void MiP::writeChestLED(const MiPChestLED& chestLED)
{
    writeChestLED(chestLED.red, chestLED.green, chestLED.blue, chestLED.onTime, chestLED.offTime);
}

void MiP::readChestLED(MiPChestLED& chestLED)
{
    int8_t result;

    // Retry the read if it should fail on the first attempt.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        result = rawGetChestLED(chestLED);
        if (result == MIP_ERROR_NONE)
        {
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    m_lastError = result;
}

// This internal protected method sends the set chest LED command with no error checking. The error handling /
// recovery happens at a higher level of the driver.
void MiP::rawSetChestLED(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t command[1+3];

    command[0] = MIP_CMD_SET_CHEST_LED;
    command[1] = red;
    command[2] = green;
    command[3] = blue;
    rawSend(command, sizeof(command));
}

// This internal protected method sends the flash chest LED command with no error checking. The error handling /
// recovery happens at a higher level of the driver.
void MiP::rawFlashChestLED(uint8_t red, uint8_t green, uint8_t blue, uint16_t onTime, uint16_t offTime)
{
    uint8_t command[1+5];

    command[0] = MIP_CMD_FLASH_CHEST_LED;
    command[1] = red;
    command[2] = green;
    command[3] = blue;
    command[4] = onTime;
    command[5] = offTime;

    rawSend(command, sizeof(command));
}

// This internal protected method sends the get chest LED command with minimal error handling. The error
// recovery happens at a higher level of the driver.
int8_t MiP::rawGetChestLED(MiPChestLED& chestLED)
{
    const uint8_t getChestLED[1] = { MIP_CMD_GET_CHEST_LED };
    uint8_t       response[1+5];
    size_t        responseLength;
    int           result;

    chestLED.clear();
    result = rawReceive(getChestLED, sizeof(getChestLED), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) || response[0] != MIP_CMD_GET_CHEST_LED )
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    chestLED.red = response[1];
    chestLED.green = response[2];
    chestLED.blue = response[3];
    // on/off time are in units of 20 msecs.
    chestLED.onTime = (uint16_t)response[4] * 20;
    chestLED.offTime = (uint16_t)response[5] * 20;
    return MIP_ERROR_NONE;
}


void MiP::writeHeadLEDs(MiPHeadLED led1, MiPHeadLED led2, MiPHeadLED led3, MiPHeadLED led4)
{
    int8_t result;
    
    // Send the set command and then issue the corresponding get command. Retry if the get fails or doesn't return the
    // expected new setting.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        rawSetHeadLEDs(led1, led2, led3, led4);

        // Read back and make sure that it was set as expected.
        MiPHeadLEDs headLEDs;
        result = rawGetHeadLEDs(headLEDs);
        if (result == MIP_ERROR_NONE && 
            headLEDs.led1 == led1 &&
            headLEDs.led2 == led2 &&
            headLEDs.led3 == led3 &&
            headLEDs.led4 == led4)
        {
            // The set was successful so return immediately.
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    if (result != MIP_ERROR_NONE)
    {
        // Kept getting an error back from read attempt.
        m_lastError = result;
    }
    else
    {
        // Read was successful but didn't match setting to which we were attempting to change.
        m_lastError = MIP_ERROR_MAX_RETRIES;
    }
}

void MiP::writeHeadLEDs(const MiPHeadLEDs& headLEDs)
{
    writeHeadLEDs(headLEDs.led1, headLEDs.led2, headLEDs.led3, headLEDs.led4);
}

void MiP::readHeadLEDs(MiPHeadLEDs& headLEDs)
{
    int8_t result;
    
    // Retry the read if it should fail on the first attempt.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        result = rawGetHeadLEDs(headLEDs);
        if (result == MIP_ERROR_NONE)
        {
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    m_lastError = result;
}

// This internal protected method sends the set head LEDs command with no error checking. The error handling /
// recovery happens at a higher level of the driver.
void MiP::rawSetHeadLEDs(MiPHeadLED led1, MiPHeadLED led2, MiPHeadLED led3, MiPHeadLED led4)
{
    uint8_t command[1+4];

    command[0] = MIP_CMD_SET_HEAD_LEDS;
    command[1] = led1;
    command[2] = led2;
    command[3] = led3;
    command[4] = led4;

    rawSend(command, sizeof(command));
}

// This internal protected method sends the get head LEDs command with minimal error handling. The error
// recovery happens at a higher level of the driver.
int8_t MiP::rawGetHeadLEDs(MiPHeadLEDs& headLEDs)
{
    const uint8_t getHeadLEDs[1] = { MIP_CMD_GET_HEAD_LEDS };
    uint8_t       response[1+4];
    size_t        responseLength;
    int           result;

    headLEDs.clear();
    result = rawReceive(getHeadLEDs, sizeof(getHeadLEDs), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) ||
        response[0] != (uint8_t)MIP_CMD_GET_HEAD_LEDS ||
        !isValidHeadLED(response[1]) ||
        !isValidHeadLED(response[2]) ||
        !isValidHeadLED(response[3]) ||
        !isValidHeadLED(response[4]))
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    headLEDs.led1 = (MiPHeadLED)response[1];
    headLEDs.led2 = (MiPHeadLED)response[2];
    headLEDs.led3 = (MiPHeadLED)response[3];
    headLEDs.led4 = (MiPHeadLED)response[4];
    return MIP_ERROR_NONE;
}

// This internal protected method is called to validate that each head LED value returned is within the expected
// range.
bool MiP::isValidHeadLED(uint8_t led)
{
    return led <= MIP_HEAD_LED_BLINK_FAST;
}


void MiP::continuousDrive(int8_t velocity, int8_t turnRate)
{
    uint8_t command[1+2];

    MIP_ASSERT( velocity >= -32 && velocity <= 32 );
    MIP_ASSERT( turnRate >= -32 && turnRate <= 32 );

    // Ignore requests if they come in too fast so that it can be done in a tight loop but not overload MiP.
    if (millis() - m_lastContinuousDriveTime < MIP_CONTINUOUS_DRIVE_DELAY)
    {
        m_lastError = MIP_ERROR_NONE;
        return;
    }
    m_lastContinuousDriveTime = millis();

    command[0] = MIP_CMD_CONTINUOUS_DRIVE;

    if (velocity == 0)
    {
        command[1] = 0x00;
    }
    else if (velocity < 0)
    {
        command[1] = 0x20 + (-velocity);
    }
    else
    {
        command[1] = velocity;
    }

    if (turnRate == 0)
    {
        command[2] = 0x00;
    }
    else if (turnRate < 0)
    {
        command[2] = 0x60 + (-turnRate);
    }
    else
    {
        command[2] = 0x40 + turnRate;
    }

    // Send this command blindly with no error checking since there is no way to determine if it has failed.
    rawSend(command, sizeof(command));
    m_lastError = MIP_ERROR_NONE;
}

void MiP::distanceDrive(MiPDriveDirection driveDirection, uint8_t cm, MiPTurnDirection turnDirection, uint16_t degrees)
{
    uint8_t command[1+5];

    MIP_ASSERT( degrees <= 360 );

    command[0] = MIP_CMD_DISTANCE_DRIVE;
    command[1] = driveDirection;
    command[2] = cm;
    command[3] = turnDirection;
    command[4] = degrees >> 8;
    command[5] = degrees & 0xFF;

    // Send this command blindly with no error checking since there is no way to determine if it has failed.
    rawSend(command, sizeof(command));
    m_lastError = MIP_ERROR_NONE;
}

void MiP::turnLeft(uint16_t degrees, uint8_t speed)
{
    // The turn command is in units of 5 degrees.
    uint8_t angle = degrees / 5;
    uint8_t command[1+2];

    MIP_ASSERT( degrees <= 255 * 5 );
    MIP_ASSERT( speed <= 24 );

    command[0] = MIP_CMD_TURN_LEFT;
    command[1] = angle;
    command[2] = speed;

    // Send this command blindly with no error checking since there is no way to determine if it has failed.
    rawSend(command, sizeof(command));
    m_lastError = MIP_ERROR_NONE;
}

void MiP::turnRight(uint16_t degrees, uint8_t speed)
{
    // The turn command is in units of 5 degrees.
    uint8_t angle = degrees / 5;
    uint8_t command[1+2];

    MIP_ASSERT( degrees <= 255 * 5 );
    MIP_ASSERT( speed <= 24 );

    command[0] = MIP_CMD_TURN_RIGHT;
    command[1] = angle;
    command[2] = speed;

    // Send this command blindly with no error checking since there is no way to determine if it has failed.
    rawSend(command, sizeof(command));
    m_lastError = MIP_ERROR_NONE;
}

void MiP::driveForward(uint8_t speed, uint16_t time)
{
    // The time parameters is in units of 7 milliseconds.
    uint8_t command[1+2];

    MIP_ASSERT( speed <= 30 );
    MIP_ASSERT( time <= 255 * 7 );

    command[0] = MIP_CMD_DRIVE_FORWARD;
    command[1] = speed;
    command[2] = time / 7;

    // Send this command blindly with no error checking since there is no way to determine if it has failed.
    rawSend(command, sizeof(command));
    m_lastError = MIP_ERROR_NONE;
}

void MiP::driveBackward(uint8_t speed, uint16_t time)
{
    // The time parameters is in units of 7 milliseconds.
    uint8_t command[1+2];

    MIP_ASSERT( speed <= 30 );
    MIP_ASSERT( time <= 255 * 7 );

    command[0] = MIP_CMD_DRIVE_BACKWARD;
    command[1] = speed;
    command[2] = time / 7;

    // Send this command blindly with no error checking since there is no way to determine if it has failed.
    rawSend(command, sizeof(command));
    m_lastError = MIP_ERROR_NONE;
}

void MiP::stop()
{
    uint8_t command[1];

    // Send this command blindly with no error checking since there is no way to determine if it has failed.
    command[0] = MIP_CMD_STOP;
    rawSend(command, sizeof(command));
    m_lastError = MIP_ERROR_NONE;
}

void MiP::fallForward()
{
    fallDown(MIP_FALL_FACE_DOWN);
    m_lastError = MIP_ERROR_NONE;
}

void MiP::fallBackward()
{
    fallDown(MIP_FALL_ON_BACK);
    m_lastError = MIP_ERROR_NONE;
}

// This internal protected method sends the desired set position command to fall forward or backward.
void MiP::fallDown(MiPFallDirection direction)
{
    uint8_t command[1+1];

    command[0] = MIP_CMD_SET_POSITION;
    command[1] = direction;

    // Send this command blindly with no error checking since there is no easy way to determine if it has failed.
    rawSend(command, sizeof(command));
}

void MiP::getUp(MiPGetUp getup /* = MIP_GETUP_FROM_EITHER */)
{
    uint8_t command[1+1];

    command[0] = MIP_CMD_GET_UP;
    command[1] = getup;

    // Send this command blindly with no error checking since there is no easy way to determine if it has failed.
    rawSend(command, sizeof(command));
    m_lastError = MIP_ERROR_NONE;
}


void MiP::playSound(MiPSoundIndex sound, MiPVolume volume /* = MIP_VOLUME_DEFAULT */)
{
    beginSoundList();
    addEntryToSoundList(sound, 0, volume);
    playSoundList();
}

void MiP::beginSoundList()
{
    m_soundIndex = 0;
    m_playVolume = 0xFF;
    m_lastError = MIP_ERROR_NONE;
}

void MiP::addEntryToSoundList(MiPSoundIndex sound, uint16_t delay /* = 0 */, MiPVolume volume /* = MIP_VOLUME_DEFAULT */)
{
    // Must call beginSoundList() before calling this function.
    MIP_ASSERT ( m_soundIndex != -1 );

    // Delay is in units of 30 msecs and can't exceed 255 * 30.
    MIP_ASSERT( delay <= 255 * 30 );

    // Volume can only be set to values between 0 and 7 or 0xFF (which means keep volume as it was).
    MIP_ASSERT ( volume <= MIP_VOLUME_7 || volume == MIP_VOLUME_DEFAULT );

    // Need to issue volume command if volume is being changed.
    if (volume != MIP_VOLUME_DEFAULT && volume != m_playVolume)
    {
        // The sound list can only hold 8 sound entries.
        MIP_ASSERT ( m_soundIndex < 8 );
        m_playCommand[1 + m_soundIndex * 2] = MIP_SOUND_VOLUME_OFF + volume;
        m_playCommand[1 + m_soundIndex * 2 + 1] = 0;
        m_playVolume = volume;
        m_soundIndex++;
    }
    
    // The sound list can only hold 8 sound entries.
    MIP_ASSERT ( m_soundIndex < 8 );
    m_playCommand[1 + m_soundIndex * 2] = sound;
    m_playCommand[1 + m_soundIndex * 2 + 1] = delay / 30;
    m_soundIndex++;

    m_lastError = MIP_ERROR_NONE;
}

void MiP::playSoundList(uint8_t repeatCount /* = 0 */)
{
    // Must call beginSoundList() and addSoundToList() before calling this function.
    MIP_ASSERT ( m_soundIndex >= 1 );

    m_playCommand[0] = MIP_CMD_PLAY_SOUND;

    // Fill out the rest of the command buffer with mute sounds.
    while (m_soundIndex < 8)
    {
        m_playCommand[1 + m_soundIndex * 2] = MIP_SOUND_SHORT_MUTE_FOR_STOP;
        m_playCommand[1 + m_soundIndex * 2 + 1] = 0;
        m_soundIndex++;
    }
    // The last byte in the command is the repeat count.
    m_playCommand[17] = repeatCount;

    // Send this command blindly with no error checking since there is no way to determine if it has failed.
    rawSend(m_playCommand, sizeof(m_playCommand));

    // Set the index to 8 to flag that no more items can be added to the sound list but you can still play it again.
    m_soundIndex = 8;
    
    m_lastError = MIP_ERROR_NONE;
}

void MiP::writeVolume(uint8_t volume)
{
    int8_t result;
    
    // Send the set command and then issue the corresponding get command. Retry if the get fails or doesn't return the
    // expected new setting.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        rawSetVolume(volume);

        // Read back and make sure that it was set as expected.
        uint8_t updatedVolume;
        result = rawGetVolume(updatedVolume);
        if (result == MIP_ERROR_NONE && updatedVolume == volume)
        {
            // The set was successful so return immediately.
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    if (result != MIP_ERROR_NONE)
    {
        // Kept getting an error back from read attempt.
        m_lastError = result;
    }
    else
    {
        // Read was successful but didn't match setting to which we were attempting to change.
        m_lastError = MIP_ERROR_MAX_RETRIES;
    }
}

uint8_t MiP::readVolume()
{
    int8_t result;

    // Retry the read if it should fail on the first attempt.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        uint8_t volume;
        result = rawGetVolume(volume);
        if (result == MIP_ERROR_NONE)
        {
            m_lastError = MIP_ERROR_NONE;
            return volume;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    m_lastError = result;
    return 0;
}

// This internal protected method sends the set volume command with no error checking. The error handling /
// recovery happens at a higher level of the driver.
void MiP::rawSetVolume(uint8_t volume)
{
    uint8_t command[1+1];

    MIP_ASSERT( volume <= 7 );

    command[0] = MIP_CMD_SET_VOLUME;
    command[1] = volume;

    rawSend(command, sizeof(command));
}

// This internal protected method sends the get volume command with minimal error handling. The error
// recovery happens at a higher level of the driver.
int8_t MiP::rawGetVolume(uint8_t& volume)
{
    const uint8_t getVolume[1] = { MIP_CMD_GET_VOLUME };
    uint8_t       response[1+1];
    size_t        responseLength;
    int8_t        result;

    volume = 0;
    result = rawReceive(getVolume, sizeof(getVolume), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) ||
        response[0] != MIP_CMD_GET_VOLUME ||
        response[1] > 7)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    volume = response[1];
    return result;
}


float MiP::readDistanceTravelled()
{
    int8_t result;

    // Retry the read if it should fail on the first attempt.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        float distance;
        result = rawReadOdometer(distance);
        if (result == MIP_ERROR_NONE)
        {
            m_lastError = MIP_ERROR_NONE;
            return distance;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    m_lastError = result;
    return 0.0f;
}

void MiP::resetDistanceTravelled()
{
    uint8_t command[1];

    command[0] = MIP_CMD_RESET_ODOMETER;

    // Send this command blindly with no error checking since there is no robust way to determine if it has failed.
    rawSend(command, sizeof(command));
}

// This internal protected method sends the read odometer command with minimal error handling. The error
// recovery happens at a higher level of the driver.
int8_t MiP::rawReadOdometer(float& distanceInCm)
{
    const uint8_t readOdometer[1] = { MIP_CMD_READ_ODOMETER };
    uint8_t       response[1+4];
    size_t        responseLength;
    uint32_t      ticks;
    int           result;

    distanceInCm = 0.0f;
    result = rawReceive(readOdometer, sizeof(readOdometer), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) ||
        response[0] != MIP_CMD_READ_ODOMETER)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    // Tick count is store as big-endian in response buffer.
    ticks = (uint32_t)response[1] << 24 | (uint32_t)response[2] << 16 | (uint32_t)response[3] << 8 | response[4];
    // Odometer has 48.5 ticks / cm.
    distanceInCm = (float)((double)ticks / 48.5);
    return result;
}


float MiP::readBatteryVoltage()
{
    // Fetch bytes from the Serial receive buffer and process any event data found within.
    processAllResponseData();
    
    m_lastError = MIP_ERROR_NONE;
    return m_lastStatus.battery;
}

MiPPosition MiP::readPosition()
{
    // Fetch bytes from the Serial receive buffer and process any event data found within.
    processAllResponseData();
    
    m_lastError = MIP_ERROR_NONE;
    return m_lastStatus.position;
}

bool MiP::isOnBack()
{
    return readPosition() == MIP_POSITION_ON_BACK;
}

bool MiP::isFaceDown()
{
    return readPosition() == MIP_POSITION_FACE_DOWN;
}

bool MiP::isUpright()
{
    return readPosition() == MIP_POSITION_UPRIGHT;
}

bool MiP::isPickedUp()
{
    return readPosition() == MIP_POSITION_PICKED_UP;
}

bool MiP::isHandStanding()
{
    return readPosition() == MIP_POSITION_HAND_STAND;
}

bool MiP::isFaceDownOnTray()
{
    return readPosition() == MIP_POSITION_FACE_DOWN_ON_TRAY;
}

bool MiP::isOnBackWithKickstand()
{
    return readPosition() == MIP_POSITION_ON_BACK_WITH_KICKSTAND;
}

// This internal protected method sends the get status command with minimal error handling. The error
// recovery happens at a higher level of the driver in begin(). All status updates after begin() come from events.
int8_t MiP::rawGetStatus(MiPStatus& status)
{
    const uint8_t getStatus[1] = { MIP_CMD_GET_STATUS };
    uint8_t       response[1+2];
    size_t        responseLength;
    int           result;

    status.clear();
    result = rawReceive(getStatus, sizeof(getStatus), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    return parseStatus(status, response, responseLength);
}

// This internal protected method takes the status response, validates it, converts it into convenient units and
// packs the result into a MiPStatus class.
int8_t MiP::parseStatus(MiPStatus& status, const uint8_t response[], size_t responseLength)
{
    if (responseLength != 3 ||
        response[0] != MIP_CMD_GET_STATUS ||
        response[2] > MIP_POSITION_ON_BACK_WITH_KICKSTAND)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    // Convert battery integer value to floating point voltage value.
    status.battery = (float)(((response[1] - 0x4D) / (float)(0x7C - 0x4D)) * (6.4f - 4.0f)) + 4.0f;
    status.position = (MiPPosition)response[2];
    return MIP_ERROR_NONE;
}


int8_t MiP::readWeight()
{
    // Fetch bytes from the Serial receive buffer and process any event data found within.
    processAllResponseData();
    
    if ((m_flags & MIP_FLAG_WEIGHT_VALID))
    {
        // Have a cached weight event already, so just return it.
        m_lastError = MIP_ERROR_NONE;
        return m_lastWeight;
    }

    // Haven't seen a weight event yet so request the weight explicitly.
    // Retry the read if it should fail on the first attempt.
    int8_t result;
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        int8_t weight;
        result = rawGetWeight(weight);
        if (result == MIP_ERROR_NONE)
        {
            // Cache the returned value and return it to the caller.
            m_lastError = MIP_ERROR_NONE;
            m_lastWeight = weight;
            m_flags |= MIP_FLAG_WEIGHT_VALID;
            return weight;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    m_lastError = result;
    return 0.0f;
}

// This internal protected method sends the get weight command with minimal error handling. The error
// recovery happens at a higher level of the driver.
int8_t MiP::rawGetWeight(int8_t& weight)
{
    const uint8_t getWeight[1] = { MIP_CMD_GET_WEIGHT };
    uint8_t       response[1+1];
    size_t        responseLength;
    int           result;

    weight = 0.0f;
    result = rawReceive(getWeight, sizeof(getWeight), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    return parseWeight(weight, response, responseLength);
}

// This internal protected method takes the weight response and validates it.
int8_t MiP::parseWeight(int8_t& weight, const uint8_t response[], size_t responseLength)
{
    if (responseLength != 2 ||
        response[0] != MIP_CMD_GET_WEIGHT)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    weight = response[1];
    return MIP_ERROR_NONE;
}


void MiP::enableClapEvents()
{
    checkedEnableClapEvents(MIP_CLAP_ENABLED);
}

void MiP::disableClapEvents()
{
    checkedEnableClapEvents(MIP_CLAP_DISABLED);
}

// This internal protected method attempts to enable/disable clap events and then reads back the clap settings to see
// if the new value has taken. Retries on errors or mismatches.
void MiP::checkedEnableClapEvents(MiPClapEnabled enabled)
{
    int8_t result;
    
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        rawEnableClap(enabled);

        // Read back and make sure that it was set as expected.
        MiPClapSettings setting;
        result = rawGetClapSettings(setting);
        if (result == MIP_ERROR_NONE && setting.enabled == enabled)
        {
            // The set was successful so return immediately.
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    if (result != MIP_ERROR_NONE)
    {
        // Kept getting an error back from read back routine.
        m_lastError = result;
    }
    else
    {
        // Read back was successful but write didn't take.
        m_lastError = MIP_ERROR_MAX_RETRIES;
    }
}

void MiP::writeClapDelay(uint16_t delayTime)
{
    int8_t result;
    
    // Send the set command and then issue the corresponding get command. Retry if the get fails or doesn't return the
    // expected new setting.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        rawSetClapDelay(delayTime);

        // Read back and make sure that it was set as expected.
        MiPClapSettings setting;
        result = rawGetClapSettings(setting);
        if (result == MIP_ERROR_NONE && setting.delay == delayTime)
        {
            // The set was successful so return immediately.
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    if (result != MIP_ERROR_NONE)
    {
        // Kept getting an error back from read back routine.
        m_lastError = result;
    }
    else
    {
        // Read back was successful but write didn't take.
        m_lastError = MIP_ERROR_MAX_RETRIES;
    }
}

// This internal protected method sends the enable/disable clap command with no error checking. The error handling /
// recovery happens at a higher level of the driver.
void MiP::rawEnableClap(MiPClapEnabled enabled)
{
    uint8_t command[1+1];

    command[0] = MIP_CMD_ENABLE_CLAP;
    command[1] = enabled;

    rawSend(command, sizeof(command));
}

// This internal protected method sends the set clap delay command with no error checking. The error handling /
// recovery happens at a higher level of the driver.
void MiP::rawSetClapDelay(uint16_t delay)
{
    uint8_t command[1+2];

    command[0] = MIP_CMD_SET_CLAP_DELAY;
    command[1] = delay >> 8;
    command[2] = delay & 0xFF;

    rawSend(command, sizeof(command));
}

bool MiP::areClapEventsEnabled()
{
    MiPClapSettings settings;

    int8_t result = readClapSettings(settings);
    if (result != MIP_ERROR_NONE)
    {
        m_lastError = result;
        return false;
    }

    m_lastError = MIP_ERROR_NONE;
    return settings.enabled == MIP_CLAP_ENABLED;
}

uint16_t MiP::readClapDelay()
{
    MiPClapSettings settings;

    int8_t result = readClapSettings(settings);
    if (result != MIP_ERROR_NONE)
    {
        m_lastError = result;
        return false;
    }

    m_lastError = MIP_ERROR_NONE;
    return settings.delay;
}

// This internal protected method issues the low level get clap settings command and retries if an error is encountered.
int8_t MiP::readClapSettings(MiPClapSettings& settings)
{
    int8_t result;

    // Retry the read if it should fail on the first attempt.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        result = rawGetClapSettings(settings);
        if (result == MIP_ERROR_NONE)
        {
            return MIP_ERROR_NONE;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    settings.clear();
    return result;
}

uint8_t MiP::availableClapEvents()
{
    // Fetch bytes from the Serial receive buffer and process any event data found within.
    processAllResponseData();

    m_lastError = MIP_ERROR_NONE;
    return m_clapEvents.available();
}

uint8_t MiP::readClapEvent()
{
    // Fetch bytes from the Serial receive buffer and process any event data found within.
    processAllResponseData();

    uint8_t clapEvent = 0;
    if (!m_clapEvents.pop(clapEvent))
    {
        // No clap event has been received yet.
        m_lastError = MIP_ERROR_NO_EVENT;
        return 0;
    }
    m_lastError = MIP_ERROR_NONE;
    return clapEvent;
}

// This internal protected method sends the get clap settings command with minimal error handling. The error
// recovery happens at a higher level of the driver.
int8_t MiP::rawGetClapSettings(MiPClapSettings& settings)
{
    const uint8_t getClapSettings[1] = { MIP_CMD_GET_CLAP_SETTINGS };
    uint8_t       response[1+3];
    size_t        responseLength;
    int8_t        result;

    settings.clear();
    result = rawReceive(getClapSettings, sizeof(getClapSettings), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) ||
        response[0] != MIP_CMD_GET_CLAP_SETTINGS ||
        (response[1] != MIP_CLAP_DISABLED && response[1] != MIP_CLAP_ENABLED))
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    settings.enabled = (MiPClapEnabled)response[1];
    settings.delay = (uint16_t)response[2] << 8 | response[3];
    return MIP_ERROR_NONE;
}


bool MiP::hasBeenShaken()
{
    // Fetch bytes from the Serial receive buffer and process any event data found within.
    processAllResponseData();

    m_lastError = MIP_ERROR_NONE;
    if (m_flags & MIP_FLAG_SHAKE_DETECTED)
    {
        // A shake event has been received since the last call to this function. Return true and clear the shake
        // detected bit.
        m_flags &= ~MIP_FLAG_SHAKE_DETECTED;
        return true;
    }
    return false;
}


void MiP::readSoftwareVersion(MiPSoftwareVersion& software)
{
    int8_t result;

    // Retry the read if it should fail on the first attempt.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        result = rawGetSoftwareVersion(software);
        if (result == MIP_ERROR_NONE)
        {
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    m_lastError = result;
}

void MiP::readHardwareInfo(MiPHardwareInfo& hardware)
{
    int8_t result;

    // Retry the read if it should fail on the first attempt.
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        result = rawGetHardwareInfo(hardware);
        if (result == MIP_ERROR_NONE)
        {
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    m_lastError = result;
}

// This internal protected method sends the get software version command with minimal error handling. The error
// recovery happens at a higher level of the driver.
int8_t MiP::rawGetSoftwareVersion(MiPSoftwareVersion& software)
{
    const uint8_t getSoftwareVersion[1] = { MIP_CMD_GET_SOFTWARE_VERSION };
    uint8_t       response[1+4];
    size_t        responseLength;
    int8_t        result;

    software.clear();
    result = rawReceive(getSoftwareVersion, sizeof(getSoftwareVersion), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) || response[0] != MIP_CMD_GET_SOFTWARE_VERSION)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    software.year = 2000 + response[1];
    software.month = response[2];
    software.day = response[3];
    software.uniqueVersion = response[4];
    return result;
}

// This internal protected method sends the get hardware info command with minimal error handling. The error
// recovery happens at a higher level of the driver.
int8_t MiP::rawGetHardwareInfo(MiPHardwareInfo& hardware)
{
    const uint8_t getHardwareInfo[1] = { MIP_CMD_GET_HARDWARE_INFO };
    uint8_t       response[1+2];
    size_t        responseLength;
    int8_t        result;

    hardware.clear();
    result = rawReceive(getHardwareInfo, sizeof(getHardwareInfo), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) || response[0] != MIP_CMD_GET_HARDWARE_INFO)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    hardware.voiceChip = response[1];
    hardware.hardware = response[2];
    return result;
}



void MiP::enableAppMode()
{
    verifiedSetGameMode(MIP_APP_MODE);
}

void MiP::enableCageMode()
{
    verifiedSetGameMode(MIP_CAGE_MODE);
}

void MiP::enableDanceMode()
{
    verifiedSetGameMode(MIP_DANCE_MODE);
}

void MiP::enableStackMode()
{
    verifiedSetGameMode(MIP_STACK_MODE);
}

void MiP::enableTrickMode()
{
    verifiedSetGameMode(MIP_TRICK_MODE);
}

void MiP::enableRoamMode()
{
    verifiedSetGameMode(MIP_ROAM_MODE);
}

bool MiP::isAppModeEnabled()
{
    return checkGameMode(MIP_APP_MODE);
}

bool MiP::isCageModeEnabled()
{
    return checkGameMode(MIP_CAGE_MODE);
}

bool MiP::isDanceModeEnabled()
{
    return checkGameMode(MIP_DANCE_MODE);
}

bool MiP::isStackModeEnabled()
{
    return checkGameMode(MIP_STACK_MODE);
}

bool MiP::isTrickModeEnabled()
{
    return checkGameMode(MIP_TRICK_MODE);
}

bool MiP::isRoamModeEnabled()
{
    return checkGameMode(MIP_ROAM_MODE);
}

bool MiP::checkGameMode(MiPGameMode expectedMode)
{
    int8_t result;
    
    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        MiPGameMode currentMode;
        result = rawGetGameMode(currentMode);
        if (result == MIP_ERROR_NONE)
        {
            return currentMode == expectedMode;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    m_lastError = result;
    return false;
}

// This internal protected method sends the command to change the game mode and then sends a request to get
// the new mode. If this request fails or the new mode isn't as expected, it will retry the command.
void MiP::verifiedSetGameMode(MiPGameMode desiredMode)
{
    int8_t result;

    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        rawSetGameMode(desiredMode);

        // Read back and make sure that it was set as expected.
        MiPGameMode actualMode;
        result = rawGetGameMode(actualMode);
        if (result == MIP_ERROR_NONE && actualMode == desiredMode)
        {
            // The set was successful so return immediately.
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    if (result != MIP_ERROR_NONE)
    {
        // Kept getting an error back from rawGetGameMode().
        m_lastError = result;
    }
    else
    {
        // rawGetGameMode() was successful but didn't match mode to which we were attempting to change.
        m_lastError = MIP_ERROR_MAX_RETRIES;
    }
}

// This internal protected method sends the set game mode command with no error checking. The error handling /
// recovery happens at a higher level of the driver.
void MiP::rawSetGameMode(MiPGameMode mode)
{
    uint8_t command[1+1];

    // Might not accept command if currently running another game mode so Stop first.
    stop();
    
    command[0] = MIP_CMD_SET_GAME_MODE;
    command[1] = mode;
    rawSend(command, sizeof(command));
}

// This internal protected method sends the get game mode command with minimal error handling. The error 
// recovery happens at a higher level of the driver.
int8_t MiP::rawGetGameMode(MiPGameMode& mode)
{
    const uint8_t getGameMode[1] = { MIP_CMD_GET_GAME_MODE };
    uint8_t       response[1+1];
    size_t        responseLength;
    int8_t        result;

    // Might not accept get game mode command when currently running a game mode so Stop first.
    stop();
    
    result = rawReceive(getGameMode, sizeof(getGameMode), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != 2 ||
        response[0] != MIP_CMD_GET_GAME_MODE ||
        (response[1] != MIP_APP_MODE &&
         response[1] != MIP_CAGE_MODE &&
         response[1] != MIP_TRACKING_MODE &&
         response[1] != MIP_DANCE_MODE &&
         response[1] != MIP_DEFAULT_MODE &&
         response[1] != MIP_STACK_MODE &&
         response[1] != MIP_TRICK_MODE &&
         response[1] != MIP_ROAM_MODE))
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    mode = (MiPGameMode)response[1];

    // Restart the game mode now that we have successfully retrieved it.
    rawSetGameMode(mode);
    
    return MIP_ERROR_NONE;
}



void MiP::setUserData(byte address, byte userData)
{
    verifiedSetUserData(address, userData);
}

int8_t MiP::getUserData(byte address, byte& userData)
{
   return rawGetUserData(address, userData);
}

void MiP::verifiedSetUserData(byte address, byte userData)
{
    int8_t result;

    for (uint8_t retry = 0 ; retry < MIP_MAX_RETRIES ; retry++)
    {
        rawSetUserData(address, userData);

        // Read back and make sure that it was set as expected.
        byte storedData = 0x00;
        result = rawGetUserData(address, storedData);
        if (result == MIP_ERROR_NONE && storedData == userData)
        {
            // The set was successful so return immediately.
            m_lastError = MIP_ERROR_NONE;
            return;
        }

        // An error was encountered so we will loop around and try again.
        // Wait for a bit before the next retry.
        delay(MIP_RETRY_WAIT);
    }

    if (result != MIP_ERROR_NONE)
    {
        // Kept getting an error back from rawGetUserData().
        m_lastError = result;
    }
    else
    {
        // rawGetUserData() was successful but didn't match the data we were expecting.
        m_lastError = MIP_ERROR_MAX_RETRIES;
    }
}

// This internal protected method sends the set user data command with no error checking. 
// The error handling and recovery happens at a higher level of the driver.
void MiP::rawSetUserData(byte address, byte userData)
{
    uint8_t command[1+1];
    
    command[0] = MIP_CMD_SET_USER_DATA;
    command[1] = userData;
    rawSend(command, sizeof(command));
}

// This internal protected method sends the get user data command with minimal error handling. 
// The error and recovery happens at a higher level of the driver.
int8_t MiP::rawGetUserData(byte address, byte& userData)
{
    const uint8_t getUserData[1] = { MIP_CMD_GET_USER_DATA };
    uint8_t       response[1+1];
    size_t        responseLength;
    int8_t        result;
    
    result = rawReceive(getUserData, sizeof(getUserData), response, sizeof(response), responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != 2 || response[0] != MIP_CMD_GET_USER_DATA)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    userData = (byte)response[1];    
    return MIP_ERROR_NONE;
}



void MiP::rawSend(const uint8_t request[], size_t requestLength)
{
    transportSendRequest(request, requestLength, MIP_EXPECT_NO_RESPONSE);
}

int8_t MiP::rawReceive(const uint8_t request[], size_t requestLength,
                          uint8_t responseBuffer[], size_t responseBufferSize, size_t& responseLength)
{
    transportSendRequest(request, requestLength, MIP_EXPECT_RESPONSE);
    return transportGetResponse(responseBuffer, responseBufferSize, &responseLength);
}



void MiP::transportSendRequest(const uint8_t* pRequest, size_t requestLength, int expectResponse)
{
    // Must call begin() and have it return 'true' before calling sending commands to the MiP.
    MIP_ASSERT( isInitialized() );

    switchSerialToMiP();

    // Let the MiP process the last request before letting another request be issued.
    while (millis() - m_lastRequestTime < MIP_REQUEST_DELAY)
    {
    }
    
    // Remember the command byte (first byte) if expecting a response to this request since the response should start
    // with the same byte.
    if (expectResponse)
    {
        m_expectedResponseCommand = pRequest[0];
    }
    else
    {
        m_expectedResponseCommand = 0;
    }
    m_expectedResponseSize = 0;
    m_responseBuffer[0] = 0;

    // Send the specified bytes to the MiP via the UART.
    while (requestLength-- > 0)
    {
        Serial.write(*pRequest++);
    }

    m_lastRequestTime = millis();
}

int8_t MiP::transportGetResponse(uint8_t* pResponseBuffer, size_t responseBufferSize, size_t* pResponseLength)
{
    // Must call begin() and have it return 'true' before calling sending commands to the MiP.
    MIP_ASSERT( isInitialized() );

    // Caller is attempting to get a response that is larger than support by the MiP and this library.
    MIP_ASSERT( responseBufferSize <= MIP_RESPONSE_MAX_LEN);

    // UNDONE: I think it would be my bug if the following assert ever fired.
    MIP_ASSERT( m_expectedResponseCommand != 0 );

    switchSerialToMiP();

    // Process all received bytes (which might include out of band notifications) until we find the response to the
    // last request made. Will timeout after a second.
    m_expectedResponseSize = (uint8_t)responseBufferSize;
    uint32_t startTime = millis();
    bool responseFound = false;
    do
    {
        responseFound = processAllResponseData();
    } while (!responseFound && (uint32_t)millis() - startTime < MIP_RESPONSE_TIMEOUT);
    if (!responseFound)
    {
        // Never received the expected response within the timeout window.
        MiPStream.println(F("MiP: Response timeout"));
        return MIP_ERROR_TIMEOUT;
    }

    // Copy reponse data into caller provided buffer and clear state in transport about the expected response.
    memcpy(pResponseBuffer, m_responseBuffer, m_expectedResponseSize);
    *pResponseLength = m_expectedResponseSize;
    m_expectedResponseCommand = 0;
    m_expectedResponseSize = 0;
    m_responseBuffer[0] = 0;

    return MIP_ERROR_NONE;
}

bool MiP::processAllResponseData()
{
    bool    responseFound = false;
    uint8_t buffer[(MIP_RESPONSE_MAX_LEN - 1) * 2];
    size_t  bytesToRead;
    size_t  bytesRead;

    switchSerialToMiP();

    while (Serial.available() >= 2)
    {
        uint8_t highNibble = Serial.read();
        uint8_t lowNibble = Serial.read();
        uint8_t commandByte = (parseHexDigit(highNibble) << 4) | parseHexDigit(lowNibble);

        if (m_expectedResponseCommand != 0 && commandByte == m_expectedResponseCommand)
        {
            // Store away the command byte that we just read into response buffer so that it isn't lost.
            m_responseBuffer[0] = commandByte;

            // Already read the command byte into element 0 of the response buffer earlier so just need to read in the 
            // rest of the expected response bytes now.
            bytesToRead = m_expectedResponseSize - 1;
            bytesRead = Serial.readBytes(buffer, bytesToRead * 2);
            if (bytesRead == bytesToRead * 2)
            {
                copyHexTextToBinary(&m_responseBuffer[1], buffer, bytesToRead);
                responseFound = true;
                // Continue to process any other bytes in the recieve buffer.
                // This would allow something like a rawGetStatus() call to receive the actual data returned for this
                // request and not an older OOB perioidic status notification.
            }
            else
            {
                // Timed out waiting for all of the response data.
                m_expectedResponseCommand = 0;
                m_expectedResponseSize = 0;
                m_responseBuffer[0] = 0;
                MiPStream.print(F("MiP: Response too short: "));
                    MiPStream.print(bytesRead);
                    MiPStream.print(','); 
                    MiPStream.println(bytesToRead * 2);
                break;
            }
        }
        else
        {
            processOobResponseData(commandByte);
        }
    }

    return responseFound;
}

void MiP::copyHexTextToBinary(uint8_t* pDest, uint8_t* pSrc, uint8_t length)
{
    while (length-- > 0)
    {
        *pDest = (parseHexDigit(pSrc[0]) << 4) | parseHexDigit(pSrc[1]);
        pDest++;
        pSrc+=2;
    }
}

uint8_t MiP::parseHexDigit(uint8_t digit)
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

void MiP::processOobResponseData(uint8_t commandByte)
{
    size_t length = 0;

    // The number of additional bytes to read depends on which notification has been found in serial buffer.
    switch (commandByte)
    {
    case MIP_CMD_GET_RADAR_RESPONSE:
    case MIP_CMD_GET_GESTURE_RESPONSE:
    case MIP_CMD_CLAP_RESPONSE:
    case MIP_CMD_GET_WEIGHT:
	case MIP_CMD_GET_USER_DATA:
        length = 1;
        break;
    case MIP_CMD_SHAKE_RESPONSE:
        length = 0;
        break;
    case MIP_CMD_GET_STATUS:
        length = 2;
        break;
    default:
        uint8_t discardedBytes = discardUnexpectedSerialData();
        MiPStream.print(F("MiP: Bad OOB command byte: "));
            MiPStream.print(commandByte, HEX);
            MiPStream.print(F(" (discarded "));
            MiPStream.print(discardedBytes);
            MiPStream.println(F(" bytes)"));
        return;
    }

    // Read in the additional bytes of the notification.
    uint8_t buffer[2 * 2];
    size_t  bytesRead = Serial.readBytes(buffer, length * 2);
    if (bytesRead != length * 2)
    {
        MiPStream.print(F("MiP: OOB too short: "));
            MiPStream.print(bytesRead);
            MiPStream.print(','); 
            MiPStream.println(length * 2);
        return;
    }

    // Convert the hex data to a binary response.
    uint8_t response[MIP_RESPONSE_MAX_LEN];
    response[0] = commandByte;
    copyHexTextToBinary(&response[1], buffer, length);

    // Process the response just received.
    switch (commandByte)
    {
    case MIP_CMD_GET_RADAR_RESPONSE:
        if (response[1] >= MIP_RADAR_NONE && response[1] <= MIP_RADAR_0CM_10CM)
        {
            m_lastRadar = (MiPRadar)response[1];
            m_flags |= MIP_FLAG_RADAR_VALID;
        }
        break;
    case MIP_CMD_GET_GESTURE_RESPONSE:
        if (response[1] >= MIP_GESTURE_LEFT && response[1] <= MIP_GESTURE_BACKWARD)
        {
            m_gestureEvents.push((MiPGesture)response[1]);
        }
        break;
    case MIP_CMD_SHAKE_RESPONSE:
        m_flags |= MIP_FLAG_SHAKE_DETECTED;
        break;
    case MIP_CMD_GET_STATUS:
        parseStatus(m_lastStatus, response, length + 1);
        break;
    case MIP_CMD_GET_WEIGHT:
        m_lastWeight = response[1];
        m_flags |= MIP_FLAG_WEIGHT_VALID;
        break;
    case MIP_CMD_CLAP_RESPONSE:
        m_clapEvents.push(response[1]);
        break;
    default:
        // Invalid notification command bytes were already handled in the previous switch so should never get here.
        MIP_ASSERT ( false );
        break;
    }
}

uint8_t MiP::discardUnexpectedSerialData()
{
    uint8_t discardedBytes = 0;
    
    // Unexpected response data encountered. Throw away all data in serial buffer since it is hard to tell
    // where next response begins.
    while (Serial.available() > 0)
    {
        discardedBytes++;
        Serial.read();
        // Delay long enough for next serial byte to be received if MiP is still actively sending at 115200 baud.
        delayMicroseconds(100);
    }
    return discardedBytes;
}



// This class can be used instead of Serial for sending text to the PC. It makes sure that the MiP ProMini Pack switches
// the UART signals away from the MiP and to the PC before actually performing the Serial write.
MiPStream::MiPStream()
{
    m_isInit = false;
}

// Methods that must be implemented for Stream subclasses.
int MiPStream::available()
{
    initIfNeeded();
    return Serial.available();
}

int MiPStream::read()
{
    initIfNeeded();
    return Serial.read();
}

int MiPStream::peek()
{
    initIfNeeded();
    return Serial.peek();
}

// Methods that must be implemented for Print subclasses.
size_t MiPStream::write(uint8_t byte)
{
    initIfNeeded();
    bool needToRestore = MiP::isInstanceSerialGoingToMiP();
    MiP::switchInstanceSerialToPC();
    size_t result = Serial.write(byte);
    if (needToRestore) 
    {
        MiP::switchInstanceSerialToMiP();
    }
    return result;
}

size_t MiPStream::write(const uint8_t *pBuffer, size_t size)
{
    initIfNeeded();
    bool needToRestore = MiP::isInstanceSerialGoingToMiP();
    MiP::switchInstanceSerialToPC();
    size_t result = Serial.write(pBuffer, size);
    if (needToRestore) 
    {
        MiP::switchInstanceSerialToMiP();
    }
    return result;
}

int MiPStream::availableForWrite()
{
    initIfNeeded();
    return Serial.availableForWrite();
}

void MiPStream::flush()
{
    initIfNeeded();
    return Serial.flush();
}

void MiPStream::begin(unsigned long baud, uint8_t mode)
{
    // Silence compiler warnings about unused parameters.
    (void)baud;
    (void)mode;
    
    if (m_isInit)
    {
        // Ignore redundant begin() calls.
        return;
    }
    m_isInit = true;

    // Fix the baud rate / mode at 115200-8-N-1 since that is required by the MiP.
    Serial.begin(115200, SERIAL_8N1);
}

void MiPStream::end()
{
    if (!m_isInit)
    {
        // Ignore end() if no begin() call has been made.
        return;
    }
    
    Serial.end();
    m_isInit = false;
}

void MiPStream::initIfNeeded()
{
    // Make sure that Serial stream has been initialized by user or MiP.
    if (!m_isInit)
    {
        begin(115200);
    }
}

// Instantiate the single instance of this stream.
class MiPStream MiPStream;
