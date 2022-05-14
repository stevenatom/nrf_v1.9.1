/*
 * Copyright (c) 2015 Intel Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>
#include <errno.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <net/if.h>
#include <linux/sockios.h>
#include <ifaddrs.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <signal.h>

#define SERVER_PORT  4242
#define CLIENT_PORT  0
#define MAX_TIMEOUT  5		/* in seconds */

#define MIN(a,b) (((a) < (b)) ? (a) : (b))

#define ENTRY(e, expect_result) { sizeof(e), e, expect_result }
#define ENTRY_OK(e) ENTRY(e, true)
#define ENTRY_FAIL(e) ENTRY(e, false)

static const unsigned char A[] = { 'A' };
static const unsigned char null_byte[] = { 0x00 };
static const unsigned char foobar[] = { 'f','o','o','b','a','r' };
static const unsigned char small_binary[] = { 0x20, 0xff, 0x00, 0x56 };

static bool do_exit;

/* Generated 2 paragraphs, 221 words, 1500 bytes of Lorem Ipsum */
static const unsigned char lorem_ipsum[] = \
	"Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed a lacinia ligula. Duis maximus ac turpis in sollicitudin. Nam luctus porta finibus. Duis id suscipit turpis. Ut ac venenatis urna. Phasellus nec elementum lectus. Interdum et malesuada fames ac ante ipsum primis in faucibus. Pellentesque id ultrices lacus. Aenean sapien orci, eleifend eu imperdiet vitae, fringilla sed felis. Sed sit amet consequat nibh, nec rhoncus ipsum. Praesent in tellus in ligula dapibus feugiat. Aliquam at lectus pulvinar, viverra sem, dignissim turpis. Pellentesque quis sagittis leo."
	"\n"
	"Ut eget neque quis nisi volutpat consectetur. Curabitur faucibus metus non arcu pharetra, et aliquam mi molestie. Fusce commodo purus a arcu porta blandit. Integer blandit posuere urna vitae feugiat. Mauris mollis tempus nulla. Aliquam quis lacinia justo, at pellentesque purus. Integer commodo, mi et egestas scelerisque, arcu orci convallis turpis, et blandit dui ante ut turpis. In metus ipsum, imperdiet ut dignissim ac, scelerisque quis purus. Mauris mattis mattis fermentum. Suspendisse a suscipit mi, in interdum massa. Duis magna mi, lacinia bibendum faucibus in, consequat in nisl. Praesent nibh mi, ullamcorper vel fringilla quis, scelerisque eu magna. Vestibulum ipsum purus, eleifend ac mi at, volutpat fermentum sapien. Etiam maximus tortor elementum egestas varius. Aenean sagittis lectus sapien, sed commodo dui consectetur id. Praesent imperdiet, risus at feugiat sodales, elit ex aliquet dui, quis porttitor.\n";

#define MAX_BUF_SIZE sizeof(lorem_ipsum)

/* 256 bytes of binary data */
static const unsigned char array_256[] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
	0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80,
	0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
	0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90,
	0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0,
	0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
	0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0,
	0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
	0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
	0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8,
	0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0,
	0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8,
	0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
	0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 0x00
};

/* 1280 bytes of binary data */
static const unsigned char array_1280[] = {
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
	0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80,
	0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
	0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90,
	0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0,
	0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
	0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0,
	0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
	0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
	0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8,
	0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0,
	0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8,
	0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
	0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 0x00,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
	0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80,
	0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
	0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90,
	0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0,
	0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
	0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0,
	0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
	0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
	0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8,
	0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0,
	0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8,
	0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
	0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 0x00,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
	0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80,
	0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
	0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90,
	0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0,
	0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
	0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0,
	0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
	0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
	0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8,
	0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0,
	0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8,
	0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
	0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 0x00,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
	0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80,
	0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
	0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90,
	0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0,
	0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
	0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0,
	0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
	0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
	0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8,
	0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0,
	0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8,
	0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
	0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 0x00,
	0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
	0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10,
	0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18,
	0x19, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f, 0x20,
	0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
	0x29, 0x2a, 0x2b, 0x2c, 0x2d, 0x2e, 0x2f, 0x30,
	0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38,
	0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e, 0x3f, 0x40,
	0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x48,
	0x49, 0x4a, 0x4b, 0x4c, 0x4d, 0x4e, 0x4f, 0x50,
	0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
	0x59, 0x5a, 0x5b, 0x5c, 0x5d, 0x5e, 0x5f, 0x60,
	0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
	0x69, 0x6a, 0x6b, 0x6c, 0x6d, 0x6e, 0x6f, 0x70,
	0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77, 0x78,
	0x79, 0x7a, 0x7b, 0x7c, 0x7d, 0x7e, 0x7f, 0x80,
	0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88,
	0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90,
	0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98,
	0x99, 0x9a, 0x9b, 0x9c, 0x9d, 0x9e, 0x9f, 0xa0,
	0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8,
	0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0,
	0xb1, 0xb2, 0xb3, 0xb4, 0xb5, 0xb6, 0xb7, 0xb8,
	0xb9, 0xba, 0xbb, 0xbc, 0xbd, 0xbe, 0xbf, 0xc0,
	0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8,
	0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0,
	0xd1, 0xd2, 0xd3, 0xd4, 0xd5, 0xd6, 0xd7, 0xd8,
	0xd9, 0xda, 0xdb, 0xdc, 0xdd, 0xde, 0xdf, 0xe0,
	0xe1, 0xe2, 0xe3, 0xe4, 0xe5, 0xe6, 0xe7, 0xe8,
	0xe9, 0xea, 0xeb, 0xec, 0xed, 0xee, 0xef, 0xf0,
	0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8,
	0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff, 0x00
};

static struct {
	int len;
	const unsigned char *buf;
	bool expecting_reply;
} data[] = {
	ENTRY_OK(A),
	ENTRY_OK(foobar),
	ENTRY_OK(small_binary),
	ENTRY_OK("a bit longer data message"),
	ENTRY_OK(lorem_ipsum),
	ENTRY_OK(null_byte),
	ENTRY_OK(array_256),

	{ 0, 0 }
};

static int get_ifindex(const char *name)
{
	struct ifreq ifr;
	int sk, err;

	if (!name)
		return -1;

	sk = socket(PF_INET6, SOCK_DGRAM | SOCK_CLOEXEC, 0);
	if (sk < 0)
		return -1;

	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, name, sizeof(ifr.ifr_name) - 1);

	err = ioctl(sk, SIOCGIFINDEX, &ifr);

	close(sk);

	if (err < 0)
		return -1;

	return ifr.ifr_ifindex;
}

static int find_address(int family, struct ifaddrs *if_address,
			const char *if_name, void *address)
{
	struct ifaddrs *tmp;
	struct sockaddr_in6 *ll = NULL;
	int error = -ENOENT;

	for (tmp = if_address; tmp; tmp = tmp->ifa_next) {
		if (tmp->ifa_addr &&
		    !strncmp(tmp->ifa_name, if_name, IF_NAMESIZE) &&
		    tmp->ifa_addr->sa_family == family) {

			switch (family) {
			case AF_INET: {
				struct sockaddr_in *in4 =
					(struct sockaddr_in *)tmp->ifa_addr;
				if (in4->sin_addr.s_addr == INADDR_ANY)
					continue;
				if ((in4->sin_addr.s_addr & IN_CLASSB_NET) ==
						((in_addr_t)0xa9fe0000))
					continue;
				memcpy(address, &in4->sin_addr,
				       sizeof(struct in_addr));
				error = 0;
				goto out;
			}
			case AF_INET6: {
				struct sockaddr_in6 *in6 =
					(struct sockaddr_in6 *)tmp->ifa_addr;
				if (!memcmp(&in6->sin6_addr, &in6addr_any,
					    sizeof(struct in6_addr)))
					continue;
				if (IN6_IS_ADDR_LINKLOCAL(&in6->sin6_addr)) {
					ll = in6;
					continue;
				}

				memcpy(address, &in6->sin6_addr,
				       sizeof(struct in6_addr));
				error = 0;
				goto out;
			}
			default:
				error = -EINVAL;
				goto out;
			}
		}
	}

out:
	if (error < 0 && ll) {
		/* As a last resort use link local address */
		memcpy(address, &ll->sin6_addr, sizeof(struct in6_addr));
		error = 0;
	}

	return error;
}

static int get_address(const char *if_name, int family, void *address)
{
	struct ifaddrs *if_address;
	int err;

	if (getifaddrs(&if_address) < 0) {
		err = -errno;
		fprintf(stderr, "Cannot get interface addresses for "
			"interface %s error %d/%s",
			if_name, err, strerror(-err));
		return err;
	}

	err = find_address(family, if_address, if_name, address);

	freeifaddrs(if_address);

	return err;
}

static void signal_handler(int sig)
{
	do_exit = true;
}

extern int optind, opterr, optopt;
extern char *optarg;

/* The application returns:
 *    < 0 : connection or similar error
 *      0 : no errors, all tests passed
 *    > 0 : could not send all the data to server
 */
int main(int argc, char**argv)
{
	int c, ret, fd, i = 0, timeout = -1, port = SERVER_PORT;
	bool flood = false, multicast = false;
	struct sockaddr_in6 addr6_send = { 0 }, addr6_recv = { 0 };
	struct sockaddr_in addr4_send = { 0 }, addr4_recv = { 0 };
	struct sockaddr *addr_send, *addr_recv;
	int family, addr_len;
	unsigned char buf[MAX_BUF_SIZE];
	const struct in6_addr any = IN6ADDR_ANY_INIT;
	const char *target = NULL, *interface = NULL;
	fd_set rfds;
	struct timeval tv = {};
	int ifindex = -1, optval = 1;
	void *address = NULL;
	bool forever = false, help = false, tcp = false, do_randomize = false;
	struct timeval start_time, end_time, diff_time;
	unsigned long long sum_time = 0ULL;
	unsigned long long count_time = 0ULL;
	unsigned long long pkt_counter = 0ULL;

	opterr = 0;

	while ((c = getopt(argc, argv, "Fi:p:ethr")) != -1) {
		switch (c) {
		case 'F':
			flood = true;
			break;
		case 'i':
			interface = optarg;
			break;
		case 'e':
			forever = true;
			break;
		case 't':
			tcp = true;
			break;
		case 'p':
			port = atoi(optarg);
			break;
		case 'r':
			gettimeofday(&start_time, NULL);
			srandom(start_time.tv_usec);
			do_randomize = true;
			break;
		case 'h':
			help = true;
			break;
		}
	}

	if (optind < argc)
		target = argv[optind];

	if (!target || help) {
		printf("usage: %s [-i iface] [-F] <IPv{6|4} address of the echo-server>\n",
		       argv[0]);
		printf("\n-i Use this network interface, needed if using "
		       "multicast server address.\n");
		printf("-e Do not quit, send packets forever\n");
		printf("-t Use TCP, default is to use UDP only\n");
		printf("-p Use this port, default port is %d\n", SERVER_PORT);
		printf("-r Send random packet lengths\n");
		printf("-F (flood) option will prevent the client from "
		       "waiting the data.\n"
		       "   The -F option will stress test the server.\n");
		exit(-EINVAL);
	}

	signal(SIGINT, signal_handler);
	signal(SIGTERM, signal_handler);

	if (inet_pton(AF_INET6, target, &addr6_send.sin6_addr) != 1) {
		if (inet_pton(AF_INET, target, &addr4_send.sin_addr) != 1) {
			printf("Invalid address family\n");
			exit(-EINVAL);
		} else {
			if (IN_MULTICAST(addr4_recv.sin_addr.s_addr))
				multicast = true;

			addr_send = (struct sockaddr *)&addr4_send;
			addr_recv = (struct sockaddr *)&addr4_recv;
			addr4_send.sin_port = htons(port);
			addr4_recv.sin_family = AF_INET;
			addr4_recv.sin_addr.s_addr = INADDR_ANY;
			if (!tcp)
				addr4_recv.sin_port = htons(CLIENT_PORT);
			family = AF_INET;
			addr_len = sizeof(addr4_send);
			address = &addr4_recv.sin_addr;
		}
	} else {
		if (IN6_IS_ADDR_MULTICAST(&addr6_send.sin6_addr))
			multicast = true;

		addr_send = (struct sockaddr *)&addr6_send;
		addr_recv = (struct sockaddr *)&addr6_recv;
		addr6_send.sin6_port = htons(port);
		addr6_recv.sin6_family = AF_INET6;
		addr6_recv.sin6_addr = any;
		if (!tcp)
			addr6_recv.sin6_port = htons(CLIENT_PORT);
		family = AF_INET6;
		addr_len = sizeof(addr6_send);
		address = &addr6_recv.sin6_addr;
	}

	addr_send->sa_family = family;
	addr_recv->sa_family = family;

	fd = socket(family, tcp ? SOCK_STREAM : SOCK_DGRAM,
		    tcp ? IPPROTO_TCP : IPPROTO_UDP);
	if (fd < 0) {
		perror("socket");
		exit(-errno);
	}

	if (interface) {
		struct ifreq ifr;
		char addr_buf[INET6_ADDRSTRLEN];

		memset(&ifr, 0, sizeof(ifr));
		snprintf(ifr.ifr_name, sizeof(ifr.ifr_name), "%s", interface);

		if (setsockopt(fd, SOL_SOCKET, SO_BINDTODEVICE,
			       (void *)&ifr, sizeof(ifr)) < 0) {
			perror("SO_BINDTODEVICE");
			exit(-errno);
		}

		ifindex = get_ifindex(interface);
		if (ifindex < 0) {
			printf("Invalid interface %s\n", interface);
			exit(-EINVAL);
		}

		ret = get_address(interface, family, address);
		if (ret < 0) {
			printf("Cannot find suitable source address "
			       "for interface %s [%d/%s]\n",
			       interface, ret, strerror(-ret));
		}

		printf("Binding to %s\n", inet_ntop(family, address,
					    addr_buf, sizeof(addr_buf)));
	}

	if (multicast) {
		if (!interface) {
			printf("Need to use -i option for multicast "
			       "addresses.\n");
			exit(-EINVAL);
		}
	}

	ret = setsockopt(fd, SOL_SOCKET, SO_REUSEPORT,
			 &optval, sizeof(optval));
	if (ret < 0) {
		perror("setsockopt");
	}

	ret = bind(fd, addr_recv, addr_len);
	if (ret < 0) {
		perror("bind");
		exit(-errno);
	}

	if (tcp) {
		ret = connect(fd, addr_send, addr_len);
		if (ret < 0) {
			perror("connect");
			exit(-errno);
		}
	}

again:
	do {
		int sent;

		while (data[i].buf) {
			const unsigned char *buf_ptr;
			int pos = 0;
			int len;

			sent = 0;

			gettimeofday(&start_time, NULL);

			if (do_randomize) {
				buf_ptr = lorem_ipsum;

				/* For UDP, we send max 1280 bytes which is
				 * the IPv6 MTU size
				 */
				if (tcp)
					len = random() % sizeof(lorem_ipsum);
				else
					len = random() %
						MIN(1280, sizeof(lorem_ipsum));

				if (len == 0)
					len = 1;
			} else {
				if (tcp)
					len = data[i].len;
				else
					len = MIN(1280, data[i].len);

				buf_ptr = data[i].buf;
			}

			if (tcp) {
				pos = 0;

				do {
					ret = write(fd, &buf_ptr[pos],
						    len - pos);
					if (ret <= 0)
						break;

					sent += len;
					pos += ret;
				} while (sent < len);
			} else
				ret = sendto(fd, buf_ptr, len, 0, addr_send,
					     addr_len);
			if (ret < 0) {
				perror("send");
				goto out;
			}

			if (flood) {
				i++;
				continue;
			}

			FD_ZERO(&rfds);
			FD_SET(fd, &rfds);

			if (do_randomize || data[i].expecting_reply) {
				tv.tv_sec = MAX_TIMEOUT;
				tv.tv_usec = 0;
			} else {
				tv.tv_sec = 0;
				tv.tv_usec = 0;
			}

			ret = select(fd + 1, &rfds, NULL, NULL, &tv);
			if (ret < 0) {
				if (!do_exit) {
					perror("select");
				}

				goto out;
			} else if (ret == 0) {
				if (do_randomize) {
					timeout++;
					continue;
				}

				if (data[i].expecting_reply) {
					fprintf(stderr,
						"Timeout while waiting "
						"idx %d len %d\n",
						i, data[i].len);
					timeout = i;
				}
				i++;
				continue;
			} else if (!FD_ISSET(fd, &rfds)) {
				fprintf(stderr, "Invalid fd\n");
				ret = i;
				goto out;
			}

			if (tcp) {
				int received = 0;

				pos = 0;

				do {
					ret = read(fd, buf + pos,
						   sizeof(buf) - pos);
					if (ret <= 0)
						break;

					received += ret;
					pos += ret;
					ret = received;
				} while (received < sent);
			} else
				ret = recv(fd, buf, sizeof(buf), 0);
			if (ret <= 0) {
				if (ret)
					perror("recv");
				else
					printf("Connection closed by peer.\n");

				ret = -EINVAL;
				goto out;
			}

			if (len != ret ||
			    memcmp(buf_ptr, buf, ret) != 0) {
				fprintf(stderr,
					"Check failed idx %d len %d\n",
					i, ret);
				ret = i;
				goto out;
			} else {
				gettimeofday(&end_time, NULL);

				timersub(&end_time, &start_time, &diff_time);

				sum_time +=
					(unsigned long long)diff_time.tv_sec *
					1000000ULL +
					(unsigned long long)diff_time.tv_usec;
				count_time++;
				pkt_counter++;

				printf(".");

				/* Flush stdout only every 10 packets */
				if (forever) {
					if (!(pkt_counter % 10))
						fflush(stdout);
				} else
					fflush(stdout);
			}

			i++;
		}

		if (flood)
			i = 0;

		if (do_exit)
			break;

	} while (flood);

	if (forever && !do_exit) {
		i = 0;
		goto again;
	}

	ret = timeout + 1;

	printf("\n");

out:
	if (count_time > 0ULL) {
		unsigned long long time_spent;
		unsigned long long ms;

		if (do_exit) {
			printf("\n");
		}

		time_spent = sum_time / count_time;
		ms = time_spent / 1000ULL;

		if (ms == 0) {
			printf("Average round trip %llu us\n", time_spent);
		} else {
			printf("Average round trip %llu ms\n", ms);
		}

		printf("Sent %llu packets\n", pkt_counter);
	} else {
		printf("No packets sent!\n");
	}

	close(fd);

	exit(ret);
}