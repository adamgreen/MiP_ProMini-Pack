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
    readSoftwareVersion()
    readHardwareInfo()
*/
#include <mip.h>

MiP     mip;

void setup() {
  bool connectResult = mip.begin();
  if (!connectResult) {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("SoftwareHardwareVersion.ino - Use readSoftwareVersion() & readHardwareInfo() functions."));

  MiPSoftwareVersion softwareVersion;
  mip.readSoftwareVersion(softwareVersion);
  Serial.print(F("software version: "));
  Serial.print(softwareVersion.year);
    Serial.print('-');
    Serial.print(softwareVersion.month);
    Serial.print('-');
    Serial.print(softwareVersion.day);
    Serial.print('.');
    Serial.println(softwareVersion.uniqueVersion);

  MiPHardwareInfo hardwareInfo;
  mip.readHardwareInfo(hardwareInfo);
  Serial.println(F("hardware info"));
  Serial.print(F("  voice chip version: "));
    Serial.println(hardwareInfo.voiceChip);
  Serial.print(F("  hardware version: "));
    Serial.println(hardwareInfo.hardware);

  Serial.println();
  Serial.println(F("Sample done."));
}

void loop() {
}

