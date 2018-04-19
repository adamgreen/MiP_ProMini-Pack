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
    mipDistanceDrive()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int result = -1;

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("DistanceDrive.ino - Use mipDistanceDrive function.\n"
                 "Drive forward, turn 360 degrees in each direction and backward.\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    // Queue up multiple commands to run in sequence.
    result = mipDistanceDrive(g_pMiP, MIP_DRIVE_FORWARD, 30, MIP_TURN_RIGHT, 0);
    // Don't queue up multiple commands too fast.
    delay(500);
    result = mipDistanceDrive(g_pMiP, MIP_DRIVE_FORWARD, 0, MIP_TURN_LEFT, 360);
    delay(500);
    result = mipDistanceDrive(g_pMiP, MIP_DRIVE_FORWARD, 0, MIP_TURN_RIGHT, 360);
    delay(500);
    result = mipDistanceDrive(g_pMiP, MIP_DRIVE_BACKWARD, 30, MIP_TURN_RIGHT, 0);
    delay(500);

    mipUninit(g_pMiP);
}

void loop()
{
}
