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
    writeChestLED()
    readChestLED()
*/
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

    Serial.println(F("SetChestLED.ino - Set Chest LED to different colors.\n"));

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

