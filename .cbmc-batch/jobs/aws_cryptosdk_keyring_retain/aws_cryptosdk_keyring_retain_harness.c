/*
 * Copyright 2019 Amazon.com, Inc. or its affiliates. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License"). You may not
 * use
 * this file except in compliance with the License. A copy of the License is
 * located at
 *
 *     http://aws.amazon.com/apache2.0/
 *
 * or in the "license" file accompanying this file. This file is distributed on
 * an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
 * or
 * implied. See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <aws/cryptosdk/materials.h>
#include <proof_helpers/cryptosdk/make_common_data_structures.h>
#include <proof_helpers/make_common_data_structures.h>
#include <proof_helpers/proof_allocators.h>
#include <proof_helpers/utils.h>

void aws_cryptosdk_keyring_retain_harness() {
    const struct aws_cryptosdk_keyring_vt vtable = { .vt_size    = sizeof(struct aws_cryptosdk_keyring_vt),
                                                     .name       = ensure_c_str_is_allocated(SIZE_MAX),
                                                     .destroy    = nondet_voidp(),
                                                     .on_encrypt = nondet_voidp(),
                                                     .on_decrypt = nondet_voidp() };
    __CPROVER_assume(aws_cryptosdk_keyring_vt_is_valid(&vtable));

    struct aws_cryptosdk_keyring keyring;  // Precondition: non-null
    keyring.vtable = &vtable;
    __CPROVER_assume(aws_cryptosdk_keyring_base_is_valid(&keyring));
    __CPROVER_assume(AWS_ATOMIC_VAR_INTVAL(&keyring.refcount) < SIZE_MAX);
    aws_cryptosdk_keyring_retain(&keyring);
}
