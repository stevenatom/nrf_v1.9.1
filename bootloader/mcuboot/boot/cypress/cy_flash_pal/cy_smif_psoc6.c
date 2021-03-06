/***************************************************************************//**
* \file cy_smif_psoc6.c
* \version 1.0
*
* \brief
*  This is the source file of external flash driver adoption layer between PSoC6
*  and standard MCUBoot code.
*
********************************************************************************
* \copyright
*
* (c) 2020, Cypress Semiconductor Corporation
* or a subsidiary of Cypress Semiconductor Corporation. All rights
* reserved.
*
* This software, including source code, documentation and related
* materials ("Software"), is owned by Cypress Semiconductor
* Corporation or one of its subsidiaries ("Cypress") and is protected by
* and subject to worldwide patent protection (United States and foreign),
* United States copyright laws and international treaty provisions.
* Therefore, you may use this Software only as provided in the license
* agreement accompanying the software package from which you
* obtained this Software ("EULA").
*
* If no EULA applies, Cypress hereby grants you a personal, non-
* exclusive, non-transferable license to copy, modify, and compile the
* Software source code solely for use in connection with Cypress?s
* integrated circuit products. Any reproduction, modification, translation,
* compilation, or representation of this Software except as specified
* above is prohibited without the express written permission of Cypress.
*
* Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO
* WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,
* BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
* PARTICULAR PURPOSE. Cypress reserves the right to make
* changes to the Software without notice. Cypress does not assume any
* liability arising out of the application or use of the Software or any
* product or circuit described in the Software. Cypress does not
* authorize its products for use in any products where a malfunction or
* failure of the Cypress product may reasonably be expected to result in
* significant property damage, injury or death ("High Risk Product"). By
* including Cypress's product in a High Risk Product, the manufacturer
* of such system or application assumes all risk of such use and in doing
* so agrees to indemnify Cypress against all liability.
*
******************************************************************************/
#include "string.h"
#include "stdlib.h"
#include "stdbool.h"

#ifdef MCUBOOT_HAVE_ASSERT_H
#include "mcuboot_config/mcuboot_assert.h"
#else
#include <assert.h>
#endif

#include "flash_map_backend/flash_map_backend.h"
#include <sysflash/sysflash.h>

#include "cy_device_headers.h"
#include "cy_smif_psoc6.h"
#include "cy_flash.h"
#include "cy_syspm.h"

#include "flash_qspi.h"

#define PSOC6_WR_SUCCESS                    (0)
#define PSOC6_WR_ERROR_INVALID_PARAMETER    (1)
#define PSOC6_WR_ERROR_FLASH_WRITE          (2)

#define PSOC6_FLASH_ERASE_BLOCK_SIZE	CY_FLASH_SIZEOF_ROW /* PSoC6 Flash erases by Row */

int psoc6_smif_read(const struct flash_area *fap,
                                        off_t addr,
                                        void *data,
                                        size_t len)
{
    int rc = -1;
    cy_stc_smif_mem_config_t *cfg;
    cy_en_smif_status_t st;
    uint32_t address;

    cfg = qspi_get_memory_config(FLASH_DEVICE_GET_EXT_INDEX(fap->fa_device_id));

    address = addr - CY_SMIF_BASE_MEM_OFFSET;

    st = Cy_SMIF_MemRead(qspi_get_device(), cfg, address, data, len, qspi_get_context());
    if (st == CY_SMIF_SUCCESS) {
        rc = 0;
    }
    return rc;
}

int psoc6_smif_write(const struct flash_area *fap,
                                        off_t addr,
                                        const void *data,
                                        size_t len)
{
    int rc = -1;
    cy_en_smif_status_t st;
    cy_stc_smif_mem_config_t *cfg;
    uint32_t address;

    cfg =  qspi_get_memory_config(FLASH_DEVICE_GET_EXT_INDEX(fap->fa_device_id));

    address = addr - CY_SMIF_BASE_MEM_OFFSET;

    st = Cy_SMIF_MemWrite(qspi_get_device(), cfg, address, data, len, qspi_get_context());
    if (st == CY_SMIF_SUCCESS) {
        rc = 0;
    }
    return rc;
}

int psoc6_smif_erase(off_t addr, size_t size)
{
    int rc = -1;
    cy_en_smif_status_t st;
    uint32_t address;

    /* It is erase sector-only
     *
     * There is no power-safe way to erase flash partially
     * this leads upgrade slots have to be at least
     * eraseSectorSize far from each other;
     */
    cy_stc_smif_mem_config_t *memCfg = qspi_get_memory_config(0);

    address = (addr - CY_SMIF_BASE_MEM_OFFSET ) & ~((uint32_t)(memCfg->deviceCfg->eraseSize - 1u));

    (void)size;

    st = Cy_SMIF_MemEraseSector(qspi_get_device(),
                                    memCfg,
                                    address,
                                    memCfg->deviceCfg->eraseSize,
                                    qspi_get_context());
    if (st == CY_SMIF_SUCCESS) {
        rc = 0;
    }
    return rc;
}
