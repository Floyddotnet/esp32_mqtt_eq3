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

#ifndef E2_COMMANDQUEUE_H_
#define E2_COMMANDQUEUE_H_

#define EQ3_CMD_DONE    0
#define EQ3_CMD_RETRY   1
#define EQ3_CMD_FAILED  2

/* Allow delay of next GATTC command */
struct tmrcmd{
    bool running;
    int cmd;
    int countdown;
};

static struct eq3cmd *cmdqueue = NULL;

static struct tmrcmd nextcmd;

/* Current TRV command being sent to EQ-3 */
static struct eq3trvCmd *trvCmd;

int setnextcmd(int cmd, int time_s);

/* Command complete success/fail acknowledgement */
static int command_complete(bool success);

static void enqueue_command(struct eq3cmd *newcmd);

#endif
