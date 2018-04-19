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
    mipRawReceiveNotification()
*/
#include <mip.h>

static MiP* g_pMiP = NULL;

void setup()
{
    int     result = -1;
    size_t  responseLength = 0;
    uint8_t response[MIP_RESPONSE_MAX_LEN];

    g_pMiP = mipInit(NULL);

    Serial.begin(115200);
    Serial.print("RawReceiveNotification.ino - Use mipRawReceiveNotification() functions.\n"
                 "In less than half a minute, a notification should be displayed.\n");
    Serial.end();

    // Connect to first MiP robot discovered.
    result = mipConnectToRobot(g_pMiP, NULL);

    // Wait for first out of band notification to arrive.
    while (MIP_ERROR_EMPTY == mipRawReceiveNotification(g_pMiP, response, sizeof(response), &responseLength))
    {
    }

    // Display notification contents.
    PRINT("notification -> ");
    for (int i = 0 ; i < responseLength ; i++)
    {
        PRINT(response[i], HEX);
        if (i != responseLength - 1)
            PRINT(',');
    }
    PRINTLN();

    mipUninit(g_pMiP);
}

void loop()
{
}
