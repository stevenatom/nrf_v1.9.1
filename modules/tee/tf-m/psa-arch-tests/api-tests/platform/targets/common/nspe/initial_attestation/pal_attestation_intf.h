/** @file
 * Copyright (c) 2019, Arm Limited or its affiliates. All rights reserved.
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

#ifndef _PAL_ATTESTATION_INTF_H_
#define _PAL_ATTESTATION_INTF_H_

#include "pal_attestation_crypto.h"

enum attestation_function_code {
    PAL_INITIAL_ATTEST_GET_TOKEN        = 0x1,
    PAL_INITIAL_ATTEST_GET_TOKEN_SIZE   = 0x2,
    PAL_INITIAL_ATTEST_VERIFY_TOKEN     = 0x3,
    PAL_INITIAL_ATTEST_COMPUTE_HASH     = 0x4,
    PAL_INITIAL_ATTEST_VERIFY_WITH_PK   = 0x5,
};

int32_t pal_attestation_function(int type, va_list valist);
#endif /* _PAL_ATTESTATION_INTF_H_ */
