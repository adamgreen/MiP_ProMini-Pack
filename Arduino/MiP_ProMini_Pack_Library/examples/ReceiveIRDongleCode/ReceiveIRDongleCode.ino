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
    receiveIRDongleCode()
*/

#include <mip.h>

MiP       mip;
uint8_t   dongleCode[2] = { 0xFF, 0xFF };
MiPIRCode receiveCode;
bool      connectResult;

void setup() {
  connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("ReceiveIRDongleCode.ino - Receive up to four bytes from another MiP using IR."));
}

void loop() {
  Serial.println(F("Looking for data."));

  mip.readIRDongleCode(receiveCode);

  if (receiveCode.received) {
    Serial.print(F("First data byte:  ")); Serial.println(receiveCode.code[0], HEX);
    Serial.print(F("Second data byte: ")); Serial.println(receiveCode.code[1], HEX);
  }
  
  receiveCode.clear();

  delay(3000);
}
