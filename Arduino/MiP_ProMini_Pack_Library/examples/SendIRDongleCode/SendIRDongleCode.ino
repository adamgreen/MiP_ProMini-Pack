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

// Try different values for transmission power (0x01 - 0x78)
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

  Serial.println(F("SendIRDongleCode.ino - Send code to another MiP using IR."));
}

void loop() {
  uint16_t dongleCode;

  char formattedOutput[14];
  
  // Try different codes for dongleCode.
  dongleCode = 0x45;
  dongleCode <<= 8;
  dongleCode |= 0x67;

  sprintf(formattedOutput, "Sending 0x%04X", dongleCode);

  Serial.println(formattedOutput);

  mip.sendIRDongleCode(dongleCode, MIP_IR_TX_POWER);

  delay(1000);
}
