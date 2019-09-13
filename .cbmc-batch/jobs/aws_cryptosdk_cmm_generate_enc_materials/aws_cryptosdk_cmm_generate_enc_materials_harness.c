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
#include <cipher_openssl.h>
#include <proof_helpers/cryptosdk/make_common_data_structures.h>
#include <proof_helpers/make_common_data_structures.h>
#include <proof_helpers/proof_allocators.h>
#include <proof_helpers/utils.h>

void make_hash_table_with_no_backing_store(struct aws_hash_table *map, size_t max_table_entries);

void ensure_aws_cryptosdk_enc_materials_has_allocated_members(struct aws_cryptosdk_enc_materials *materials) {
    // unencrypted_data_key
    ensure_byte_buf_has_allocated_buffer_member(&materials->unencrypted_data_key);

    // keyring_trace
    __CPROVER_assume(aws_array_list_is_bounded(
        &materials->keyring_trace, KEYRING_TRACE_LEN, sizeof(struct aws_cryptosdk_keyring_trace_record)));
    __CPROVER_assume(materials->keyring_trace.item_size == sizeof(struct aws_cryptosdk_keyring_trace_record));
    ensure_array_list_has_allocated_data_member(&materials->keyring_trace);
    __CPROVER_assume(aws_array_list_is_valid(&materials->keyring_trace));
    ensure_trace_has_allocated_records(&materials->keyring_trace, MAX_STRING_LEN);

    // TODO: restore once CBMC bug is fixed https://issues.amazon.com/issues/Padstone-1581
    //__CPROVER_assume(aws_cryptosdk_edk_list_is_bounded(&materials->encrypted_data_keys, NUM_EDK_LIST_ELEMS));
    // ensure_cryptosdk_edk_list_has_allocated_list(&materials->encrypted_data_keys);

    // signctx
    materials->signctx = malloc(sizeof(struct aws_cryptosdk_sig_ctx));
    ensure_sig_ctx_has_allocated_members(materials->signctx);
}

int generate_enc_materials(
    struct aws_cryptosdk_cmm *cmm,
    struct aws_cryptosdk_enc_materials **output,
    struct aws_cryptosdk_enc_request *request) {
    assert(AWS_OBJECT_PTR_IS_WRITABLE(output));
    assert(aws_cryptosdk_cmm_base_is_valid(cmm));
    assert(aws_cryptosdk_enc_request_is_valid(request));

    if (nondet_bool()) {
        struct aws_cryptosdk_enc_materials *temp = malloc(sizeof(struct aws_cryptosdk_enc_materials));
        ensure_aws_cryptosdk_enc_materials_has_allocated_members(temp);
        __CPROVER_assume(aws_cryptosdk_enc_materials_is_valid(temp));
        *output = temp;
        return AWS_OP_SUCCESS;
    } else {
        *output = NULL;
        return AWS_OP_ERR;
    }
}

void aws_cryptosdk_cmm_generate_enc_materials_harness() {
    const struct aws_cryptosdk_cmm_vt vtable = { .vt_size                = sizeof(struct aws_cryptosdk_cmm_vt),
                                                 .name                   = ensure_c_str_is_allocated(SIZE_MAX),
                                                 .destroy                = nondet_voidp(),
                                                 .generate_enc_materials = nondet_bool() ? NULL : generate_enc_materials,
                                                 .decrypt_materials      = nondet_voidp() };
    __CPROVER_assume(aws_cryptosdk_cmm_vtable_is_valid(&vtable));

    struct aws_cryptosdk_cmm cmm;
    cmm.vtable = &vtable;
    __CPROVER_assume(aws_cryptosdk_cmm_base_is_valid(&cmm));

    struct aws_cryptosdk_enc_materials *output;  // used as ** in the call
    struct aws_cryptosdk_enc_request request;    // precondition: non-null
    request.enc_ctx = malloc(sizeof(struct aws_hash_table));
    make_hash_table_with_no_backing_store(request.enc_ctx, SIZE_MAX);
    __CPROVER_assume(aws_cryptosdk_enc_request_is_valid(&request));

    if (aws_cryptosdk_cmm_generate_enc_materials(&cmm, &output, &request) == AWS_OP_SUCCESS) {
        assert(aws_cryptosdk_enc_materials_is_valid(output));
    } else {
        assert(output == NULL);
    }
}
