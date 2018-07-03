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
    enableGameMode()
*/
#include <mip.h>

static MiP         mip;

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(30000);
}

void loop() {
  displayGameMode();
  mip.enableCageMode();
  displayGameMode();
  mip.enableTrackingMode();
  displayGameMode();
  mip.enableDanceMode();
  displayGameMode();
  mip.enableDefaultMode();
  displayGameMode();
  mip.enableStackMode();
  displayGameMode();
  mip.enableTrickMode();
  displayGameMode();
  mip.enableRoamMode();
  displayGameMode();
  mip.enableAppMode();
}

void displayGameMode() {
  delay(5000);
  Serial.print(F("Mode: "));
  Serial.println(mip.readGameMode());
  delay(5000);
}

