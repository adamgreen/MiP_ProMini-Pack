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
/* This header file describes the public API that an application can use to communicate with the WowWee MiP
   self-balancing robot.
*/
#ifndef MIP_H_
#define MIP_H_

#include <Arduino.h>
#include <stdint.h>
#include <stdlib.h>

// Default pin used by the TS3USB221A switch to connect Serial to the MiP or PC.
//   Set to HIGH, it selects the MiP.
//   Set to LOW, it selects the PC.
#define MIP_UART_SELECT_PIN 2

// Wait for any queued Serial data destined for the MiP to be transmitted, switch Serial output to the PC, send
// desired print/println output to the PC, and then switch Serial output back to the MiP (if that is where it was
// switched before.)
#define PRINT(...) \
    do { \
        bool needToRestore = MiP::isInstanceSerialGoingToMiP(); \
        MiP::switchInstanceSerialToPC(); \
        Serial.print(__VA_ARGS__); \
        if (needToRestore) MiP::switchInstanceSerialToMiP(); \
    } while(0);

#define PRINTLN(...) \
    do { \
        bool needToRestore = MiP::isInstanceSerialGoingToMiP(); \
        MiP::switchInstanceSerialToPC(); \
        Serial.println(__VA_ARGS__); \
        if (needToRestore) MiP::switchInstanceSerialToMiP(); \
    } while(0);

// Integer error codes that can be returned from most of these MiP API functions.
#define MIP_ERROR_NONE          0 // Success
#define MIP_ERROR_CONNECT       1 // Connection to MiP failed.
#define MIP_ERROR_PARAM         2 // Invalid parameter passed to API.
#define MIP_ERROR_NOT_CONNECTED 3 // No MiP robot connected.
#define MIP_ERROR_NO_REQUEST    4 // Not waiting for a response from a request.
#define MIP_ERROR_TIMEOUT       5 // Timed out waiting for response.
#define MIP_ERROR_EMPTY         6 // The queue was empty.
#define MIP_ERROR_BAD_RESPONSE  7 // Unexpected response from MiP.

// Maximum length of MiP request and response buffer lengths.
#define MIP_REQUEST_MAX_LEN     (17 + 1)    // Longest request is MPI_CMD_PLAY_SOUND.
#define MIP_RESPONSE_MAX_LEN    (5 + 1)     // Longest response is MPI_CMD_REQUEST_CHEST_LED.

typedef enum MiPGestureRadarMode
{
    MIP_GESTURE_RADAR_DISABLED = 0x00,
    MIP_GESTURE                = 0x02,
    MIP_RADAR                  = 0x04
} MiPGestureRadarMode;

typedef enum MiPRadar
{
    MIP_RADAR_NONE      = 0x01,
    MIP_RADAR_10CM_30CM = 0x02,
    MIP_RADAR_0CM_10CM  = 0x03
} MiPRadar;

typedef enum MiPGesture
{
    MIP_GESTURE_LEFT               = 0x0A,
    MIP_GESTURE_RIGHT              = 0x0B,
    MIP_GESTURE_CENTER_SWEEP_LEFT  = 0x0C,
    MIP_GESTURE_CENTER_SWEEP_RIGHT = 0x0D,
    MIP_GESTURE_CENTER_HOLD        = 0x0E,
    MIP_GESTURE_FORWARD            = 0x0F,
    MIP_GESTURE_BACKWARD           = 0x10
} MiPGesture;

typedef enum MiPHeadLED
{
    MIP_HEAD_LED_OFF        = 0,
    MIP_HEAD_LED_ON         = 1,
    MIP_HEAD_LED_BLINK_SLOW = 2,
    MIP_HEAD_LED_BLINK_FAST = 3
} MiPHeadLED;

typedef enum MiPDriveDirection
{
    MIP_DRIVE_FORWARD  = 0x00,
    MIP_DRIVE_BACKWARD = 0x01
} MiPDriveDirection;

typedef enum MiPTurnDirection
{
    MIP_TURN_LEFT  = 0x00,
    MIP_TURN_RIGHT = 0x01
} MiPTurnDirection;

typedef enum MiPFallDirection
{
    MIP_FALL_ON_BACK   = 0x00,
    MIP_FALL_FACE_DOWN = 0x01
} MiPFallDirection;

typedef enum MiPPosition
{
    MIP_POSITION_ON_BACK                = 0x00,
    MIP_POSITION_FACE_DOWN              = 0x01,
    MIP_POSITION_UPRIGHT                = 0x02,
    MIP_POSITION_PICKED_UP              = 0x03,
    MIP_POSITION_HAND_STAND             = 0x04,
    MIP_POSITION_FACE_DOWN_ON_TRAY      = 0x05,
    MIP_POSITION_ON_BACK_WITH_KICKSTAND = 0x06
} MiPPosition;

typedef enum MiPGetUp
{
    MIP_GETUP_FROM_FRONT  = 0x00,
    MIP_GETUP_FROM_BACK   = 0x01,
    MIP_GETUP_FROM_EITHER = 0x02
} MiPGetUp;

typedef enum MiPSoundIndex
{
    MIP_SOUND_ONEKHZ_500MS_8K16BIT = 1,
    MIP_SOUND_ACTION_BURPING,
    MIP_SOUND_ACTION_DRINKING,
    MIP_SOUND_ACTION_EATING,
    MIP_SOUND_ACTION_FARTING_SHORT,
    MIP_SOUND_ACTION_OUT_OF_BREATH,
    MIP_SOUND_BOXING_PUNCHCONNECT_1,
    MIP_SOUND_BOXING_PUNCHCONNECT_2,
    MIP_SOUND_BOXING_PUNCHCONNECT_3,
    MIP_SOUND_FREESTYLE_TRACKING_1,
    MIP_SOUND_MIP_1,
    MIP_SOUND_MIP_2,
    MIP_SOUND_MIP_3,
    MIP_SOUND_MIP_APP,
    MIP_SOUND_MIP_AWWW,
    MIP_SOUND_MIP_BIG_SHOT,
    MIP_SOUND_MIP_BLEH,
    MIP_SOUND_MIP_BOOM,
    MIP_SOUND_MIP_BYE,
    MIP_SOUND_MIP_CONVERSE_1,
    MIP_SOUND_MIP_CONVERSE_2,
    MIP_SOUND_MIP_DROP,
    MIP_SOUND_MIP_DUNNO,
    MIP_SOUND_MIP_FALL_OVER_1,
    MIP_SOUND_MIP_FALL_OVER_2,
    MIP_SOUND_MIP_FIGHT,
    MIP_SOUND_MIP_GAME,
    MIP_SOUND_MIP_GLOAT,
    MIP_SOUND_MIP_GO,
    MIP_SOUND_MIP_GOGOGO,
    MIP_SOUND_MIP_GRUNT_1,
    MIP_SOUND_MIP_GRUNT_2,
    MIP_SOUND_MIP_GRUNT_3,
    MIP_SOUND_MIP_HAHA_GOT_IT,
    MIP_SOUND_MIP_HI_CONFIDENT,
    MIP_SOUND_MIP_HI_NOT_SURE,
    MIP_SOUND_MIP_HI_SCARED,
    MIP_SOUND_MIP_HUH,
    MIP_SOUND_MIP_HUMMING_1,
    MIP_SOUND_MIP_HUMMING_2,
    MIP_SOUND_MIP_HURT,
    MIP_SOUND_MIP_HUUURGH,
    MIP_SOUND_MIP_IN_LOVE,
    MIP_SOUND_MIP_IT,
    MIP_SOUND_MIP_JOKE,
    MIP_SOUND_MIP_K,
    MIP_SOUND_MIP_LOOP_1,
    MIP_SOUND_MIP_LOOP_2,
    MIP_SOUND_MIP_LOW_BATTERY,
    MIP_SOUND_MIP_MIPPEE,
    MIP_SOUND_MIP_MORE,
    MIP_SOUND_MIP_MUAH_HA,
    MIP_SOUND_MIP_MUSIC,
    MIP_SOUND_MIP_OBSTACLE,
    MIP_SOUND_MIP_OHOH,
    MIP_SOUND_MIP_OH_YEAH,
    MIP_SOUND_MIP_OOPSIE,
    MIP_SOUND_MIP_OUCH_1,
    MIP_SOUND_MIP_OUCH_2,
    MIP_SOUND_MIP_PLAY,
    MIP_SOUND_MIP_PUSH,
    MIP_SOUND_MIP_RUN,
    MIP_SOUND_MIP_SHAKE,
    MIP_SOUND_MIP_SIGH,
    MIP_SOUND_MIP_SINGING,
    MIP_SOUND_MIP_SNEEZE,
    MIP_SOUND_MIP_SNORE,
    MIP_SOUND_MIP_STACK,
    MIP_SOUND_MIP_SWIPE_1,
    MIP_SOUND_MIP_SWIPE_2,
    MIP_SOUND_MIP_TRICKS,
    MIP_SOUND_MIP_TRIIICK,
    MIP_SOUND_MIP_TRUMPET,
    MIP_SOUND_MIP_WAAAAA,
    MIP_SOUND_MIP_WAKEY,
    MIP_SOUND_MIP_WHEEE,
    MIP_SOUND_MIP_WHISTLING,
    MIP_SOUND_MIP_WHOAH,
    MIP_SOUND_MIP_WOO,
    MIP_SOUND_MIP_YEAH,
    MIP_SOUND_MIP_YEEESSS,
    MIP_SOUND_MIP_YO,
    MIP_SOUND_MIP_YUMMY,
    MIP_SOUND_MOOD_ACTIVATED,
    MIP_SOUND_MOOD_ANGRY,
    MIP_SOUND_MOOD_ANXIOUS,
    MIP_SOUND_MOOD_BORING,
    MIP_SOUND_MOOD_CRANKY,
    MIP_SOUND_MOOD_ENERGETIC,
    MIP_SOUND_MOOD_EXCITED,
    MIP_SOUND_MOOD_GIDDY,
    MIP_SOUND_MOOD_GRUMPY,
    MIP_SOUND_MOOD_HAPPY,
    MIP_SOUND_MOOD_IDEA,
    MIP_SOUND_MOOD_IMPATIENT,
    MIP_SOUND_MOOD_NICE,
    MIP_SOUND_MOOD_SAD,
    MIP_SOUND_MOOD_SHORT,
    MIP_SOUND_MOOD_SLEEPY,
    MIP_SOUND_MOOD_TIRED,
    MIP_SOUND_SOUND_BOOST,
    MIP_SOUND_SOUND_CAGE,
    MIP_SOUND_SOUND_GUNS,
    MIP_SOUND_SOUND_ZINGS,
    MIP_SOUND_SHORT_MUTE_FOR_STOP,
    MIP_SOUND_FREESTYLE_TRACKING_2,
    MIP_SOUND_VOLUME_OFF = 0xF7,
    MIP_SOUND_VOLUME_1   = 0xF8,
    MIP_SOUND_VOLUME_2   = 0xF9,
    MIP_SOUND_VOLUME_3   = 0xFA,
    MIP_SOUND_VOLUME_4   = 0xFB,
    MIP_SOUND_VOLUME_5   = 0xFC,
    MIP_SOUND_VOLUME_6   = 0xFD,
    MIP_SOUND_VOLUME_7   = 0xFE
} MiPSoundIndex;

typedef enum MiPClapEnabled
{
    MIP_CLAP_DISABLED = 0x00,
    MIP_CLAP_ENABLED  = 0x01
} MiPClapEnabled;

typedef struct MiPRadarNotification
{
    uint32_t millisec;
    MiPRadar radar;
} MiPRadarNotification;

typedef struct MiPGestureNotification
{
    uint32_t   millisec;
    MiPGesture gesture;
} MiPGestureNotification;

typedef struct MiPStatus
{
    uint32_t    millisec;
    float       battery;
    MiPPosition position;
} MiPStatus;

typedef struct MiPWeight
{
    uint32_t millisec;
    int8_t   weight;
} MiPWeight;

typedef struct MiPClap
{
    uint32_t millisec;
    uint8_t  count;
} MiPClap;

typedef struct MiPChestLED
{
    uint16_t onTime;
    uint16_t offTime;
    uint8_t  red;
    uint8_t  green;
    uint8_t  blue;
} MiPChestLED;

typedef struct MiPHeadLEDs
{
    MiPHeadLED led1;
    MiPHeadLED led2;
    MiPHeadLED led3;
    MiPHeadLED led4;
} MiPHeadLEDs;

typedef struct MiPSoftwareVersion
{
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  uniqueVersion;
} MiPSoftwareVersion;

typedef struct MiPHardwareInfo
{
    uint8_t voiceChip;
    uint8_t hardware;
} MiPHardwareInfo;

typedef struct MiPSound
{
    MiPSoundIndex sound;
    uint16_t      delay;
} MiPSound;

typedef struct MiPClapSettings
{
    MiPClapEnabled enabled;
    uint16_t       delay;
} MiPClapSettings;



class MiP
{
public:
    // Constructor/Destructors.
    // UNDONE: Switch releaseSerialToPC back to true if possible.
    MiP(int8_t serialSelectPin = MIP_UART_SELECT_PIN, bool releaseSerialToPc = false);
    ~MiP();

    int  begin();
    void end();

    // Will return false if begin() wasn't successful in connecting to the MiP.
    bool isInitialized()
    {
        return (m_flags & MRI_FLAGS_INITIALIZED);
    }

    // Serial is shared between the MiP and the PC on the MiP ProMini Pack.
    // The following methods switch between the two destinations.
    // It flushes any outstanding output destined to the current destination before switching to the new one.
    void switchSerialToMiP()
    {
        Serial.flush();
        digitalWrite(m_serialSelectPin, HIGH);
    }
    void switchSerialToPC()
    {
        Serial.flush();
        digitalWrite(m_serialSelectPin, LOW);
    }
    bool isSerialGoingToMiP()
    {
        return digitalRead(m_serialSelectPin) == HIGH;
    }
    static void switchInstanceSerialToMiP()
    {
        if (s_pInstance)
        {
            s_pInstance->switchSerialToMiP();
        }
    }
    static void switchInstanceSerialToPC()
    {
        if (s_pInstance)
        {
            s_pInstance->switchSerialToPC();
        }
    }
    static bool isInstanceSerialGoingToMiP()
    {
        if (s_pInstance)
        {
            return s_pInstance->isSerialGoingToMiP();
        }
        else
        {
            return false;
        }
    }

    int setGestureRadarMode(MiPGestureRadarMode mode);
    int getGestureRadarMode(MiPGestureRadarMode* pMode);
    
    int setChestLED(uint8_t red, uint8_t green, uint8_t blue);
    int flashChestLED(uint8_t red, uint8_t green, uint8_t blue, uint16_t onTime, uint16_t offTime);
    int getChestLED(MiPChestLED* pChestLED);
    int setHeadLEDs(MiPHeadLED led1, MiPHeadLED led2, MiPHeadLED led3, MiPHeadLED led4);
    int getHeadLEDs(MiPHeadLEDs* pHeadLEDs);
    
    int continuousDrive(int8_t velocity, int8_t turnRate);
    int distanceDrive(MiPDriveDirection driveDirection, uint8_t cm,
                                    MiPTurnDirection turnDirection, uint16_t degrees);
    int turnLeft(uint16_t degrees, uint8_t speed);
    int turnRight(uint16_t degrees, uint8_t speed);
    int driveForward(uint8_t speed, uint16_t time);
    int driveBackward(uint8_t speed, uint16_t time);
    int stop();
    int fallDown(MiPFallDirection direction);
    int getUp(MiPGetUp getup);
    
    int playSound(const MiPSound* pSounds, size_t soundCount, uint8_t repeatCount);
    int setVolume(uint8_t volume);
    int getVolume(uint8_t* pVolume);
    
    int readOdometer(float* pDistanceInCm);
    int resetOdometer();
    
    int getStatus(MiPStatus* pStatus);
    
    int getWeight(MiPWeight* pWeight);
    
    int getClapSettings(MiPClapSettings* pSettings);
    int enableClap(MiPClapEnabled enabled);
    int setClapDelay(uint16_t delay);
    
    int getLatestRadarNotification(MiPRadarNotification* pNotification);
    int getLatestGestureNotification(MiPGestureNotification* pNotification);
    int getLatestStatusNotification(MiPStatus* pStatus);
    int getLatestShakeNotification();
    int getLatestWeightNotification(MiPWeight* pWeight);
    int getLatestClapNotification(MiPClap* pClap);
    
    int getSoftwareVersion(MiPSoftwareVersion* pSoftware);
    int getHardwareInfo(MiPHardwareInfo* pHardware);
    
    int rawSend(const uint8_t* pRequest, size_t requestLength);
    int rawReceive(const uint8_t* pRequest, size_t requestLength,
                   uint8_t* pResponseBuffer, size_t responseBufferSize, size_t* pResponseLength);

protected:
    void clear();
    bool shouldReleaseSerialBeforeReturning()
    {
        return (m_flags & MIP_FLAG_RELEASE_SERIAL);
    }
    int     isValidHeadLED(MiPHeadLED led);
    int     parseStatus(MiPStatus* pStatus, const uint8_t* pResponse, size_t responseLength);
    int     parseWeight(MiPWeight* pWeight, const uint8_t* pResponse, size_t responseLength);
    int     transportSendRequest(const uint8_t* pRequest, size_t requestLength, int expectResponse);
    int     transportGetResponse(uint8_t* pResponseBuffer, size_t responseBufferSize, size_t* pResponseLength);
    bool    processAllResponseData();
    void    copyHexTextToBinary(uint8_t* pDest, uint8_t* pSrc, uint8_t length);
    uint8_t parseHexDigit(uint8_t digit);
    void    processOobResponseData(uint8_t commandByte);
    uint8_t discardUnexpectedSerialData();

    // Bits that can be set in m_flags bitfield.
    enum FlagBits
    {
        MIP_FLAG_RADAR_VALID     = (1 << 0),
        MIP_FLAG_STATUS_VALID    = (1 << 1),
        MIP_FLAG_GESTURE_VALID   = (1 << 2),
        MIP_FLAG_SHAKE_VALID     = (1 << 3),
        MIP_FLAG_WEIGHT_VALID    = (1 << 4),
        MIP_FLAG_CLAP_VALID      = (1 << 5),
        MIP_FLAG_RELEASE_SERIAL  = (1 << 6),
        MRI_FLAGS_INITIALIZED    = (1 << 7)
    };

    uint32_t                m_lastRequestTime;
    MiPRadarNotification    m_lastRadar;
    MiPGestureNotification  m_lastGesture;
    MiPStatus               m_lastStatus;
    MiPWeight               m_lastWeight;
    MiPClap                 m_lastClap;
    uint8_t                 m_flags;
    int8_t                  m_serialSelectPin;
    uint8_t                 m_responseBuffer[MIP_RESPONSE_MAX_LEN];
    uint8_t                 m_expectedResponseCommand;
    uint8_t                 m_expectedResponseSize;

    static MiP*             s_pInstance;
};

#endif // MIP_H_
