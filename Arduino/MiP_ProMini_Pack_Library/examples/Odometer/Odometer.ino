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
    readOdometer()
    resetOdometer()
*/
#include <mip.h>

MiP     mip;

void setup()
{
    mip.begin();

    PRINTLN(F("Odometer.ino - Use readOdometer() and resetOdometer.\n"
              "Read out current odometer reading and reset."));

    float cm;
    int result = mip.readOdometer(&cm);
    PRINT(F("MiP has travelled "));
        PRINT(cm);
        PRINTLN(F(" cm since the last reset."));

    result = mip.resetOdometer();
    delay(1000);

    mip.end();
}

void loop()
{
}
