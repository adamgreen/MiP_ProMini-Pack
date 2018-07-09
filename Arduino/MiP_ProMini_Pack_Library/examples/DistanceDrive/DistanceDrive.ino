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
    distanceDrive()
*/
#include <mip.h>

MiP     mip;

void setup() {
  bool connectResult = mip.begin();
  if (!connectResult) {
    Serial.println(F("Failed connecting to MiP!"));
    return;
  }

  Serial.println(F("DistanceDrive.ino - Use distanceDrive function. Drive forward, turn 360 degrees in each direction and backward."));

  // Queue up multiple commands to run in sequence.
  mip.distanceDrive(MIP_DRIVE_FORWARD, 30, MIP_TURN_RIGHT, 0);
  mip.distanceDrive(MIP_DRIVE_FORWARD, 0, MIP_TURN_LEFT, 360);
  mip.distanceDrive(MIP_DRIVE_FORWARD, 0, MIP_TURN_RIGHT, 360);
  mip.distanceDrive(MIP_DRIVE_BACKWARD, 30, MIP_TURN_RIGHT, 0);

  Serial.println();
  Serial.println(F("Sample done."));
}

void loop() {
}

