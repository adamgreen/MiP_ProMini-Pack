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
    rawSend()
    rawReceive()
*/
#include <mip.h>

MiP     mip;

void setup()
{
    mip.begin();

    PRINTLN(F("RawSendReceive.ino - Use raw*() functions.\n"
              "Should set chest LED to purple and display MiP firmware revision"));

    // Send 4-byte MiP command to set Chest LED to Purple.
    uint8_t setChestPurple[] = "\x84\xFF\x01\xFF";
    int result = mip.rawSend(setChestPurple, sizeof(setChestPurple)-1);

    // Request the MiP firmware revision information and display it.
    uint8_t getMiPSoftwareVersion[] = "\x14";
    size_t  responseLength = 0;
    uint8_t response[5];
    result = mip.rawReceive(getMiPSoftwareVersion, sizeof(getMiPSoftwareVersion)-1,
                            response, sizeof(response), &responseLength);
    if (result == MIP_ERROR_NONE && responseLength == 5 && response[0] == 0x14)
    {
        PRINT(F("MiP Software Version: "));
            PRINT(response[1] + 2000);
            PRINT('-');
            PRINT(response[2]);
            PRINT('-');
            PRINT(response[3]);
            PRINT(F(" (build #"));
            PRINT(response[4]);
            PRINT(')');
    }

    mip.end();
}

void loop()
{
}
