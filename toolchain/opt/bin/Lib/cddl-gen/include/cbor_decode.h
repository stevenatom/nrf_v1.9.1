/*
 * Copyright (c) 2020 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef CBOR_DECODE_H__
#define CBOR_DECODE_H__
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "cbor_common.h"

/** The cbor_decode library provides functions for decoding CBOR data elements.
 *
 * This library is primarily meant to be called from code generated by
 * $CDDL_GEN_BASE/cddl_gen/cddl_gen.py script, or its equivalent cddl_gen
 * command line executable.
 *
 * Some details to notice about this library:
 *  - Integers are all 32 bits (uint32_t). This means that CBOR's 64 bit values
 *    are not supported, even when the code is running on a 64 bit architecture.
 *    This applies to integer types, as well as lengths for other types.
 *  - Strings are kept in the container type cbor_string_type_t, which is a
 *    pointer and a length.
 *  - When a function returns false, it only means that decoding that particular
 *    value failed. If a value is allowed to take multiple different values,
 *    another decoding function can be called if the first fails.
 *  - There is some type casting going on under the hood to make the code
 *    generator friendly. See especially the processor_t type which is compatible
 *    with all functions except multi_decode, but the compiler doesn't "know"
 *    this because they are defined with different pointer types. It also means
 *    any usage of multi_decode must be made with care for function types.
 *
 *
 * CBOR's format is described well on Wikipedia
 *  - https://en.wikipedia.org/wiki/CBOR
 * but here's a synopsis:
 *
 * Encoded CBOR data elements look like this.
 *
 * | Header             | Value                  | Payload                   |
 * | 1 byte             | 0, 1, 2, 4, or 8 bytes | 0 - 2^64-1 bytes/elements |
 * | 3 bits | 5 bits    |
 * | Major  | Additional|
 * | Type   | info      |
 *
 * The available major types can be seen in @ref cbor_major_type_t.
 *
 * For all types, Values 0-23 are encoded directly in the "Additional info",
 * meaning that the "Value" field is 0 bytes long. If "Additional info" is 24,
 * 25, 26, or 27, the "Value" field is 1, 2, 4, or 8 bytes long, respectively.
 *
 * Major types PINT, NINT, TAG, and PRIM elements have no payload, only Value.
 * PINT: Interpret the Value as a positive integer.
 * NINT: Interpret the Value as a positive integer, then multiply by -1 and
 *       subtract 1.
 * TAG: The Value says something about the next non-tag element.
 *      See https://www.iana.org/assignments/cbor-tags/cbor-tags.xhtml
 * PRIM: Different Values mean different things:
 *       20: "false"
 *       21: "true"
 *       22: "null"
 *       23: "undefined"
 *       >=0x10000: Interpret as IEEE 754 float with given precision
 *
 * For BSTR, TSTR, LIST, and MAP, the Value describes the length of the payload.
 * For BSTR and TSTR, the length is in bytes, for LIST, the length is in number
 * of elements, and for MAP, the length is in number of key/value element pairs.
 *
 * For LIST and MAP, sub elements are regular CBOR elements with their own
 * Header, Value and Payload. LISTs and MAPs can be recursively encoded.
 *
 * The additional info means slightly different things for different major
 * types.
 */

/** Decode a PINT/NINT into a int32_t.
 *
 * @param[inout] state        The current state of the decoding.
 * @param[out]   result       Where to place the decoded value.
 *
 * @retval true   If the value was decoded correctly.
 * @retval false  If the value has the wrong type, the payload overflowed, the
 *                element count was exhausted, or the value was larger than can
 *                fit in the result variable.
 */
bool intx32_decode(cbor_state_t *state, int32_t *result);

/** Expect a PINT/NINT with a certain value. Uses intx32_decode internally.
 *
 * @param[inout] state        The current state of the decoding.
 * @param[in]    result       The expected value
 *
 * @retval true   If the result was decoded correctly and has the expected value.
 * @retval false  If intx32_decode failed or the result doesn't have the
 *                expected value.
 */
bool intx32_expect(cbor_state_t *state, int32_t result);

/** Decode a PINT. */
bool uintx32_decode(cbor_state_t *state, uint32_t *result);
bool uintx32_expect(cbor_state_t *state, uint32_t result);
bool uintx32_expect_union(cbor_state_t *state, uint32_t result);

/** Decode and consume a BSTR header.
 *
 * The rest of the string can be decoded as CBOR.
 * A state backup is created to keep track of the element count.
 *
 * @retval true   Header decoded correctly
 * @retval false  Header decoded incorrectly, or backup failed.
 */
bool bstrx_cbor_start_decode(cbor_state_t *state, cbor_string_type_t *result);

/** Finalize decoding a CBOR-encoded bstr.
 *
 * Restore element count from backup.
 */
bool bstrx_cbor_end_decode(cbor_state_t *state);

/** Decode and consume a BSTR */
bool bstrx_decode(cbor_state_t *state, cbor_string_type_t *result);
bool bstrx_expect(cbor_state_t *state, cbor_string_type_t *result);

/** Decode and consume a TSTR */
bool tstrx_decode(cbor_state_t *state, cbor_string_type_t *result);
bool tstrx_expect(cbor_state_t *state, cbor_string_type_t *result);

/** Decode and consume a LIST header.
 *
 * The contents of the list can be decoded via subsequent function calls.
 * A state backup is created to keep track of the element count.
 *
 * @retval true   Header decoded correctly
 * @retval false  Header decoded incorrectly, or backup failed.
 */
bool list_start_decode(cbor_state_t *state);

/** Decode and consume a MAP header. */
bool map_start_decode(cbor_state_t *state);

/** Finalize decoding a LIST
 *
 * Check that the list had the correct number of elements, and restore previous
 * element count from backup.
 *
 * @retval true   Everything ok.
 * @retval false  Element count not correct.
 */
bool list_end_decode(cbor_state_t *state);

/** Finalize decoding a MAP */
bool map_end_decode(cbor_state_t *state);

/** Decode a "nil" primitive value. */
bool nilx_expect(cbor_state_t *state, void *result);

/** Decode a boolean primitive value. */
bool boolx_decode(cbor_state_t *state, bool *result);
bool boolx_expect(cbor_state_t *state, bool result);

/** Decode a float */
bool float_decode(cbor_state_t *state, double *result);
bool float_expect(cbor_state_t *state, double *result);

/** Skip a single element, regardless of type and value. */
bool any_decode(cbor_state_t *state, void *result);

/** Decode a tag. */
bool tag_decode(cbor_state_t *state, uint32_t *result);
bool tag_expect(cbor_state_t *state, uint32_t result);

/** Decode 0 or more elements with the same type and constraints.
 *
 * @details This must not necessarily decode all elements in a list. E.g. if
 *          the list contains 3 INTS between 0 and 100 followed by 0 to 2 BSTRs
 *          with length 8, that could be done with:
 *
 *          @code{c}
 *              uint32_t int_min = 0;
 *              uint32_t int_max = 100;
 *              uint32_t bstr_size = 8;
 *              uint32_t ints[3];
 *              cbor_string_type_t bstrs[2];
 *              bool res;
 *
 *              res = list_start_encode(state, 3, 5);
 *              // check res
 *              res = multi_encode(3, 3, &num_encode, uintx32_encode, state,
 *                           ints, &int_min, &int_max, 4);
 *              // check res
 *              res = multi_encode(0, 2, &num_encode, strx_encode, state,
 *                           bstrs, &bstr_size, &bstr_size,
 *                           sizeof(cbor_string_type_t));
 *              // check res
 *              res = list_end_encode(state, 3, 5);
 *              // check res
 *          @endcode
 *
 * @param[in]  min_decode    The minimum acceptable number of elements.
 * @param[in]  max_decode    The maximum acceptable number of elements.
 * @param[out] num_decode    The actual number of elements.
 * @param[in]  decoder       The decoder function to call under the hood. This
 *                           function will be called with the provided arguments
 *                           repeatedly until the function fails (returns false)
 *                           or until it has been called @p max_decode times.
 *                           result is moved @p result_len bytes for each call
 *                           to @p decoder, i.e. @p result refers to an array
 *                           of result variables.
 * @param[out] result      Where to place the decoded values. Must be an array
 *                           of length at least @p max_decode.
 * @param[in]  result_len    The length of the result variables. Must be the
 *                           length matching the elements of @p result.
 *
 * @retval true   If at least @p min_decode variables were correctly decoded.
 * @retval false  If @p decoder failed before having decoded @p min_decode
 *                values.
 */
bool multi_decode(uint32_t min_decode, uint32_t max_decode, uint32_t *num_decode,
		cbor_decoder_t decoder, cbor_state_t *state, void *result,
		uint32_t result_len);

bool present_decode(uint32_t *present,
		cbor_decoder_t decoder,
		cbor_state_t *state,
		void *result);

#endif /* CBOR_DECODE_H__ */