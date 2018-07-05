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
#include "queue.h"

// Default pin used by the TS3USB221A switch to connect Serial to the MiP or PC.
//   Set to HIGH, it selects the MiP.
//   Set to LOW, it selects the PC.
#define MIP_UART_SELECT_PIN 2


// Integer error codes that can be encountered by MiP API functions.
#define MIP_ERROR_NONE          0 // Success
#define MIP_ERROR_TIMEOUT       1 // Timed out waiting for response.
#define MIP_ERROR_NO_EVENT      2 // No event has arrived from MiP yet.
#define MIP_ERROR_BAD_RESPONSE  3 // Unexpected response from MiP.
#define MIP_ERROR_MAX_RETRIES   4 // Exceeded maximum number of retries to get this operation to succeed.

// Maximum length of MiP request and response buffer lengths.
#define MIP_REQUEST_MAX_LEN     (17 + 1)    // Longest request is MPI_CMD_PLAY_SOUND.
#define MIP_RESPONSE_MAX_LEN    (5 + 1)     // Longest response is MPI_CMD_REQUEST_CHEST_LED.

enum MiPGestureRadarMode
{
    MIP_GESTURE_RADAR_DISABLED = 0x00,
    MIP_GESTURE                = 0x02,
    MIP_RADAR                  = 0x04,
};

enum MiPRadar
{
    MIP_RADAR_NONE      = 0x01,
    MIP_RADAR_10CM_30CM = 0x02,
    MIP_RADAR_0CM_10CM  = 0x03,
    MIP_RADAR_INVALID   = 0xFF          // Is set to this value when there are no current radar events.
};

enum MiPGesture
{
    MIP_GESTURE_LEFT               = 0x0A,
    MIP_GESTURE_RIGHT              = 0x0B,
    MIP_GESTURE_CENTER_SWEEP_LEFT  = 0x0C,
    MIP_GESTURE_CENTER_SWEEP_RIGHT = 0x0D,
    MIP_GESTURE_CENTER_HOLD        = 0x0E,
    MIP_GESTURE_FORWARD            = 0x0F,
    MIP_GESTURE_BACKWARD           = 0x10,
    MIP_GESTURE_INVALID            = 0xFF   // Is set to this value when there are no current gesture events.
};

enum MiPHeadLED
{
    MIP_HEAD_LED_OFF        = 0,
    MIP_HEAD_LED_ON         = 1,
    MIP_HEAD_LED_BLINK_SLOW = 2,
    MIP_HEAD_LED_BLINK_FAST = 3,
};

enum MiPDriveDirection
{
    MIP_DRIVE_FORWARD  = 0x00,
    MIP_DRIVE_BACKWARD = 0x01,
};

enum MiPTurnDirection
{
    MIP_TURN_LEFT  = 0x00,
    MIP_TURN_RIGHT = 0x01
};

enum MiPFallDirection
{
    MIP_FALL_ON_BACK   = 0x00,
    MIP_FALL_FACE_DOWN = 0x01
};

enum MiPPosition
{
    MIP_POSITION_ON_BACK                = 0x00,
    MIP_POSITION_FACE_DOWN              = 0x01,
    MIP_POSITION_UPRIGHT                = 0x02,
    MIP_POSITION_PICKED_UP              = 0x03,
    MIP_POSITION_HAND_STAND             = 0x04,
    MIP_POSITION_FACE_DOWN_ON_TRAY      = 0x05,
    MIP_POSITION_ON_BACK_WITH_KICKSTAND = 0x06,
};

enum MiPGetUp
{
    MIP_GETUP_FROM_FRONT  = 0x00,
    MIP_GETUP_FROM_BACK   = 0x01,
    MIP_GETUP_FROM_EITHER = 0x02
};

enum MiPSoundIndex
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
};

enum MiPVolume
{
    MIP_VOLUME_OFF = 0,
    MIP_VOLUME_1   = 1,
    MIP_VOLUME_2   = 2,
    MIP_VOLUME_3   = 3,
    MIP_VOLUME_4   = 4,
    MIP_VOLUME_5   = 5,
    MIP_VOLUME_6   = 6,
    MIP_VOLUME_7   = 7,
    MIP_VOLUME_DEFAULT = 0xFF
};

enum MiPClapEnabled
{
    MIP_CLAP_DISABLED = 0x00,
    MIP_CLAP_ENABLED  = 0x01,
};

enum MiPGameMode
{
    MIP_APP_MODE      = 0x01,
    MIP_CAGE_MODE     = 0x02,
    MIP_TRACKING_MODE = 0x03,
    MIP_DANCE_MODE    = 0x04,
    MIP_DEFAULT_MODE  = 0x05,
    MIP_STACK_MODE    = 0x06,
    MIP_TRICK_MODE    = 0x07,
    MIP_ROAM_MODE     = 0x08
};



class MiPStatus
{
public:
    MiPStatus()
    {
        clear();
    }

    void clear()
    {
        battery = 0.0f;
        position = MIP_POSITION_ON_BACK_WITH_KICKSTAND;
    }

    float       battery;
    MiPPosition position;
};

class MiPChestLED
{
public:
    MiPChestLED()
    {
        clear();
    }

    void clear()
    {
        onTime = 0;
        offTime = 0;
        red = 0;
        green = 0;
        blue = 0;
    }

    uint16_t onTime;
    uint16_t offTime;
    uint8_t  red;
    uint8_t  green;
    uint8_t  blue;
};

class MiPHeadLEDs
{
public:
    MiPHeadLEDs()
    {
        clear();
    }

    void clear()
    {
        led1 = MIP_HEAD_LED_OFF;
        led2 = MIP_HEAD_LED_OFF;
        led3 = MIP_HEAD_LED_OFF;
        led4 = MIP_HEAD_LED_OFF;
    }

    MiPHeadLED led1;
    MiPHeadLED led2;
    MiPHeadLED led3;
    MiPHeadLED led4;
};

class MiPSoftwareVersion
{
public:
    MiPSoftwareVersion()
    {
        clear();
    }

    void clear()
    {
        year = 0;
        month = 0;
        day = 0;
        uniqueVersion = 0;
    }

    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  uniqueVersion;
};

class MiPHardwareInfo
{
public:
    MiPHardwareInfo()
    {
        clear();
    }

    void clear()
    {
        voiceChip = 0;
        hardware = 0;
    }

    uint8_t voiceChip;
    uint8_t hardware;
};

class MiPClapSettings
{
public:
    MiPClapSettings()
    {
        clear();
    }

    void clear()
    {
        enabled = MIP_CLAP_DISABLED;
        delay = 0;
    }

    MiPClapEnabled enabled;
    uint16_t       delay;
};



class MiP
{
public:
    // Constructor/Destructors.
    MiP(int8_t serialSelectPin = MIP_UART_SELECT_PIN);
    ~MiP();

    bool begin();
    void end();
    void sleep();

    // Will return false if begin() wasn't successful in connecting to the MiP.
    bool isInitialized()
    {
        return (m_flags & MRI_FLAG_INITIALIZED);
    }

    // When calling the public functions listed below, the MiP library will try its best to handle any errors
    // encountered by retrying the read/write operations behind the scenes. If the worst happens and it just can't
    // recover from a communication issue with the MiP, it will provide details about the cause of the problem through
    // the following functions.
    int8_t lastCallResult()
    {
        return m_lastError;
    }
    bool didLastCallFail()
    {
        return m_lastError != MIP_ERROR_NONE;
    }
    void printLastCallResult();

    void enableRadarMode();
    void disableRadarMode();
    void enableGestureMode();
    void disableGestureMode();
    bool isRadarModeEnabled();
    bool isGestureModeEnabled();
    bool areGestureAndRadarModesDisabled();
    MiPRadar readRadar();
    uint8_t availableGestureEvents();
    MiPGesture readGestureEvent();

    void writeChestLED(uint8_t red, uint8_t green, uint8_t blue);
    void writeChestLED(uint8_t red, uint8_t green, uint8_t blue, uint16_t onTime, uint16_t offTime);
    void writeChestLED(const MiPChestLED& chestLED);
    void readChestLED(MiPChestLED& chestLED);

    void writeHeadLEDs(MiPHeadLED led1, MiPHeadLED led2, MiPHeadLED led3, MiPHeadLED led4);
    void writeHeadLEDs(const MiPHeadLEDs& headLEDs);
    void readHeadLEDs(MiPHeadLEDs& headLEDs);

    void continuousDrive(int8_t velocity, int8_t turnRate);
    void distanceDrive(MiPDriveDirection driveDirection, uint8_t cm, MiPTurnDirection turnDirection, uint16_t degrees);
    void turnLeft(uint16_t degrees, uint8_t speed);
    void turnRight(uint16_t degrees, uint8_t speed);
    void driveForward(uint8_t speed, uint16_t time);
    void driveBackward(uint8_t speed, uint16_t time);
    void stop();
    void fallForward();
    void fallBackward();
    void getUp(MiPGetUp getup = MIP_GETUP_FROM_EITHER);

    void playSound(MiPSoundIndex sound, MiPVolume volume = MIP_VOLUME_DEFAULT);

    void beginSoundList();
    void addEntryToSoundList(MiPSoundIndex sound, uint16_t delay = 0, MiPVolume volume = MIP_VOLUME_DEFAULT);
    void playSoundList(uint8_t repeatCount = 0);

    void writeVolume(uint8_t volume);
    uint8_t readVolume();

    float readDistanceTravelled();
    void  resetDistanceTravelled();

    float readBatteryVoltage();
    MiPPosition readPosition();
    bool  isOnBack();
    bool  isFaceDown();
    bool  isUpright();
    bool  isPickedUp();
    bool  isHandStanding();
    bool  isFaceDownOnTray();
    bool  isOnBackWithKickstand();

    int8_t readWeight();

    void     enableClapEvents();
    void     disableClapEvents();
    bool     areClapEventsEnabled();
    void     writeClapDelay(uint16_t delay);
    uint16_t readClapDelay();
    uint8_t  availableClapEvents();
    uint8_t  readClapEvent();

    bool hasBeenShaken();

    void readSoftwareVersion(MiPSoftwareVersion& software);
    void readHardwareInfo(MiPHardwareInfo& hardware);

    void enableAppMode();
    void enableCageMode();
    void enableDanceMode();
    void enableStackMode();
    void enableTrickMode();
    void enableRoamMode();
    bool isAppModeEnabled();
    bool isCageModeEnabled();
    bool isDanceModeEnabled();
    bool isStackModeEnabled();
    bool isTrickModeEnabled();
    bool isRoamModeEnabled();

    void   setUserData(uint8_t addressOffset, uint8_t userData);
    uint8_t getUserData(uint8_t addressOffset);

    void   rawSend(const uint8_t request[], size_t requestLength);
    int8_t rawReceive(const uint8_t request[], size_t requestLength,
                      uint8_t responseBuffer[], size_t responseBufferSize, size_t& responseLength);

    // Serial is shared between the MiP and the PC on the MiP ProMini Pack.
    // You shouldn't need to use these function directly as just calling Serial.print() or Serial.println() from your
    // code will automatically end up calling these functions for you as needed.
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

protected:
    void    clear();

    void    verifiedSetGestureRadarMode(MiPGestureRadarMode desiredMode);
    bool    checkGestureRadarMode(MiPGestureRadarMode expectedMode);
    void    rawSetGestureRadarMode(MiPGestureRadarMode mode);
    int8_t  rawGetGestureRadarMode(MiPGestureRadarMode& mode);

    void    rawSetChestLED(uint8_t red, uint8_t green, uint8_t blue);
    void    rawFlashChestLED(uint8_t red, uint8_t green, uint8_t blue, uint16_t onTime, uint16_t offTime);
    int8_t  rawGetChestLED(MiPChestLED& chestLED);

    void    rawSetHeadLEDs(MiPHeadLED led1, MiPHeadLED led2, MiPHeadLED led3, MiPHeadLED led4);
    int8_t  rawGetHeadLEDs(MiPHeadLEDs& headLEDs);
    bool    isValidHeadLED(uint8_t led);

    void    fallDown(MiPFallDirection direction);

    void    rawSetVolume(uint8_t volume);
    int8_t  rawGetVolume(uint8_t& volume);

    int8_t  rawReadOdometer(float& distanceInCm);

    int8_t  rawGetStatus(MiPStatus& status);
    int8_t  parseStatus(MiPStatus& status, const uint8_t response[], size_t responseLength);

    int8_t  rawGetWeight(int8_t& weight);
    int8_t  parseWeight(int8_t& weight, const uint8_t response[], size_t responseLength);

    void    checkedEnableClapEvents(MiPClapEnabled enabled);
    int8_t  readClapSettings(MiPClapSettings& settings);
    void    rawEnableClap(MiPClapEnabled enabled);
    void    rawSetClapDelay(uint16_t delay);
    int8_t  rawGetClapSettings(MiPClapSettings& settings);

    int8_t  rawGetSoftwareVersion(MiPSoftwareVersion& software);
    int8_t  rawGetHardwareInfo(MiPHardwareInfo& hardware);

    void    verifiedSetGameMode(MiPGameMode desiredMode);
    bool    checkGameMode(MiPGameMode expectedMode);
    void    rawSetGameMode(MiPGameMode mode);
    int8_t  rawGetGameMode(MiPGameMode& mode);

    void    rawSetUserData(uint8_t address, uint8_t userData);
    int8_t  rawGetUserData(uint8_t address, uint8_t& userData);

    void    transportSendRequest(const uint8_t* pRequest, size_t requestLength, int expectResponse);
    int8_t  transportGetResponse(uint8_t* pResponseBuffer, size_t responseBufferSize, size_t* pResponseLength);
    bool    processAllResponseData();
    void    copyHexTextToBinary(uint8_t* pDest, uint8_t* pSrc, uint8_t length);
    uint8_t parseHexDigit(uint8_t digit);
    void    processOobResponseData(uint8_t commandByte);
    uint8_t discardUnexpectedSerialData();

    // Bits that can be set in m_flags bitfield.
    enum FlagBits
    {
        MIP_FLAG_RADAR_VALID     = (1 << 0),
        MIP_FLAG_SHAKE_DETECTED  = (1 << 1),
        MIP_FLAG_WEIGHT_VALID    = (1 << 2),
        MRI_FLAG_INITIALIZED     = (1 << 3)
    };

    uint32_t                     m_lastRequestTime;
    uint32_t                     m_lastContinuousDriveTime;
    uint8_t                      m_flags;
    int8_t                       m_serialSelectPin;
    uint8_t                      m_responseBuffer[MIP_RESPONSE_MAX_LEN];
    uint8_t                      m_expectedResponseCommand;
    uint8_t                      m_expectedResponseSize;
    int8_t                       m_lastError;
    uint8_t                      m_playCommand[1+17];
    int8_t                       m_soundIndex;
    uint8_t                      m_playVolume;
    MiPRadar                     m_lastRadar;
    MiPStatus                    m_lastStatus;
    int8_t                       m_lastWeight;
    CircularQueue<uint8_t, 8>    m_clapEvents;
    CircularQueue<MiPGesture, 8> m_gestureEvents;

    static MiP*                  s_pInstance;
};



// This class can be used instead of Serial for sending text to the PC. It makes sure that the MiP ProMini Pack switches
// the UART signals away from the MiP and to the PC before actually performing the Serial write.
class MiPStream : public Stream
{
public:
    MiPStream();

    // Methods that must be implemented for Stream subclasses.
    virtual int available();
    virtual int read();
    virtual int peek();

    // Methods that must be implemented for Print subclasses.
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t *buffer, size_t size);
    virtual int    availableForWrite();
    virtual void   flush();

    // Additional methods defined by HardwareSerial that user might call.
    void begin(unsigned long baud) { begin(baud, SERIAL_8N1); }
    void begin(unsigned long, uint8_t);
    void end();
    inline size_t write(unsigned long n) { return write((uint8_t)n); }
    inline size_t write(long n) { return write((uint8_t)n); }
    inline size_t write(unsigned int n) { return write((uint8_t)n); }
    inline size_t write(int n) { return write((uint8_t)n); }
    operator bool() { return true; }

protected:
    void initIfNeeded();

    bool m_isInit;
} extern MiPStream;

// This macro will force user code which does something like: Serial.println("Interesting text")
// to be redirected to MiPStream which will make sure that it actually gets sent to the PC and not the MiP.
#define Serial MiPStream


#endif // MIP_H_
