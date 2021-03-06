/*
 *
 *    Copyright (c) 2021 Project CHIP Authors
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
 * @file - This file contains PKCS7 test vectors.
 */

#pragma once

#include <cstdint>

namespace chip {
namespace Crypto {

static const char * pem_pkcs7_blob = "-----BEGIN PKCS7-----\n"
                                     "MIAGCSqGSIb3DQEHAqCAMIACAQExDzANBglghkgBZQMEAgEFADCABgkqhkiG9w0B\n"
                                     "BwGggCSAAAAAAAAAoIAwggG6MIIBYKADAgECAhQS9bfGREvf8V0R6DdgaWvs/pTP\n"
                                     "GzAKBggqhkjOPQQDAjAeMRwwGgYDVQQDDBNDSElQIERFViBDbGFzczMgSUNBMB4X\n"
                                     "DTIxMDMwMzA5NDk0MVoXDTIyMDMwMzExNDk0MVowGzEZMBcGA1UEAwwQY2E2Mjdj\n"
                                     "MTA5OWM4MmU3YjBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABOF38X/slwBPv4i4\n"
                                     "lAsD298m13wpIhGyQzqiy5UX+XjNN36cU9yYgMoleAm+XWHM1gGsRQk2U1uhpXBn\n"
                                     "wVTSQ22jfzB9MB0GA1UdJQQWMBQGCCsGAQUFBwMCBggrBgEFBQcDATAMBgNVHRMB\n"
                                     "Af8EAjAAMB8GA1UdIwQYMBaAFJ5vRKqN+2JbeM0Pdh/JIbpMO5qQMB0GA1UdDgQW\n"
                                     "BBQhrb6e1N7JpOeZVz40T7w2/2vvVjAOBgNVHQ8BAf8EBAMCBaAwCgYIKoZIzj0E\n"
                                     "AwIDSAAwRQIgXv1eUKk+1+n5L3Q8xGHilh9fsRP1kYjmwVOxTbcqKbECIQD3AsTc\n"
                                     "csVzNSsxX+gpwGVxSie02xT/NPNOCiQYYKQbczCCAb4wggFloAMCAQICFCJDphvl\n"
                                     "8NtDeBzC8nl8m5NI5paPMAoGCCqGSM49BAMCMDkxNzA1BgNVBAMMLkNvbWNhc3Qg\n"
                                     "SW50ZWdyYXRpb24gVGVzdGluZyBFQ0MgQ2xhc3MgSUlJIFJvb3QwHhcNMjAwNzA3\n"
                                     "MjMwNzUxWhcNNDUwNTA1MjMxMTM4WjAeMRwwGgYDVQQDDBNDSElQIERFViBDbGFz\n"
                                     "czMgSUNBMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAENMJg22Jh5lRZ+i8vy6hR\n"
                                     "WUJhUvtbXe7PsmQ/C26DN43KwVv2sktwJyDunDRw+IiQ4qDSxTGm/cVM1RpkmIKF\n"
                                     "AqNmMGQwEgYDVR0TAQH/BAgwBgEB/wIBADAfBgNVHSMEGDAWgBQ29AqlCvQrzk5X\n"
                                     "1Mx/sDKsqsvYtTAdBgNVHQ4EFgQUnm9Eqo37Ylt4zQ92H8khukw7mpAwDgYDVR0P\n"
                                     "AQH/BAQDAgGGMAoGCCqGSM49BAMCA0cAMEQCIGmUEcWQAPEVcow9aEzcanRJCjQr\n"
                                     "nnsPbTPB82ffqxk+AiAxQ/94ucA3JNhsYEKoENQwlIA7sKQrxaLD2dRs5gwXKDCC\n"
                                     "AdowggGAoAMCAQICFEqcODQ4I6Unn2PHw35xFcN6UYkyMAoGCCqGSM49BAMCMDkx\n"
                                     "NzA1BgNVBAMMLkNvbWNhc3QgSW50ZWdyYXRpb24gVGVzdGluZyBFQ0MgQ2xhc3Mg\n"
                                     "SUlJIFJvb3QwHhcNMjAwNTExMjMxMTM4WhcNNDUwNTA1MjMxMTM4WjA5MTcwNQYD\n"
                                     "VQQDDC5Db21jYXN0IEludGVncmF0aW9uIFRlc3RpbmcgRUNDIENsYXNzIElJSSBS\n"
                                     "b290MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEHh70NuJnbOB5EpGbTHLs8kDu\n"
                                     "cEliEDEXzkOH18/otdQP3HifH2nOI+Gudrw/JZv9T5h4PhcfFUgVfQ4rJoOkj6Nm\n"
                                     "MGQwEgYDVR0TAQH/BAgwBgEB/wIBATAfBgNVHSMEGDAWgBQ29AqlCvQrzk5X1Mx/\n"
                                     "sDKsqsvYtTAdBgNVHQ4EFgQUNvQKpQr0K85OV9TMf7AyrKrL2LUwDgYDVR0PAQH/\n"
                                     "BAQDAgGGMAoGCCqGSM49BAMCA0gAMEUCIQCTWCwX7MdPiRZYnGJdQWRHJIEWs53X\n"
                                     "VwLDMO/l0DaNzwIgKb5DZJEyZOHAMbkKbVvDv6L/fQt53PXa3jwb6NkQPGYAADGC\n"
                                     "ARUwggERAgEBMFEwOTE3MDUGA1UEAwwuQ29tY2FzdCBJbnRlZ3JhdGlvbiBUZXN0\n"
                                     "aW5nIEVDQyBDbGFzcyBJSUkgUm9vdAIUIkOmG+Xw20N4HMLyeXybk0jmlo8wDQYJ\n"
                                     "YIZIAWUDBAIBBQCggZgwGAYJKoZIhvcNAQkDMQsGCSqGSIb3DQEHATAcBgkqhkiG\n"
                                     "9w0BCQUxDxcNMjEwMzAzMTE0OTQyWjAtBgkqhkiG9w0BCTQxIDAeMA0GCWCGSAFl\n"
                                     "AwQCAQUAoQ0GCSqGSIb3DQEBCwUAMC8GCSqGSIb3DQEJBDEiBCDjsMRCmPwcFJr7\n"
                                     "9MiZb7kkJ65B5GSbk0yklZkbeFK4VTANBgkqhkiG9w0BAQsFAAQAAAAAAAAA\n"
                                     "-----END PKCS7-----\n";

static const uint8_t certificate_blob_leaf[] = {
    0x30, 0x82, 0x01, 0xBA, 0x30, 0x82, 0x01, 0x60, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x14, 0x12, 0xF5, 0xB7, 0xC6, 0x44, 0x4B,
    0xDF, 0xF1, 0x5D, 0x11, 0xE8, 0x37, 0x60, 0x69, 0x6B, 0xEC, 0xFE, 0x94, 0xCF, 0x1B, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48,
    0xCE, 0x3D, 0x04, 0x03, 0x02, 0x30, 0x1E, 0x31, 0x1C, 0x30, 0x1A, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x13, 0x43, 0x48, 0x49,
    0x50, 0x20, 0x44, 0x45, 0x56, 0x20, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x33, 0x20, 0x49, 0x43, 0x41, 0x30, 0x1E, 0x17, 0x0D, 0x32,
    0x31, 0x30, 0x33, 0x30, 0x33, 0x30, 0x39, 0x34, 0x39, 0x34, 0x31, 0x5A, 0x17, 0x0D, 0x32, 0x32, 0x30, 0x33, 0x30, 0x33, 0x31,
    0x31, 0x34, 0x39, 0x34, 0x31, 0x5A, 0x30, 0x1B, 0x31, 0x19, 0x30, 0x17, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x10, 0x63, 0x61,
    0x36, 0x32, 0x37, 0x63, 0x31, 0x30, 0x39, 0x39, 0x63, 0x38, 0x32, 0x65, 0x37, 0x62, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A,
    0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0xE1,
    0x77, 0xF1, 0x7F, 0xEC, 0x97, 0x00, 0x4F, 0xBF, 0x88, 0xB8, 0x94, 0x0B, 0x03, 0xDB, 0xDF, 0x26, 0xD7, 0x7C, 0x29, 0x22, 0x11,
    0xB2, 0x43, 0x3A, 0xA2, 0xCB, 0x95, 0x17, 0xF9, 0x78, 0xCD, 0x37, 0x7E, 0x9C, 0x53, 0xDC, 0x98, 0x80, 0xCA, 0x25, 0x78, 0x09,
    0xBE, 0x5D, 0x61, 0xCC, 0xD6, 0x01, 0xAC, 0x45, 0x09, 0x36, 0x53, 0x5B, 0xA1, 0xA5, 0x70, 0x67, 0xC1, 0x54, 0xD2, 0x43, 0x6D,
    0xA3, 0x7F, 0x30, 0x7D, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x25, 0x04, 0x16, 0x30, 0x14, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05,
    0x05, 0x07, 0x03, 0x02, 0x06, 0x08, 0x2B, 0x06, 0x01, 0x05, 0x05, 0x07, 0x03, 0x01, 0x30, 0x0C, 0x06, 0x03, 0x55, 0x1D, 0x13,
    0x01, 0x01, 0xFF, 0x04, 0x02, 0x30, 0x00, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x9E,
    0x6F, 0x44, 0xAA, 0x8D, 0xFB, 0x62, 0x5B, 0x78, 0xCD, 0x0F, 0x76, 0x1F, 0xC9, 0x21, 0xBA, 0x4C, 0x3B, 0x9A, 0x90, 0x30, 0x1D,
    0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0x21, 0xAD, 0xBE, 0x9E, 0xD4, 0xDE, 0xC9, 0xA4, 0xE7, 0x99, 0x57, 0x3E,
    0x34, 0x4F, 0xBC, 0x36, 0xFF, 0x6B, 0xEF, 0x56, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01, 0xFF, 0x04, 0x04, 0x03,
    0x02, 0x05, 0xA0, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02,
    0x20, 0x5E, 0xFD, 0x5E, 0x50, 0xA9, 0x3E, 0xD7, 0xE9, 0xF9, 0x2F, 0x74, 0x3C, 0xC4, 0x61, 0xE2, 0x96, 0x1F, 0x5F, 0xB1, 0x13,
    0xF5, 0x91, 0x88, 0xE6, 0xC1, 0x53, 0xB1, 0x4D, 0xB7, 0x2A, 0x29, 0xB1, 0x02, 0x21, 0x00, 0xF7, 0x02, 0xC4, 0xDC, 0x72, 0xC5,
    0x73, 0x35, 0x2B, 0x31, 0x5F, 0xE8, 0x29, 0xC0, 0x65, 0x71, 0x4A, 0x27, 0xB4, 0xDB, 0x14, 0xFF, 0x34, 0xF3, 0x4E, 0x0A, 0x24,
    0x18, 0x60, 0xA4, 0x1B, 0x73
};

static const uint8_t certificate_blob_intermediate[] = {
    0x30, 0x82, 0x01, 0xBE, 0x30, 0x82, 0x01, 0x65, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x14, 0x22, 0x43, 0xA6, 0x1B, 0xE5, 0xF0,
    0xDB, 0x43, 0x78, 0x1C, 0xC2, 0xF2, 0x79, 0x7C, 0x9B, 0x93, 0x48, 0xE6, 0x96, 0x8F, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48,
    0xCE, 0x3D, 0x04, 0x03, 0x02, 0x30, 0x39, 0x31, 0x37, 0x30, 0x35, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x2E, 0x43, 0x6F, 0x6D,
    0x63, 0x61, 0x73, 0x74, 0x20, 0x49, 0x6E, 0x74, 0x65, 0x67, 0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x54, 0x65, 0x73, 0x74,
    0x69, 0x6E, 0x67, 0x20, 0x45, 0x43, 0x43, 0x20, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x49, 0x49, 0x49, 0x20, 0x52, 0x6F, 0x6F,
    0x74, 0x30, 0x1E, 0x17, 0x0D, 0x32, 0x30, 0x30, 0x37, 0x30, 0x37, 0x32, 0x33, 0x30, 0x37, 0x35, 0x31, 0x5A, 0x17, 0x0D, 0x34,
    0x35, 0x30, 0x35, 0x30, 0x35, 0x32, 0x33, 0x31, 0x31, 0x33, 0x38, 0x5A, 0x30, 0x1E, 0x31, 0x1C, 0x30, 0x1A, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x0C, 0x13, 0x43, 0x48, 0x49, 0x50, 0x20, 0x44, 0x45, 0x56, 0x20, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x33, 0x20, 0x49,
    0x43, 0x41, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE,
    0x3D, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x34, 0xC2, 0x60, 0xDB, 0x62, 0x61, 0xE6, 0x54, 0x59, 0xFA, 0x2F, 0x2F, 0xCB,
    0xA8, 0x51, 0x59, 0x42, 0x61, 0x52, 0xFB, 0x5B, 0x5D, 0xEE, 0xCF, 0xB2, 0x64, 0x3F, 0x0B, 0x6E, 0x83, 0x37, 0x8D, 0xCA, 0xC1,
    0x5B, 0xF6, 0xB2, 0x4B, 0x70, 0x27, 0x20, 0xEE, 0x9C, 0x34, 0x70, 0xF8, 0x88, 0x90, 0xE2, 0xA0, 0xD2, 0xC5, 0x31, 0xA6, 0xFD,
    0xC5, 0x4C, 0xD5, 0x1A, 0x64, 0x98, 0x82, 0x85, 0x02, 0xA3, 0x66, 0x30, 0x64, 0x30, 0x12, 0x06, 0x03, 0x55, 0x1D, 0x13, 0x01,
    0x01, 0xFF, 0x04, 0x08, 0x30, 0x06, 0x01, 0x01, 0xFF, 0x02, 0x01, 0x00, 0x30, 0x1F, 0x06, 0x03, 0x55, 0x1D, 0x23, 0x04, 0x18,
    0x30, 0x16, 0x80, 0x14, 0x36, 0xF4, 0x0A, 0xA5, 0x0A, 0xF4, 0x2B, 0xCE, 0x4E, 0x57, 0xD4, 0xCC, 0x7F, 0xB0, 0x32, 0xAC, 0xAA,
    0xCB, 0xD8, 0xB5, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0x9E, 0x6F, 0x44, 0xAA, 0x8D, 0xFB, 0x62,
    0x5B, 0x78, 0xCD, 0x0F, 0x76, 0x1F, 0xC9, 0x21, 0xBA, 0x4C, 0x3B, 0x9A, 0x90, 0x30, 0x0E, 0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01,
    0x01, 0xFF, 0x04, 0x04, 0x03, 0x02, 0x01, 0x86, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x04, 0x03, 0x02, 0x03,
    0x47, 0x00, 0x30, 0x44, 0x02, 0x20, 0x69, 0x94, 0x11, 0xC5, 0x90, 0x00, 0xF1, 0x15, 0x72, 0x8C, 0x3D, 0x68, 0x4C, 0xDC, 0x6A,
    0x74, 0x49, 0x0A, 0x34, 0x2B, 0x9E, 0x7B, 0x0F, 0x6D, 0x33, 0xC1, 0xF3, 0x67, 0xDF, 0xAB, 0x19, 0x3E, 0x02, 0x20, 0x31, 0x43,
    0xFF, 0x78, 0xB9, 0xC0, 0x37, 0x24, 0xD8, 0x6C, 0x60, 0x42, 0xA8, 0x10, 0xD4, 0x30, 0x94, 0x80, 0x3B, 0xB0, 0xA4, 0x2B, 0xC5,
    0xA2, 0xC3, 0xD9, 0xD4, 0x6C, 0xE6, 0x0C, 0x17, 0x28
};

static const uint8_t certificate_blob_root[] = {
    0x30, 0x82, 0x01, 0xDA, 0x30, 0x82, 0x01, 0x80, 0xA0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x14, 0x4A, 0x9C, 0x38, 0x34, 0x38, 0x23,
    0xA5, 0x27, 0x9F, 0x63, 0xC7, 0xC3, 0x7E, 0x71, 0x15, 0xC3, 0x7A, 0x51, 0x89, 0x32, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48,
    0xCE, 0x3D, 0x04, 0x03, 0x02, 0x30, 0x39, 0x31, 0x37, 0x30, 0x35, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0C, 0x2E, 0x43, 0x6F, 0x6D,
    0x63, 0x61, 0x73, 0x74, 0x20, 0x49, 0x6E, 0x74, 0x65, 0x67, 0x72, 0x61, 0x74, 0x69, 0x6F, 0x6E, 0x20, 0x54, 0x65, 0x73, 0x74,
    0x69, 0x6E, 0x67, 0x20, 0x45, 0x43, 0x43, 0x20, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20, 0x49, 0x49, 0x49, 0x20, 0x52, 0x6F, 0x6F,
    0x74, 0x30, 0x1E, 0x17, 0x0D, 0x32, 0x30, 0x30, 0x35, 0x31, 0x31, 0x32, 0x33, 0x31, 0x31, 0x33, 0x38, 0x5A, 0x17, 0x0D, 0x34,
    0x35, 0x30, 0x35, 0x30, 0x35, 0x32, 0x33, 0x31, 0x31, 0x33, 0x38, 0x5A, 0x30, 0x39, 0x31, 0x37, 0x30, 0x35, 0x06, 0x03, 0x55,
    0x04, 0x03, 0x0C, 0x2E, 0x43, 0x6F, 0x6D, 0x63, 0x61, 0x73, 0x74, 0x20, 0x49, 0x6E, 0x74, 0x65, 0x67, 0x72, 0x61, 0x74, 0x69,
    0x6F, 0x6E, 0x20, 0x54, 0x65, 0x73, 0x74, 0x69, 0x6E, 0x67, 0x20, 0x45, 0x43, 0x43, 0x20, 0x43, 0x6C, 0x61, 0x73, 0x73, 0x20,
    0x49, 0x49, 0x49, 0x20, 0x52, 0x6F, 0x6F, 0x74, 0x30, 0x59, 0x30, 0x13, 0x06, 0x07, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x02, 0x01,
    0x06, 0x08, 0x2A, 0x86, 0x48, 0xCE, 0x3D, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0x1E, 0x1E, 0xF4, 0x36, 0xE2, 0x67, 0x6C,
    0xE0, 0x79, 0x12, 0x91, 0x9B, 0x4C, 0x72, 0xEC, 0xF2, 0x40, 0xEE, 0x70, 0x49, 0x62, 0x10, 0x31, 0x17, 0xCE, 0x43, 0x87, 0xD7,
    0xCF, 0xE8, 0xB5, 0xD4, 0x0F, 0xDC, 0x78, 0x9F, 0x1F, 0x69, 0xCE, 0x23, 0xE1, 0xAE, 0x76, 0xBC, 0x3F, 0x25, 0x9B, 0xFD, 0x4F,
    0x98, 0x78, 0x3E, 0x17, 0x1F, 0x15, 0x48, 0x15, 0x7D, 0x0E, 0x2B, 0x26, 0x83, 0xA4, 0x8F, 0xA3, 0x66, 0x30, 0x64, 0x30, 0x12,
    0x06, 0x03, 0x55, 0x1D, 0x13, 0x01, 0x01, 0xFF, 0x04, 0x08, 0x30, 0x06, 0x01, 0x01, 0xFF, 0x02, 0x01, 0x01, 0x30, 0x1F, 0x06,
    0x03, 0x55, 0x1D, 0x23, 0x04, 0x18, 0x30, 0x16, 0x80, 0x14, 0x36, 0xF4, 0x0A, 0xA5, 0x0A, 0xF4, 0x2B, 0xCE, 0x4E, 0x57, 0xD4,
    0xCC, 0x7F, 0xB0, 0x32, 0xAC, 0xAA, 0xCB, 0xD8, 0xB5, 0x30, 0x1D, 0x06, 0x03, 0x55, 0x1D, 0x0E, 0x04, 0x16, 0x04, 0x14, 0x36,
    0xF4, 0x0A, 0xA5, 0x0A, 0xF4, 0x2B, 0xCE, 0x4E, 0x57, 0xD4, 0xCC, 0x7F, 0xB0, 0x32, 0xAC, 0xAA, 0xCB, 0xD8, 0xB5, 0x30, 0x0E,
    0x06, 0x03, 0x55, 0x1D, 0x0F, 0x01, 0x01, 0xFF, 0x04, 0x04, 0x03, 0x02, 0x01, 0x86, 0x30, 0x0A, 0x06, 0x08, 0x2A, 0x86, 0x48,
    0xCE, 0x3D, 0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02, 0x21, 0x00, 0x93, 0x58, 0x2C, 0x17, 0xEC, 0xC7, 0x4F, 0x89,
    0x16, 0x58, 0x9C, 0x62, 0x5D, 0x41, 0x64, 0x47, 0x24, 0x81, 0x16, 0xB3, 0x9D, 0xD7, 0x57, 0x02, 0xC3, 0x30, 0xEF, 0xE5, 0xD0,
    0x36, 0x8D, 0xCF, 0x02, 0x20, 0x29, 0xBE, 0x43, 0x64, 0x91, 0x32, 0x64, 0xE1, 0xC0, 0x31, 0xB9, 0x0A, 0x6D, 0x5B, 0xC3, 0xBF,
    0xA2, 0xFF, 0x7D, 0x0B, 0x79, 0xDC, 0xF5, 0xDA, 0xDE, 0x3C, 0x1B, 0xE8, 0xD9, 0x10, 0x3C, 0x66
};

} // namespace Crypto
} // namespace chip
