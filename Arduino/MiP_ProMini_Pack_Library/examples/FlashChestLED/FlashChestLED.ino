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
    mipFlashChestLED()
    mipGetChestLED()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int            result = -1;
    const uint8_t  red = 0xff;
    const uint8_t  green = 0x00;
    const uint8_t  blue = 0x00;
    const uint16_t onTime = 1000;    // 1000 msecs / sec
    const uint16_t offTime = 1000;   // 1000 msecs / sec
    MiPChestLED    chestLED;

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("FlashChestLED.ino - Use mipFlashChestLED() and mipGetChestLED() functions.\n"
                 "Should flash chest LED red.\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    result = mipFlashChestLED(g_pMiP, red, green, blue, onTime, offTime);

    delay(4000);

    result = mipGetChestLED(g_pMiP, &chestLED);
    Serial.println("chestLED");
    Serial.print("red: ");
        Serial.println(chestLED.red);
    Serial.print("green: ");
        Serial.println(chestLED.green);
    Serial.print("blue: ");
        Serial.println(chestLED.blue);
    Serial.print("on time: ");
        Serial.print(chestLED.onTime);
        Serial.println(" milliseconds");
    Serial.print("off time: ");
        Serial.print(chestLED.offTime);
        Serial.println(" milliseconds");

    mipUninit(g_pMiP);
}

void loop()
{
}
