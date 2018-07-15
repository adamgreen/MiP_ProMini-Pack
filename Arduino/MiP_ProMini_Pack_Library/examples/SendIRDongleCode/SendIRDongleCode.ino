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

MiP           mip;

void setup() {
  // Initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    return;
  }

  mip.enableIRRemoteControl();
}

void loop() {
  // put your main code here, to run repeatedly:
  uint8_t dongleCode[4] = {0x00, 0x01, 0x02, 0x03};

  delay(3000);

  mip.sendIRDongleCode(dongleCode, 4, 120);
}
