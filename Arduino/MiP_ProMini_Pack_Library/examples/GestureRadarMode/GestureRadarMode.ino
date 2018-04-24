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
    enableRadarMode();
    disableRadarMode();
    enableGestureMode();
    disableGestureMode();
    isRadarModeEnabled();
    isGestureModeEnabled();
    isGestureAndRadarModeDisabled();
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
