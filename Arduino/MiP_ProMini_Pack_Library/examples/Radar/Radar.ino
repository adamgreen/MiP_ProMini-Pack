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
    readRadar()
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

