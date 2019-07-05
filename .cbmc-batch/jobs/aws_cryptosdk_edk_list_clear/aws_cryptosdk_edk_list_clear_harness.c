/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License").
 * You may not use this file except in compliance with the License.
 * A copy of the License is located at
 *
 *  http://aws.amazon.com/apache2.0
 *
 * or in the "license" file accompanying this file. This file is distributed
 * on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
 * express or implied. See the License for the specific language governing
 * permissions and limitations under the License.
 */

#include <aws/cryptosdk/edk.h>
#include <proof_helpers/make_common_data_structures.h>
#include <make_common_data_structures.h>

void harness() {
    /* parameter */
    struct aws_array_list edk_list;

    /* assumptions */
    __CPROVER_assume(aws_array_list_is_bounded(&edk_list, MAX_INITIAL_ITEM_ALLOCATION, sizeof(struct aws_cryptosdk_edk)));
    ensure_array_list_has_allocated_data_member(&edk_list);
    __CPROVER_assume(aws_array_list_is_valid(&edk_list));
    for (size_t i = 0; i < edk_list->length; ++i) {
        struct aws_cryptosdk_edk edk;
    }

    /* operation under verification */
    aws_cryptosdk_edk_list_clear(&edk_list);

    /* assertions */
    //assert(aws_cryptosdk_edk_is_valid(&edk));
}
