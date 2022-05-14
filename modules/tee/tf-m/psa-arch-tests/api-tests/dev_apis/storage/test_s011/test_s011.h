/** @file
 * Copyright (c) 2019-2020, Arm Limited or its affiliates. All rights reserved.
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
#ifndef _TEST_S011_CLIENT_TESTS_H_
#define _TEST_S011_CLIENT_TESTS_H_

#define test_entry CONCAT(test_entry_,  s011)

#include "test_storage_common.h"

extern const client_test_t s011_storage_test_list[];

int32_t s011_storage_test(caller_security_t caller);

#endif /* _TEST_S011_CLIENT_TESTS_H_ */
