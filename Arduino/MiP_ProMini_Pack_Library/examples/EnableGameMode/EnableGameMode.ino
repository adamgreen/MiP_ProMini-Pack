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

MiP mip;

int delayPeriod = 10000;              // Use short delays for bench testing with serial monitor
                                      // or long delays to see it in action.

void setup() {
  // First need to initialize the serial connection with the MiP.
  bool connectResult = mip.begin();
  if (!connectResult)
  {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("EnableGameMode.ino - Cycles through each mode available."));

  delay(500);
}

void loop() {
  mip.enableCageMode();
  if (mip.isCageModeEnabled()) Serial.println(F("Cage mode enabled."));
  delay(delayPeriod);

  mip.enableDanceMode();
  if (mip.isDanceModeEnabled()) Serial.println(F("Dance mode enabled."));
  delay(delayPeriod);

  mip.enableStackMode();
  if (mip.isStackModeEnabled()) Serial.println(F("Stack mode enabled."));
  delay(delayPeriod);

  mip.enableTrickMode();
  if (mip.isTrickModeEnabled()) Serial.println(F("Trick mode enabled."));
  delay(delayPeriod);

  mip.enableRoamMode();
  if (mip.isRoamModeEnabled()) Serial.println(F("Roam mode enabled."));
  delay(delayPeriod);

  mip.enableAppMode();
  if (mip.isAppModeEnabled()) Serial.println(F("App mode enabled."));
  delay(delayPeriod);
}
