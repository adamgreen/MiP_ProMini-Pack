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
    mipRawSend()
    mipRawReceive()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int     result = -1;
    size_t  responseLength = 0;
    uint8_t response[5];

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("RawSendReceive.ino - Use mipRaw*() functions.\n"
                 "Should set chest LED to purple and display MiP firmware revision\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    // Send 4-byte MiP command to set Chest LED to Purple.
    static const uint8_t setChestPurple[] = "\x84\xFF\x01\xFF";
    result = mipRawSend(g_pMiP, setChestPurple, sizeof(setChestPurple)-1);

    // Request the MiP firmware revision information and display it.
    static const uint8_t getMiPSoftwareVersion[] = "\x14";
    result = mipRawReceive(g_pMiP, getMiPSoftwareVersion, sizeof(getMiPSoftwareVersion)-1,
                                 response, sizeof(response), &responseLength);
    if (result == MIP_ERROR_NONE && responseLength == 5 && response[0] == 0x14)
    {
        Serial.print("MiP Software Version: ");
            Serial.print(response[1] + 2000);
            Serial.print('-');
            Serial.print(response[2]);
            Serial.print('-');
            Serial.print(response[3]);
            Serial.print(" (build #");
            Serial.print(response[4]);
            Serial.println(')');
    }

    mipUninit(g_pMiP);
}

void loop()
{
}
