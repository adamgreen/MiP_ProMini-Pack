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
    PRINTLN(F("Clap.ino - Use clap related functions."));

    MiPClapSettings settings;
    int result = mip.getClapSettings(settings);
    PRINTLN(F("Initial clap settings."));
    printClapSettings(&settings);

    // Modify clap settings.
    // NOTE: Need some delay between settings or second one will be dropped.
    result = mip.enableClap(MIP_CLAP_ENABLED);
    MIP_PRINT_ERRORS(result);
    delay(1000);
    result = mip.setClapDelay(501);
    MIP_PRINT_ERRORS(result);

    result = mip.getClapSettings(settings);
    MIP_PRINT_ERRORS(result);
    PRINTLN(F("Updated clap settings."));
    printClapSettings(&settings);

    PRINTLN(F("Waiting for user to clap."));
    MiPClap clap;
    while (MIP_ERROR_NONE != mip.getLatestClapNotification(clap))
    {
    }
    PRINT(F("Detected "));
        PRINT(clap.count);
        PRINTLN(F(" claps"));

    PRINTLN();
    PRINTLN(F("Sample done."));
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
