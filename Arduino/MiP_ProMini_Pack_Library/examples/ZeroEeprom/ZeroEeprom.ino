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
    setUserData()
    getUserData()
*/
#include <mip.h>

MiP     mip;
uint8_t eepromContents;
char    outputString;

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("ZeroEeprom.ino - Writes zeros to each byte in EEPROM."));

  for (uint8_t i = 0x00; i <= 0x0F; i++) { // Variable i is the EEPROM address offset where we will start writing zeroes.
    mip.setUserData(i, 0x00);
    delay(1000);

    Serial.print("0x2"); Serial.print(i, HEX); Serial.print(": "); Serial.print("0x0"); Serial.println(mip.getUserData(i), HEX);
  }
}

void loop() {
}
