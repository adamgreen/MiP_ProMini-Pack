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
    continuousDrive()
*/
#include <mip.h>

MiP     mip;

void setup() {
  bool connectResult = mip.begin();
  if (!connectResult) {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("ContinuousDrive.ino - Use continuousDrive() function. Drive forward with right turn and then backward with left turn."));
}

void loop()
{
  static enum States {
    RIGHT_TURN,
    LEFT_TURN,
    DONE
  } state = RIGHT_TURN;
  static uint32_t startTime = millis();
  uint32_t        currentTime = millis();
  uint32_t        elapsedTime = currentTime - startTime;

  switch (state) {
    case RIGHT_TURN:
      if (elapsedTime < 2000) {
        // Drive forward at half-speed and turn right at quarter rate.
        mip.continuousDrive(16, 8);
      } else {
        startTime = currentTime;
        state = LEFT_TURN;
      }
      break;
    case LEFT_TURN:
      if (elapsedTime < 2000) {
        // Drive backward at half-speed and turn left at quarter rate.
        mip.continuousDrive(-16, -8);
      } else {
        Serial.println();
        Serial.println(F("Sample done."));
        state = DONE;
      }
      break;
    default:
      break;
  }
}

