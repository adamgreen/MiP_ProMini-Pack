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
    mipPlaySound()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int result = -1;

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("PlaySound.ino - Use mipPlaySound().\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    // Play 1 sound.
    const MiPSound sounds1[] = {{MIP_SOUND_ONEKHZ_500MS_8K16BIT, 0}};
    result = mipPlaySound(g_pMiP, sounds1, sizeof(sounds1)/sizeof(sounds1[0]), 0);

    delay(2000);

    // Play 2 sounds with 1 second delay between them, repeating them a second time.
    // Play the first at a lower volume than the second.
    const MiPSound sounds2[] = {{MIP_SOUND_VOLUME_4, 0},            // Play eating sound at half volume.
                                {MIP_SOUND_ACTION_EATING, 1000},
                                {MIP_SOUND_VOLUME_7, 0},            // Play burping sound at full volume.
                                {MIP_SOUND_ACTION_BURPING, 0},
                                {MIP_SOUND_VOLUME_1, 0}};           // Finish by setting volume low.
    result = mipPlaySound(g_pMiP, sounds2, sizeof(sounds2)/sizeof(sounds2[0]), 1);

    delay(7000);

    mipUninit(g_pMiP);
}

void loop()
{
}
