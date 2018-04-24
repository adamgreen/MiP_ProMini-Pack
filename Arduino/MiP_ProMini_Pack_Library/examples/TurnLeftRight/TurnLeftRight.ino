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
    turnLeft()
    turnRight()
*/
#include <mip.h>

MiP     mip;

void setup()
{
    bool connectResult = mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("TurnLeftRight.ino - Use turnLeft & turnRight() functions. Turn 180 degrees to left and then 180 degrees to right."));

    mip.turnLeft(180, 12);
    delay(2000);
    mip.turnRight(180, 12);
    delay(2000);

    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}
