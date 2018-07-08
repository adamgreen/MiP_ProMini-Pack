/* Copyright (C) 2018  Adam Green (https://github.com/adamgreen)

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
    readBatteryVoltage();
    readPosition();
    isOnBack();
    isFaceDown();
    isUpright();
    isPickedUp();
    isHandStanding();
    isFaceDownOnTray();
    isOnBackWithKickstand();
*/
#include <mip.h>

MiP     mip;

void setup() {
  bool connectResult = mip.begin();
  if (!connectResult) {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("Status.ino - Display MiP status as it changes."));
}

void loop() {
  static float       lastBatteryLevel = 0.0f;
  static MiPPosition lastPosition = (MiPPosition) - 1;

  float              currentBatteryLevel = mip.readBatteryVoltage();
  MiPPosition        currentPosition = mip.readPosition();

  if (currentBatteryLevel != lastBatteryLevel) {
    Serial.print(F("Battery: "));
      Serial.print(currentBatteryLevel);
      Serial.println(F("V"));
    lastBatteryLevel = currentBatteryLevel;
  }

  if (currentPosition != lastPosition) {
    if (mip.isOnBack()) {
      Serial.println(F("Position: On Back"));
    }
    if (mip.isFaceDown()) {
      Serial.println(F("Position: Face Down"));
    }
    if (mip.isUpright()) {
      Serial.println(F("Position: Upright"));
    }
    if (mip.isPickedUp()) {
      Serial.println(F("Position: Picked Up"));
    }
    if (mip.isHandStanding()) {
      Serial.println(F("Position: Hand Stand"));
    }
    if (mip.isFaceDownOnTray()) {
      Serial.println(F("Position: Face Down on Tray"));
    }
    if (mip.isOnBackWithKickstand()) {
      Serial.println(F("Position: On Back With Kickstand"));
    }

    lastPosition = currentPosition;
  }
}

