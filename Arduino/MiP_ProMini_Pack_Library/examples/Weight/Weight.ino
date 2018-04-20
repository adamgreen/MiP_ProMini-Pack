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
    getWeight()
    getLatestWeightNotification()
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
    PRINTLN(F("Weight.ino - Use weight update functions."));

    MiPWeight weight;
    int result = mip.getWeight(&weight);
    PRINT(F("weight = "));
        PRINTLN(weight.weight);
    PRINTLN(F("Waiting for next weight update."));
    while (MIP_ERROR_NONE != mip.getLatestWeightNotification(&weight))
    {
    }
    PRINT(F("weight = "));
        PRINTLN(weight.weight);

    mip.end();
}

void loop()
{
}
