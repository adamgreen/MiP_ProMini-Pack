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
    availableGestureEvents()
    readGestureEvent()
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

    Serial.println(F("Gesture.ino - Detect gesture and inform user as they occur."));

    Serial.println(F("Waiting for robot to be standing upright."));
    while (!mip.isUpright())
    {
    }
    mip.enableGestureMode();
}

void loop()
{
    while (mip.availableGestureEvents() > 0)
    {
        MiPGesture gesture = mip.readGestureEvent();
        Serial.print(F("Detected "));
        switch (gesture)
        {
        case MIP_GESTURE_LEFT:
            Serial.println(F("Left gesture!"));
            break;
        case MIP_GESTURE_RIGHT:
            Serial.println(F("Right gesture!"));
            break;
        case MIP_GESTURE_CENTER_SWEEP_LEFT:
            Serial.println(F("Center Sweep Left gesture!"));
            break;
        case MIP_GESTURE_CENTER_SWEEP_RIGHT:
            Serial.println(F("Center Sweep Right gesture!"));
            break;
        case MIP_GESTURE_CENTER_HOLD:
            Serial.println(F("Center Hold gesture!"));
            break;
        case MIP_GESTURE_FORWARD:
            Serial.println(F("Forward gesture!"));
            break;
        case MIP_GESTURE_BACKWARD:
            Serial.println(F("Backward gesture!"));
            break;
        case MIP_GESTURE_INVALID:
            // This shouldn't really happen since mip.availableGestureEvents() returned > 0.
            Serial.println(F("INVALID gesture!"));
            break;
        }
    }
}

