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
    mipEnableClap()
    mipSetClapDelay()
    mipGetClapSettings()
    mipGetLatestClapNotification()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int result = -1;

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("Clap.ino - Use clap related functions.\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    MiPClapSettings settings;
    result = mipGetClapSettings(g_pMiP, &settings);
    PRINTLN("Initial clap settings.");
    printClapSettings(&settings);

    // Modify clap settings.
    // NOTE: Need some delay between settings or second one will be dropped.
    result = mipEnableClap(g_pMiP, MIP_CLAP_ENABLED);
    delay(1000);
    result = mipSetClapDelay(g_pMiP, 501);

    result = mipGetClapSettings(g_pMiP, &settings);
    PRINTLN("Updated clap settings.");
    printClapSettings(&settings);

    PRINTLN("Waiting for user to clap.");
    MiPClap clap;
    while (MIP_ERROR_NONE != mipGetLatestClapNotification(g_pMiP, &clap))
    {
    }
    PRINT("Detected ");
        PRINT(clap.count);
        PRINTLN(" claps");

    mipUninit(g_pMiP);
}

void loop()
{
}

static void printClapSettings(const MiPClapSettings* pSettings)
{
    PRINT("  Enabled = ");
        PRINTLN(pSettings->enabled ? "ON" : "OFF");
    PRINT("    Delay = ");
        PRINTLN(pSettings->delay);
}
