// ****************************************************************************
//
// EQ-3 Thermostatic Radiator Valve control
//
// ****************************************************************************
//
// by Peter Becker (C) 2019
// derived from gatt_client example from Espressif Systems
// derived from by Paul Tupper (C) 2017
//
// Copyright 2015-2017 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

#include "esp_gatt_defs.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"

#include "eq3_commandqueue.h"
#include "eq3_device.h"

#define CQ_TAG "EQ3_CQ"

#define REQUEUE_RETRY

int setnextcmd(int cmd, int time_s){
    if(nextcmd.running != true){
        nextcmd.cmd = cmd;
        nextcmd.countdown = time_s;
        nextcmd.running = true;
    }else{
        ESP_LOGI(CQ_TAG, "setnextcmd when timer running!");
    }
    return 0;
}


void enqueue_command(struct eq3cmd *newcmd)
{
    struct eq3cmd *qwalk = cmdqueue;
    struct eq3cmd *lastCommandForDevice = NULL;

    if(cmdqueue == NULL)
    {
        cmdqueue = newcmd;
        ESP_LOGI(CQ_TAG, "Add queue head");
    }
    else
    {
        if(memcmp(qwalk->bleda, newcmd->bleda, sizeof(esp_bd_addr_t)) == 0)
            lastCommandForDevice = qwalk;

        while(qwalk->next != NULL)
        {
            qwalk = qwalk->next;
            if(memcmp(qwalk->bleda, newcmd->bleda, sizeof(esp_bd_addr_t)) == 0)
                lastCommandForDevice = qwalk;
        }

        //don't add the same command again if it already is the last command for a specific device
        if(lastCommandForDevice != NULL
                && lastCommandForDevice->cmd == newcmd->cmd
                && memcmp(lastCommandForDevice->cmdparms, newcmd->cmdparms, MAX_CMD_BYTES) == 0)
        {
            ESP_LOGI(CQ_TAG, "Command still pending");
            return;
        }

        qwalk->next = newcmd;
        ESP_LOGI(CQ_TAG, "Add queue end");
     }
}


int command_complete(bool success){
    bool deletehead = false;
    int rc = EQ3_CMD_RETRY;

    if(success == true){
        deletehead = true;
        rc = EQ3_CMD_DONE;
    }
    else if(cmdqueue == NULL) {
        rc = EQ3_CMD_DONE;
    }
    else{
        /* Command failed - retry if there are any retries left */

        /* Normal operation - retry the same command until all attempts are exhausted
         * OR
         * define REQUEUE_RETRY to push the command to the end of the list to retry once all other currently queued commands are complete. */
        if(--cmdqueue->retries <= 0){
            deletehead = true;
            ESP_LOGE(CQ_TAG, "Command failed - retries exhausted");
            rc = EQ3_CMD_FAILED;
        }else{
#ifdef REQUEUE_RETRY
            ESP_LOGE(CQ_TAG, "Command failed - requeue for retry");
            /* If there are no other queued commands just retry this one */
            if(cmdqueue->next != NULL){
                struct eq3cmd *mvcmd = cmdqueue;
                while(mvcmd->next != NULL)
                    mvcmd = mvcmd->next;
                /* Attach head to tail */
                mvcmd->next = cmdqueue;
                cmdqueue = cmdqueue->next;
                /* Detach head from new tail */
                mvcmd->next->next = NULL;
            }
#else
            ESP_LOGE(CQ_TAG, "Command failed - retry");
#endif
        }
    }
    if(deletehead && cmdqueue != NULL){
        /* Delete this command from the queue */
        struct eq3cmd *delcmd = cmdqueue;
        cmdqueue = cmdqueue->next;
        free(delcmd);
    }
    return rc;
}

