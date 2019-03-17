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

#ifndef E3_DEVICE_BLE_ID_H_
#define E3_DEVICE_BLE_ID_H_

/* EQ-3 service identifier */
static esp_gatt_srvc_id_t eq3_service_id = {
        .id = {
                .uuid = {
                        .len = ESP_UUID_LEN_128,
                        .uuid = {.uuid128 = {0x46, 0x70, 0xb7, 0x5b, 0xff, 0xa6, 0x4a, 0x13, 0x90, 0x90, 0x4f, 0x65, 0x42, 0x51, 0x13, 0x3e},},
                },
                .inst_id = 0,
        },
        .is_primary = true,
};

/* EQ-3 characteristic identifier for setting parameters */
static esp_gatt_id_t eq3_char_id = {
        .uuid = {
                .len = ESP_UUID_LEN_128,
                .uuid = {.uuid128 = {0x09, 0xea, 0x79, 0x81, 0xdf, 0xb8, 0x4b, 0xdb, 0xad, 0x3b, 0x4a, 0xce, 0x5a, 0x58, 0xa4, 0x3f},},
        },
        .inst_id = 0,
};

static esp_bt_uuid_t eq3_filter_char_uuid = {
        .len = ESP_UUID_LEN_128,
        .uuid = {.uuid128 = {0x09, 0xea, 0x79, 0x81, 0xdf, 0xb8, 0x4b, 0xdb, 0xad, 0x3b, 0x4a, 0xce, 0x5a, 0x58, 0xa4, 0x3f},},
};

/* EQ-3 characteristic used to notify settings from trv in response to parameter set */
static esp_gatt_id_t eq3_resp_char_id = {
        .uuid = {
                .len = ESP_UUID_LEN_128,
                .uuid = {.uuid128 = {0x2a, 0xeb, 0xe0, 0xf4, 0x90, 0x6c, 0x41, 0xaf, 0x96, 0x09, 0x29, 0xcd, 0x4d, 0x43, 0xe8, 0xd0},},
        },
        .inst_id = 0,
};

static esp_bt_uuid_t eq3_resp_filter_char_uuid = {
        .len = ESP_UUID_LEN_128,
        .uuid = {.uuid128 = {0x2a, 0xeb, 0xe0, 0xf4, 0x90, 0x6c, 0x41, 0xaf, 0x96, 0x09, 0x29, 0xcd, 0x4d, 0x43, 0xe8, 0xd0},},
};

#endif
