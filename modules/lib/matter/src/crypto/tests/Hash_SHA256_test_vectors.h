/*
 *
 *    Copyright (c) 2020 Project CHIP Authors
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *    you may not use this file except in compliance with the License.
 *    You may obtain a copy of the License at
 *
 *        http://www.apache.org/licenses/LICENSE-2.0
 *
 *    Unless required by applicable law or agreed to in writing, software
 *    distributed under the License is distributed on an "AS IS" BASIS,
 *    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *    See the License for the specific language governing permissions and
 *    limitations under the License.
 */

/**
 * @file - This file contains SHA256 hash test vectors
 *         from NIST Cryptographic Algorithm Validation Program (CAVP)
 *         http://csrc.nist.gov/groups/STM/cavp/index.html
 */

#pragma once

#include <stddef.h>

typedef struct hash_sha256_vector
{
    const uint8_t * data;
    const size_t data_length;
    const uint8_t * hash;
} hash_sha256_vector;

// Short message test case (zero length)
const uint8_t data01[] = {};
const uint8_t hash01[] = { 0xe3, 0xb0, 0xc4, 0x42, 0x98, 0xfc, 0x1c, 0x14, 0x9a, 0xfb, 0xf4, 0xc8, 0x99, 0x6f, 0xb9, 0x24,
                           0x27, 0xae, 0x41, 0xe4, 0x64, 0x9b, 0x93, 0x4c, 0xa4, 0x95, 0x99, 0x1b, 0x78, 0x52, 0xb8, 0x55 };

hash_sha256_vector v01 = { .data = data01, .data_length = sizeof(data01), .hash = hash01 };

// Short message test case (8 bits; 1 byte)
const uint8_t data02[] = { 0xd3 };
const uint8_t hash02[] = { 0x28, 0x96, 0x9c, 0xdf, 0xa7, 0x4a, 0x12, 0xc8, 0x2f, 0x3b, 0xad, 0x96, 0x0b, 0x0b, 0x00, 0x0a,
                           0xca, 0x2a, 0xc3, 0x29, 0xde, 0xea, 0x5c, 0x23, 0x28, 0xeb, 0xc6, 0xf2, 0xba, 0x98, 0x02, 0xc1 };

hash_sha256_vector v02 = { .data = data02, .data_length = sizeof(data02), .hash = hash02 };

// Short message test case (16 bits; 2 bytes)
const uint8_t data03[] = { 0x11, 0xaf };
const uint8_t hash03[] = { 0x5c, 0xa7, 0x13, 0x3f, 0xa7, 0x35, 0x32, 0x60, 0x81, 0x55, 0x8a, 0xc3, 0x12, 0xc6, 0x20, 0xee,
                           0xca, 0x99, 0x70, 0xd1, 0xe7, 0x0a, 0x4b, 0x95, 0x53, 0x3d, 0x95, 0x6f, 0x07, 0x2d, 0x1f, 0x98 };

hash_sha256_vector v03 = { .data = data03, .data_length = sizeof(data03), .hash = hash03 };

// Short message test case (64 bits; 8 bytes)
const uint8_t data04[] = { 0x57, 0x38, 0xc9, 0x29, 0xc4, 0xf4, 0xcc, 0xb6 };
const uint8_t hash04[] = { 0x96, 0x3b, 0xb8, 0x8f, 0x27, 0xf5, 0x12, 0x77, 0x7a, 0xab, 0x6c, 0x8b, 0x1a, 0x02, 0xc7, 0x0e,
                           0xc0, 0xad, 0x65, 0x1d, 0x42, 0x8f, 0x87, 0x00, 0x36, 0xe1, 0x91, 0x71, 0x20, 0xfb, 0x48, 0xbf };

hash_sha256_vector v04 = { .data = data04, .data_length = sizeof(data04), .hash = hash04 };

// Short message test case (120 bits; 15 bytes)
const uint8_t data05[] = { 0x29, 0x4a, 0xf4, 0x80, 0x2e, 0x5e, 0x92, 0x5e, 0xb1, 0xc6, 0xcc, 0x9c, 0x72, 0x4f, 0x09 };
const uint8_t hash05[] = { 0xdc, 0xba, 0xf3, 0x35, 0x36, 0x0d, 0xe8, 0x53, 0xb9, 0xcd, 0xdf, 0xda, 0xfb, 0x90, 0xfa, 0x75,
                           0x56, 0x7d, 0x0d, 0x3d, 0x58, 0xaf, 0x8d, 0xb9, 0xd7, 0x64, 0x11, 0x3a, 0xef, 0x57, 0x01, 0x25 };

hash_sha256_vector v05 = { .data = data05, .data_length = sizeof(data05), .hash = hash05 };

// Short message test case (128 bits; 16 bytes)
const uint8_t data06[] = { 0x0a, 0x27, 0x84, 0x7c, 0xdc, 0x98, 0xbd, 0x6f, 0x62, 0x22, 0x0b, 0x04, 0x6e, 0xdd, 0x76, 0x2b };
const uint8_t hash06[] = { 0x80, 0xc2, 0x5e, 0xc1, 0x60, 0x05, 0x87, 0xe7, 0xf2, 0x8b, 0x18, 0xb1, 0xb1, 0x8e, 0x3c, 0xdc,
                           0x89, 0x92, 0x8e, 0x39, 0xca, 0xb3, 0xbc, 0x25, 0xe4, 0xd4, 0xa4, 0xc1, 0x39, 0xbc, 0xed, 0xc4 };

hash_sha256_vector v06 = { .data = data06, .data_length = sizeof(data06), .hash = hash06 };

// Short message test case (136 bits; 17 bytes)
const uint8_t data07[] = { 0x1b, 0x50, 0x3f, 0xb9, 0xa7, 0x3b, 0x16, 0xad, 0xa3, 0xfc, 0xf1, 0x04, 0x26, 0x23, 0xae, 0x76, 0x10 };
const uint8_t hash07[] = { 0xd5, 0xc3, 0x03, 0x15, 0xf7, 0x2e, 0xd0, 0x5f, 0xe5, 0x19, 0xa1, 0xbf, 0x75, 0xab, 0x5f, 0xd0,
                           0xff, 0xec, 0x5a, 0xc1, 0xac, 0xb0, 0xda, 0xf6, 0x6b, 0x6b, 0x76, 0x95, 0x98, 0x59, 0x45, 0x09 };

hash_sha256_vector v07 = { .data = data07, .data_length = sizeof(data07), .hash = hash07 };

// Short message test case (472 bits; 59 bytes)
const uint8_t data08[] = { 0xd1, 0xbe, 0x3f, 0x13, 0xfe, 0xba, 0xfe, 0xfc, 0x14, 0x41, 0x4d, 0x9f, 0xb7, 0xf6, 0x93,
                           0xdb, 0x16, 0xdc, 0x1a, 0xe2, 0x70, 0xc5, 0xb6, 0x47, 0xd8, 0x0d, 0xa8, 0x58, 0x35, 0x87,
                           0xc1, 0xad, 0x8c, 0xb8, 0xcb, 0x01, 0x82, 0x43, 0x24, 0x41, 0x1c, 0xa5, 0xac, 0xe3, 0xca,
                           0x22, 0xe1, 0x79, 0xa4, 0xff, 0x49, 0x86, 0xf3, 0xf2, 0x11, 0x90, 0xf3, 0xd7, 0xf3 };
const uint8_t hash08[] = { 0x02, 0x80, 0x49, 0x78, 0xeb, 0xa6, 0xe1, 0xde, 0x65, 0xaf, 0xdb, 0xc6, 0xa6, 0x09, 0x1e, 0xd6,
                           0xb1, 0xec, 0xee, 0x51, 0xe8, 0xbf, 0xf4, 0x06, 0x46, 0xa2, 0x51, 0xde, 0x66, 0x78, 0xb7, 0xef };

hash_sha256_vector v08 = { .data = data08, .data_length = sizeof(data08), .hash = hash08 };

// Short message test case (512 bits; 64 bytes)
const uint8_t data09[] = { 0x5a, 0x86, 0xb7, 0x37, 0xea, 0xea, 0x8e, 0xe9, 0x76, 0xa0, 0xa2, 0x4d, 0xa6, 0x3e, 0x7e, 0xd7,
                           0xee, 0xfa, 0xd1, 0x8a, 0x10, 0x1c, 0x12, 0x11, 0xe2, 0xb3, 0x65, 0x0c, 0x51, 0x87, 0xc2, 0xa8,
                           0xa6, 0x50, 0x54, 0x72, 0x08, 0x25, 0x1f, 0x6d, 0x42, 0x37, 0xe6, 0x61, 0xc7, 0xbf, 0x4c, 0x77,
                           0xf3, 0x35, 0x39, 0x03, 0x94, 0xc3, 0x7f, 0xa1, 0xa9, 0xf9, 0xbe, 0x83, 0x6a, 0xc2, 0x85, 0x09 };
const uint8_t hash09[] = { 0x42, 0xe6, 0x1e, 0x17, 0x4f, 0xbb, 0x38, 0x97, 0xd6, 0xdd, 0x6c, 0xef, 0x3d, 0xd2, 0x80, 0x2f,
                           0xe6, 0x7b, 0x33, 0x19, 0x53, 0xb0, 0x61, 0x14, 0xa6, 0x5c, 0x77, 0x28, 0x59, 0xdf, 0xc1, 0xaa };

hash_sha256_vector v09 = { .data = data09, .data_length = sizeof(data09), .hash = hash09 };

// Long message test case (1304 bits; 163 bytes)
const uint8_t data10[] = { 0x45, 0x11, 0x01, 0x25, 0x0e, 0xc6, 0xf2, 0x66, 0x52, 0x24, 0x9d, 0x59, 0xdc, 0x97, 0x4b, 0x73, 0x61,
                           0xd5, 0x71, 0xa8, 0x10, 0x1c, 0xdf, 0xd3, 0x6a, 0xba, 0x3b, 0x58, 0x54, 0xd3, 0xae, 0x08, 0x6b, 0x5f,
                           0xdd, 0x45, 0x97, 0x72, 0x1b, 0x66, 0xe3, 0xc0, 0xdc, 0x5d, 0x8c, 0x60, 0x6d, 0x96, 0x57, 0xd0, 0xe3,
                           0x23, 0x28, 0x3a, 0x52, 0x17, 0xd1, 0xf5, 0x3f, 0x2f, 0x28, 0x4f, 0x57, 0xb8, 0x5c, 0x8a, 0x61, 0xac,
                           0x89, 0x24, 0x71, 0x1f, 0x89, 0x5c, 0x5e, 0xd9, 0x0e, 0xf1, 0x77, 0x45, 0xed, 0x2d, 0x72, 0x8a, 0xbd,
                           0x22, 0xa5, 0xf7, 0xa1, 0x34, 0x79, 0xa4, 0x62, 0xd7, 0x1b, 0x56, 0xc1, 0x9a, 0x74, 0xa4, 0x0b, 0x65,
                           0x5c, 0x58, 0xed, 0xfe, 0x0a, 0x18, 0x8a, 0xd2, 0xcf, 0x46, 0xcb, 0xf3, 0x05, 0x24, 0xf6, 0x5d, 0x42,
                           0x3c, 0x83, 0x7d, 0xd1, 0xff, 0x2b, 0xf4, 0x62, 0xac, 0x41, 0x98, 0x00, 0x73, 0x45, 0xbb, 0x44, 0xdb,
                           0xb7, 0xb1, 0xc8, 0x61, 0x29, 0x8c, 0xdf, 0x61, 0x98, 0x2a, 0x83, 0x3a, 0xfc, 0x72, 0x8f, 0xae, 0x1e,
                           0xda, 0x2f, 0x87, 0xaa, 0x2c, 0x94, 0x80, 0x85, 0x8b, 0xec };
const uint8_t hash10[] = { 0x3c, 0x59, 0x3a, 0xa5, 0x39, 0xfd, 0xcd, 0xae, 0x51, 0x6c, 0xdf, 0x2f, 0x15, 0x00, 0x0f, 0x66,
                           0x34, 0x18, 0x5c, 0x88, 0xf5, 0x05, 0xb3, 0x97, 0x75, 0xfb, 0x9a, 0xb1, 0x37, 0xa1, 0x0a, 0xa2 };

hash_sha256_vector v10 = { .data = data10, .data_length = sizeof(data10), .hash = hash10 };

// Long message test case (6056 bits; 757 bytes)
const uint8_t data11[] = {
    0x42, 0x17, 0x2d, 0x5f, 0xda, 0xe4, 0x47, 0xc4, 0xcf, 0xdd, 0x13, 0x83, 0x6b, 0xb8, 0xb8, 0x33, 0xc0, 0xcb, 0x85, 0xf7, 0x32,
    0x7f, 0x92, 0x98, 0x35, 0x01, 0xa4, 0xd7, 0x58, 0x3a, 0x52, 0x01, 0x83, 0x02, 0x66, 0xc3, 0x7c, 0x90, 0x86, 0x40, 0xb0, 0x35,
    0x14, 0x61, 0x31, 0x4b, 0x52, 0x6c, 0xfb, 0x68, 0xca, 0xd9, 0x7b, 0xd7, 0xed, 0x61, 0x52, 0x48, 0xfa, 0x57, 0x56, 0xc6, 0x21,
    0x3b, 0xd9, 0xea, 0xe9, 0x8d, 0x2f, 0x4e, 0xcf, 0xdf, 0x6a, 0x45, 0x2f, 0x2e, 0x68, 0xc9, 0x68, 0x72, 0x10, 0xb5, 0x3c, 0x74,
    0xd8, 0x35, 0x75, 0xe0, 0x8a, 0x7a, 0xce, 0x9b, 0x49, 0xb2, 0x10, 0x56, 0xcf, 0x37, 0x7c, 0x64, 0xf8, 0x06, 0x69, 0xc8, 0x84,
    0x74, 0x2e, 0x93, 0x18, 0x1c, 0x42, 0x6d, 0x87, 0x1c, 0xa2, 0x71, 0x50, 0x81, 0x73, 0x3e, 0x68, 0xff, 0xe9, 0x4a, 0x39, 0xe6,
    0x67, 0x7a, 0xea, 0x51, 0xe8, 0xf0, 0xe1, 0xa0, 0x9d, 0x25, 0x86, 0x29, 0xd7, 0x37, 0x4a, 0x2b, 0x28, 0x84, 0xe9, 0x03, 0xc5,
    0x77, 0xeb, 0xa3, 0x2f, 0xa2, 0x71, 0x3f, 0x13, 0x0d, 0x2e, 0x49, 0x6e, 0xce, 0xb4, 0xa0, 0xf4, 0xda, 0xf1, 0x05, 0xb3, 0x1b,
    0xf9, 0xce, 0xf4, 0xc3, 0x06, 0xde, 0x62, 0xdf, 0xbc, 0xd4, 0x6e, 0x2f, 0xb2, 0x83, 0xf1, 0x35, 0x2f, 0xa3, 0x13, 0x8c, 0x31,
    0xc5, 0x6d, 0x7b, 0xb4, 0x8d, 0x6a, 0xca, 0x30, 0x1b, 0xf3, 0xd4, 0x64, 0xca, 0x4b, 0xde, 0x52, 0x1d, 0x37, 0xa7, 0x8b, 0xf6,
    0x63, 0x40, 0xac, 0x09, 0x01, 0x1e, 0x29, 0x91, 0xb3, 0x6e, 0x49, 0x41, 0xab, 0xa8, 0x72, 0x7e, 0x10, 0x67, 0xa7, 0xcb, 0xa4,
    0x78, 0x4f, 0x85, 0xa5, 0x31, 0x38, 0xd0, 0xf1, 0x04, 0xdb, 0xd1, 0x6d, 0x54, 0xe2, 0x1e, 0xa6, 0x86, 0xe7, 0x72, 0xb9, 0x5c,
    0x7f, 0xa6, 0x71, 0x7e, 0x77, 0xdc, 0xb0, 0x5a, 0x5d, 0xfe, 0x10, 0x2e, 0x42, 0x67, 0xc9, 0x63, 0xbf, 0xdf, 0xd6, 0x1d, 0x36,
    0xcd, 0x53, 0x10, 0x5a, 0xa8, 0x2a, 0x95, 0xf2, 0xaf, 0xee, 0xfd, 0xda, 0xda, 0x07, 0x25, 0x4a, 0x10, 0x10, 0x4a, 0x5a, 0x9a,
    0x7d, 0x1f, 0xc6, 0xd8, 0x81, 0x1d, 0xef, 0x32, 0x2f, 0x1b, 0x23, 0x52, 0xdf, 0x1e, 0x1e, 0x90, 0xd3, 0x72, 0xd1, 0xae, 0x1a,
    0xfa, 0x62, 0xc6, 0xb5, 0xc4, 0x73, 0x80, 0xf9, 0xe0, 0xa7, 0x88, 0x34, 0x73, 0x62, 0x40, 0x93, 0x07, 0xd1, 0xb2, 0x43, 0x25,
    0x2b, 0xc8, 0xd7, 0x26, 0x36, 0xbf, 0xea, 0x46, 0x0c, 0xd9, 0x05, 0xfa, 0x1f, 0x52, 0xc3, 0x84, 0x7b, 0x96, 0x32, 0xc4, 0x4b,
    0xb1, 0x7d, 0x51, 0x9f, 0x07, 0xc8, 0xc8, 0x6c, 0x45, 0x5c, 0x64, 0xd4, 0x97, 0x04, 0xcf, 0xa8, 0x1c, 0xb6, 0x38, 0x2c, 0x97,
    0x76, 0xa6, 0x1a, 0x67, 0x78, 0x8c, 0xe9, 0xb9, 0x85, 0x9d, 0x4e, 0xfc, 0x9f, 0xe1, 0x04, 0x95, 0xe8, 0x09, 0xc9, 0xd4, 0xc0,
    0x00, 0xa9, 0x27, 0x2e, 0xc2, 0x7e, 0x8e, 0x81, 0x71, 0xb8, 0x4f, 0x37, 0xa6, 0x5a, 0xeb, 0x1d, 0x05, 0x45, 0x50, 0xb8, 0x14,
    0xb9, 0x50, 0xe4, 0x4d, 0x19, 0x52, 0xbb, 0x71, 0xee, 0x48, 0xb8, 0x20, 0x2f, 0xe1, 0x1c, 0xa7, 0xc0, 0xff, 0x91, 0x19, 0x38,
    0x6b, 0x0e, 0xa1, 0xe7, 0xc8, 0xfa, 0x16, 0x18, 0xc5, 0x94, 0xd0, 0x93, 0x97, 0x92, 0xba, 0x66, 0xa7, 0x08, 0xa9, 0xe5, 0x87,
    0x8c, 0xec, 0xf0, 0x2b, 0x98, 0x25, 0x74, 0x56, 0x30, 0x57, 0x34, 0x52, 0xc4, 0x3f, 0xca, 0xe4, 0x57, 0xe8, 0xe8, 0x7f, 0xe1,
    0x7a, 0xe4, 0xb8, 0xf2, 0x52, 0x74, 0xfa, 0x99, 0x58, 0xb6, 0x7b, 0x84, 0x8d, 0x73, 0x6e, 0x68, 0xe4, 0xa4, 0x7b, 0xa4, 0x53,
    0x35, 0x6c, 0x21, 0x29, 0x0a, 0x29, 0x7c, 0xa2, 0x40, 0xe6, 0x67, 0xb9, 0xb5, 0x9b, 0x4c, 0x3d, 0xca, 0xb4, 0x34, 0x27, 0x67,
    0x0a, 0xe8, 0x2b, 0x40, 0x13, 0x55, 0x8d, 0x57, 0x55, 0x35, 0x36, 0xc2, 0x21, 0xec, 0x07, 0xaf, 0x7d, 0xb0, 0x6d, 0xa5, 0x62,
    0xed, 0x36, 0x0d, 0x28, 0xe8, 0xa3, 0xf0, 0x3e, 0xa2, 0xbe, 0x02, 0x1e, 0xff, 0xed, 0xe0, 0x80, 0x27, 0xc8, 0x96, 0xce, 0x2d,
    0x28, 0x64, 0xd9, 0xef, 0x80, 0xc2, 0xca, 0x3d, 0x71, 0xa1, 0x5b, 0x3d, 0x98, 0xf4, 0x47, 0x0d, 0xab, 0x6f, 0xfe, 0xab, 0xc4,
    0x8e, 0x9e, 0x12, 0xfc, 0xda, 0x1f, 0xa6, 0x3c, 0x68, 0xcd, 0xd2, 0x50, 0xa2, 0xfc, 0xf0, 0x3d, 0x49, 0xf7, 0x69, 0xd5, 0xbb,
    0x39, 0x1d, 0x88, 0x72, 0xe0, 0x05, 0x7d, 0xce, 0x5e, 0x16, 0xe2, 0x14, 0x72, 0x69, 0x80, 0xb6, 0x57, 0x9a, 0x92, 0xd5, 0x3b,
    0x6e, 0xd7, 0x04, 0xf2, 0xb8, 0xe6, 0x4f, 0xec, 0x7d, 0xc2, 0x7c, 0x64, 0x56, 0xae, 0x90, 0xdb, 0x16, 0x42, 0x95, 0xc5, 0xad,
    0xbf, 0x9b, 0x82, 0x4c, 0xa0, 0xfd, 0x8f, 0xca, 0x71, 0xe5, 0xfe, 0x47, 0xe4, 0x12, 0x23, 0x0f, 0x22, 0xd9, 0x91, 0xc0, 0x5f,
    0x6a, 0x45, 0xb0, 0xb1, 0x55, 0x20, 0x89, 0x22, 0x4d, 0x9b, 0x36, 0x04, 0x2b, 0xb6, 0x03, 0x84, 0x36, 0x31, 0xff, 0x82, 0xa1,
    0xff, 0xa5, 0xa0, 0x55, 0xf8, 0xbc, 0x99, 0xf1, 0xce, 0x7c, 0xd5, 0x0f, 0x42, 0xf2, 0x3a, 0xca, 0x97, 0xa6, 0x44, 0x7d, 0x47,
    0x7a, 0x58, 0xcc, 0xf6, 0xd5, 0x55, 0xe9, 0xa4, 0x01, 0x6d, 0x10, 0x26, 0xd2, 0x33, 0x54, 0xd7, 0x89, 0xf4, 0x9e, 0x8b, 0xf7,
    0x4b, 0xf3, 0xc4, 0xe6, 0xf0, 0xf5, 0x29, 0xb4, 0xd1, 0xad, 0x33, 0x41, 0x64, 0x87, 0x2a, 0x0c, 0x3b, 0x9e, 0x50, 0x98, 0xd9,
    0x3a
};
const uint8_t hash11[] = { 0x6c, 0x83, 0xf9, 0xb6, 0x97, 0x54, 0xfa, 0xcc, 0x31, 0x55, 0xda, 0x93, 0x26, 0x1e, 0xd9, 0x9c,
                           0x38, 0xe4, 0x22, 0x5e, 0x74, 0x8e, 0x8e, 0xbc, 0xd0, 0x4e, 0xd6, 0x27, 0x19, 0xfa, 0x56, 0xdb };

hash_sha256_vector v11 = { .data = data11, .data_length = sizeof(data11), .hash = hash11 };

hash_sha256_vector hash_sha256_test_vectors[] = { v01, v02, v03, v04, v05, v06, v07, v08, v09, v10, v11 };
