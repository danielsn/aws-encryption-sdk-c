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

#include <aws/common/byte_buf.h>
#include <aws/cryptosdk/edk.h>
#include <proof_helpers/make_common_data_structures.h>
#include <make_common_data_structures.h>

void harness() {
    /* assumptions */
    struct aws_cryptosdk_edk dest;
    ensure_cryptosdk_edk_has_allocated_members(&dest);
    __CPROVER_assume(aws_cryptosdk_edk_is_valid(&dest));

    struct aws_cryptosdk_edk src;
    ensure_cryptosdk_edk_has_allocated_members(&src);
    __CPROVER_assume(aws_cryptosdk_edk_is_valid(&src));

    struct aws_allocator allocator = can_fail_allocator();

    /* operation under verification */
    aws_cryptosdk_edk_init_clone(&alloc, &dest, &src);

    /* assertions */
    assert(aws_cryptosdk_edk_is_valid(&dest));
    assert(aws_cryptosdk_edk_is_valid(&src));
}
