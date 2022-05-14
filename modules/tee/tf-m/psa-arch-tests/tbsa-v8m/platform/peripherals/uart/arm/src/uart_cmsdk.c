/** @file
 * Copyright (c) 2018, Arm Limited or its affiliates. All rights reserved.
 * SPDX-License-Identifier : Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
**/

#include "uart_cmsdk.h"

/**
    @brief    - This function initializes the UART
**/
int32_t uart_init(uart_t *uart)
{
    /* Disable RX and TX interrupts, disable overrun interrupts, enable TX,
    * disable RX */
    uart->CTRL = CMSDK_UART_CTRL_TXEN_Msk;
    /* However, the UART resets to a BAUDDIV of 0, which is invalid. Set it to
    * the minimum valid number. */
    uart->BAUDDIV = 16;

    return 0;
}

/**
    @brief    - This function checks for empty TX FIFO
**/
static int uart_is_tx_empty(uart_t *uart)
{
    /* Note: Check for empty TX FIFO */
    return (!(uart->STATE & CMSDK_UART_STATE_TXBF_Msk));
}

/**
    @brief    - This function checks for empty TX FIFO and writes to FIFO register
**/
int32_t uart_putc(uart_t *uart, const void *data, uint32_t num)
{
    const uint8_t *pdata = (const uint8_t *)data;

    /* ensure TX buffer to be empty */
    while(!uart_is_tx_empty(uart));

    /* write the data (upper 24 bits are reserved) */
    uart->DATA = *pdata;

    return 0;
}
