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
    mipGetSoftwareVersion()
    mipGetHardwareInfo()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int                result = -1;
    MiPSoftwareVersion softwareVersion;
    MiPHardwareInfo    hardwareInfo;

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("GetSoftwareHardwareVersion.ino - Use mipGetSoftwareVersion() & mipGetHardwareInfo() functions.\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    result = mipGetSoftwareVersion(g_pMiP, &softwareVersion);
    Serial.print("software version: ");
        Serial.print(softwareVersion.year);
        Serial.print('-');
        Serial.print(softwareVersion.month);
        Serial.print('-');
        Serial.print(softwareVersion.day);
        Serial.print('.');
        Serial.println(softwareVersion.uniqueVersion);

    result = mipGetHardwareInfo(g_pMiP, &hardwareInfo);
    Serial.println("hardware info");
    Serial.print("  voice chip version: ");
        Serial.println(hardwareInfo.voiceChip);
    Serial.print("  hardware version: ");
        Serial.println(hardwareInfo.hardware);

    mipUninit(g_pMiP);
}

void loop()
{
}
