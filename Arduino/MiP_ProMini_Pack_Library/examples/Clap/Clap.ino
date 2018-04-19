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
    enableClap()
    setClapDelay()
    getClapSettings()
    getLatestClapNotification()
*/
#include <mip.h>

MiP     mip;

void setup()
{
    mip.begin();

    PRINTLN(F("Clap.ino - Use clap related functions."));

    MiPClapSettings settings;
    int result = mip.getClapSettings(&settings);
    PRINTLN(F("Initial clap settings."));
    printClapSettings(&settings);

    // Modify clap settings.
    // NOTE: Need some delay between settings or second one will be dropped.
    result = mip.enableClap(MIP_CLAP_ENABLED);
    delay(1000);
    result = mip.setClapDelay(501);

    result = mip.getClapSettings(&settings);
    PRINTLN(F("Updated clap settings."));
    printClapSettings(&settings);

    PRINTLN(F("Waiting for user to clap."));
    MiPClap clap;
    while (MIP_ERROR_NONE != mip.getLatestClapNotification(&clap))
    {
    }
    PRINT(F("Detected "));
        PRINT(clap.count);
        PRINTLN(F(" claps"));

    mip.end();
}

void loop()
{
}

static void printClapSettings(const MiPClapSettings* pSettings)
{
    PRINT(F("  Enabled = "));
        PRINTLN(pSettings->enabled ? F("ON") : F("OFF"));
    PRINT(F("    Delay = "));
        PRINTLN(pSettings->delay);
}
