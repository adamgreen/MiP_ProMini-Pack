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
    mipSetHeadLEDs()
    mipGetHeadLEDs()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int         result = -1;
    MiPHeadLEDs headLEDs;

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("HeadLEDs.ino - Use head LED functions.\n"
                 "Should set each head LED to different state.\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    result = mipSetHeadLEDs(g_pMiP, MIP_HEAD_LED_OFF, MIP_HEAD_LED_ON, MIP_HEAD_LED_BLINK_SLOW, MIP_HEAD_LED_BLINK_FAST);

    delay(4000);

    result = mipGetHeadLEDs(g_pMiP, &headLEDs);
    Serial.println("Head LEDs");
    Serial.print("led1: ");
        printLEDString(headLEDs.led1);
    Serial.print("led2: ");
        printLEDString(headLEDs.led2);
    Serial.print("led3: ");
        printLEDString(headLEDs.led3);
    Serial.print("led4: ");
        printLEDString(headLEDs.led4);

    mipUninit(g_pMiP);
}

static void printLEDString(MiPHeadLED led)
{
    switch (led)
    {
    case MIP_HEAD_LED_OFF:
        Serial.println("Off");
        break;
    case MIP_HEAD_LED_ON:
        Serial.println("On");
        break;
    case MIP_HEAD_LED_BLINK_SLOW:
        Serial.println("Blink Slow");
        break;
    case MIP_HEAD_LED_BLINK_FAST:
        Serial.println("Blink Fast");
        break;
    default:
        Serial.println("");
        break;
    }
}

void loop()
{
}
