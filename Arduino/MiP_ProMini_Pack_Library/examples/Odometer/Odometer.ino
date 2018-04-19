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
    mipReadOdometer()
    mipResetOdometer()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int result = -1;

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("Odometer.ino - Use mipReadOdometer() and mipResetOdometer.\n"
                 "Read out current odometer reading and reset.\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    float cm = 0.0f;
    result = mipReadOdometer(g_pMiP, &cm);
    Serial.print("MiP has travelled ");
        Serial.print(cm);
        Serial.println(" cm since the last reset.");

    result = mipResetOdometer(g_pMiP);
    delay(1000);

    mipUninit(g_pMiP);
}

void loop()
{
}
