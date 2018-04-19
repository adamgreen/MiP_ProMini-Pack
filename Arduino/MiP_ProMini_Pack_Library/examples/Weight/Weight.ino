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
    mipGetWeight()
    mipGetLatestWeightNotification()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int result = -1;

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("Weight.ino - Use weight update functions.\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    MiPWeight weight;
    result = mipGetWeight(g_pMiP, &weight);
    PRINT("weight = ");
        PRINTLN(weight.weight);
    PRINTLN("Waiting for next weight update.");
    while (MIP_ERROR_NONE != mipGetLatestWeightNotification(g_pMiP, &weight))
    {
    }
    PRINT("weight = ");
        PRINTLN(weight.weight);

    mipUninit(g_pMiP);
}

void loop()
{
}
