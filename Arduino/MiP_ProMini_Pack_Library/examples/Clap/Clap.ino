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
    enableClapEvents()
    disableClapEvents()
    areClapEventsEnabled()
    writeClapDelay(uint16_t delay)
    readClapDelay()
    availableClapEvents()
    readClapEvent()
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
