# MiP ProMini Pack Arduino Library
![MiP Photo](https://github.com/WowWeeLabs/MiP-BLE-Protocol/blob/master/Images/MiP.png)<br>
The [WowWee MiP](https://wowwee.com/mip) is a hacker friendly self-balancing robot. WowWee not only publish the [MiP Protocol Specification on GitHub](https://github.com/WowWeeLabs/MiP-BLE-Protocol), they also provide a [4-pin hacker port](https://cdn.sparkfun.com/assets/learn_tutorials/2/8/5/HackingPortAnnotated.png), complete with JST connector, right on the mainboard. This connector makes it easy to connect the Arduino compatible [MiP ProMini Pack](https://github.com/adamgreen/MiP_ProMini-Pack) and take control of your MiP. Once connected, you can start:
* Commanding the speed and direction of motion for the gravity defying MiP.
* Commanding the individual control (on, off, blink) of the 4 LED eye segments on the head.
* Taking full control of the RGB LED in the MiP's chest.
* Commanding the playback of sound lists using the >100 built-in sounds.
* Using the head mounted IR sensors to read 'radar' distance measurements or detected user hand gestures.
* Detecting user claps with the built-in microphone.
* Detecting the MiP's current pose via its inertial sensors, the same sensors that make its balancing magic possible.
* And More!

The MiP ProMini Pack was [originally designed by Sparkfun](https://www.sparkfun.com/products/retired/13058) but has since been retired. [This retired Sparkfun tutorial](https://learn.sparkfun.com/tutorials/hacking-the-mip---promini-pack) is still the best place to learn about opening up your MiP and installing the MiP ProMini Pack. Once installed, this is what your MiP will look like:<br>
<img src="https://raw.githubusercontent.com/adamgreen/MiP_ProMini-Pack/master/images/20180416-02.jpg" alt="MiP with ProMini Pack on its back" width="240" height="320" border="10" /><br>
Once you have it mounted, attach your [FTDI cable](https://www.sparkfun.com/products/9718), [install the MiP ProMini Pack library in the Arduino IDE](https://learn.sparkfun.com/tutorials/installing-an-arduino-library), start hacking and have fun!

## Limitations
One of the cool features that Sparkfun built into the original MiP ProMini Pack is the ability to use the hardware Serial port for communicating with both the MiP and the Arduino IDE's serial monitor. It accomplishes this great feat by using a switch that can direct the Serial traffic to the appropriate target based on the setting of pin 2. Once you call begin() on the MiP object, the Serial traffic will be directed to the MiP except when your code issues print commands such as ```Serial.println("Hello World");```. The library will detect such print calls and automatically direct the resulting Serial traffic to the Arduino IDE and then switch it back to the MiP once the data has been received by the Arduino IDE.

What limitations does this put on your usage of ```Serial.println()``` and ```Serial.print()``` calls?
* Don't use other libraries which send information to the Arduino IDE. Their data will end up being sent to the MiP instead. Only code which ```#include <mip.h>``` will automatically have their print output redirected to the Arduino IDE.
* There are some functions in the library that rely on receiving updates/events that the MiP sends to your Arduino while you are running your awesome code. These include:
  * Radar measurement updates
  * Hand gesture events
  * Battery level updates
  * MiP pose updates (standing upright, laying face down, etc)
  * Clap events
  * Weight updates
  * Shake events
* If you are sending a lot of data to the Arduino IDE in between MiP function calls, you might cause these updates/events to be lost.
* There is a LED labelled **MiP UART** towards the lower right corner of the MiP ProMini Pack. This will be lit when Serial is switched to exchange data with the MiP.
* If you see that it is off most of the time, then you are likely to miss these updates/events.
* If you find your code isn't receiving expected updates/events then double check that you aren't sending too much data to the Arduino IDE.
* It is probably best to not attempt receiving data sent from the Arduino IDE. Calling functions like ```Serial.read()``` from your code is likely to read MiP updates and not Arduino IDE data from the Serial receive buffer.

## Examples
* [ChestLED](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/ChestLED/ChestLED.ino): Take control of the RGB LED in the chest of the MiP.
* [Clap](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/Clap/Clap.ino): Send descriptive messages to the Arduino IDE about each clap event deteced by the MiP robot.
* [ContinuousDrive](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/ContinuousDrive/ContinuousDrive.ino): You want to control the motion of the MiP in real time? This is the example for you.
* [DisconnectApp](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/DisconnectApp/DisconnectApp.ino): Disconnect all apps, including this sketch and any app that may be connected to Bluetooth.
* [DistanceDrive](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/DistanceDrive/DistanceDrive.ino): Tell the MiP robot exactly how far to travel and forget about it.
* [DriveForwardBackward](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/DriveForwardBackward/DriveForwardBackward.ino): Tell the MiP robot how long to drive forward/backward and forget about it.
* [EnableGameMode](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/EnableGameMode/EnableGameMode.ino): Cycle through each of the game modes available.
* [FallDown](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/FallDown/FallDown.ino): Tired of standing around? Command MiP to fall flat on his face.
* [Gesture](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/Gesture/Gesture.ino): Use your hand to make gesture to your MiP robot. Send descriptive messages to the Arduio IDE about each gesture event detected.
* [GestureRadarMode](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/GestureRadarMode/GestureRadarMode.ino): Want to learn more about how to enable/disable IR based gesture and radar measurements? Check out this example.
* [GetUp](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/GetUp/GetUp.ino): If your MiP robot falls down, learn how to have him try getting back up on his ~~feet~~wheels.
* [HeadLEDs](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/HeadLEDs/HeadLEDs.ino): Take control of the 4 individual eye LEDs on the MiP robot's head.
* [Odometer](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/Odometer/Odometer.ino): How far has your MiP robot been traveling around your personal robot laboratory? This example shows you how to find out and reset its measurement.
* [PlaySound](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/PlaySound/PlaySound.ino): Learn how to get the MiP robot vocalizing under your control!
* [Radar](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/Radar/Radar.ino): Is there anything in front of your MiP robot? This example sends descriptive text to the Arduino IDE when it detects changes in the obstacles around it.
* [RawSendReceive](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/RawSendReceive/RawSendReceive.ino): You found a command in WowWee's Protocol Specification that isn't supported by this library? This example shows you how to experiment with these new commands.
* [ReadWriteEeprom](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/ReadWriteEeprom/ReadWriteEeprom.ino): Read and write your own data to MiP's EEPROM. This is useful for storing data across power cycles. See also [ZeroEeprom](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/ZeroEeprom/ZeroEeprom.ino).
* [SRSdemo](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/SRSdemo/SRSdemo.ino): The MiP robot made an appearance at the [Seattle Robotics Society](http://www.seattlerobotics.org) meeting on April 21st, 2018. This is what he was running!
* [Shake](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/Shake/Shake.ino): Is someone shaking your poor little MiP robot? This example shows you how to detect such rudeness and report it to the Arduino IDE.
* [SoftwareHardwareVersion](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/SoftwareHardwareVersion/SoftwareHardwareVersion.ino): This example shows you how to peek under the covers and see what hardware / software is running inside your MiP robot.
* [Status](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/Status/Status.ino): Detect changes in the MiP robot's battery level or pose and report them to the Arduino IDE.
* [Stop](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/Stop/Stop.ino): Danger! Danger! MiP Robot! Stop your MiP robot in its tracks before it gets itself into more trouble.
* [TurnLeftRight](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/TurnLeftRight/TurnLeftRight.ino): Tell the MiP robot exactly how many degrees to turn and forget about it.
* [Volume](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/Volume/Volume.ino): Your MiP robot is too loud? Turn down the volume with this example.
* [Weight](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/Weight/Weight.ino): Detect weight changes in what the MiP is carrying and report them to the Arduino IDE.
* [ZeroEeprom](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/ZeroEeprom/ZeroEeprom.ino): Write zeroes to each available byte in EEPROM. See also [ReadWriteEeprom](https://github.com/adamgreen/MiP_ProMini-Pack/blob/master/Arduino/MiP_ProMini_Pack_Library/examples/ReadWriteEeprom/ReadWriteEeprom.ino).

## Functions
Group | Function
----- | --------
Initialization  | [MiP()](#mip)
<br>            | [begin()](#begin)
<br>            | [end()](#end)
<br>            | [sleep()](#sleep)
<br>            | [isInitialized()](#isinitialized)
Radar           | [enableRadarMode()](#enableradarmode)
<br>            | [disableRadarMode()](#disableradarmode)
<br>            | [isRadarModeEnabled()](#isradarmodeenabled)
<br>            | [areGestureAndRadarModesDisabled()](#aregestureandradarmodesdisabled)
<br>            | [readRadar()](#readradar)
Gesture         | [enableGestureMode()](#enablegesturemode)
<br>            | [disableGestureMode()](#disablegesturemode)
<br>            | [isGestureModeEnabled()](#isgesturemodeenabled)
<br>            | [areGestureAndRadarModesDisabled()](#aregestureandradarmodesdisabled)
<br>            | [availableGestureEvents()](#availablegestureevents)
<br>            | [readGestureEvent()](#readgestureevent)
Chest LED       | [writeChestLED()](#writechestled)
<br>            | [readChestLED()](#readchestled)
Head LEDs       | [writeHeadLEDs()](#writeheadleds)
<br>            | [readHeadLEDs()](#readheadleds)
Motion          | [continuousDrive()](#continuousdrive)
<br>            | [distanceDrive()](#distancedrive)
<br>            | [turnLeft()](#turnleft)
<br>            | [turnRight()](#turnright)
<br>            | [driveForward()](#driveforward)
<br>            | [driveBackward()](#drivebackward)
<br>            | [stop()](#stop)
<br>            | [fallForward()](#fallforward)
<br>            | [fallBackward()](#fallbackward)
<br>            | [getUp()](#getup)
Sound           | [playSound()](#playsound)
<br>            | [beginSoundList()](#beginsoundlist)
<br>            | [addEntryToSoundList()](#addentrytosoundlist)
<br>            | [playSoundList()](#playsoundlist)
<br>            | [writeVolume()](#writevolume)
<br>            | [readVolume()](#readvolume)
Odometer        | [readDistanceTravelled()](#readdistancetravelled)
<br>            | [resetDistanceTravelled()](#resetdistancetravelled)
Battery Level   | [readBatteryVoltage()](#readbatteryvoltage)
Position        | [readPosition()](#readposition)
<br>            | [isOnBack()](#isonback)
<br>            | [isFaceDown()](#isfacedown)
<br>            | [isUpright()](#isupright)
<br>            | [isPickedUp()](#ispickedup)
<br>            | [isHandStanding()](#ishandstanding)
<br>            | [isFaceDownOnTray()](#isfacedownontray)
<br>            | [isOnBackWithKickstand()](#isonbackwithkickstand)
Weight          | [readWeight()](#readweight)
Clap            | [enableClapEvents()](#enableclapevents)
<br>            | [disableClapEvents()](#disableclapevents)
<br>            | [areClapEventsEnabled()](#areclapeventsenabled)
<br>            | [writeClapDelay()](#writeclapdelay)
<br>            | [readClapDelay()](#readclapdelay)
<br>            | [availableClapEvents()](#availableclapevents)
<br>            | [readClapEvent()](#readclapevent)
Shake           | [hasBeenShaken()](#hasbeenshaken)
Version Info    | [readSoftwareVersion()](#readsoftwareversion)
<br>            | [readHardwareInfo()](#readhardwareinfo)
Game Modes      | [enableAppMode()](#enableappmode)
<br>            | [enableCageMode()](#enablecagemode)
<br>            | [enableDanceMode()](#enabledancemode)
<br>            | [enableStackMode()](#enablestackmode)
<br>            | [enableTrickMode()](#enabletrickmode)
<br>            | [enableRoamMode()](#enableroammode)
<br>            | [isAppModeEnabled()](#isappmodeenabled)
<br>            | [isCageModeEnabled()](#iscagemodeenabled)
<br>            | [isDanceModeEnabled()](#isdancemodeenabled)
<br>            | [isStackModeEnabled()](#isstackmodeenabled)
<br>            | [isTrickModeEnabled()](#istrickmodeenabled)
<br>            | [isRoamModeEnabled()](#isroammodeenabled)
EEPROM          | [setUserData()](#setUserData)
<br>            | [getUserData()](#getUserData)


---
### MiP()
```MiP(int8_t serialSelectPin = MIP_UART_SELECT_PIN)```
#### Description
This MiP constructor is used to create an instance of a MiP object, whose name you need to provide as in the example below. Only one MiP object should be created in your sketch.

You still need to call [begin()](#begin) to initialize communication with the MiP before calling any other MiP functions.

#### Parameters
* **serialSelectPin** is the pin used to switch the Serial output between the MiP and Arduino IDE. It defaults to MIP_UART_SELECT_PIN, pin 2. The MiP ProMini Pack hardware is designed to use this pin as well. Switching serialSelectPin to another pin would therefore require that you make hardware changes to the MiP ProMini Pack as well.

#### Returns
Nothing

#### Example
```c++
// Include the MiP library so that you can use its functions.
#include <mip.h>

// Setup new MiP object.
MiP     mip;
```


---
### begin()
```bool MiP::begin()```
#### Description
Initializes the MiP robot to be controlled by the Arduino instead of Bluetooth Low Energy. The LED on the chest of the MiP robot will switch from blue to green once this switch has been made. The MiP robot requires a power reset to switch out of serial mode and accept Bluetooth Low Energy connections again.

Calling another MiP function before begin() has successfully been called will typically generate an error message and your program will be halted so that you can diagnose and correct the problem.

[isInitialized()](#isinitialized) can be called to determine if begin() has been successfully called or not.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully initialized.
* **false** if begin() failed to properly initialize the MiP robot.

#### Example
```c++
// Include the MiP library so that you can use its functions.
#include <mip.h>

// Setup new MiP object.
MiP     mip;

void setup()
{
    // Call begin() to initialize the MiP robot.
    // Returns false if it fails to initialize the MiP robot.
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    // ...
}

void loop()
{
    // ...
}
```


---
### end()
```void end()```
#### Description
Disconnects from the MiP, disables serial communication, and allows the Serial Rx/Tx pins and serialSelectPin to be used for general input/output.

[isInitialized()](#isinitialized) will return false after end() has been called.

#### Parameters
None

#### Returns
None

#### Example
```c++
    mip.end();
```


---
### sleep()
```void sleep()```
#### Description
Puts the MiP to sleep. The MiP robot will need to be shut off and back on to wake it from this sleep. Calling [begin()](#begin) will not wake it.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.begin(115200);
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Sleep.ino - Shows begin()/end()/sleep() functionality."));
    Serial.println(F("Chest LED should be green to indicate UART connection."));

    delay(5000);
    Serial.println(F("Disconnecting from MiP. Chest LED should revert to blue."));
    mip.end();

    delay(5000);
    Serial.println(F("Attempting to reconnect to MiP. Chest LED should turn green again."));
    connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed reconnecting to MiP!"));
        return;
    }

    delay(5000);
    Serial.println(F("Putting MiP to sleep. Will require power cycle before it will accept UART connections again."));
    mip.sleep();
}

void loop()
{
}
```


---
### isInitialized()
```bool isInitialized()```
#### Description
Returns whether [begin()](#begin) has been called successfully yet or not.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully initialized.<br>
* **false** if begin() failed to properly initialize the MiP robot or [end()](#end) has been called since the successful begin().

#### Example
```c++
    if (!mip.isInitialized())
    {
        // Handle the fact that the MiP robot hasn't been initialized yet.
    }
```


---
### enableRadarMode()
```void enableRadarMode()```
#### Description
Switches the MiP robot's head mounted IR sensors into radar mode. Once this mode is enabled, your code can call [readRadar()](#readradar) to read the radar measurements.

#### Parameters
None

#### Returns
None

#### Notes
* When enableRadarMode() is called, gesture mode will be disabled. If you later enable gesture mode, radar mode will be disabled.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Radar.ino - Display current radar readings to user."));

    Serial.println(F("Waiting for robot to be standing upright."));
    while (!mip.isUpright())
    {
    }
    mip.enableRadarMode();
}

void loop()
{
    static MiPRadar lastRadar = MIP_RADAR_INVALID;
    MiPRadar        currentRadar = mip.readRadar();

    if (currentRadar != MIP_RADAR_INVALID && lastRadar != currentRadar)
    {
        Serial.print(F("Radar = "));
        switch(currentRadar)
        {
        case MIP_RADAR_NONE:
            Serial.println(F("None"));
            break;
        case MIP_RADAR_10CM_30CM:
            Serial.println(F("10cm - 30cm"));
            break;
        case MIP_RADAR_0CM_10CM:
            Serial.println(F("0cm - 10cm"));
            break;
        default:
            break;
        }
        lastRadar = currentRadar;
    }
}
```


---
### disableRadarMode()
```void disableRadarMode()```
#### Description
Switches the MiP robot's head mounted IR sensors out of radar mode.

#### Parameters
None

#### Returns
None

#### Notes
* Calling disableRadarMode() actually disables all IR sensing so it will also shutdown gesture mode as well.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("GestureRadarMode.ino - Switches between gesture, radar, and default modes."));

    Serial.println(F("Calling mip.enableRadarMode()"));
    mip.enableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableRadarMode()"));
    mip.disableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }

    Serial.println(F("Calling mip.enableGestureMode()"));
    mip.enableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableGestureMode()"));
    mip.disableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }
    Serial.print(F("mip.areGestureAndRadarModesDisabled() = "));
    if (mip.areGestureAndRadarModesDisabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### isRadarModeEnabled()
```bool isRadarModeEnabled()```
#### Description
Returns whether the MiP robot's head mounted IR sensors are in radar mode.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully placed in radar mode with a previous call to [enableRadarMode()](#enableradarmode).
* **false** if it is not in radar mode.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("GestureRadarMode.ino - Switches between gesture, radar, and default modes."));

    Serial.println(F("Calling mip.enableRadarMode()"));
    mip.enableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableRadarMode()"));
    mip.disableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }

    Serial.println(F("Calling mip.enableGestureMode()"));
    mip.enableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableGestureMode()"));
    mip.disableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }
    Serial.print(F("mip.areGestureAndRadarModesDisabled() = "));
    if (mip.areGestureAndRadarModesDisabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### areGestureAndRadarModesDisabled()
```bool areGestureAndRadarModesDisabled()```
#### Description
Returns whether the MiP robot's head mounted IR sensors are inactive, not performing radar measurements or gesture detection.

#### Parameters
None

#### Returns
* **true** if the MiP robot is not in gesture detection or radar measurement mode.
* **false** if it is in gesture detection or radar measurement mode.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("GestureRadarMode.ino - Switches between gesture, radar, and default modes."));

    Serial.println(F("Calling mip.enableRadarMode()"));
    mip.enableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableRadarMode()"));
    mip.disableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }

    Serial.println(F("Calling mip.enableGestureMode()"));
    mip.enableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableGestureMode()"));
    mip.disableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }
    Serial.print(F("mip.areGestureAndRadarModesDisabled() = "));
    if (mip.areGestureAndRadarModesDisabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### readRadar()
```MiPRadar readRadar()```
#### Description
Reads the current radar measurement from the MiP robot's head mounted IR sensors. The MiP robot must have already been placed in radar mode via a call to [enableRadarMode()](#enableradarmode).

#### Parameters
None

#### Returns
* **MIP_RADAR_NONE** if no object has been detected in front of the MiP's head.
* **MIP_RADAR_10CM_30CM** if an object has been detected between 10cm and 30cm in front of the MiP's head.
* **MIP_RADAR_0CM_10CM** if an object has been detected at less than 10cm in front of the MiP's head.
* **MIP_RADAR_INVALID** if the MiP robot hasn't sent back a radar measurement. This could happen for a few reasons:
  * You just called enableRadarMode() and the MiP robot hasn't had a chance to make its first reading yet.
  * The MiP robot isn't currently in radar mode. Call enableRadarMode() to let it know that it should start taking these measurements.
  * You are spending so much time sending output back to the serial monitor in the Arduino IDE that the MiP's radar measurements are getting dropped. See [Limitations](#limitations) section above.

#### Notes
* These radar measurements will be suppressed while the MiP is executing long running commands. Shorter commands like mipContinuousDrive() do allow the MiP to make the measurements.
* These radar measurements will only be sent when the MiP is actively balancing. They will be suppressed if the MiP is laying back on its kickstand for example.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Radar.ino - Display current radar readings to user."));

    Serial.println(F("Waiting for robot to be standing upright."));
    while (!mip.isUpright())
    {
    }
    mip.enableRadarMode();
}

void loop()
{
    static MiPRadar lastRadar = MIP_RADAR_INVALID;
    MiPRadar        currentRadar = mip.readRadar();

    if (currentRadar != MIP_RADAR_INVALID && lastRadar != currentRadar)
    {
        Serial.print(F("Radar = "));
        switch(currentRadar)
        {
        case MIP_RADAR_NONE:
            Serial.println(F("None"));
            break;
        case MIP_RADAR_10CM_30CM:
            Serial.println(F("10cm - 30cm"));
            break;
        case MIP_RADAR_0CM_10CM:
            Serial.println(F("0cm - 10cm"));
            break;
        default:
            break;
        }
        lastRadar = currentRadar;
    }
}
```


---
### enableGestureMode()
```void enableGestureMode()```
#### Description
Switches the MiP robot's head mounted IR sensors into gesture detection mode. Once this mode is enabled, you can call [availableGestureEvents()](#availablegestureevents) and [readGestureEvent()](#readgestureevent) to read gesture events as the MiP detects them.

#### Parameters
None

#### Returns
Nothing

#### Notes
* When enableGestureMode() is called, radar mode will be disabled. If you later enable radar mode, gesture mode will be disabled.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Gesture.ino - Detect gesture and inform user as they occur."));

    Serial.println(F("Waiting for robot to be standing upright."));
    while (!mip.isUpright())
    {
    }
    mip.enableGestureMode();
}

void loop()
{
    while (mip.availableGestureEvents() > 0)
    {
        MiPGesture gesture = mip.readGestureEvent();
        Serial.print(F("Detected "));
        switch (gesture)
        {
        case MIP_GESTURE_LEFT:
            Serial.println(F("Left gesture!"));
            break;
        case MIP_GESTURE_RIGHT:
            Serial.println(F("Right gesture!"));
            break;
        case MIP_GESTURE_CENTER_SWEEP_LEFT:
            Serial.println(F("Center Sweep Left gesture!"));
            break;
        case MIP_GESTURE_CENTER_SWEEP_RIGHT:
            Serial.println(F("Center Sweep Right gesture!"));
            break;
        case MIP_GESTURE_CENTER_HOLD:
            Serial.println(F("Center Hold gesture!"));
            break;
        case MIP_GESTURE_FORWARD:
            Serial.println(F("Forward gesture!"));
            break;
        case MIP_GESTURE_BACKWARD:
            Serial.println(F("Backward gesture!"));
            break;
        case MIP_GESTURE_INVALID:
            // This shouldn't really happen since mip.availableGestureEvents() returned > 0.
            Serial.println(F("INVALID gesture!"));
            break;
        }
    }
}
```


---
### disableGestureMode()
```void disableGestureMode()```
#### Description
Switches the MiP robot's head mounted IR sensors out of gesture detection mode.

#### Parameters
None

#### Returns
Nothing

#### Notes
* Calling disableGestureMode() actually disables all IR sensing so it will also shutdown radar mode as well.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("GestureRadarMode.ino - Switches between gesture, radar, and default modes."));

    Serial.println(F("Calling mip.enableRadarMode()"));
    mip.enableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableRadarMode()"));
    mip.disableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }

    Serial.println(F("Calling mip.enableGestureMode()"));
    mip.enableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableGestureMode()"));
    mip.disableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }
    Serial.print(F("mip.areGestureAndRadarModesDisabled() = "));
    if (mip.areGestureAndRadarModesDisabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### isGestureModeEnabled()
```bool isGestureModeEnabled()```
#### Description
Returns whether the MiP robot's head mounted IR sensors are in gesture detection mode.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully placed in gesture detection mode with a previous call to [enableGestureMode()](#enablegesturemode).
* **false** if it is not in gesture detection mode.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("GestureRadarMode.ino - Switches between gesture, radar, and default modes."));

    Serial.println(F("Calling mip.enableRadarMode()"));
    mip.enableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableRadarMode()"));
    mip.disableRadarMode();
    Serial.print(F("mip.isRadarModeEnabled() = "));
    if (mip.isRadarModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }

    Serial.println(F("Calling mip.enableGestureMode()"));
    mip.enableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println(F("Calling mip.disableGestureMode()"));
    mip.disableGestureMode();
    Serial.print(F("mip.isGestureModeEnabled() = "));
    if (mip.isGestureModeEnabled())
    {
        Serial.println(F("true - Failed"));
    }
    else
    {
        Serial.println(F("false - Pass"));
    }
    Serial.print(F("mip.areGestureAndRadarModesDisabled() = "));
    if (mip.areGestureAndRadarModesDisabled())
    {
        Serial.println(F("true - Pass"));
    }
    else
    {
        Serial.println(F("false - Failed"));
    }

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### availableGestureEvents()
```uint8_t availableGestureEvents()```
#### Description
Returns the number of gesture detection events that the library currently has sitting in its queue, ready to be read by calling [readGestureEvent()](#readgestureevent). The MiP robot must have already been placed in gesture detection mode via a call to [enableGestureMode()](#enablegesturemode) for new gesture detection events to be added to this queue.

#### Parameters
None

#### Returns
* **0** if there are currently no gesture detection events ready for reading. Calling [readGestureEvent()](#readgestureevent) now would retun MIP_GESTURE_INVALID.
* **Non-zero** value indicates the number of readGestureEvent() calls that can be made and successfully return a valid gesture detection event.

#### Notes
* These gesture detection events will be suppressed while the MiP is executing long running commands. Shorter commands like mipContinuousDrive() do allow the MiP to make the gesture detections.
* These gesture detection events will only be sent when the MiP is actively balancing. They will be suppressed if the MiP is laying back on its kickstand for example.
* The maximum number of gesture detection events that can be queued up between calls to readGestureEvent() is 8. If this count is exceeded, the oldest events will be overwritten.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Gesture.ino - Detect gesture and inform user as they occur."));

    Serial.println(F("Waiting for robot to be standing upright."));
    while (!mip.isUpright())
    {
    }
    mip.enableGestureMode();
}

void loop()
{
    while (mip.availableGestureEvents() > 0)
    {
        MiPGesture gesture = mip.readGestureEvent();
        Serial.print(F("Detected "));
        switch (gesture)
        {
        case MIP_GESTURE_LEFT:
            Serial.println(F("Left gesture!"));
            break;
        case MIP_GESTURE_RIGHT:
            Serial.println(F("Right gesture!"));
            break;
        case MIP_GESTURE_CENTER_SWEEP_LEFT:
            Serial.println(F("Center Sweep Left gesture!"));
            break;
        case MIP_GESTURE_CENTER_SWEEP_RIGHT:
            Serial.println(F("Center Sweep Right gesture!"));
            break;
        case MIP_GESTURE_CENTER_HOLD:
            Serial.println(F("Center Hold gesture!"));
            break;
        case MIP_GESTURE_FORWARD:
            Serial.println(F("Forward gesture!"));
            break;
        case MIP_GESTURE_BACKWARD:
            Serial.println(F("Backward gesture!"));
            break;
        case MIP_GESTURE_INVALID:
            // This shouldn't really happen since mip.availableGestureEvents() returned > 0.
            Serial.println(F("INVALID gesture!"));
            break;
        }
    }
}
```


---
### readGestureEvent()
```MiPGesture readGestureEvent()```
#### Description
Returns a gesture detection event from the library's queue. They will be returned in the order that the MiP robot detected them. The MiP robot must have already been placed in gesture detection mode via a call to [enableGestureMode()](#enablegesturemode) for new gesture detection events to be added to this queue.

#### Parameters
None

#### Returns
* **MIP_GESTURE_INVALID** if the gesture detection event queue is empty. [availableGestureEvents()](availablegestureevents) would return 0 in this scenario.

| Other Valid Return Values       |
| ------------------------------- |
| MIP_GESTURE_LEFT                |
| MIP_GESTURE_RIGHT               |
| MIP_GESTURE_CENTER_SWEEP_LEFT   |
| MIP_GESTURE_CENTER_SWEEP_RIGHT  |
| MIP_GESTURE_CENTER_HOLD         |
| MIP_GESTURE_FORWARD             |
| MIP_GESTURE_BACKWARD            |


#### Notes
* These gesture detection events will be suppressed while the MiP is executing long running commands. Shorter commands like mipContinuousDrive() do allow the MiP to make the gesture detections.
* These gesture detection events will only be sent when the MiP is actively balancing. They will be suppressed if the MiP is laying back on its kickstand for example.
* The maximum number of gesture detection events that can be queued up between calls to readGestureEvent() is 8. If this count is exceeded, the oldest events will be overwritten.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Gesture.ino - Detect gesture and inform user as they occur."));

    Serial.println(F("Waiting for robot to be standing upright."));
    while (!mip.isUpright())
    {
    }
    mip.enableGestureMode();
}

void loop()
{
    while (mip.availableGestureEvents() > 0)
    {
        MiPGesture gesture = mip.readGestureEvent();
        Serial.print(F("Detected "));
        switch (gesture)
        {
        case MIP_GESTURE_LEFT:
            Serial.println(F("Left gesture!"));
            break;
        case MIP_GESTURE_RIGHT:
            Serial.println(F("Right gesture!"));
            break;
        case MIP_GESTURE_CENTER_SWEEP_LEFT:
            Serial.println(F("Center Sweep Left gesture!"));
            break;
        case MIP_GESTURE_CENTER_SWEEP_RIGHT:
            Serial.println(F("Center Sweep Right gesture!"));
            break;
        case MIP_GESTURE_CENTER_HOLD:
            Serial.println(F("Center Hold gesture!"));
            break;
        case MIP_GESTURE_FORWARD:
            Serial.println(F("Forward gesture!"));
            break;
        case MIP_GESTURE_BACKWARD:
            Serial.println(F("Backward gesture!"));
            break;
        case MIP_GESTURE_INVALID:
            // This shouldn't really happen since mip.availableGestureEvents() returned > 0.
            Serial.println(F("INVALID gesture!"));
            break;
        }
    }
}
```


---
### writeChestLED()
```void writeChestLED(uint8_t red, uint8_t green, uint8_t blue)```<br>
```void writeChestLED(uint8_t red, uint8_t green, uint8_t blue, uint16_t onTime, uint16_t offTime)```<br>
```void writeChestLED(const MiPChestLED& chestLED)```
#### Description
Sets the colour and flashing period of the RGB LED in the MiP robot's chest.

#### Parameters
* **red** is the intensity of the red colour channel (0 - 255).
* **green** is the intensity of the green colour channel (0 - 255).
* **blue** is the intensity of the blue colour channel (0 - 255). The MiP robot actually clears the lower 2 bits of the blue colour channel to treat it as a 6-bit value.
* **onTime** is the time in milliseconds that the LED should remain on with the specified colour. This parameter can be set to a value between 0 and 5100 milliseconds.
* **offTime** is the time in milliseconds that the LED should remain off before turning on again with the specified colour. This parameter can be set to a value between 0 and 5100 milliseconds.
* **chestLED** is an object which contains red, green, blue, onTime, and offTime fields for setting the LED colour and flashing period.
```c++
class MiPChestLED
{
    // ...
    uint16_t onTime;
    uint16_t offTime;
    uint8_t  red;
    uint8_t  green;
    uint8_t  blue;
};
```

#### Returns
Nothing

#### Notes
* The MiP only supports a granularity of 20 milliseconds for the onTime and offTime parameters.
* The ```void writeChestLED(uint8_t red, uint8_t green, uint8_t blue)``` version can only set the LED to a solid colour with no flashing.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("ChestLED.ino - Set Chest LED to different colors.\n"));

    Serial.println(F("Set chest LED to magenta."));
    uint8_t red = 0xff;
    uint8_t green = 0x01;
    uint8_t blue = 0xfe;
    mip.writeChestLED(red, green, blue);
    printCurrentChestLEDSetting();
    delay(1000);

    Serial.println(F("Set chest LED to blink red."));
    red = 0xff;
    green = 0x01;
    blue = 0x05;
    const uint16_t onTime = 990;
    const uint16_t offTime = 989;
    mip.writeChestLED(red, green, blue, onTime, offTime);
    printCurrentChestLEDSetting();
    delay(4000);

    Serial.println(F("Set chest LED back to green."));
    MiPChestLED chestLED;
    chestLED.red = 0x00;
    chestLED.green = 0xff;
    chestLED.blue = 0x00;
    chestLED.onTime = 0;
    chestLED.offTime = 0;
    mip.writeChestLED(chestLED);
    printCurrentChestLEDSetting();
    delay(1000);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}

static void printCurrentChestLEDSetting()
{
    MiPChestLED chestLED;
    mip.readChestLED(chestLED);

    Serial.println(F("Current Chest LED Setting"));
    Serial.print(F("    red: "));
        Serial.println(chestLED.red);
    Serial.print(F("    green: "));
        Serial.println(chestLED.green);
    Serial.print(F("    blue: "));
        Serial.println(chestLED.blue);
    Serial.print(F("    on time: "));
        Serial.print(chestLED.onTime);
        Serial.println(F(" milliseconds"));
    Serial.print(F("    off time: "));
        Serial.print(chestLED.offTime);
        Serial.println(F(" milliseconds"));
    Serial.println();
}
```


---
### readChestLED()
```void readChestLED(MiPChestLED& chestLED)```
#### Description
Reads the current setting for the colour and flashing period of the RGB LED in the MiP robot's chest.

#### Parameters
* **chestLED** is an object which contains red, green, blue, onTime, and offTime fields to be filled in with the current LED colour and flashing period.
```c++
class MiPChestLED
{
    // ...
    uint16_t onTime;
    uint16_t offTime;
    uint8_t  red;
    uint8_t  green;
    uint8_t  blue;
};
```

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("ChestLED.ino - Set Chest LED to different colors.\n"));

    Serial.println(F("Set chest LED to magenta."));
    uint8_t red = 0xff;
    uint8_t green = 0x01;
    uint8_t blue = 0xfe;
    mip.writeChestLED(red, green, blue);
    printCurrentChestLEDSetting();
    delay(1000);

    Serial.println(F("Set chest LED to blink red."));
    red = 0xff;
    green = 0x01;
    blue = 0x05;
    const uint16_t onTime = 990;
    const uint16_t offTime = 989;
    mip.writeChestLED(red, green, blue, onTime, offTime);
    printCurrentChestLEDSetting();
    delay(4000);

    Serial.println(F("Set chest LED back to green."));
    MiPChestLED chestLED;
    chestLED.red = 0x00;
    chestLED.green = 0xff;
    chestLED.blue = 0x00;
    chestLED.onTime = 0;
    chestLED.offTime = 0;
    mip.writeChestLED(chestLED);
    printCurrentChestLEDSetting();
    delay(1000);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}

static void printCurrentChestLEDSetting()
{
    MiPChestLED chestLED;
    mip.readChestLED(chestLED);

    Serial.println(F("Current Chest LED Setting"));
    Serial.print(F("    red: "));
        Serial.println(chestLED.red);
    Serial.print(F("    green: "));
        Serial.println(chestLED.green);
    Serial.print(F("    blue: "));
        Serial.println(chestLED.blue);
    Serial.print(F("    on time: "));
        Serial.print(chestLED.onTime);
        Serial.println(F(" milliseconds"));
    Serial.print(F("    off time: "));
        Serial.print(chestLED.offTime);
        Serial.println(F(" milliseconds"));
    Serial.println();
}
```


---
### writeHeadLEDs()
```void writeHeadLEDs(MiPHeadLED led1, MiPHeadLED led2, MiPHeadLED led3, MiPHeadLED led4)```<br>
```void writeHeadLEDs(const MiPHeadLEDs& headLEDs)```<br>
#### Description
Sets the state of the four eye LEDs on the MiP robot's head.

#### Parameters
* **led1** sets the state of led1 on the MiP robot's head. Valid states are **MIP_HEAD_LED_OFF**, **MIP_HEAD_LED_ON**, **MIP_HEAD_LED_BLINK_SLOW**, **MIP_HEAD_LED_BLINK_FAST**.
* **led2** sets the state of led2 on the MiP robot's head. Valid states are **MIP_HEAD_LED_OFF**, **MIP_HEAD_LED_ON**, **MIP_HEAD_LED_BLINK_SLOW**, **MIP_HEAD_LED_BLINK_FAST**.
* **led3** sets the state of led3 on the MiP robot's head. Valid states are **MIP_HEAD_LED_OFF**, **MIP_HEAD_LED_ON**, **MIP_HEAD_LED_BLINK_SLOW**, **MIP_HEAD_LED_BLINK_FAST**.
* **led4** sets the state of led4 on the MiP robot's head. Valid states are **MIP_HEAD_LED_OFF**, **MIP_HEAD_LED_ON**, **MIP_HEAD_LED_BLINK_SLOW**, **MIP_HEAD_LED_BLINK_FAST**.
* **headLEDs** is an object which contains led1, led2, led3, and led4 fields for setting the state of all 4 LEDs.
```c++
class MiPHeadLEDs
{
public:
    // ...
    MiPHeadLED led1;
    MiPHeadLED led2;
    MiPHeadLED led3;
    MiPHeadLED led4;
};
```

#### Returns
Nothing

#### Notes
* The 4 head LEDs are numbered from left to right, with led1 being the leftmost and led4 being the rightmost.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("HeadLEDs.ino - Use head LED functions. Should set each head LED to different state."));
    mip.writeHeadLEDs(MIP_HEAD_LED_OFF, MIP_HEAD_LED_ON, MIP_HEAD_LED_BLINK_SLOW, MIP_HEAD_LED_BLINK_FAST);

    MiPHeadLEDs headLEDs;
    mip.readHeadLEDs(headLEDs);
    Serial.println(F("Head LEDs"));
    Serial.print(F("    led1: "));
        printLEDString(headLEDs.led1);
    Serial.print(F("    led2: "));
        printLEDString(headLEDs.led2);
    Serial.print(F("    led3: "));
        printLEDString(headLEDs.led3);
    Serial.print(F("    led4: "));
        printLEDString(headLEDs.led4);

    delay(4000);

    // Turn all the LEDs back on now.
    Serial.println(F("Turning all eye LEDs back on now."));
    headLEDs.led1 = headLEDs.led2 = headLEDs.led3 = headLEDs.led4 = MIP_HEAD_LED_ON;
    mip.writeHeadLEDs(headLEDs);

    Serial.println();
    Serial.println(F("Sample done."));
}

static void printLEDString(MiPHeadLED led)
{
    switch (led)
    {
    case MIP_HEAD_LED_OFF:
        Serial.println(F("Off"));
        break;
    case MIP_HEAD_LED_ON:
        Serial.println(F("On"));
        break;
    case MIP_HEAD_LED_BLINK_SLOW:
        Serial.println(F("Blink Slow"));
        break;
    case MIP_HEAD_LED_BLINK_FAST:
        Serial.println(F("Blink Fast"));
        break;
    default:
        Serial.println();
        break;
    }
}

void loop()
{
}
```


---
### readHeadLEDs()
```void readHeadLEDs(MiPHeadLEDs& headLEDs)```
#### Description
Reads the current setting for the MiP robot's four head LEDs.

#### Parameters
* **headLEDs** is an object which contains led1, led2, led3, and led4 fields to be filled in with the current state of all 4 LEDs.
```c++
class MiPHeadLEDs
{
public:
    // ...
    MiPHeadLED led1;
    MiPHeadLED led2;
    MiPHeadLED led3;
    MiPHeadLED led4;
};
```

#### Returns
Nothing

#### Notes
* The 4 head LEDs are numbered from left to right, with led1 being the leftmost and led4 being the rightmost.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("HeadLEDs.ino - Use head LED functions. Should set each head LED to different state."));
    mip.writeHeadLEDs(MIP_HEAD_LED_OFF, MIP_HEAD_LED_ON, MIP_HEAD_LED_BLINK_SLOW, MIP_HEAD_LED_BLINK_FAST);

    MiPHeadLEDs headLEDs;
    mip.readHeadLEDs(headLEDs);
    Serial.println(F("Head LEDs"));
    Serial.print(F("    led1: "));
        printLEDString(headLEDs.led1);
    Serial.print(F("    led2: "));
        printLEDString(headLEDs.led2);
    Serial.print(F("    led3: "));
        printLEDString(headLEDs.led3);
    Serial.print(F("    led4: "));
        printLEDString(headLEDs.led4);

    delay(4000);

    // Turn all the LEDs back on now.
    Serial.println(F("Turning all eye LEDs back on now."));
    headLEDs.led1 = headLEDs.led2 = headLEDs.led3 = headLEDs.led4 = MIP_HEAD_LED_ON;
    mip.writeHeadLEDs(headLEDs);

    Serial.println();
    Serial.println(F("Sample done."));
}

static void printLEDString(MiPHeadLED led)
{
    switch (led)
    {
    case MIP_HEAD_LED_OFF:
        Serial.println(F("Off"));
        break;
    case MIP_HEAD_LED_ON:
        Serial.println(F("On"));
        break;
    case MIP_HEAD_LED_BLINK_SLOW:
        Serial.println(F("Blink Slow"));
        break;
    case MIP_HEAD_LED_BLINK_FAST:
        Serial.println(F("Blink Fast"));
        break;
    default:
        Serial.println();
        break;
    }
}

void loop()
{
}
```


---
### continuousDrive()
```void continuousDrive(int8_t velocity, int8_t turnRate)```
#### Description
Instruct the MiP robot to drive in a particular direction.

#### Parameters
* **velocity** is the forward/reverse velocity.  This parameter can have a value between -32 and 32, where -32 is maximum  reverse velocity and 32 is maximum forward velocity.  A velocity of 0 indicates that no forward/reverse motion is desired at this time.
* **turnRate** is the amount of left/right turn rate.  This parameter can have a value between -32 and 32, where -32 is  maximum left turn rate and 32 is maximum right turn velocity.  A turn rate of 0 indicates that no left/right turning is desired at this time.

#### Returns
Nothing

#### Notes
* This command must be sent at regular intervals to keep the MiP robot moving in the desired direction.  This interval  should be ~50 milliseconds. If you send them faster than this interval, they will be ignored by the library so as not to overload the MiP robot.
* When sent at longer intervals the MiP robot's motion will become more jerky as it thinks that there will not be another  motion command coming so it starts to stop all motion and then starts moving again once the next command does finally arrive.
* If your robot code is constantly sensing it's environment and using those ongoing sensor readings to make decisions on which direction the robot should be currently headed, then this command will be the most useful for updating the MiP's motion in real time.
* Radar measurements and gesture detection events will still be generated by the MiP robot when continuous drive commands are being sent to it.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("ContinuousDrive.ino - Use continuousDrive() function. Drive forward with right turn and then backward with left turn."));
}

void loop()
{
    static enum States
    {
        RIGHT_TURN,
        LEFT_TURN,
        DONE
    } state = RIGHT_TURN;
    static uint32_t startTime = millis();
    uint32_t        currentTime = millis();
    uint32_t        elapsedTime = currentTime - startTime;

    switch (state)
    {
    case RIGHT_TURN:
        if (elapsedTime < 2000)
        {
          // Drive forward at half-speed and turn right at quarter rate.
          mip.continuousDrive(16, 8);
        }
        else
        {
            startTime = currentTime;
            state = LEFT_TURN;
        }
        break;
    case LEFT_TURN:
        if (elapsedTime < 2000)
        {
            // Drive backward at half-speed and turn left at quarter rate.
            mip.continuousDrive(-16, -8);
        }
        else
        {
            Serial.println();
            Serial.println(F("Sample done."));
            state = DONE;
        }
        break;
    default:
        break;
    }
}
```


---
### distanceDrive()
```void distanceDrive(MiPDriveDirection driveDirection, uint8_t cm, MiPTurnDirection turnDirection, uint16_t degrees)```
#### Description
Used to queue up a drive/turn command to the MiP robot.

#### Parameters
* **driveDirection** specifies the direction the robot should drive: **MIP_DRIVE_FORWARD** or **MIP_DRIVE_BACKWARD**.
* **cm** specifies the number of centimeters the robot should proceed in driveDirection.  This parameter can have a value between 0 and 255 cm.  Can be set to 0 if only a turn is desired.
* **turnDirection** specifies the direction the robot should turn: **MIP_TURN_LEFT** or **MIP_TURN_RIGHT**.
* **degrees** is the number of degrees the robot should turn.  This parameter can have a value between 0 and 360 degrees.  Can be set to 0 if only a forward/reverse drive is desired.

#### Returns
Nothing

#### Notes
* It is possible to queue up to 20 of these distance drive commands in the MiP's memory.
* There is no way to specify the speed to be used for driving and/or turning.
* The distance travelled isn't super accurate.  Usually it travels further than commanded.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("DistanceDrive.ino - Use distanceDrive function. Drive forward, turn 360 degrees in each direction and backward."));

    // Queue up multiple commands to run in sequence.
    mip.distanceDrive( MIP_DRIVE_FORWARD, 30, MIP_TURN_RIGHT, 0);
    mip.distanceDrive(MIP_DRIVE_FORWARD, 0, MIP_TURN_LEFT, 360);
    mip.distanceDrive(MIP_DRIVE_FORWARD, 0, MIP_TURN_RIGHT, 360);
    mip.distanceDrive(MIP_DRIVE_BACKWARD, 30, MIP_TURN_RIGHT, 0);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### turnLeft()
```void turnLeft(uint16_t degrees, uint8_t speed)```
#### Description
Commands the MiP robot to turn left the desired number of degrees at the desired speed.

#### Parameters
* **degrees** specifies the number of degrees the robot should turn.  This parameter can have a value between 0 and 1275 degrees.
* **speed** is the speed at which the turn should take place.  The speed can have a value between 0 and 24 with 0 being the slowest rate and 24 the fastest.

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("TurnLeftRight.ino - Use turnLeft & turnRight() functions. Turn 180 degrees to left and then 180 degrees to right."));

    mip.turnLeft(180, 12);
    delay(2000);
    mip.turnRight(180, 12);
    delay(2000);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### turnRight()
```void turnRight(uint16_t degrees, uint8_t speed)```
#### Description
Commands the MiP robot to turn right the desired number of degrees at the desired speed.

#### Parameters
* **degrees** specifies the number of degrees the robot should turn.  This parameter can have a value between 0 and 1275 degrees.
* **speed** is the speed at which the turn should take place.  The speed can have a value between 0 and 24 with 0 being the slowest rate and 24 the fastest.

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("TurnLeftRight.ino - Use turnLeft & turnRight() functions. Turn 180 degrees to left and then 180 degrees to right."));

    mip.turnLeft(180, 12);
    delay(2000);
    mip.turnRight(180, 12);
    delay(2000);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### driveForward()
```void driveForward(uint8_t speed, uint16_t time)```
#### Description
Commands the MiP robot to drive forward.

#### Parameters
* **speed** specifies the speed at which the motion should take place.  The speed can have a value between 0 and 30 with 0 being the slowest rate and 30 the fastest.
* **time** specifies the time to drive forward in milliseconds.  This parameter can have a value between 0 and 1785 milliseconds. The MiP only supports a granularity of 7 milliseconds for this parameter.

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("DriveForwardBackward.ino - Use driveForward() & driveBackward() functions. Drive ahead and back, 1 second in each direction."));

    mip.driveForward(15, 1000);
    delay(2000);
    mip.driveBackward(15, 1000);
    delay(2000);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### driveBackward()
```void driveBackward(uint8_t speed, uint16_t time)```
#### Description
Commands the MiP robot to drive backward.

#### Parameters
* **speed** specifies the speed at which the motion should take place.  The speed can have a value between 0 and 30 with 0 being the slowest rate and 30 the fastest.
* **time** specifies the time to drive backward in milliseconds.  This parameter can have a value between 0 and 1785 milliseconds. The MiP only supports a granularity of 7 milliseconds for this parameter.

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("DriveForwardBackward.ino - Use driveForward() & driveBackward() functions. Drive ahead and back, 1 second in each direction."));

    mip.driveForward(15, 1000);
    delay(2000);
    mip.driveBackward(15, 1000);
    delay(2000);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### stop()
```void stop()```
#### Description
Interrupt and stop current MiP robot motion.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Stop.ino - Use stop() function. Interrupt a 360 degree turn with stop()."));

    mip.turnLeft(360, 6);
    delay(1000);
    mip.stop();
    delay(1000);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### fallForward()
```void fallForward()```
#### Description
Have the MiP fall forward.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("FallDown.ino - Fall forward and backward.\n"));

    Serial.println(F("Waiting for robot to be standing upright."));
    while (!mip.isUpright())
    {
    }
    delay(1000);

    Serial.println(F("Falling forward."));
    mip.fallForward();

    delay(1000);
    Serial.println(F("Waiting for robot to be standing upright again."));
    while (!mip.isUpright())
    {
    }
    delay(1000);

    Serial.println(F("Falling backward."));
    mip.fallBackward();

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### fallBackward()
```void fallBackward()```
#### Description
Have the MiP fall backward.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("FallDown.ino - Fall forward and backward.\n"));

    Serial.println(F("Waiting for robot to be standing upright."));
    while (!mip.isUpright())
    {
    }
    delay(1000);

    Serial.println(F("Falling forward."));
    mip.fallForward();

    delay(1000);
    Serial.println(F("Waiting for robot to be standing upright again."));
    while (!mip.isUpright())
    {
    }
    delay(1000);

    Serial.println(F("Falling backward."));
    mip.fallBackward();

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### getUp()
```void getUp(MiPGetUp getup = MIP_GETUP_FROM_EITHER)```
#### Description
Attempt to get the MiP robot back up and balancing after falling to the back or front.

#### Parameters
* **getup** specifies in which direction should the MiP attempt to get up from: **MIP_GETUP_FROM_FRONT**, **MIP_GETUP_FROM_BACK**, or **MIP_GETUP_FROM_EITHER**. MIP_GETUP_FROM_EITHER is the default if this parameter is ommitted.

#### Returns
Nothing

#### Notes
* *I haven't seen my MiP robot actually be successful in accomplishing this operation.*

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("GetUp.ino - Use getUp(). Attempt to get up from a front fall."));

    mip.fallForward();
    delay(3000);

    mip.getUp(MIP_GETUP_FROM_FRONT);
    delay(3000);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### playSound()
```void playSound(MiPSoundIndex sound, MiPVolume volume = MIP_VOLUME_DEFAULT)```
#### Description
Start playing a single sound immediately. You can use the [beginSoundList()](#beginsoundlist), [addEntryToSoundList()](#addentrytosoundlist), and [playSoundList()](#playsoundlist) functions instead if you want to queue up multiple sounds instead.

#### Parameters
* **sound** is used to specify the index of the sound to be played. The MiPSoundIndex enumeration is described below.
* **volume** is an optional parameter used to set the volume level to be used when playing this sound. If not specified, the volume will remain at its current value.

| MiPSoundIndex |
| ------------- |
| MIP_SOUND_ONEKHZ_500MS_8K16BIT |
| MIP_SOUND_ACTION_BURPING |
| MIP_SOUND_ACTION_DRINKING |
| MIP_SOUND_ACTION_EATING |
| MIP_SOUND_ACTION_FARTING_SHORT |
| MIP_SOUND_ACTION_OUT_OF_BREATH |
| MIP_SOUND_BOXING_PUNCHCONNECT_1 |
| MIP_SOUND_BOXING_PUNCHCONNECT_2 |
| MIP_SOUND_BOXING_PUNCHCONNECT_3 |
| MIP_SOUND_FREESTYLE_TRACKING_1 |
| MIP_SOUND_MIP_1 |
| MIP_SOUND_MIP_2 |
| MIP_SOUND_MIP_3 |
| MIP_SOUND_MIP_APP |
| MIP_SOUND_MIP_AWWW |
| MIP_SOUND_MIP_BIG_SHOT |
| MIP_SOUND_MIP_BLEH |
| MIP_SOUND_MIP_BOOM |
| MIP_SOUND_MIP_BYE |
| MIP_SOUND_MIP_CONVERSE_1 |
| MIP_SOUND_MIP_CONVERSE_2 |
| MIP_SOUND_MIP_DROP |
| MIP_SOUND_MIP_DUNNO |
| MIP_SOUND_MIP_FALL_OVER_1 |
| MIP_SOUND_MIP_FALL_OVER_2 |
| MIP_SOUND_MIP_FIGHT |
| MIP_SOUND_MIP_GAME |
| MIP_SOUND_MIP_GLOAT |
| MIP_SOUND_MIP_GO |
| MIP_SOUND_MIP_GOGOGO |
| MIP_SOUND_MIP_GRUNT_1 |
| MIP_SOUND_MIP_GRUNT_2 |
| MIP_SOUND_MIP_GRUNT_3 |
| MIP_SOUND_MIP_HAHA_GOT_IT |
| MIP_SOUND_MIP_HI_CONFIDENT |
| MIP_SOUND_MIP_HI_NOT_SURE |
| MIP_SOUND_MIP_HI_SCARED |
| MIP_SOUND_MIP_HUH |
| MIP_SOUND_MIP_HUMMING_1 |
| MIP_SOUND_MIP_HUMMING_2 |
| MIP_SOUND_MIP_HURT |
| MIP_SOUND_MIP_HUUURGH |
| MIP_SOUND_MIP_IN_LOVE |
| MIP_SOUND_MIP_IT |
| MIP_SOUND_MIP_JOKE |
| MIP_SOUND_MIP_K |
| MIP_SOUND_MIP_LOOP_1 |
| MIP_SOUND_MIP_LOOP_2 |
| MIP_SOUND_MIP_LOW_BATTERY |
| MIP_SOUND_MIP_MIPPEE |
| MIP_SOUND_MIP_MORE |
| MIP_SOUND_MIP_MUAH_HA |
| MIP_SOUND_MIP_MUSIC |
| MIP_SOUND_MIP_OBSTACLE |
| MIP_SOUND_MIP_OHOH |
| MIP_SOUND_MIP_OH_YEAH |
| MIP_SOUND_MIP_OOPSIE |
| MIP_SOUND_MIP_OUCH_1 |
| MIP_SOUND_MIP_OUCH_2 |
| MIP_SOUND_MIP_PLAY |
| MIP_SOUND_MIP_PUSH |
| MIP_SOUND_MIP_RUN |
| MIP_SOUND_MIP_SHAKE |
| MIP_SOUND_MIP_SIGH |
| MIP_SOUND_MIP_SINGING |
| MIP_SOUND_MIP_SNEEZE |
| MIP_SOUND_MIP_SNORE |
| MIP_SOUND_MIP_STACK |
| MIP_SOUND_MIP_SWIPE_1 |
| MIP_SOUND_MIP_SWIPE_2 |
| MIP_SOUND_MIP_TRICKS |
| MIP_SOUND_MIP_TRIIICK |
| MIP_SOUND_MIP_TRUMPET |
| MIP_SOUND_MIP_WAAAAA |
| MIP_SOUND_MIP_WAKEY |
| MIP_SOUND_MIP_WHEEE |
| MIP_SOUND_MIP_WHISTLING |
| MIP_SOUND_MIP_WHOAH |
| MIP_SOUND_MIP_WOO |
| MIP_SOUND_MIP_YEAH |
| MIP_SOUND_MIP_YEEESSS |
| MIP_SOUND_MIP_YO |
| MIP_SOUND_MIP_YUMMY |
| MIP_SOUND_MOOD_ACTIVATED |
| MIP_SOUND_MOOD_ANGRY |
| MIP_SOUND_MOOD_ANXIOUS |
| MIP_SOUND_MOOD_BORING |
| MIP_SOUND_MOOD_CRANKY |
| MIP_SOUND_MOOD_ENERGETIC |
| MIP_SOUND_MOOD_EXCITED |
| MIP_SOUND_MOOD_GIDDY |
| MIP_SOUND_MOOD_GRUMPY |
| MIP_SOUND_MOOD_HAPPY |
| MIP_SOUND_MOOD_IDEA |
| MIP_SOUND_MOOD_IMPATIENT |
| MIP_SOUND_MOOD_NICE |
| MIP_SOUND_MOOD_SAD |
| MIP_SOUND_MOOD_SHORT |
| MIP_SOUND_MOOD_SLEEPY |
| MIP_SOUND_MOOD_TIRED |
| MIP_SOUND_SOUND_BOOST |
| MIP_SOUND_SOUND_CAGE |
| MIP_SOUND_SOUND_GUNS |
| MIP_SOUND_SOUND_ZINGS |
| MIP_SOUND_SHORT_MUTE_FOR_STOP |
| MIP_SOUND_FREESTYLE_TRACKING_2 |

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("PlaySound.ino - Play a few sounds."));

    // Play a single sound.
    mip.playSound(MIP_SOUND_ACTION_DRINKING, MIP_VOLUME_4);
    delay(3000);

    // Play 2 sounds with 1 second delay between them, repeating them a second time.
    // Play the first at a lower volume than the second.
    mip.beginSoundList();
    mip.addEntryToSoundList(MIP_SOUND_ACTION_EATING, 1000, MIP_VOLUME_4);
    mip.addEntryToSoundList(MIP_SOUND_ACTION_BURPING, 0, MIP_VOLUME_7);
    mip.playSoundList(1);

    // Play the sound list again after waiting for the previous play to complete.
    delay(10000);
    mip.playSoundList();

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### beginSoundList()
```void beginSoundList()```
#### Description
Begins the process of having the MiP robot play sounds. After calling beginSoundList(), you can call [addEntryToSoundList()](#addentrytosoundlist) to queue up sounds to play and finally have the queued up sounds start to play by calling [playSoundList()](#playsoundlist).

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("PlaySound.ino - Play a few sounds."));

    // Play a single sound.
    mip.playSound(MIP_SOUND_ACTION_DRINKING, MIP_VOLUME_4);
    delay(3000);

    // Play 2 sounds with 1 second delay between them, repeating them a second time.
    // Play the first at a lower volume than the second.
    mip.beginSoundList();
    mip.addEntryToSoundList(MIP_SOUND_ACTION_EATING, 1000, MIP_VOLUME_4);
    mip.addEntryToSoundList(MIP_SOUND_ACTION_BURPING, 0, MIP_VOLUME_7);
    mip.playSoundList(1);

    // Play the sound list again after waiting for the previous play to complete.
    delay(10000);
    mip.playSoundList();

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### addEntryToSoundList()
```void addEntryToSoundList(MiPSoundIndex sound)```<br>
```void addEntryToSoundList(MiPSoundIndex sound, uint16_t delay)```<br>
```void addEntryToSoundList(MiPSoundIndex sound, uint16_t delay, MiPVolume volume)```<br>
#### Description
Adds a sound or volume update entry to the sound list. Before calling addEntryToSoundList(), you must first call [beginSoundList()](#beginsoundlist) and the sounds won't start playing until you call [playSoundList()](#playsoundlist). You can add a maximum of 8 entries to the sound list. Changing the volume levels will use up additional entries so you may actually get less than 8 sounds in the list.

#### Parameters
* **sound** is used to specify the index of the sound to be played for this entry. The MiPSoundIndex enumeration is described below.
* **delay** is an optional parameter used to specify the delay in milliseconds that the MiP should wait after playing this sound before playing the next sound (if any) in the sound list. The granularity of this delay is 30 milliseconds. This parameter defaults to a 0 millisecond delay.
* **volume** is an optional parameter used to set the volume level to be used when playing this sound. If not specified, the volume will remain the same as it was for the previous sound in the list.

| MiPSoundIndex |
| ------------- |
| MIP_SOUND_ONEKHZ_500MS_8K16BIT |
| MIP_SOUND_ACTION_BURPING |
| MIP_SOUND_ACTION_DRINKING |
| MIP_SOUND_ACTION_EATING |
| MIP_SOUND_ACTION_FARTING_SHORT |
| MIP_SOUND_ACTION_OUT_OF_BREATH |
| MIP_SOUND_BOXING_PUNCHCONNECT_1 |
| MIP_SOUND_BOXING_PUNCHCONNECT_2 |
| MIP_SOUND_BOXING_PUNCHCONNECT_3 |
| MIP_SOUND_FREESTYLE_TRACKING_1 |
| MIP_SOUND_MIP_1 |
| MIP_SOUND_MIP_2 |
| MIP_SOUND_MIP_3 |
| MIP_SOUND_MIP_APP |
| MIP_SOUND_MIP_AWWW |
| MIP_SOUND_MIP_BIG_SHOT |
| MIP_SOUND_MIP_BLEH |
| MIP_SOUND_MIP_BOOM |
| MIP_SOUND_MIP_BYE |
| MIP_SOUND_MIP_CONVERSE_1 |
| MIP_SOUND_MIP_CONVERSE_2 |
| MIP_SOUND_MIP_DROP |
| MIP_SOUND_MIP_DUNNO |
| MIP_SOUND_MIP_FALL_OVER_1 |
| MIP_SOUND_MIP_FALL_OVER_2 |
| MIP_SOUND_MIP_FIGHT |
| MIP_SOUND_MIP_GAME |
| MIP_SOUND_MIP_GLOAT |
| MIP_SOUND_MIP_GO |
| MIP_SOUND_MIP_GOGOGO |
| MIP_SOUND_MIP_GRUNT_1 |
| MIP_SOUND_MIP_GRUNT_2 |
| MIP_SOUND_MIP_GRUNT_3 |
| MIP_SOUND_MIP_HAHA_GOT_IT |
| MIP_SOUND_MIP_HI_CONFIDENT |
| MIP_SOUND_MIP_HI_NOT_SURE |
| MIP_SOUND_MIP_HI_SCARED |
| MIP_SOUND_MIP_HUH |
| MIP_SOUND_MIP_HUMMING_1 |
| MIP_SOUND_MIP_HUMMING_2 |
| MIP_SOUND_MIP_HURT |
| MIP_SOUND_MIP_HUUURGH |
| MIP_SOUND_MIP_IN_LOVE |
| MIP_SOUND_MIP_IT |
| MIP_SOUND_MIP_JOKE |
| MIP_SOUND_MIP_K |
| MIP_SOUND_MIP_LOOP_1 |
| MIP_SOUND_MIP_LOOP_2 |
| MIP_SOUND_MIP_LOW_BATTERY |
| MIP_SOUND_MIP_MIPPEE |
| MIP_SOUND_MIP_MORE |
| MIP_SOUND_MIP_MUAH_HA |
| MIP_SOUND_MIP_MUSIC |
| MIP_SOUND_MIP_OBSTACLE |
| MIP_SOUND_MIP_OHOH |
| MIP_SOUND_MIP_OH_YEAH |
| MIP_SOUND_MIP_OOPSIE |
| MIP_SOUND_MIP_OUCH_1 |
| MIP_SOUND_MIP_OUCH_2 |
| MIP_SOUND_MIP_PLAY |
| MIP_SOUND_MIP_PUSH |
| MIP_SOUND_MIP_RUN |
| MIP_SOUND_MIP_SHAKE |
| MIP_SOUND_MIP_SIGH |
| MIP_SOUND_MIP_SINGING |
| MIP_SOUND_MIP_SNEEZE |
| MIP_SOUND_MIP_SNORE |
| MIP_SOUND_MIP_STACK |
| MIP_SOUND_MIP_SWIPE_1 |
| MIP_SOUND_MIP_SWIPE_2 |
| MIP_SOUND_MIP_TRICKS |
| MIP_SOUND_MIP_TRIIICK |
| MIP_SOUND_MIP_TRUMPET |
| MIP_SOUND_MIP_WAAAAA |
| MIP_SOUND_MIP_WAKEY |
| MIP_SOUND_MIP_WHEEE |
| MIP_SOUND_MIP_WHISTLING |
| MIP_SOUND_MIP_WHOAH |
| MIP_SOUND_MIP_WOO |
| MIP_SOUND_MIP_YEAH |
| MIP_SOUND_MIP_YEEESSS |
| MIP_SOUND_MIP_YO |
| MIP_SOUND_MIP_YUMMY |
| MIP_SOUND_MOOD_ACTIVATED |
| MIP_SOUND_MOOD_ANGRY |
| MIP_SOUND_MOOD_ANXIOUS |
| MIP_SOUND_MOOD_BORING |
| MIP_SOUND_MOOD_CRANKY |
| MIP_SOUND_MOOD_ENERGETIC |
| MIP_SOUND_MOOD_EXCITED |
| MIP_SOUND_MOOD_GIDDY |
| MIP_SOUND_MOOD_GRUMPY |
| MIP_SOUND_MOOD_HAPPY |
| MIP_SOUND_MOOD_IDEA |
| MIP_SOUND_MOOD_IMPATIENT |
| MIP_SOUND_MOOD_NICE |
| MIP_SOUND_MOOD_SAD |
| MIP_SOUND_MOOD_SHORT |
| MIP_SOUND_MOOD_SLEEPY |
| MIP_SOUND_MOOD_TIRED |
| MIP_SOUND_SOUND_BOOST |
| MIP_SOUND_SOUND_CAGE |
| MIP_SOUND_SOUND_GUNS |
| MIP_SOUND_SOUND_ZINGS |
| MIP_SOUND_SHORT_MUTE_FOR_STOP |
| MIP_SOUND_FREESTYLE_TRACKING_2 |

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("PlaySound.ino - Play a few sounds."));

    // Play a single sound.
    mip.playSound(MIP_SOUND_ACTION_DRINKING, MIP_VOLUME_4);
    delay(3000);

    // Play 2 sounds with 1 second delay between them, repeating them a second time.
    // Play the first at a lower volume than the second.
    mip.beginSoundList();
    mip.addEntryToSoundList(MIP_SOUND_ACTION_EATING, 1000, MIP_VOLUME_4);
    mip.addEntryToSoundList(MIP_SOUND_ACTION_BURPING, 0, MIP_VOLUME_7);
    mip.playSoundList(1);

    // Play the sound list again after waiting for the previous play to complete.
    delay(10000);
    mip.playSoundList();

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### playSoundList()
```void playSoundList(uint8_t repeatCount = 0)```
#### Description
Starts playing the entries found in the sound list. Before you call playSoundList(), the sound list must have been initialized and populated with calls to [beginSoundList()](#beginsoundlist) and [addEntryToSoundList()](#addentrytosoundlist).

#### Parameters
* **repeatCount** is an optional parameter which specifies the number of times that the entries in the sound list should be repeated (0 - 255). A value of 0 means that the sound list will play once and then stop whereas a value of 1 means that it would play the sequence a total of 2 times. The default value for this parameter is 0 additional repeats.

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("PlaySound.ino - Play a few sounds."));

    // Play a single sound.
    mip.playSound(MIP_SOUND_ACTION_DRINKING, MIP_VOLUME_4);
    delay(3000);

    // Play 2 sounds with 1 second delay between them, repeating them a second time.
    // Play the first at a lower volume than the second.
    mip.beginSoundList();
    mip.addEntryToSoundList(MIP_SOUND_ACTION_EATING, 1000, MIP_VOLUME_4);
    mip.addEntryToSoundList(MIP_SOUND_ACTION_BURPING, 0, MIP_VOLUME_7);
    mip.playSoundList(1);

    // Play the sound list again after waiting for the previous play to complete.
    delay(10000);
    mip.playSoundList();

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### writeVolume()
```void writeVolume(uint8_t volume)```
#### Description
Set the MiP robot's volume level.

#### Parameters
* **volume** is the new volume level (0 - 7).  A level of 0 turns off the sound and 7 is the loudest setting.

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Volume.ino - Use read/writeVolume(). Set volume level to 1 and read out afterwards."));

    mip.writeVolume(1);

    uint8_t volume = mip.readVolume();

    Serial.print(F("Volume = "));
      Serial.println(volume);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```

---
### readVolume()
```uint8_t readVolume()```
#### Description
Read the MiP robot's current volume level setting.

#### Parameters
None

#### Returns
The current volume level (0 - 7).  A level of 0 means the sound is off and 7 is the loudest setting.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Volume.ino - Use read/writeVolume(). Set volume level to 1 and read out afterwards."));

    mip.writeVolume(1);

    uint8_t volume = mip.readVolume();

    Serial.print(F("Volume = "));
      Serial.println(volume);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### readDistanceTravelled()
```float readDistanceTravelled()```
#### Description
Read the current MiP odometer tally.

#### Parameters
None

#### Returns
The distance travelled (in centimeters) since the last reset.

#### Notes
* The MiP robot's odometer is similar to that in a car.  It maintains the total tally of the distance travelled, even across power cycles.
* The [resetDistanceTravelled()](#resetdistancetravelled) function can be used to reset the odometer.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Odometer.ino - Read out current odometer reading and reset."));

    float cm = mip.readDistanceTravelled();
    Serial.print(F("MiP has travelled "));
        Serial.print(cm);
        Serial.println(F(" cm since the last reset."));

    mip.resetDistanceTravelled();

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### resetDistanceTravelled()
```void  resetDistanceTravelled()```
#### Description
Reset the MiP robot's odometer. The current odometer distance measurement is available by calling the [readDistanceTravelled()](#readdistancetravelled) function.

#### Parameters
None

#### Returns
Nothing

#### Notes
* The MiP robot's odometer doesn't automatically reset during a power cycle.
* You should call this function if you want to manually reset it.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Odometer.ino - Read out current odometer reading and reset."));

    float cm = mip.readDistanceTravelled();
    Serial.print(F("MiP has travelled "));
        Serial.print(cm);
        Serial.println(F(" cm since the last reset."));

    mip.resetDistanceTravelled();

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### readBatteryVoltage()
```float readBatteryVoltage()```
#### Description
Read the MiP robot's current battery level.

#### Parameters
None

#### Returns
The current battery voltage in Volts.

#### Notes
* The MiP robot status updates, which include the battery level, may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.
* These status updates are usually sent every 30 seconds or earlier if the MiP detects a change in position.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop()
{
    static float       lastBatteryLevel = 0.0f;
    static MiPPosition lastPosition = (MiPPosition)-1;

    float              currentBatteryLevel = mip.readBatteryVoltage();
    MiPPosition        currentPosition = mip.readPosition();

    if (currentBatteryLevel != lastBatteryLevel)
    {
        Serial.println(F("Battery: "));
            Serial.print(currentBatteryLevel);
            Serial.println(F("V"));
        lastBatteryLevel = currentBatteryLevel;
    }

    if (currentPosition != lastPosition)
    {
        if (mip.isOnBack())
        {
            Serial.println(F("Position: On Back"));
        }
        if (mip.isFaceDown())
        {
            Serial.println(F("Position: Face Down"));
        }
        if (mip.isUpright())
        {
            Serial.println(F("Position: Upright"));
        }
        if (mip.isPickedUp())
        {
            Serial.println(F("Position: Picked Up"));
        }
        if (mip.isHandStanding())
        {
            Serial.println(F("Position: Hand Stand"));
        }
        if (mip.isFaceDownOnTray())
        {
            Serial.println(F("Position: Face Down on Tray"));
        }
        if (mip.isOnBackWithKickstand())
        {
            Serial.println(F("Position: On Back With Kickstand"));
        }

        lastPosition = currentPosition;
    }
}
```


---
### readPosition()
```MiPPosition readPosition()```
#### Description
Read the MiP robot's current position.

#### Parameters
None

#### Returns
One of the values from the following table:

| MiPPosition Value |
| ----------------- |
| MIP_POSITION_ON_BACK |
| MIP_POSITION_FACE_DOWN |
| MIP_POSITION_UPRIGHT |
| MIP_POSITION_PICKED_UP |
| MIP_POSITION_HAND_STAND |
| MIP_POSITION_FACE_DOWN_ON_TRAY |
| MIP_POSITION_ON_BACK_WITH_KICKSTAND |

#### Notes
* The MiP robot status updates, which include the position, may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.
* These status updates are usually sent every 30 seconds or earlier if the MiP detects a change in position.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop()
{
    static float       lastBatteryLevel = 0.0f;
    static MiPPosition lastPosition = (MiPPosition)-1;

    float              currentBatteryLevel = mip.readBatteryVoltage();
    MiPPosition        currentPosition = mip.readPosition();

    if (currentBatteryLevel != lastBatteryLevel)
    {
        Serial.println(F("Battery: "));
            Serial.print(currentBatteryLevel);
            Serial.println(F("V"));
        lastBatteryLevel = currentBatteryLevel;
    }

    if (currentPosition != lastPosition)
    {
        if (mip.isOnBack())
        {
            Serial.println(F("Position: On Back"));
        }
        if (mip.isFaceDown())
        {
            Serial.println(F("Position: Face Down"));
        }
        if (mip.isUpright())
        {
            Serial.println(F("Position: Upright"));
        }
        if (mip.isPickedUp())
        {
            Serial.println(F("Position: Picked Up"));
        }
        if (mip.isHandStanding())
        {
            Serial.println(F("Position: Hand Stand"));
        }
        if (mip.isFaceDownOnTray())
        {
            Serial.println(F("Position: Face Down on Tray"));
        }
        if (mip.isOnBackWithKickstand())
        {
            Serial.println(F("Position: On Back With Kickstand"));
        }

        lastPosition = currentPosition;
    }
}
```


---
### isOnBack()
```bool  isOnBack()```
#### Description
Is the MiP robot currently lying on its back?

#### Parameters
None

#### Returns
* **true** if the MiP robot is currently lying on its back.
* **false** otherwise.

#### Notes
* The MiP robot status updates, which include the position, may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.
* These status updates are usually sent every 30 seconds or earlier if the MiP detects a change in position.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop()
{
    static float       lastBatteryLevel = 0.0f;
    static MiPPosition lastPosition = (MiPPosition)-1;

    float              currentBatteryLevel = mip.readBatteryVoltage();
    MiPPosition        currentPosition = mip.readPosition();

    if (currentBatteryLevel != lastBatteryLevel)
    {
        Serial.println(F("Battery: "));
            Serial.print(currentBatteryLevel);
            Serial.println(F("V"));
        lastBatteryLevel = currentBatteryLevel;
    }

    if (currentPosition != lastPosition)
    {
        if (mip.isOnBack())
        {
            Serial.println(F("Position: On Back"));
        }
        if (mip.isFaceDown())
        {
            Serial.println(F("Position: Face Down"));
        }
        if (mip.isUpright())
        {
            Serial.println(F("Position: Upright"));
        }
        if (mip.isPickedUp())
        {
            Serial.println(F("Position: Picked Up"));
        }
        if (mip.isHandStanding())
        {
            Serial.println(F("Position: Hand Stand"));
        }
        if (mip.isFaceDownOnTray())
        {
            Serial.println(F("Position: Face Down on Tray"));
        }
        if (mip.isOnBackWithKickstand())
        {
            Serial.println(F("Position: On Back With Kickstand"));
        }

        lastPosition = currentPosition;
    }
}
```


---
### isFaceDown()
```bool  isFaceDown()```
#### Description
Is the MiP robot currently lying on its front?

#### Parameters
None

#### Returns
* **true** if the MiP robot is currently lying on its front.
* **false** otherwise.

#### Notes
* The MiP robot status updates, which include the position, may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.
* These status updates are usually sent every 30 seconds or earlier if the MiP detects a change in position.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop()
{
    static float       lastBatteryLevel = 0.0f;
    static MiPPosition lastPosition = (MiPPosition)-1;

    float              currentBatteryLevel = mip.readBatteryVoltage();
    MiPPosition        currentPosition = mip.readPosition();

    if (currentBatteryLevel != lastBatteryLevel)
    {
        Serial.println(F("Battery: "));
            Serial.print(currentBatteryLevel);
            Serial.println(F("V"));
        lastBatteryLevel = currentBatteryLevel;
    }

    if (currentPosition != lastPosition)
    {
        if (mip.isOnBack())
        {
            Serial.println(F("Position: On Back"));
        }
        if (mip.isFaceDown())
        {
            Serial.println(F("Position: Face Down"));
        }
        if (mip.isUpright())
        {
            Serial.println(F("Position: Upright"));
        }
        if (mip.isPickedUp())
        {
            Serial.println(F("Position: Picked Up"));
        }
        if (mip.isHandStanding())
        {
            Serial.println(F("Position: Hand Stand"));
        }
        if (mip.isFaceDownOnTray())
        {
            Serial.println(F("Position: Face Down on Tray"));
        }
        if (mip.isOnBackWithKickstand())
        {
            Serial.println(F("Position: On Back With Kickstand"));
        }

        lastPosition = currentPosition;
    }
}
```


---
### isUpright()
```bool  isUpright()```
#### Description
Is the MiP robot currently standing upright and self-balancing?

#### Parameters
None

#### Returns
* **true** if the MiP robot is currently standing up right and self-balancing.
* **false** otherwise.

#### Notes
* The MiP robot status updates, which include the position, may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.
* These status updates are usually sent every 30 seconds or earlier if the MiP detects a change in position.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop()
{
    static float       lastBatteryLevel = 0.0f;
    static MiPPosition lastPosition = (MiPPosition)-1;

    float              currentBatteryLevel = mip.readBatteryVoltage();
    MiPPosition        currentPosition = mip.readPosition();

    if (currentBatteryLevel != lastBatteryLevel)
    {
        Serial.println(F("Battery: "));
            Serial.print(currentBatteryLevel);
            Serial.println(F("V"));
        lastBatteryLevel = currentBatteryLevel;
    }

    if (currentPosition != lastPosition)
    {
        if (mip.isOnBack())
        {
            Serial.println(F("Position: On Back"));
        }
        if (mip.isFaceDown())
        {
            Serial.println(F("Position: Face Down"));
        }
        if (mip.isUpright())
        {
            Serial.println(F("Position: Upright"));
        }
        if (mip.isPickedUp())
        {
            Serial.println(F("Position: Picked Up"));
        }
        if (mip.isHandStanding())
        {
            Serial.println(F("Position: Hand Stand"));
        }
        if (mip.isFaceDownOnTray())
        {
            Serial.println(F("Position: Face Down on Tray"));
        }
        if (mip.isOnBackWithKickstand())
        {
            Serial.println(F("Position: On Back With Kickstand"));
        }

        lastPosition = currentPosition;
    }
}
```


---
### isPickedUp()
```bool  isPickedUp()```
#### Description
Has the user picked the MiP robot up?

#### Parameters
None

#### Returns
* **true** if the MiP robot has been picked up by the user.
* **false** otherwise.

#### Notes
* The MiP robot status updates, which include the position, may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.
* These status updates are usually sent every 30 seconds or earlier if the MiP detects a change in position.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop()
{
    static float       lastBatteryLevel = 0.0f;
    static MiPPosition lastPosition = (MiPPosition)-1;

    float              currentBatteryLevel = mip.readBatteryVoltage();
    MiPPosition        currentPosition = mip.readPosition();

    if (currentBatteryLevel != lastBatteryLevel)
    {
        Serial.println(F("Battery: "));
            Serial.print(currentBatteryLevel);
            Serial.println(F("V"));
        lastBatteryLevel = currentBatteryLevel;
    }

    if (currentPosition != lastPosition)
    {
        if (mip.isOnBack())
        {
            Serial.println(F("Position: On Back"));
        }
        if (mip.isFaceDown())
        {
            Serial.println(F("Position: Face Down"));
        }
        if (mip.isUpright())
        {
            Serial.println(F("Position: Upright"));
        }
        if (mip.isPickedUp())
        {
            Serial.println(F("Position: Picked Up"));
        }
        if (mip.isHandStanding())
        {
            Serial.println(F("Position: Hand Stand"));
        }
        if (mip.isFaceDownOnTray())
        {
            Serial.println(F("Position: Face Down on Tray"));
        }
        if (mip.isOnBackWithKickstand())
        {
            Serial.println(F("Position: On Back With Kickstand"));
        }

        lastPosition = currentPosition;
    }
}
```


---
### isHandStanding()
```bool  isHandStanding()```
#### Description
Is the MiP robot currently in a hand stand pose?

#### Parameters
None

#### Returns
* **true** if the MiP robot is currently in a hand stand pose.
* **false** otherwise.

#### Notes
* The MiP robot status updates, which include the position, may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.
* These status updates are usually sent every 30 seconds or earlier if the MiP detects a change in position.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop()
{
    static float       lastBatteryLevel = 0.0f;
    static MiPPosition lastPosition = (MiPPosition)-1;

    float              currentBatteryLevel = mip.readBatteryVoltage();
    MiPPosition        currentPosition = mip.readPosition();

    if (currentBatteryLevel != lastBatteryLevel)
    {
        Serial.println(F("Battery: "));
            Serial.print(currentBatteryLevel);
            Serial.println(F("V"));
        lastBatteryLevel = currentBatteryLevel;
    }

    if (currentPosition != lastPosition)
    {
        if (mip.isOnBack())
        {
            Serial.println(F("Position: On Back"));
        }
        if (mip.isFaceDown())
        {
            Serial.println(F("Position: Face Down"));
        }
        if (mip.isUpright())
        {
            Serial.println(F("Position: Upright"));
        }
        if (mip.isPickedUp())
        {
            Serial.println(F("Position: Picked Up"));
        }
        if (mip.isHandStanding())
        {
            Serial.println(F("Position: Hand Stand"));
        }
        if (mip.isFaceDownOnTray())
        {
            Serial.println(F("Position: Face Down on Tray"));
        }
        if (mip.isOnBackWithKickstand())
        {
            Serial.println(F("Position: On Back With Kickstand"));
        }

        lastPosition = currentPosition;
    }
}
```


---
### isFaceDownOnTray()
```bool  isFaceDownOnTray()```
#### Description
Is the MiP robot currently face down on the tray?

#### Parameters
None

#### Returns
* **true** if the MiP robot is currently face down on the tray.
* **false** otherwise.

#### Notes
* The MiP robot status updates, which include the position, may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.
* These status updates are usually sent every 30 seconds or earlier if the MiP detects a change in position.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop()
{
    static float       lastBatteryLevel = 0.0f;
    static MiPPosition lastPosition = (MiPPosition)-1;

    float              currentBatteryLevel = mip.readBatteryVoltage();
    MiPPosition        currentPosition = mip.readPosition();

    if (currentBatteryLevel != lastBatteryLevel)
    {
        Serial.println(F("Battery: "));
            Serial.print(currentBatteryLevel);
            Serial.println(F("V"));
        lastBatteryLevel = currentBatteryLevel;
    }

    if (currentPosition != lastPosition)
    {
        if (mip.isOnBack())
        {
            Serial.println(F("Position: On Back"));
        }
        if (mip.isFaceDown())
        {
            Serial.println(F("Position: Face Down"));
        }
        if (mip.isUpright())
        {
            Serial.println(F("Position: Upright"));
        }
        if (mip.isPickedUp())
        {
            Serial.println(F("Position: Picked Up"));
        }
        if (mip.isHandStanding())
        {
            Serial.println(F("Position: Hand Stand"));
        }
        if (mip.isFaceDownOnTray())
        {
            Serial.println(F("Position: Face Down on Tray"));
        }
        if (mip.isOnBackWithKickstand())
        {
            Serial.println(F("Position: On Back With Kickstand"));
        }

        lastPosition = currentPosition;
    }
}
```


---
### isOnBackWithKickstand()
```bool  isOnBackWithKickstand()```
#### Description
Is the MiP robot currently laying back on its kickstand?

#### Parameters
None

#### Returns
* **true** if the MiP robot is currently laying back on its kickstand.
* **false** otherwise.

#### Notes
* The MiP robot status updates, which include the position, may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.
* These status updates are usually sent every 30 seconds or earlier if the MiP detects a change in position.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop()
{
    static float       lastBatteryLevel = 0.0f;
    static MiPPosition lastPosition = (MiPPosition)-1;

    float              currentBatteryLevel = mip.readBatteryVoltage();
    MiPPosition        currentPosition = mip.readPosition();

    if (currentBatteryLevel != lastBatteryLevel)
    {
        Serial.println(F("Battery: "));
            Serial.print(currentBatteryLevel);
            Serial.println(F("V"));
        lastBatteryLevel = currentBatteryLevel;
    }

    if (currentPosition != lastPosition)
    {
        if (mip.isOnBack())
        {
            Serial.println(F("Position: On Back"));
        }
        if (mip.isFaceDown())
        {
            Serial.println(F("Position: Face Down"));
        }
        if (mip.isUpright())
        {
            Serial.println(F("Position: Upright"));
        }
        if (mip.isPickedUp())
        {
            Serial.println(F("Position: Picked Up"));
        }
        if (mip.isHandStanding())
        {
            Serial.println(F("Position: Hand Stand"));
        }
        if (mip.isFaceDownOnTray())
        {
            Serial.println(F("Position: Face Down on Tray"));
        }
        if (mip.isOnBackWithKickstand())
        {
            Serial.println(F("Position: On Back With Kickstand"));
        }

        lastPosition = currentPosition;
    }
}
```


---
### readWeight()
```int8_t readWeight()```
#### Description
Read the MiP robot's current estimate of how much weight it is carrying.

#### Parameters
None

#### Returns
A signed integer representing how much it is tilted to keep its center of mass directly over the wheels. A positive value if it is front heavy and a negative value if it is back heavy.

#### Notes
* The MiP robot weight updates may be lost if you are spending too much time sending output back to the Arduino IDE's serial monitor. See [Limitations](#limitations) section above.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Weight.ino - Use weight update functions."));
}

void loop()
{
    static int8_t lastWeight = -128;
    int8_t currentWeight = mip.readWeight();

    if (currentWeight != lastWeight)
    {
        Serial.print(F("Weight = "));
          Serial.println(currentWeight);
        lastWeight = currentWeight;
    }
}
```


---
### enableClapEvents()
```void enableClapEvents()```
#### Description
Enable the MiP robot to detect user claps. Once this is enabled, your code can call [availableClapEvents()](#availableclapevents) and [readClapEvent](#readclapevent) to determine when clap events have been detected by the MiP.

#### Parameters
None

#### Returns
None

#### Notes
* The MiP robot defaults to having the clap detection feature disabled after the ProMini Pack attaches.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Clap.ino - Use clap related functions."));

    Serial.println(F("Calling disableClapEvents()"));
    mip.disableClapEvents();
    bool isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - fail"));
    }
    else
    {
        Serial.println(F("false - pass"));
    }

    Serial.println(F("Calling writeClapDelay(501)"));
    mip.writeClapDelay(501);
    uint16_t delay = mip.readClapDelay();
    Serial.print(F("readClapDelay() returns "));
    Serial.println(delay);

    Serial.println(F("Calling enableClapEvents()"));
    mip.enableClapEvents();
    isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - pass"));
    }
    else
    {
        Serial.println(F("false - fail"));
    }

    Serial.println();
    Serial.println(F("Waiting for clap events!"));
}

void loop()
{
    while (mip.availableClapEvents() > 0)
    {
        uint8_t clapCount = mip.readClapEvent();
        Serial.print(F("Detected "));
            Serial.print(clapCount);
            Serial.println(F(" claps"));
    }
}
```


---
### disableClapEvents()
```void disableClapEvents()```
#### Description
Disables the MiP robot's ability to detect user claps.

#### Parameters
None

#### Returns
None

#### Notes
* The MiP robot defaults to having the clap detection feature disabled after the ProMini Pack attaches.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Clap.ino - Use clap related functions."));

    Serial.println(F("Calling disableClapEvents()"));
    mip.disableClapEvents();
    bool isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - fail"));
    }
    else
    {
        Serial.println(F("false - pass"));
    }

    Serial.println(F("Calling writeClapDelay(501)"));
    mip.writeClapDelay(501);
    uint16_t delay = mip.readClapDelay();
    Serial.print(F("readClapDelay() returns "));
    Serial.println(delay);

    Serial.println(F("Calling enableClapEvents()"));
    mip.enableClapEvents();
    isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - pass"));
    }
    else
    {
        Serial.println(F("false - fail"));
    }

    Serial.println();
    Serial.println(F("Waiting for clap events!"));
}

void loop()
{
    while (mip.availableClapEvents() > 0)
    {
        uint8_t clapCount = mip.readClapEvent();
        Serial.print(F("Detected "));
            Serial.print(clapCount);
            Serial.println(F(" claps"));
    }
}
```


---
### areClapEventsEnabled()
```bool areClapEventsEnabled()```
#### Description
Returns whether the MiP robot's clap detection feature is currently enabled.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully placed in clap detection mode with a previous call to [enableClapEvents()](#enableclapevents).
* **false** if it is not in clap detection mode.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Clap.ino - Use clap related functions."));

    Serial.println(F("Calling disableClapEvents()"));
    mip.disableClapEvents();
    bool isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - fail"));
    }
    else
    {
        Serial.println(F("false - pass"));
    }

    Serial.println(F("Calling writeClapDelay(501)"));
    mip.writeClapDelay(501);
    uint16_t delay = mip.readClapDelay();
    Serial.print(F("readClapDelay() returns "));
    Serial.println(delay);

    Serial.println(F("Calling enableClapEvents()"));
    mip.enableClapEvents();
    isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - pass"));
    }
    else
    {
        Serial.println(F("false - fail"));
    }

    Serial.println();
    Serial.println(F("Waiting for clap events!"));
}

void loop()
{
    while (mip.availableClapEvents() > 0)
    {
        uint8_t clapCount = mip.readClapEvent();
        Serial.print(F("Detected "));
            Serial.print(clapCount);
            Serial.println(F(" claps"));
    }
}
```


---
### writeClapDelay()
```void writeClapDelay(uint16_t delay)```
#### Description
Sets the expected delay between user claps.

#### Parameters
* **delay** is the new delay to be used for clap detection.

#### Returns
Nothing

#### Notes
* *I don't know what the units are for this setting.  Maybe milliseconds?*
* The MiP defaults to having the clap delay set to 500 after the application first connects.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Clap.ino - Use clap related functions."));

    Serial.println(F("Calling disableClapEvents()"));
    mip.disableClapEvents();
    bool isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - fail"));
    }
    else
    {
        Serial.println(F("false - pass"));
    }

    Serial.println(F("Calling writeClapDelay(501)"));
    mip.writeClapDelay(501);
    uint16_t delay = mip.readClapDelay();
    Serial.print(F("readClapDelay() returns "));
    Serial.println(delay);

    Serial.println(F("Calling enableClapEvents()"));
    mip.enableClapEvents();
    isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - pass"));
    }
    else
    {
        Serial.println(F("false - fail"));
    }

    Serial.println();
    Serial.println(F("Waiting for clap events!"));
}

void loop()
{
    while (mip.availableClapEvents() > 0)
    {
        uint8_t clapCount = mip.readClapEvent();
        Serial.print(F("Detected "));
            Serial.print(clapCount);
            Serial.println(F(" claps"));
    }
}
```


---
### readClapDelay()
```uint16_t readClapDelay()```
#### Description
Read the MiP robot's current clap delay.

#### Parameters
None

#### Returns
The current setting of the delay expected between claps.  It defaults to 500.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Clap.ino - Use clap related functions."));

    Serial.println(F("Calling disableClapEvents()"));
    mip.disableClapEvents();
    bool isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - fail"));
    }
    else
    {
        Serial.println(F("false - pass"));
    }

    Serial.println(F("Calling writeClapDelay(501)"));
    mip.writeClapDelay(501);
    uint16_t delay = mip.readClapDelay();
    Serial.print(F("readClapDelay() returns "));
    Serial.println(delay);

    Serial.println(F("Calling enableClapEvents()"));
    mip.enableClapEvents();
    isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - pass"));
    }
    else
    {
        Serial.println(F("false - fail"));
    }

    Serial.println();
    Serial.println(F("Waiting for clap events!"));
}

void loop()
{
    while (mip.availableClapEvents() > 0)
    {
        uint8_t clapCount = mip.readClapEvent();
        Serial.print(F("Detected "));
            Serial.print(clapCount);
            Serial.println(F(" claps"));
    }
}
```


---
### availableClapEvents()
```uint8_t  availableClapEvents()```
#### Description
Returns the number of clap detection events that the library currently has sitting in its queue, ready to be read by calling [readClapEvent()](#readclapevent). The MiP robot must have already been placed in clap detection mode via a call to [enableClapEvents()](#enableclapevents) for new clap detection events to be added to this queue.

#### Parameters
None

#### Returns
* **0** if there are currently no clap detection events ready for reading. Calling [readClapEvent()](#readclapevent) now would retun 0.
* **Non-zero** value indicates the number of readClapEvent() calls that can be made and successfully return a valid clap detection event.

#### Notes
* The maximum number of clap detection events that can be queued up between calls to readClapEvent() is 8. If this count is exceeded, the oldest events will be overwritten.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Clap.ino - Use clap related functions."));

    Serial.println(F("Calling disableClapEvents()"));
    mip.disableClapEvents();
    bool isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - fail"));
    }
    else
    {
        Serial.println(F("false - pass"));
    }

    Serial.println(F("Calling writeClapDelay(501)"));
    mip.writeClapDelay(501);
    uint16_t delay = mip.readClapDelay();
    Serial.print(F("readClapDelay() returns "));
    Serial.println(delay);

    Serial.println(F("Calling enableClapEvents()"));
    mip.enableClapEvents();
    isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - pass"));
    }
    else
    {
        Serial.println(F("false - fail"));
    }

    Serial.println();
    Serial.println(F("Waiting for clap events!"));
}

void loop()
{
    while (mip.availableClapEvents() > 0)
    {
        uint8_t clapCount = mip.readClapEvent();
        Serial.print(F("Detected "));
            Serial.print(clapCount);
            Serial.println(F(" claps"));
    }
}
```


---
### readClapEvent()
```uint8_t  readClapEvent()```
#### Description
Returns a clap detection event from the library's queue. They will be returned in the order that the MiP robot detected them. The MiP robot must have already been placed in clap detection mode via a call to [enableClapEvents()](#enableclapevents) for new clap detection events to be added to this queue.

#### Parameters
None

#### Returns
* **0** if the clap detection event queue is empty. [availableClapEvents()](availableclapevents) would return 0 in this scenario.
* The number of claps detected in this event (ie. 2 for a double clap).


#### Notes
* The maximum number of clap detection events that can be queued up between calls to readClapEvent() is 8. If this count is exceeded, the oldest events will be overwritten.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Clap.ino - Use clap related functions."));

    Serial.println(F("Calling disableClapEvents()"));
    mip.disableClapEvents();
    bool isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - fail"));
    }
    else
    {
        Serial.println(F("false - pass"));
    }

    Serial.println(F("Calling writeClapDelay(501)"));
    mip.writeClapDelay(501);
    uint16_t delay = mip.readClapDelay();
    Serial.print(F("readClapDelay() returns "));
    Serial.println(delay);

    Serial.println(F("Calling enableClapEvents()"));
    mip.enableClapEvents();
    isEnabled = mip.areClapEventsEnabled();
    Serial.print(F("areClapEventsEnabled() returns "));
    if (isEnabled)
    {
        Serial.println(F("true - pass"));
    }
    else
    {
        Serial.println(F("false - fail"));
    }

    Serial.println();
    Serial.println(F("Waiting for clap events!"));
}

void loop()
{
    while (mip.availableClapEvents() > 0)
    {
        uint8_t clapCount = mip.readClapEvent();
        Serial.print(F("Detected "));
            Serial.print(clapCount);
            Serial.println(F(" claps"));
    }
}
```


---
### hasBeenShaken()
```bool hasBeenShaken()```
#### Description
Returns whether the MiP robot has been shaken since the previous call.

#### Parameters
None

#### Returns
* **true** if the MiP robot has been shaken by the user since the previous call.
* **false** otherwise.

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Shake.ino - Detect shakes."));
}

void loop()
{
    if (mip.hasBeenShaken())
    {
        Serial.println(F("Shake detected!"));
    }
}
```


---
### readSoftwareVersion()
```void readSoftwareVersion(MiPSoftwareVersion& software)```
#### Description
Get the version information for the software in the MiP robot.

#### Parameters
* **software** is a MiPSoftwareVersion object to be filled in with the version information of the MiP robot software.
```c++
class MiPSoftwareVersion
{
public:
    // ...
    uint16_t year;
    uint8_t  month;
    uint8_t  day;
    uint8_t  uniqueVersion;
};

```

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.begin(115200);
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("SoftwareHardwareVersion.ino - Use readSoftwareVersion() & readHardwareInfo() functions."));

    MiPSoftwareVersion softwareVersion;
    mip.readSoftwareVersion(softwareVersion);
    Serial.print(F("software version: "));
        Serial.print(softwareVersion.year);
        Serial.print('-');
        Serial.print(softwareVersion.month);
        Serial.print('-');
        Serial.print(softwareVersion.day);
        Serial.print('.');
        Serial.println(softwareVersion.uniqueVersion);

    MiPHardwareInfo hardwareInfo;
    mip.readHardwareInfo(hardwareInfo);
    Serial.println(F("hardware info"));
    Serial.print(F("  voice chip version: "));
        Serial.println(hardwareInfo.voiceChip);
    Serial.print(F("  hardware version: "));
        Serial.println(hardwareInfo.hardware);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### readHardwareInfo()
```void readHardwareInfo(MiPHardwareInfo& hardware)```
#### Description
Get the version information for the hardware in the MiP robot.

#### Parameters
* **hardware** is a MiPHardwareInfo object to be filled in with the version information of the MiP  robot's hardware.
```c++
class MiPHardwareInfo
{
public:
    // ...
    uint8_t voiceChip;
    uint8_t hardware;
};
```

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.begin(115200);
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("SoftwareHardwareVersion.ino - Use readSoftwareVersion() & readHardwareInfo() functions."));

    MiPSoftwareVersion softwareVersion;
    mip.readSoftwareVersion(softwareVersion);
    Serial.print(F("software version: "));
        Serial.print(softwareVersion.year);
        Serial.print('-');
        Serial.print(softwareVersion.month);
        Serial.print('-');
        Serial.print(softwareVersion.day);
        Serial.print('.');
        Serial.println(softwareVersion.uniqueVersion);

    MiPHardwareInfo hardwareInfo;
    mip.readHardwareInfo(hardwareInfo);
    Serial.println(F("hardware info"));
    Serial.print(F("  voice chip version: "));
        Serial.println(hardwareInfo.voiceChip);
    Serial.print(F("  hardware version: "));
        Serial.println(hardwareInfo.hardware);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
```


---
### enableAppMode()
```void enableAppMode()```
#### Description
Place MiP in app mode. The chest LED will turn green.  App mode allows the MiP to interface with applications running on either the UART port or via Bluetooth.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### enableCageMode()
```void enableCageMode()```
#### Description
Place MiP in cage mode.  The chest LED will turn white and the MiP will try to escape from you.  Place your hand in front of MiP's eyes to block its path.  MiP moves faster as the game progesses and makes victory sounds if it escapes from you.  To restart the game, clap twice.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### enableDanceMode()
```void enableDanceMode()```
#### Description
Place MiP in dance mode.  The chest LED will turn turquoise and MiP will dance around and play music.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### enableStackMode()
```void enableStackMode()```
#### Description
Place MiP in stack mode.  The chest LED will turn pink and MiP will play the stack game with you.  You will have only a few seconds to stack a heavy object or many light objects on top of the tray. If it can balance correctly, MiP will add additional time for you to stack. If you fail to reset the timer and the clock runs out, MiP spins around, tossing all the stacked items off the tray.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### enableTrickMode()
```void enableTrickMode()```
#### Description
Place MiP in trick mode.  The chest LED will flash red.  While the chest LED is flashing MiP will record your hand gestures.  MiP will signal each successful gesture by saying "OK" and lighting up its eyes.  You can record up to 50 gestures.  When complete, clap twice and MiP will replay the movements given by your gestures.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### enableRoamMode()
```void enableRoamMode()```
#### Description
Place MiP in roam mode.  The chest LED will turn yellow and roam freely, avoiding obstacles detected by the IR sensors.

#### Parameters
None

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### isAppModeEnabled()
```bool isAppModeEnabled()```
#### Description
Returns whether the MiP is in app mode.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully placed in app mode with a previous call to [enableAppMode()](#enableappmode).
* **false** if it is not in app mode.

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### isCageModeEnabled()
```bool isCageModeEnabled()```
#### Description
Returns whether the MiP is in cage mode.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully placed in cage mode with a previous call to [enableCageMode()](#enablecagemode).
* **false** if it is not in cage mode.

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### isDanceModeEnabled()
```bool isDanceModeEnabled()```
#### Description
Returns whether the MiP is in dance mode.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully placed in dance mode with a previous call to [enableDanceMode()](#enabledancemode).
* **false** if it is not in dance mode.

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```

---
### isStackModeEnabled()
```bool isStackModeEnabled()```
#### Description
Returns whether the MiP is in stack mode.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully placed in stack mode with a previous call to [enableStackMode()](#enablestackmode).
* **false** if it is not in stack mode.

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### isTrickModeEnabled()
```bool isTrickModeEnabled()```
#### Description
Returns whether the MiP is in trick mode.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully placed in trick mode with a previous call to [enableTrickMode()](#enabletrickmode).
* **false** if it is not in trick mode.

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```


---
### isRoamModeEnabled()
```bool isRoamModeEnabled()```
#### Description
Returns whether the MiP is in roam mode.

#### Parameters
None

#### Returns
* **true** if the MiP robot was successfully placed in roam mode with a previous call to [enableRoamMode()](#enableroammode).
* **false** if it is not in roam mode.

#### Example
```c++
#include <mip.h>

static MiP         mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  delay(delayPeriod);
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));

  mip.enableDanceMode();
  delay(delayPeriod);
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));

  mip.enableStackMode();
  delay(delayPeriod);
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));

  mip.enableTrickMode();
  delay(delayPeriod);
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));

  mip.enableRoamMode();
  delay(delayPeriod);
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  mip.stop();                         // MiP will not switch out of roaming mode while moving.

  mip.enableAppMode();
  delay(delayPeriod);
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
}
```



---
### setUserData()
```void setUserData(uint8_t addressOffset, uint8_t userData)```
#### Description
Stores one byte of user data to the offset specified.  Valid offsets are 0x00-0x0F.

#### Parameters
* **addressOffset** is the address offset at which to write the user's data.
* **userData** is the one byte of data to be written to EEPROM.

#### Returns
Nothing

#### Example
```c++
#include <mip.h>

MiP           mip;
const uint8_t eepromAddressOffset = 0x00;     // Use an offset between 0x00 and 0x0F.
uint8_t       secretPassword = 0x0D;          // Try different hex values here to see them stored and
                                              // recovered from EEPROM.
void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("ReadWriteEeprom.ino - Writes data to EEPROM and reads it back."));

  Serial.print(F("Original password: "));
  Serial.println(secretPassword, HEX);

  // Power-off the MiP, comment out this line, recompile and load to the ProMini-Pack to see EEPROM
  // data preserved across power cycles.
  mip.setUserData(eepromAddressOffset, secretPassword);

  // "Scramble" the secret password.
  secretPassword = 0xFF;
  Serial.print(F("Scrambled password: "));
  Serial.println(secretPassword, HEX);

  Serial.print(F("Recovered password: "));
  Serial.print(mip.getUserData(eepromAddressOffset), HEX);
}

void loop() {
}
```



---
### getUserData()
```uint8_t getUserData(uint8_t addressOffset)```
#### Description
Reads one byte of user data from the address offset specified.  Valid addresses are 0x00-0x0F.

#### Parameters
* **addressOffset** is the address at which to read stored data.

#### Returns
* The contents of the EEPROM read from the given address offset.

#### Example
```c++
#include <mip.h>

MiP           mip;
const uint8_t eepromAddressOffset = 0x00;     // Use an offset between 0x00 and 0x0F.
uint8_t       secretPassword = 0x0D;          // Try different hex values here to see them stored and
                                              // recovered from EEPROM.
void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("ReadWriteEeprom.ino - Writes data to EEPROM and reads it back."));

  Serial.print(F("Original password: "));
  Serial.println(secretPassword, HEX);

  // Power-off the MiP, comment out this line, recompile and load to the ProMini-Pack to see EEPROM
  // data preserved across power cycles.
  mip.setUserData(eepromAddressOffset, secretPassword);

  // "Scramble" the secret password.
  secretPassword = 0xFF;
  Serial.print(F("Scrambled password: "));
  Serial.println(secretPassword, HEX);

  Serial.print(F("Recovered password: "));
  Serial.print(mip.getUserData(eepromAddressOffset), HEX);
}

void loop() {
}
```
