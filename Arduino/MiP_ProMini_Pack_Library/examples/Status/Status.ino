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
    mipGetStatus()
    mipGetLatestStatusNotification()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int result = -1;

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("Status.ino - Use mipGetStatus() and mipGetLatestStatusNotification().\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    MiPStatus status;
    PRINTLN("Call mipGetStatus()");
    result = mipGetStatus(g_pMiP, &status);
    printStatus(&status);

    PRINTLN("Waiting for next MiP status notification.");
    while (MIP_ERROR_NONE != mipGetLatestStatusNotification(g_pMiP, &status))
    {
    }
    printStatus(&status);

    mipUninit(g_pMiP);
}

void loop()
{
}

static void printStatus(const MiPStatus* pStatus)
{
    PRINT("Battery voltage: ");
        PRINTLN(pStatus->battery);
    PRINT("Position: ");
        PRINTLN(pStatus->position);
}
