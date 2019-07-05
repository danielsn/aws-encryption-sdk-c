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

#include <aws/common/array_list.h>
#include <aws/cryptosdk/edk.h>
#include <proof_helpers/make_common_data_structures.h>
#include <make_cryptosdk_data_structures.h>

void harness() {
    /* parameters */
    struct aws_allocator *alloc = (nondet_bool()) ? NULL : can_fail_allocator();
    struct aws_array_list edk_list;

    /* operation under verification */
    if (aws_cryptosdk_edk_list_init(alloc, &edk_list) == AWS_OP_SUCCESS) {
        /* assertions */
        assert(aws_array_list_is_valid(&edk_list));
        assert(edk_list.alloc == alloc);
        assert(edk_list.item_size == sizeof(struct aws_cryptosdk_edk));
        assert(edk_list.length == 0);
        assert(edk_list.current_size == sizeof(struct aws_cryptosdk_edk) * 4);
    }
}
