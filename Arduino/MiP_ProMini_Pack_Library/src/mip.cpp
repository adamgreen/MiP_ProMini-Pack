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
#include <assert.h>
#include "mip.h"


// Number of times that begin() method should try to initialize the MiP.
#define MIP_MAX_INIT_RETRIES 2

// Should timeout if expected response doesn't arrive back in this amount of time (in milliseconds).
#define MIP_RESPONSE_TIMEOUT 1000

// Delay between requests sent to MiP (in milliseconds). The MiP will sometimes ignore requests sent faster than this.
#define MIP_REQUEST_DELAY 4

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

// expectResponse parameter values for transportSendRequest() parameter.
#define MIP_EXPECT_NO_RESPONSE 0
#define MIP_EXPECT_RESPONSE    1



MiP* MiP::s_pInstance = NULL;


MiP::MiP(int8_t serialSelectPin /* = MIP_UART_SELECT_PIN*/ , bool releaseSerialToPc /* = false */)
{
    m_serialSelectPin = serialSelectPin;
    m_flags = 0;
    if (releaseSerialToPc)
    {
        m_flags |= MIP_FLAG_RELEASE_SERIAL;
    }

    clear();

    // Track this instance in class specific global so that PRINT/PRINTLN macros can find it.
    s_pInstance = this;
}

MiP::~MiP()
{
    end();
    s_pInstance = NULL;
}

void MiP::clear()
{
    // Clear all flags but don't forget the releaseSerialToPc parameter sent to the constructor.
    m_flags &= ~MIP_FLAG_RELEASE_SERIAL;
    
    // UNDONE: If these are turned into classes then we don't need to memset them (constructors will do it for us).
    memset(&m_lastRadar, 0, sizeof(m_lastRadar));
    memset(&m_lastGesture, 0, sizeof(m_lastGesture));
    memset(&m_lastStatus, 0, sizeof(m_lastStatus));
    memset(&m_lastWeight, 0, sizeof(m_lastWeight));
    memset(&m_lastClap, 0, sizeof(m_lastClap));

    memset(m_responseBuffer, 0, sizeof(m_responseBuffer));
    m_expectedResponseCommand = 0;
    m_expectedResponseSize = 0;
    m_oobQueueRead = 0;
    m_oobQueueWrite = 0;
    m_oobQueueCount = 0;
    memset(m_oobQueue, 0, sizeof(m_oobQueue));
}

int MiP::begin()
{
    // Configure the pin used to select the UART destination between the MiP and PC.
    pinMode(m_serialSelectPin, OUTPUT);
    digitalWrite(m_serialSelectPin, LOW);

    // The MiP requires the UART to communicate at 115200-N-8-1.
    Serial.begin(115200);
    Serial.setTimeout(MIP_RESPONSE_TIMEOUT);

    clear();

    // Assume that the connection to the MiP will be successfully initialized. Will clear the flag if a connection
    // error is detected. If this wasn't done then the calls to rawSend() & getStatus() below would fail.
    m_flags |= MRI_FLAGS_INITIALIZED;

    // Sometimes the init fails. It seem to happen when the MiP is busy at power on doing other things like
    // attempting to start balancing.
    int8_t retry;
    for (retry = 0 ; retry < MIP_MAX_INIT_RETRIES ; retry++)
    {
        // Send 0xFF to the MiP via UART to enable the UART communication channel in the MiP.
        const uint8_t initMipCommand[] = { 0xFF };
        int result = rawSend(initMipCommand, sizeof(initMipCommand));
        if (result != MIP_ERROR_NONE)
        {
            // Retry on error.
            continue;
        }

        // Issue GetStatus command to see if we have successfully connected or not.
        MiPStatus status;
        result = getStatus(&status);
        if (result == MIP_ERROR_NONE)
        {
            // Connection must be successful since this request was successful.
            break;
        }
    }
    if (retry == MIP_MAX_INIT_RETRIES)
    {
        m_flags &= ~MRI_FLAGS_INITIALIZED;
        end();
        return MIP_ERROR_CONNECT;
    }

    // The MiP UART documentation indicates that this delay is required after sending 0xFF.
    delay(30);

    return MIP_ERROR_NONE;
}

void MiP::end()
{
    if (isInitialized())
    {
        // Send 0xFA to the MiP via UART to put the MiP to sleep.
        const uint8_t sleepMipCommand[] = { 0xFA };
        rawSend(sleepMipCommand, sizeof(sleepMipCommand));
    }

    clear();

    Serial.end();
    pinMode(m_serialSelectPin, INPUT);
}


int MiP::setGestureRadarMode(MiPGestureRadarMode mode)
{
    uint8_t command[1+1];

    assert( mode == MIP_GESTURE_RADAR_DISABLED || mode == MIP_GESTURE || mode == MIP_RADAR );

    command[0] = MIP_CMD_SET_GESTURE_RADAR_MODE;
    command[1] = mode;
    return rawSend(command, sizeof(command));
}

int MiP::getGestureRadarMode(MiPGestureRadarMode* pMode)
{
    const uint8_t getGestureRadarMode[1] = { MIP_CMD_GET_GESTURE_RADAR_MODE };
    uint8_t       response[1+1];
    size_t        responseLength;
    int           result;

    assert ( pMode );
    
    result = rawReceive(getGestureRadarMode, sizeof(getGestureRadarMode), response, sizeof(response), &responseLength);
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

    *pMode = response[1];
    return result;
}

int MiP::setChestLED(uint8_t red, uint8_t green, uint8_t blue)
{
    uint8_t command[1+3];

    command[0] = MIP_CMD_SET_CHEST_LED;
    command[1] = red;
    command[2] = green;
    command[3] = blue;
    return rawSend(command, sizeof(command));
}

int MiP::flashChestLED(uint8_t red, uint8_t green, uint8_t blue, uint16_t onTime, uint16_t offTime)
{
    uint8_t command[1+5];

    // on/off time are in units of 20 msecs.
    assert( onTime / 20 <= 255 && offTime / 20 <= 255 );

    command[0] = MIP_CMD_FLASH_CHEST_LED;
    command[1] = red;
    command[2] = green;
    command[3] = blue;
    command[4] = onTime / 20;
    command[5] = offTime / 20;

    return rawSend(command, sizeof(command));
}

int MiP::getChestLED(MiPChestLED* pChestLED)
{
    const uint8_t getChestLED[1] = { MIP_CMD_GET_CHEST_LED };
    uint8_t       response[1+5];
    size_t        responseLength;
    int           result;

    assert( pChestLED );

    result = rawReceive(getChestLED, sizeof(getChestLED), response, sizeof(response), &responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) || response[0] != MIP_CMD_GET_CHEST_LED )
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    pChestLED->red = response[1];
    pChestLED->green = response[2];
    pChestLED->blue = response[3];
    // on/off time are in units of 20 msecs.
    pChestLED->onTime = (uint16_t)response[4] * 20;
    pChestLED->offTime = (uint16_t)response[5] * 20;
    return result;
}

int MiP::setHeadLEDs(MiPHeadLED led1, MiPHeadLED led2, MiPHeadLED led3, MiPHeadLED led4)
{
    uint8_t command[1+4];

    command[0] = MIP_CMD_SET_HEAD_LEDS;
    command[1] = led1;
    command[2] = led2;
    command[3] = led3;
    command[4] = led4;

    return rawSend(command, sizeof(command));
}

int MiP::getHeadLEDs(MiPHeadLEDs* pHeadLEDs)
{
    const uint8_t getHeadLEDs[1] = { MIP_CMD_GET_HEAD_LEDS };
    uint8_t       response[1+4];
    size_t        responseLength;
    int           result;

    assert( pHeadLEDs );

    result = rawReceive(getHeadLEDs, sizeof(getHeadLEDs), response, sizeof(response), &responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) ||
        response[0] != MIP_CMD_GET_HEAD_LEDS ||
        !isValidHeadLED(response[1]) ||
        !isValidHeadLED(response[2]) ||
        !isValidHeadLED(response[3]) ||
        !isValidHeadLED(response[4]))
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    pHeadLEDs->led1 = response[1];
    pHeadLEDs->led2 = response[2];
    pHeadLEDs->led3 = response[3];
    pHeadLEDs->led4 = response[4];
    return result;
}

int MiP::isValidHeadLED(MiPHeadLED led)
{
    return led >= MIP_HEAD_LED_OFF && led <= MIP_HEAD_LED_BLINK_FAST;
}

int MiP::continuousDrive(int8_t velocity, int8_t turnRate)
{
    uint8_t command[1+2];

    assert( velocity >= -32 && velocity <= 32 );
    assert( turnRate >= -32 && turnRate <= 32 );

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

    return rawSend(command, sizeof(command));
}

int MiP::distanceDrive(MiPDriveDirection driveDirection, uint8_t cm,
                       MiPTurnDirection turnDirection, uint16_t degrees)
{
    uint8_t command[1+5];

    assert( degrees <= 360 );

    command[0] = MIP_CMD_DISTANCE_DRIVE;
    command[1] = driveDirection;
    command[2] = cm;
    command[3] = turnDirection;
    command[4] = degrees >> 8;
    command[5] = degrees & 0xFF;

    return rawSend(command, sizeof(command));
}

int MiP::turnLeft(uint16_t degrees, uint8_t speed)
{
    // The turn command is in units of 5 degrees.
    uint8_t angle = degrees / 5;
    uint8_t command[1+2];

    assert( degrees <= 255 * 5 );
    assert( speed <= 24 );

    command[0] = MIP_CMD_TURN_LEFT;
    command[1] = angle;
    command[2] = speed;

    return rawSend(command, sizeof(command));
}

int MiP::turnRight(uint16_t degrees, uint8_t speed)
{
    // The turn command is in units of 5 degrees.
    uint8_t angle = degrees / 5;
    uint8_t command[1+2];

    assert( degrees <= 255 * 5 );
    assert( speed <= 24 );

    command[0] = MIP_CMD_TURN_RIGHT;
    command[1] = angle;
    command[2] = speed;

    return rawSend(command, sizeof(command));
}

int MiP::driveForward(uint8_t speed, uint16_t time)
{
    // The time parameters is in units of 7 milliseconds.
    uint8_t command[1+2];

    assert( speed <= 30 );
    assert( time <= 255 * 7 );

    command[0] = MIP_CMD_DRIVE_FORWARD;
    command[1] = speed;
    command[2] = time / 7;

    return rawSend(command, sizeof(command));
}

int MiP::driveBackward(uint8_t speed, uint16_t time)
{
    // The time parameters is in units of 7 milliseconds.
    uint8_t command[1+2];

    assert( speed <= 30 );
    assert( time <= 255 * 7 );

    command[0] = MIP_CMD_DRIVE_BACKWARD;
    command[1] = speed;
    command[2] = time / 7;

    return rawSend(command, sizeof(command));
}

int MiP::stop()
{
    uint8_t command[1];

    command[0] = MIP_CMD_STOP;
    return rawSend(command, sizeof(command));
}

int MiP::fallDown(MiPFallDirection direction)
{
    uint8_t command[1+1];

    command[0] = MIP_CMD_SET_POSITION;
    command[1] = direction;

    return rawSend(command, sizeof(command));
}

int MiP::getUp(MiPGetUp getup)
{
    uint8_t command[1+1];

    command[0] = MIP_CMD_GET_UP;
    command[1] = getup;

    return rawSend(command, sizeof(command));
}

int MiP::playSound(const MiPSound* pSounds, size_t soundCount, uint8_t repeatCount)
{
    size_t  i;
    uint8_t command[1+17];

    assert( pSounds );
    assert( soundCount <= 8 );

    command[0] = MIP_CMD_PLAY_SOUND;
    for (i = 0 ; i < 8 ; i++)
    {
        if (i < soundCount)
        {
            // Delay is in units of 30 msecs.
            assert( pSounds[i].delay <= 255 * 30 );
            command[1 + i*2] = pSounds[i].sound;
            command[1 + i*2 +1 ] = pSounds[i].delay / 30;
        }
        else
        {
            command[1 + i*2] = MIP_SOUND_SHORT_MUTE_FOR_STOP;
            command[1 + i*2 + 1] = 0;
        }
    }
    command[17] = repeatCount;

    return rawSend(command, sizeof(command));
}

int MiP::setVolume(uint8_t volume)
{
    uint8_t command[1+1];

    assert( volume <= 7 );

    command[0] = MIP_CMD_SET_VOLUME;
    command[1] = volume;

    return rawSend(command, sizeof(command));
}

int MiP::getVolume(uint8_t* pVolume)
{
    const uint8_t getVolume[1] = { MIP_CMD_GET_VOLUME };
    uint8_t       response[1+1];
    size_t        responseLength;
    int           result;

    assert( pVolume );

    result = rawReceive(getVolume, sizeof(getVolume), response, sizeof(response), &responseLength);
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

    *pVolume = response[1];
    return result;
}

int MiP::readOdometer(float* pDistanceInCm)
{
 const uint8_t readOdometer[1] = { MIP_CMD_READ_ODOMETER };
    uint8_t    response[1+4];
    size_t     responseLength;
    uint32_t   ticks;
    int        result;

    assert( pDistanceInCm );

    result = rawReceive(readOdometer, sizeof(readOdometer), response, sizeof(response), &responseLength);
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
    ticks = response[1] << 24 | response[2] << 16 | response[3] << 8 | response[4];
    // Odometer has 48.5 ticks / cm.
    *pDistanceInCm = (float)((double)ticks / 48.5);
    return result;
}

int MiP::resetOdometer()
{
    uint8_t command[1];

    command[0] = MIP_CMD_RESET_ODOMETER;

    return rawSend(command, sizeof(command));
}

int MiP::getStatus(MiPStatus* pStatus)
{
    const uint8_t getStatus[1] = { MIP_CMD_GET_STATUS };
    uint8_t       response[1+2];
    size_t        responseLength;
    int           result;

    assert( pStatus );

    result = rawReceive(getStatus, sizeof(getStatus), response, sizeof(response), &responseLength);
    if (result)
    {
        return result;
    }
    return parseStatus(pStatus, response, responseLength);
}

int MiP::parseStatus(MiPStatus* pStatus, const uint8_t* pResponse, size_t responseLength)
{
    if (responseLength != 3 ||
        pResponse[0] != MIP_CMD_GET_STATUS ||
        pResponse[2] > MIP_POSITION_ON_BACK_WITH_KICKSTAND)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    // Convert battery integer value to floating point voltage value.
    pStatus->millisec = millis();
    pStatus->battery = (float)(((pResponse[1] - 0x4D) / (float)(0x7C - 0x4D)) * (6.4f - 4.0f)) + 4.0f;
    pStatus->position = pResponse[2];
    return MIP_ERROR_NONE;
}

int MiP::getWeight(MiPWeight* pWeight)
{
    const uint8_t getWeight[1] = { MIP_CMD_GET_WEIGHT };
    uint8_t       response[1+1];
    size_t        responseLength;
    int           result;

    assert( pWeight );

    result = rawReceive(getWeight, sizeof(getWeight), response, sizeof(response), &responseLength);
    if (result)
    {
        return result;
    }
    return parseWeight(pWeight, response, responseLength);
}

int MiP::parseWeight(MiPWeight* pWeight, const uint8_t* pResponse, size_t responseLength)
{
    if (responseLength != 2 ||
        pResponse[0] != MIP_CMD_GET_WEIGHT)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    pWeight->millisec = millis();
    pWeight->weight = pResponse[1];
    return MIP_ERROR_NONE;
}

int MiP::getClapSettings(MiPClapSettings* pSettings)
{
    const uint8_t getClapSettings[1] = { MIP_CMD_GET_CLAP_SETTINGS };
    uint8_t       response[1+3];
    size_t        responseLength;
    int           result;

    assert( pSettings );

    result = rawReceive(getClapSettings, sizeof(getClapSettings), response, sizeof(response), &responseLength);
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

    pSettings->enabled = response[1];
    pSettings->delay = response[2] << 8 | response[3];
    return MIP_ERROR_NONE;
}

int MiP::enableClap(MiPClapEnabled enabled)
{
    uint8_t command[1+1];

    command[0] = MIP_CMD_ENABLE_CLAP;
    command[1] = enabled;

    return rawSend(command, sizeof(command));
}

int MiP::setClapDelay(uint16_t delay)
{
    uint8_t command[1+2];

    command[0] = MIP_CMD_SET_CLAP_DELAY;
    command[1] = delay >> 8;
    command[2] = delay & 0xFF;

    return rawSend(command, sizeof(command));
}

int MiP::getLatestRadarNotification(MiPRadarNotification* pNotification)
{
    MiPRadar radar;

    readNotifications();

    if ((m_flags & MIP_FLAG_RADAR_VALID) == 0)
    {
        return MIP_ERROR_EMPTY;
    }
    radar = m_lastRadar.radar;
    if (radar != MIP_RADAR_NONE && radar != MIP_RADAR_10CM_30CM && radar != MIP_RADAR_0CM_10CM)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    *pNotification = m_lastRadar;
    return MIP_ERROR_NONE;
}

void MiP::readNotifications()
{
    int     result = -1;
    size_t  responseLength = 0;
    uint8_t response[MIP_RESPONSE_MAX_LEN];

    while (MIP_ERROR_NONE == rawReceiveNotification(response, sizeof(response), &responseLength))
    {
        // Must have at least one byte to indicate which response is being given.
        if (responseLength < 1)
        {
            continue;
        }
        switch (response[0])
        {
        case MIP_CMD_GET_RADAR_RESPONSE:
            if (responseLength == 2)
            {
                m_lastRadar.millisec = millis();
                m_lastRadar.radar = response[1];
                m_flags |= MIP_FLAG_RADAR_VALID;
            }
            break;
        case MIP_CMD_GET_GESTURE_RESPONSE:
            if (responseLength == 2)
            {
                m_lastGesture.millisec = millis();
                m_lastGesture.gesture = response[1];
                m_flags |= MIP_FLAG_GESTURE_VALID;
            }
            break;
        case MIP_CMD_SHAKE_RESPONSE:
            if (responseLength == 1)
            {
                m_flags |= MIP_FLAG_SHAKE_VALID;
            }
            break;
        case MIP_CMD_GET_STATUS:
            result = parseStatus(&m_lastStatus, response, responseLength);
            if (result == MIP_ERROR_NONE)
            {
                m_flags |= MIP_FLAG_STATUS_VALID;
            }
            break;
        case MIP_CMD_GET_WEIGHT:
            result = parseWeight(&m_lastWeight, response, responseLength);
            if (result == MIP_ERROR_NONE)
            {
                m_flags |= MIP_FLAG_WEIGHT_VALID;
            }
            break;
        case MIP_CMD_CLAP_RESPONSE:
            if (responseLength == 2)
            {
                m_lastClap.millisec = millis();
                m_lastClap.count = response[1];
                m_flags |= MIP_FLAG_CLAP_VALID;
            }
            break;
        default:
            PRINT(F("MiP: Bad notification: "));
            for (int i = 0 ; i < responseLength ; i++)
            {
                PRINT(response[i], HEX);
                if (i != responseLength - 1)
                {
                    PRINT(',');
                }
            }
            PRINTLN();
            break;
        }
    }
}

int MiP::getLatestGestureNotification(MiPGestureNotification* pNotification)
{
    MiPGesture gesture;

    assert ( pNotification );
    
    readNotifications();

    if ((m_flags & MIP_FLAG_GESTURE_VALID) == 0)
    {
        return MIP_ERROR_EMPTY;
    }
    gesture = m_lastGesture.gesture;
    if (gesture < MIP_GESTURE_LEFT || gesture > MIP_GESTURE_BACKWARD)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    *pNotification = m_lastGesture;
    return MIP_ERROR_NONE;
}

int MiP::getLatestStatusNotification(MiPStatus* pStatus)
{
    assert ( pStatus );
    
    readNotifications();

    if ((m_flags & MIP_FLAG_STATUS_VALID) == 0)
    {
        return MIP_ERROR_EMPTY;
    }

    *pStatus = m_lastStatus;
    return MIP_ERROR_NONE;
}

int MiP::getLatestShakeNotification()
{
    readNotifications();

    if ((m_flags & MIP_FLAG_SHAKE_VALID) == 0)
    {
        return MIP_ERROR_EMPTY;
    }
    m_flags &= ~MIP_FLAG_SHAKE_VALID;
    return MIP_ERROR_NONE;
}

int MiP::getLatestWeightNotification(MiPWeight* pWeight)
{
    assert ( pWeight );
    
    readNotifications();

    if ((m_flags & MIP_FLAG_WEIGHT_VALID) == 0)
    {
        return MIP_ERROR_EMPTY;
    }

    *pWeight = m_lastWeight;
    return MIP_ERROR_NONE;
}

int MiP::getLatestClapNotification(MiPClap* pClap)
{
    assert ( pClap );
    
    readNotifications();

    if ((m_flags & MIP_FLAG_CLAP_VALID) == 0)
    {
        return MIP_ERROR_EMPTY;
    }

    *pClap = m_lastClap;
    return MIP_ERROR_NONE;
}

int MiP::getSoftwareVersion(MiPSoftwareVersion* pSoftware)
{
    const uint8_t getSoftwareVersion[1] = { MIP_CMD_GET_SOFTWARE_VERSION };
    uint8_t       response[1+4];
    size_t        responseLength;
    int           result;

    assert( pSoftware );

    result = rawReceive(getSoftwareVersion, sizeof(getSoftwareVersion), response, sizeof(response), &responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) || response[0] != MIP_CMD_GET_SOFTWARE_VERSION)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    pSoftware->year = 2000 + response[1];
    pSoftware->month = response[2];
    pSoftware->day = response[3];
    pSoftware->uniqueVersion = response[4];
    return result;
}

int MiP::getHardwareInfo(MiPHardwareInfo* pHardware)
{
    const uint8_t getHardwareInfo[1] = { MIP_CMD_GET_HARDWARE_INFO };
    uint8_t       response[1+2];
    size_t        responseLength;
    int           result;

    assert( pHardware );

    result = rawReceive(getHardwareInfo, sizeof(getHardwareInfo), response, sizeof(response), &responseLength);
    if (result)
    {
        return result;
    }
    if (responseLength != sizeof(response) || response[0] != MIP_CMD_GET_HARDWARE_INFO)
    {
        return MIP_ERROR_BAD_RESPONSE;
    }

    pHardware->voiceChip = response[1];
    pHardware->hardware = response[2];
    return result;
}

int MiP::rawSend(const uint8_t* pRequest, size_t requestLength)
{
    return transportSendRequest(pRequest, requestLength, MIP_EXPECT_NO_RESPONSE);
}

int MiP::rawReceive(const uint8_t* pRequest, size_t requestLength,
                          uint8_t* pResponseBuffer, size_t responseBufferSize, size_t* pResponseLength)
{
    int result = -1;

    result = transportSendRequest(pRequest, requestLength, MIP_EXPECT_RESPONSE);
    if (result)
    {
        return result;
    }
    return transportGetResponse(pResponseBuffer, responseBufferSize, pResponseLength);
}

int MiP::rawReceiveNotification(uint8_t* pNotifyBuffer, size_t notifyBufferSize, size_t* pNotifyLength)
{
    return transportGetOutOfBandResponse(pNotifyBuffer, notifyBufferSize, pNotifyLength);
}




int MiP::transportSendRequest(const uint8_t* pRequest, size_t requestLength, int expectResponse)
{
    // Must call begin() successfully before calling this function.
    if (!isInitialized())
    {
        return MIP_ERROR_NOT_CONNECTED;
    }

    switchSerialToMiP();

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

    // If expecting a response then rawReceive() will immediately call transportGetResponse() after this call returns
    // so don't bother to release Serial back to the PC.
    if (!expectResponse && shouldReleaseSerialBeforeReturning())
    {
        switchSerialToPC();
    }

    // UNDONE: Might be better to record time here and spin at top if calling again too soon.
    //         That frees up CPU to do other things while waiting.
    // Let the MiP process the request before letting another request be issued.
    delay(MIP_REQUEST_DELAY);
    
    return MIP_ERROR_NONE;
}

int MiP::transportGetResponse(uint8_t* pResponseBuffer, size_t responseBufferSize, size_t* pResponseLength)
{
    int result = MIP_ERROR_NONE;
    
    // Must call begin() successfully before calling this function.
    if (!isInitialized())
    {
        return MIP_ERROR_NOT_CONNECTED;
    }
    if (m_expectedResponseCommand == 0)
    {
        return MIP_ERROR_NO_REQUEST;
    }
    if (responseBufferSize > MIP_RESPONSE_MAX_LEN)
    {
        return MIP_ERROR_PARAM;
    }

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
        result = MIP_ERROR_TIMEOUT;
        goto Cleanup;
    }

    // Copy reponse data into caller provided buffer and clear state in transport about the expected response.
    memcpy(pResponseBuffer, m_responseBuffer, m_expectedResponseSize);
    *pResponseLength = m_expectedResponseSize;
    m_expectedResponseCommand = 0;
    m_expectedResponseSize = 0;
    m_responseBuffer[0] = 0;

Cleanup:
    if (shouldReleaseSerialBeforeReturning())
    {
        switchSerialToPC();
    }

    return result;
}

bool MiP::processAllResponseData()
{
    bool    responseFound = false;
    uint8_t buffer[(MIP_RESPONSE_MAX_LEN - 1) * 2];
    size_t  bytesToRead;
    size_t  bytesRead;

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
                break;
            }
            else
            {
                // Timed out waiting for all of the response data.
                m_expectedResponseCommand = 0;
                m_expectedResponseSize = 0;
                m_responseBuffer[0] = 0;
                PRINT(F("MiP: Response too short: "));
                    PRINT(bytesRead);
                    PRINT(','); 
                    PRINTLN(bytesToRead * 2);
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
        PRINT(F("MiP: Bad OOB command byte: "));
            PRINT(commandByte, HEX);
            PRINT(F(" (discarded "));
            PRINT(discardedBytes);
            PRINTLN(F(" bytes)"));
        return;
    }

    uint8_t buffer[2 * 2];
    size_t  bytesRead = Serial.readBytes(buffer, length * 2);
    if (bytesRead != length * 2)
    {
        PRINT(F("MiP: OOB too short: "));
            PRINT(bytesRead);
            PRINT(','); 
            PRINTLN(length * 2);
        return;
    }

    // Add this response to the Out Of Band circular queue. It's okay to overwrite oldest item in queue if full.
    OobResponse* pResponse = &m_oobQueue[m_oobQueueWrite];
    pResponse->buffer[0] = commandByte;
    copyHexTextToBinary(&pResponse->buffer[1], buffer, length);
    pResponse->length = (uint8_t)length + 1;

    advanceOobQueueWriteIndex();
    if (m_oobQueueCount < MIP_OOB_RESPONSE_QUEUE_SIZE)
    {
        m_oobQueueCount++;
    }
    else
    {
        // Queue was full so oldest response was overwritten. Increment read index to discard oldest.
        advanceOobQueueReadIndex();
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

void MiP::advanceOobQueueWriteIndex()
{
    if (m_oobQueueWrite == MIP_OOB_RESPONSE_QUEUE_SIZE - 1)
    {
        // Wrap around to beginning of circular queue.
        m_oobQueueWrite = 0;
    }
    else
    {
        m_oobQueueWrite++;
    }
}

void MiP::advanceOobQueueReadIndex()
{
    if (m_oobQueueRead == MIP_OOB_RESPONSE_QUEUE_SIZE - 1)
    {
        // Wrap around to beginning of circular queue.
        m_oobQueueRead = 0;
    }
    else
    {
        m_oobQueueRead++;
    }
}

int MiP::transportGetOutOfBandResponse(uint8_t* pResponseBuffer, size_t responseBufferSize, size_t* pResponseLength)
{
    // Must call begin() successfully before calling this function.
    if (!isInitialized())
    {
        return MIP_ERROR_NOT_CONNECTED;
    }

    switchSerialToMiP();

    processAllResponseData();

    if (m_oobQueueCount == 0)
    {
        return MIP_ERROR_EMPTY;
    }

    // Pop the oldest out of band response from the circular queue.
    OobResponse* pResponse = &m_oobQueue[m_oobQueueRead];
    uint8_t length = pResponse->length;
    if (length > responseBufferSize)
    {
        length = responseBufferSize;
    }
    memcpy(pResponseBuffer, pResponse->buffer, length);
    *pResponseLength = length;
    pResponse->length = 0;
    advanceOobQueueReadIndex();
    m_oobQueueCount--;

    if (shouldReleaseSerialBeforeReturning())
    {
        switchSerialToPC();
    }

    return MIP_ERROR_NONE;
}
