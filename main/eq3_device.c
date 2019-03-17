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

#include "esp_log.h"

#include "eq3_device.h"

/* Get the next command off the queue and encode the characteristic parameters */
int setup_command(struct eq3cmd *cmd, struct eq3trvCmd *trvCmd){
    if(cmd != NULL){
        int parm;
        switch(cmd->cmd){
        case EQ3_SETTIME:
            trvCmd->cmd_val[0] = PROP_INFO_QUERY;
            for(parm=0; parm < SET_TIME_BYTES; parm++)
                trvCmd->cmd_val[1 + parm] = cmd->cmdparms[parm];
            trvCmd->cmd_len = 1 + SET_TIME_BYTES;
            break;
        case EQ3_BOOST:
            trvCmd->cmd_val[0] = PROP_BOOST;
            trvCmd->cmd_val[1] = 0x01;
            trvCmd->cmd_len = 2;
            break;
        case EQ3_UNBOOST:
            trvCmd->cmd_val[0] = PROP_BOOST;
            trvCmd->cmd_val[1] = 0x00;
            trvCmd->cmd_len = 2;
            break;
        case EQ3_AUTO:
            trvCmd->cmd_val[0] = PROP_MODE_WRITE;
            trvCmd->cmd_val[1] = 0x00;
            trvCmd->cmd_len = 2;
            break;
        case EQ3_MANUAL:
            trvCmd->cmd_val[0] = PROP_MODE_WRITE;
            trvCmd->cmd_val[1] = 0x40;
            trvCmd->cmd_len = 2;
            break;
        case EQ3_SETTEMP:
            trvCmd->cmd_val[0] = PROP_TEMPERATURE_WRITE;
            trvCmd->cmd_val[1] = cmd->cmdparms[0];
            trvCmd->cmd_len = 2;
            break;
        case EQ3_OFFSET:
            trvCmd->cmd_val[0] = PROP_OFFSET;
            trvCmd->cmd_val[1] = cmd->cmdparms[0];
            trvCmd->cmd_len = 2;
            break;
        case EQ3_LOCK:
            trvCmd->cmd_val[0] = PROP_LOCK;
            trvCmd->cmd_val[1] = 1;
            trvCmd->cmd_len = 2;
            break;
        case EQ3_UNLOCK:
            trvCmd->cmd_val[0] = PROP_LOCK;
            trvCmd->cmd_val[1] = 0;
            trvCmd->cmd_len = 2;
            break;
        default:
            //ESP_LOGI(DEVICE_TAG, "Can't handle that command yet");
            break;
        }
        memcpy(trvCmd->cmd_bleda, cmd->bleda, sizeof(esp_bd_addr_t));
    }
    return 0;
}
