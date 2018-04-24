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
// Program that I demoed at the Seattle Robotics Society meeting on April 21, 2018.
#include <mip.h>

static MiP         g_mip;
static MiPHeadLEDs g_headLEDs;


void setup()
{
    // First need to initialize the serial connection with the MiP.
    bool connectResult = g_mip.begin();
    if (!connectResult)
    {
        Serial.println(F("Failed connecting to MiP!"));
        return;
    }

    Serial.println(F("Seattle Robotics Society Demo for April 21st, 2018."));
}

void loop()
{
    enum States
    {
        RESTART,
        WAITING_TO_STANDUP,
        WAITING_FOR_GESTURE,
        PLAYING_EYE_ANIMATION
    } static state = RESTART;
    static int8_t   animationDirection = 0;
    static uint32_t keyframeStart = 0;
    static uint8_t  frameCount = 0;
    MiPGesture      gesture;

    switch (state)
    {
    case RESTART:
        // Turn all of the eye LEDs off and prepare to wait for the MiP to indicate that is it standing.
        g_mip.disableGestureMode();
        g_headLEDs.clear();
        g_mip.writeHeadLEDs(g_headLEDs);
        state = WAITING_TO_STANDUP;
        break;
        
    case WAITING_TO_STANDUP:
        // Waiting for the robot to indicate that it is standing upright and balancing on its own.
        if (g_mip.isUpright())
        {
            // Switch into gesture mode now that robot is up and balancing.
            g_mip.enableGestureMode();

            // Blink the left and and right most eye LEDs in a fast mode.
            blinkOuterEyes();
            state = WAITING_FOR_GESTURE;
        }
        break;

    case WAITING_FOR_GESTURE:
        if (!g_mip.isUpright())
        {
            // Robot is no longer up and balancing so go back to waiting for it to balance again.
            state = RESTART;
            return;
        }

        // See if the user has moved their hand in front of the robot from left to right or vice versa.
        gesture = g_mip.readGestureEvent();
        if (gesture == MIP_GESTURE_LEFT)
        {
            // The user has moved their hand from right to left.
            animationDirection = 1;
        }
        else if (gesture == MIP_GESTURE_RIGHT)
        {
            // The user has moved their hand from left to right.
            animationDirection = -1;
        }
        else
        {
            // The user hasn't performed either gesture so just exit loop() and check again on next loop.
            return;
        }

        // Start playing eye animation in desired direction.
        g_headLEDs.clear();
        if (animationDirection > 0)
        {
            g_headLEDs.led1 = MIP_HEAD_LED_ON;
        }
        else
        {
            g_headLEDs.led4 = MIP_HEAD_LED_ON;
        }
        keyframeStart = millis();
        frameCount = 5;
        g_mip.writeHeadLEDs(g_headLEDs);
        state = PLAYING_EYE_ANIMATION;
        break;
        
    case PLAYING_EYE_ANIMATION:
        if (!g_mip.isUpright())
        {
            // Robot is no longer up and balancing so go back to waiting for it to balance again.
            state = RESTART;
            return;
        }
        
        // Update to next animation keyframe in 250 milliseconds.
        if (millis() - keyframeStart >= 250)
        {
            keyframeStart = millis();
            animateEyes(animationDirection);
            frameCount--;
            if (frameCount == 0)
            {
                // Animation is done so switch back to the state which just waits for the next user gesture.
                blinkOuterEyes();
                state = WAITING_FOR_GESTURE;
            }
        }
        break;
    }
}

static void blinkOuterEyes()
{
    g_headLEDs.led1 = MIP_HEAD_LED_BLINK_FAST;
    g_headLEDs.led2 = MIP_HEAD_LED_OFF;
    g_headLEDs.led3 = MIP_HEAD_LED_OFF;
    g_headLEDs.led4 = MIP_HEAD_LED_BLINK_FAST;
    g_mip.writeHeadLEDs(g_headLEDs);
}

static void animateEyes(int8_t direction)
{
    if (direction > 0)
    {
        g_headLEDs.led4 = g_headLEDs.led3;
        g_headLEDs.led3 = g_headLEDs.led2;
        g_headLEDs.led2 = g_headLEDs.led1;
        g_headLEDs.led1 = MIP_HEAD_LED_OFF;
    }
    else
    {
        g_headLEDs.led1 = g_headLEDs.led2;
        g_headLEDs.led2 = g_headLEDs.led3;
        g_headLEDs.led3 = g_headLEDs.led4;
        g_headLEDs.led4 = MIP_HEAD_LED_OFF;
    }
    g_mip.writeHeadLEDs(g_headLEDs);
}

