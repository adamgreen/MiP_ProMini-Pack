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
    getSoftwareVersion()
    getHardwareInfo()
*/
#include <mip.h>

MiP     mip;

void setup()
{
    mip.begin();

    PRINTLN(F("GetSoftwareHardwareVersion.ino - Use getSoftwareVersion() & getHardwareInfo() functions."));

    MiPSoftwareVersion softwareVersion;
    int result = mip.getSoftwareVersion(&softwareVersion);
    PRINT(F("software version: "));
        PRINT(softwareVersion.year);
        PRINT('-');
        PRINT(softwareVersion.month);
        PRINT('-');
        PRINT(softwareVersion.day);
        PRINT('.');
        PRINTLN(softwareVersion.uniqueVersion);

    MiPHardwareInfo hardwareInfo;
    result = mip.getHardwareInfo(&hardwareInfo);
    PRINTLN(F("hardware info"));
    PRINT(F("  voice chip version: "));
        PRINTLN(hardwareInfo.voiceChip);
    PRINT(F("  hardware version: "));
        PRINTLN(hardwareInfo.hardware);

    mip.end();
}

void loop()
{
}
