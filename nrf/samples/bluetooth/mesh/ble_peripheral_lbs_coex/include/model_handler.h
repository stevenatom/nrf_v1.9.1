/*
 * Copyright (c) 2021 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-Nordic-5-Clause
 */

/**
 * @file
 * @brief Model handler
 */

#ifndef MODEL_HANDLER_H__
#define MODEL_HANDLER_H__

#include <bluetooth/mesh.h>

#ifdef __cplusplus
extern "C" {
#endif

void ble_conn_init(void);

const struct bt_mesh_comp *model_handler_init(void);

#ifdef __cplusplus
}
#endif

#endif /* MODEL_HANDLER_H__ */
