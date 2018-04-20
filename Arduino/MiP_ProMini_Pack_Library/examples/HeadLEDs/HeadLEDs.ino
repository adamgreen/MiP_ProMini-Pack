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
/* Example used in following API documentation:
    setHeadLEDs()
    getHeadLEDs()
*/
#include <mip.h>

MiP     mip;

void setup()
{
    int connectResult = mip.begin();
    if (connectResult != 0)
    {
        Serial.begin(115200);
        Serial.print(F("Failed connecting to MiP! Error="));
            Serial.println(connectResult);
        return;
    }

    // Use PRINT() & PRINTLN() instead of Serial.print() & Serial.println() so that output will always be
    // sent to the PC and not to the MiP by mistake.
    PRINTLN(F("HeadLEDs.ino - Use head LED functions.\n"
              "Should set each head LED to different state."));
    int result = mip.setHeadLEDs(MIP_HEAD_LED_OFF, MIP_HEAD_LED_ON, MIP_HEAD_LED_BLINK_SLOW, MIP_HEAD_LED_BLINK_FAST);

    delay(4000);

    MiPHeadLEDs headLEDs;
    result = mip.getHeadLEDs(headLEDs);
    PRINTLN(F("Head LEDs"));
    PRINT(F("led1: "));
        printLEDString(headLEDs.led1);
    PRINT(F("led2: "));
        printLEDString(headLEDs.led2);
    PRINT(F("led3: "));
        printLEDString(headLEDs.led3);
    PRINT(F("led4: "));
        printLEDString(headLEDs.led4);

    PRINTLN();
    PRINTLN(F("Sample done."));
}

static void printLEDString(MiPHeadLED led)
{
    switch (led)
    {
    case MIP_HEAD_LED_OFF:
        PRINTLN(F("Off"));
        break;
    case MIP_HEAD_LED_ON:
        PRINTLN(F("On"));
        break;
    case MIP_HEAD_LED_BLINK_SLOW:
        PRINTLN(F("Blink Slow"));
        break;
    case MIP_HEAD_LED_BLINK_FAST:
        PRINTLN(F("Blink Fast"));
        break;
    default:
        PRINTLN();
        break;
    }
}

void loop()
{
}
