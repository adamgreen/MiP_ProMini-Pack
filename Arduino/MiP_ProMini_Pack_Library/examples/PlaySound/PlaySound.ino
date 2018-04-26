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
    beginSoundList()
    addSoundToList()
    playSoundList()
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

    Serial.println(F("PlaySound.ino - Play a few sounds."));

    // Play 2 sounds with 1 second delay between them, repeating them a second time.
    // Play the first at a lower volume than the second.
    mip.beginSoundList();
    mip.addEntryToSoundList(MIP_SOUND_VOLUME_4, 0);
    mip.addEntryToSoundList(MIP_SOUND_ACTION_EATING, 1000);
    mip.addEntryToSoundList(MIP_SOUND_VOLUME_7, 0);
    mip.addEntryToSoundList(MIP_SOUND_ACTION_BURPING, 0);
    mip.addEntryToSoundList(MIP_SOUND_VOLUME_1, 0);
    mip.playSoundList(1);
    
    Serial.println();
    Serial.println(F("Sample done."));
}

void loop()
{
}

