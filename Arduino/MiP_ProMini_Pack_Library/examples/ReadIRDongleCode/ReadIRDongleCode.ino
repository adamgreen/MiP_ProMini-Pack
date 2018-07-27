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
    readIRDongleCode()
    availableIRCodeEvents()
*/
#include <mip.h>

MiP       mip;
bool      connectResult;

void setup() {
  connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("ReadIRDongleCode.ino - Receive code from another MiP using IR."));
}

void loop() {
  uint32_t receiveCode;

  if (mip.availableIRCodeEvents()) {
    receiveCode = mip.readIRDongleCode();

    Serial.print(F("Received "));
    Serial.print(((receiveCode >> 28) & 0xFF), HEX);
    Serial.print(F(" "));
    Serial.print(((receiveCode >> 16) & 0xFF), HEX);
    Serial.print(F(" "));
    Serial.print(((receiveCode >> 8) & 0xFF), HEX);
    Serial.print(F(" "));
    Serial.print((receiveCode & 0xFF), HEX);
    Serial.println();
  }
}
