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


#include "val_test_common.h"

/*  Publish these functions to the external world as associated to this test ID */
TBSA_TEST_PUBLISH(CREATE_TEST_ID(TBSA_DEBUG_BASE, 4),
                  CREATE_TEST_TITLE("DPM trusted functionality must implement fuse-controlled Locked State"),
                  CREATE_REF_TAG("R090_TBSA_DEBUG"),
                  entry_hook,
                  test_payload,
                  exit_hook);

void entry_hook(tbsa_val_api_t *val)
{
}

void test_payload(tbsa_val_api_t *val)
{
}

void exit_hook(tbsa_val_api_t *val)
{
}

