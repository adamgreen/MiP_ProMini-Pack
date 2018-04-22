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
    getLatestRadarNotification()
*/
#include <mip.h>

// Pass false into MiP contructor to enable notifications.
MiP     mip(false);

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
    PRINTLN(F("Radar.ino - Use getLatestRadarNotification() function.\n"
              "Program should end once you place your handle <10cm from MiP's face."));

    MiPRadarNotification radar;
    int result = mip.setGestureRadarMode(MIP_RADAR);
    MIP_PRINT_ERRORS(result);
    do
    {
        result = mip.getLatestRadarNotification(radar);
    } while (result != MIP_ERROR_NONE || radar.radar != MIP_RADAR_0CM_10CM);
    PRINTLN(F("Hand detected. Shutting down."));

    PRINTLN();
    PRINTLN(F("Sample done."));
}

void loop()
{
}
