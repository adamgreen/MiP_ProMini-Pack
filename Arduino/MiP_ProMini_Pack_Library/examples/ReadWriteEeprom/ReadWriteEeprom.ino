/* Copyright (C) 2018  Samuel Trassare (https://github.com/tiogaplanet)

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
    enableAppMode()
    enableCageMode()
    enableDanceMode()
    enableStackMode()
    enableTrickMode()
    enableRoamMode()
    isAppModeEnabled()
    isCageModeEnabled()
    isDanceModeEnabled()
    isStackModeEnabled()
    isTrickModeEnabled()
    isRoamModeEnabled()
*/
#include <mip.h>

MiP         mip;
const byte eepromAddress = 0x20;
byte secretPassword = 0x12;

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("ReadWriteEeprom.ino - Writes data to EEPROM and reads it back."));

  mip.setUserData(eepromAddress, secretPassword);

  // "Scramble" the secret password.
  secretPassword = 0x00;
  Serial.print(F("Scrambled password: "));
  Serial.println(secretPassword);

  //mip.getUserData(eepromAddress, secretPassword);
  Serial.print(F("Recovered password: "));
  Serial.print(secretPassword);
}

void loop() {
}
