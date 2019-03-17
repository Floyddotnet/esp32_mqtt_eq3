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

#include "driver/uart.h"

#define UART_TAG "EQ3_UART"

#define BUF_SIZE (1024)

/* Task to handle local UART and accept EQ-3 commands for test/debug */
static void uart_task()
{
    const int uart_num = UART_NUM_0;
    uint8_t cmd_buf[1024] = {0};
    uint16_t cmdidx = 0;

    uart_config_t uart_config = {
            .baud_rate = 115200,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
            .rx_flow_ctrl_thresh = 122,
    };
    //Configure UART1 parameters
    uart_param_config(uart_num, &uart_config);
    //Set UART1 pins(TX: IO4, RX: I05, RTS: IO18, CTS: IO19)
    //uart_set_pin(uart_num, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS);
    //Install UART driver (we don't need an event queue here)
    //In this example we don't even use a buffer for sending data.
    uart_driver_install(uart_num, BUF_SIZE * 2, 0, 0, NULL, 0);

    uint8_t* data = (uint8_t*) malloc(BUF_SIZE);
    while(1) {
        //Read data from UART
        int len = uart_read_bytes(uart_num, data, BUF_SIZE - 1, 20 / portTICK_RATE_MS);
        if(cmdidx + len < 1024){
            int cpylen = 0;
            while(cpylen < len){
                if(data[cpylen] == '\n' || data[cpylen] == '\r'){
                    if(cmdidx > 0 && msgQueue != NULL){
                        uint8_t *newMsg = (uint8_t *)malloc(cmdidx + 1);
                        if(newMsg != NULL){
                            memcpy(newMsg, cmd_buf, cmdidx);
                            newMsg[cmdidx] = 0;
                            ESP_LOGI(UART_TAG, "Send");
                            xQueueSend( msgQueue, (void *)&newMsg, ( TickType_t ) 0 );

                            /* TODO - check if the above failed */

                        }
                    }
                    cmdidx = 0;
                    cpylen++;
                }else{
                    cmd_buf[cmdidx++] = data[cpylen++];
                }
            }
        }
        //Write data back to UART
        uart_write_bytes(uart_num, (const char*) data, len);
    }
}
