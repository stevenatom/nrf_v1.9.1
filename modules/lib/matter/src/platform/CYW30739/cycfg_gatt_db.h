/***************************************************************************/
/**
 * File Name: cycfg_gatt_db.h
 * Version: 2.1
 *
 * Description:
 * Definitions for constants used in the device's GATT
 *database and function prototypes. This file should
 *not be modified. It was automatically generated by
 * Bluetooth Configurator 2.1.0.2188
 *
 ********************************************************************************
 * Copyright 2020 Cypress Semiconductor Corporation
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the
 *"License"); you may not use this file except in
 *compliance with the License. You may obtain a copy
 *of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in
 *writing, software distributed under the License is
 *distributed on an "AS IS" BASIS, WITHOUT WARRANTIES
 *OR CONDITIONS OF ANY KIND, either express or
 *implied. See the License for the specific language
 *governing permissions and limitations under the
 *License.
 *******************************************************************************/

#if !defined(CYCFG_GATT_DB_H)
#define CYCFG_GATT_DB_H

#include "stdint.h"

#define __UUID_SERVICE_GENERIC_ACCESS 0x1800
#define __UUID_CHARACTERISTIC_DEVICE_NAME 0x2A00
#define __UUID_CHARACTERISTIC_APPEARANCE 0x2A01
#define __UUID_SERVICE_GENERIC_ATTRIBUTE 0x1801
#define __UUID16_CHIPoBLEService 0xFFF6
#define __UUID128_CHIPoBLEChar_C1 0x11, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18
#define __UUID128_CHIPoBLEChar_C2 0x12, 0x9D, 0x9F, 0x42, 0x9C, 0x4F, 0x9F, 0x95, 0x59, 0x45, 0x3D, 0x26, 0xF5, 0x2E, 0xEE, 0x18
#define __UUID_DESCRIPTOR_CLIENT_CHARACTERISTIC_CONFIGURATION 0x2902

/* Service Generic Access */
#define HDLS_GAP 0x01
/* Characteristic Device Name */
#define HDLC_GAP_DEVICE_NAME 0x02
#define HDLC_GAP_DEVICE_NAME_VALUE 0x03
/* Characteristic Appearance */
#define HDLC_GAP_APPEARANCE 0x04
#define HDLC_GAP_APPEARANCE_VALUE 0x05

/* Service Generic Attribute */
#define HDLS_GATT 0x06

/* Service CHIP Service */
#define HDLS_CHIP_SERVICE 0x07

/* Characteristic CHIP C1 */
#define HDLC_CHIP_SERVICE_CHAR_C1 0x08
#define HDLC_CHIP_SERVICE_CHAR_C1_VALUE 0x09

/* Characteristic CHIP C2 */
#define HDLC_CHIP_SERVICE_CHAR_C2 0x0A
#define HDLC_CHIP_SERVICE_CHAR_C2_VALUE 0x0B

/* Descriptor Client Characteristic Configuration */
#define HDLD_CHIP_SERVICE_RX_CLIENT_CHAR_CONFIG 0x0C

/* External Lookup Table Entry */
typedef struct
{
    uint16_t handle;
    uint16_t max_len;
    uint16_t cur_len;
    uint8_t * p_data;
} gatt_db_lookup_table_t;

#define kMaxDeviceNameLength 32

/* External definitions */
extern const uint8_t gatt_database[];
extern const uint16_t gatt_database_len;
extern gatt_db_lookup_table_t app_gatt_db_ext_attr_tbl[];
extern const uint16_t app_gatt_db_ext_attr_tbl_size;
extern uint8_t app_gap_device_name[kMaxDeviceNameLength];
extern const uint16_t app_gap_device_name_len;
extern uint8_t app_gap_appearance[];
extern const uint16_t app_gap_appearance_len;
extern uint8_t app_chip_service_char_tx_client_char_config[];
extern const uint16_t app_chip_service_char_tx_client_char_config_len;

#endif /* CYCFG_GATT_DB_H */
