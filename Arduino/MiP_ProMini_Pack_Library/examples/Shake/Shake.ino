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
    getLatestShakeNotification()
*/
#include <mip.h>

MiP     mip;

void setup()
{
    mip.begin();
    
    PRINTLN(F("Shake.ino - Use getLatestShakeNotification() function."));

    PRINTLN(F("Waiting for user to shake MiP."));
    while (MIP_ERROR_NONE != mip.getLatestShakeNotification())
    {
    }
    PRINTLN(F("Shake detected."));

    mip.end();
}

void loop()
{
}
