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
    sendIRDongleCode()
*/

#include <mip.h>

// Try different values for VALID_DATA_BYTES (0x10, 0x18, 0x20)
#define VALID_DATA_BYTES 0x20
#define MIP_IR_TX_POWER  0x78

MiP  mip;
bool connectResult;

void setup() {
  connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("SendIRDongleCode.ino - Send up to four bytes to another MiP using IR."));
}

void loop() {
  // Don't run if no connection was established. Useful for development when J1 is jumpered.
  if (!connectResult)
    return;

  // Try different values for dongleCode[].
  uint8_t dongleCode[5] = { 0x04, 0x03, 0x02, 0x01 };

  delay(3000);

  mip.sendIRDongleCode(dongleCode, VALID_DATA_BYTES, MIP_IR_TX_POWER);
}
