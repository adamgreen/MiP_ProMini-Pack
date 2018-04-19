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
    stop()
*/
#include <mip.h>

MiP     mip;

void setup()
{
    mip.begin();

    PRINTLN(F("Stop.ino - Use stop() function.\n"
              "Interrupt a 360 degree turn with stop()."));

    int result = mip.turnLeft(360, 6);
    delay(1000);
    result = mip.stop();
    delay(1000);

    mip.end();
}

void loop()
{
}
