/** @file
 * Copyright (c) 2019-2021, Arm Limited or its affiliates. All rights reserved.
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

#include "val_interfaces.h"
#include "val_target.h"
#include "test_c028.h"
#include "test_data.h"

const client_test_t test_c028_crypto_list[] = {
    NULL,
    psa_mac_sign_finish_test,
    NULL,
};

extern  uint32_t g_test_count;



int32_t psa_mac_sign_finish_test(caller_security_t caller __UNUSED)
{
    int32_t               num_checks = sizeof(check1)/sizeof(check1[0]);
    int32_t               i, status;
    size_t                mac_length;
    psa_mac_operation_t   operation = PSA_MAC_OPERATION_INIT;
    psa_key_attributes_t  attributes = PSA_KEY_ATTRIBUTES_INIT;
    psa_key_id_t          key;

    if (num_checks == 0)
    {
        val->print(PRINT_TEST, "No test available for the selected crypto configuration\n", 0);
        return RESULT_SKIP(VAL_STATUS_NO_TESTS);
    }

    /* Initialize the PSA crypto library*/
    status = val->crypto_function(VAL_CRYPTO_INIT);
    TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(1));

    for (i = 0; i < num_checks; i++)
    {
        val->print(PRINT_TEST, "[Check %d] ", i+1);
        val->print(PRINT_TEST, check1[i].test_desc, 0);

        val->crypto_function(VAL_CRYPTO_KEY_ATTRIBUTES_INIT,
                             &attributes);
        val->crypto_function(VAL_CRYPTO_MAC_OPERATION_INIT,
                             &operation);

        /* Setting up the watchdog timer for each check */
        status = val->wd_reprogram_timer(WD_CRYPTO_TIMEOUT);
        TEST_ASSERT_EQUAL(status, VAL_STATUS_SUCCESS, TEST_CHECKPOINT_NUM(2));

        /* Setup the attributes for the key */
        val->crypto_function(VAL_CRYPTO_SET_KEY_TYPE,
                             &attributes,
                             check1[i].type);
        val->crypto_function(VAL_CRYPTO_SET_KEY_USAGE_FLAGS,
                             &attributes,
                             check1[i].usage_flags);
        val->crypto_function(VAL_CRYPTO_SET_KEY_ALGORITHM,
                             &attributes,
                             check1[i].alg);

        /* Import the key data into the key slot */
        status = val->crypto_function(VAL_CRYPTO_IMPORT_KEY,
                                      &attributes,
                                      check1[i].data,
                                      check1[i].data_length,
                                      &key);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(3));

        if (check1[i].operation_state) {
            /* Start a multipart MAC calculation operation */
            status = val->crypto_function(VAL_CRYPTO_MAC_SIGN_SETUP,
                                          &operation,
                                          key,
                                          check1[i].alg);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(4));

            /* Add a message fragment to a multipart MAC operation */
            status = val->crypto_function(VAL_CRYPTO_MAC_UPDATE,
                                          &operation,
                                          check1[i].input,
                                          check1[i].input_length);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(5));
        }

        /* Finish the calculation of the MAC of a message */
        status = val->crypto_function(VAL_CRYPTO_MAC_SIGN_FINISH,
                                      &operation,
                                      check1[i].mac,
                                      check1[i].mac_size,
                                      &mac_length);
        TEST_ASSERT_EQUAL(status, check1[i].expected_status, TEST_CHECKPOINT_NUM(6));

        if (check1[i].expected_status != PSA_SUCCESS)
        {
            /* Abort a MAC operation */
            status = val->crypto_function(VAL_CRYPTO_MAC_ABORT,
                                          &operation);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(7));

            /* Destroy the key */
            status = val->crypto_function(VAL_CRYPTO_DESTROY_KEY,
                                          key);
            TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(8));

            continue;
        }

        /* Check if the MAC length matches with the expected length */
        TEST_ASSERT_EQUAL(mac_length, check1[i].expected_mac_length, TEST_CHECKPOINT_NUM(9));

        /* Check if the MAC data matches with the expected data */
        TEST_ASSERT_MEMCMP(check1[i].mac, check1[i].expected_mac, mac_length,
                           TEST_CHECKPOINT_NUM(10));

        /* Calling mac finish twice in a row.
         * Finish the calculation of the MAC of a message using same operation
         * should return error
         */
        status = val->crypto_function(VAL_CRYPTO_MAC_SIGN_FINISH,
                                      &operation,
                                      check1[i].mac,
                                      check1[i].mac_size,
                                      &mac_length);
        TEST_ASSERT_EQUAL(status, PSA_ERROR_BAD_STATE, TEST_CHECKPOINT_NUM(11));

        /* Abort a MAC operation */
        status = val->crypto_function(VAL_CRYPTO_MAC_ABORT,
                                      &operation);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(12));

        /* Destroy the key */
        status = val->crypto_function(VAL_CRYPTO_DESTROY_KEY,
                                      key);
        TEST_ASSERT_EQUAL(status, PSA_SUCCESS, TEST_CHECKPOINT_NUM(14));
    }

    return VAL_STATUS_SUCCESS;
}
