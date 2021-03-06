/**
 * Copyright (c) 2019 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */
#include <stdint.h>
#include <stddef.h>

#include "nrf_cc3xx_platform_abort.h"
#include "FreeRTOS.h"

/** @brief Definition of platform handle used during abort in FreeRTOS
 *
 * @note Currently not used
 */
static nrf_cc3xx_platform_abort_handle_t platform_abort_handle;


/** @brief Definition of abort function used for FreeRTOS
 */
static void abort_function(char const * const reason)
{
    configASSERT(!reason);
}


/** @brief Definition of abort API to set in nrf_cc3xx_platform
 */
static const nrf_cc3xx_platform_abort_apis_t apis =
{
    .abort_handle = &platform_abort_handle,
    .abort_fn = abort_function
};

/** @brief Function to initialize the nrf_cc3xx_platform abort APIs.
 */
void nrf_cc3xx_platform_abort_init(void)
{
    nrf_cc3xx_platform_set_abort(&apis);
}
