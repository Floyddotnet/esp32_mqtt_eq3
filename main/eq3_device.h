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
#include "esp_bt_defs.h"

#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"

#include "esp_bt_main.h"

#include "eq3_gap.h"

#ifndef E3_DEVICE_H_
#define E3_DEVICE_H_

/* Request ids for TRV */
#define PROP_ID_QUERY            0x00
#define PROP_ID_RETURN           0x01
#define PROP_INFO_RETURN         0x02
#define PROP_INFO_QUERY          0x03
#define PROP_COMFORT_ECO_CONFIG  0x11
#define PROP_OFFSET              0x13
#define PROP_WINDOW_OPEN_CONFIG  0x14
#define PROP_SCHEDULE_QUERY      0x20
#define PROP_SCHEDULE_RETURN     0x21
#define PROP_MODE_WRITE          0x40
#define PROP_TEMPERATURE_WRITE   0x41
#define PROP_COMFORT             0x43
#define PROP_ECO                 0x44
#define PROP_BOOST               0x45
#define PROP_LOCK                0x80

/* Status bits */
#define AUTO                     0x00
#define MANUAL                   0x01
#define AWAY                     0x02
#define BOOST                    0x04
#define DST                      0x08
#define WINDOW                   0x10
#define LOCKED                   0x20
#define UNKNOWN                  0x40
#define LOW_BATTERY              0x80

#define DEVICE_TAG "EQ3_DEVICE"


typedef enum {
    EQ3_BOOST = 0,
    EQ3_UNBOOST,
    EQ3_AUTO,
    EQ3_MANUAL,
    EQ3_ECO,
    EQ3_SETTEMP,
    EQ3_OFFSET,
    EQ3_SETTIME,
    EQ3_LOCK,
    EQ3_UNLOCK,
}eq3_bt_cmd;

#define SET_TIME_BYTES 6
#define MAX_CMD_BYTES 6

struct eq3cmd{
    esp_bd_addr_t bleda;
    eq3_bt_cmd cmd;
    unsigned char cmdparms[MAX_CMD_BYTES];
    int retries;
    struct eq3cmd *next;
};

struct eq3trvCmd {
    uint16_t cmd_len;
    uint8_t cmd_val[20];
    esp_bd_addr_t cmd_bleda;   /* BLE Device Address */
};

/* Get the next command off the queue and encode the characteristic parameters */
int setup_command(struct eq3cmd *cmd, struct eq3trvCmd *trvCmd);

#endif
