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
    enableIRRemoteControl()
    disableIRRemoteControl()
    isIRRemoteControlEnabled()
*/

#include <mip.h>

MiP mip;

// Load this sketch on a pair of MiPs facing each other and use a different MIP_ID_NO for each.
#define MIP_ID_NO       0x10
#define MIP_IR_TX_POWER 0x78

void setup() {
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableIRRemoteControl.ino - Enable your MiP to be controlled using IR."));

  if (!mip.isIRRemoteControlEnabled())
    Serial.println(F("IR remote control is disabled."));

  mip.enableIRRemoteControl();

  if (mip.isIRRemoteControlEnabled())
    Serial.println(F("IR remote control is enabled."));

  delay(3000);

  mip.disableIRRemoteControl();

  if (!mip.isIRRemoteControlEnabled())
    Serial.println(F("IR remote control is disabled."));
}

void loop() {}
