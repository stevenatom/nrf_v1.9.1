//
// Copyright 2019 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
// These materials are licensed under the Amazon Software License in connection with the Alexa Gadgets Program.
// The Agreement is available at https://aws.amazon.com/asl/.
// See the Agreement for the specific terms and conditions of the Agreement.
// Capitalized terms not defined in this file have the meanings given to them in the Agreement.
//

#include <stdio.h>
#include <assert.h>
#include "gadget_packet.h"

static uint16_t compute_checksum(spp_directive_response_packet *resp, int len)
{
    //payload_len: The actual length of the packet
    //subtracting 2 here, to remove checksum from len
    //else we will end up computing checksum of the checksum
    int payload_len = len;
    uint16_t checksum = 0;
    checksum += (uint16_t) resp->header.cmd;
    checksum += (uint16_t) resp->header.err;
    int index;

    for (index = 0; index < payload_len; index++)
    {
        checksum += (uint16_t)resp->data[index];
    }
    return checksum;
}

int main()
{
    uint8_t buffer[1024];
    spp_directive_response_packet *resp = (spp_directive_response_packet*) buffer;
uint8_t buf[] = {0xf0, 0x02, 0x00, 0x01, 0x0a, 0x87, 0x02, 0x0a, 0x24, 0x0a, 0x0f, 0x41, 0x6c, 0x65, 0x78, 0x61, 0x2e, 0x44, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x12, 0x11, 0x44, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x2e, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x12, 0xde, 0x01, 0x0a, 0xdb, 0x01, 0x0a, 0x0c, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47, 0x31, 0x32, 0x33, 0x34, 0x35, 0x12, 0x0b, 0x42, 0x45, 0x43, 0x75, 0x72, 0x69, 0x6f, 0x73, 0x69, 0x74, 0x79, 0x1a, 0x0c, 0x54, 0x69, 0x6e, 0x6b, 0x65, 0x72, 0x20, 0x54, 0x61, 0x62, 0x6c, 0x65, 0x22, 0x06, 0x4b, 0x69, 0x64, 0x73, 0x49, 0x49, 0x5a, 0x53, 0x0a, 0x0e, 0x41, 0x6c, 0x65, 0x78, 0x61, 0x49, 0x6e, 0x74, 0x65, 0x72, 0x66, 0x61, 0x63, 0x65, 0x12, 0x1a, 0x41, 0x6c, 0x65, 0x78, 0x61, 0x2e, 0x47, 0x61, 0x64, 0x67, 0x65, 0x74, 0x2e, 0x53, 0x74, 0x61, 0x74, 0x65, 0x4c, 0x69, 0x73, 0x74, 0x65, 0x6e, 0x65, 0x72, 0x1a, 0x03, 0x31, 0x2e, 0x30, 0x22, 0x20, 0x0a, 0x0a, 0x0a, 0x08, 0x77, 0x61, 0x6b, 0x65, 0x77, 0x6f, 0x72, 0x64, 0x0a, 0x08, 0x0a, 0x06, 0x74, 0x69, 0x6d, 0x65, 0x72, 0x73, 0x0a, 0x08, 0x0a, 0x06, 0x61, 0x6c, 0x61, 0x72, 0x6d, 0x73, 0x62, 0x53, 0x0a, 0x01, 0x31, 0x12, 0x20, 0x6e, 0x6a, 0x21, 0x26, 0x8e, 0xa9, 0xea, 0xfb, 0xf2, 0x03, 0x56, 0x5e, 0x58, 0x80, 0xeb, 0xc6, 0xfe, 0x03, 0xa3, 0xb9, 0xda, 0x6e, 0xc9, 0xef, 0x94, 0x15, 0x47, 0x2f, 0x90, 0xcf, 0xae, 0xcd, 0xad, 0x1a, 0x01, 0x31, 0x22, 0x0e, 0x41, 0x31, 0x56, 0x35, 0x33, 0x31, 0x48, 0x46, 0x59, 0x30, 0x5a, 0x4f, 0x42, 0x35, 0x2a, 0x0b, 0x42, 0x45, 0x43, 0x75, 0x72, 0x69, 0x6f, 0x73, 0x69, 0x74, 0x79, 0x32, 0x0c, 0x32, 0x30, 0x37, 0x33, 0x35, 0x62, 0x30, 0x37, 0x36, 0x39, 0x33, 0x31, 0x56, 0xbf, 0xf1};

    uint16_t index = 0;
    resp->header.sof = buf[index++];

    assert(resp->header.sof == SOP);

    resp->header.cmd = buf[index++];
    resp->header.err = buf[index++];
    resp->header.seqId = buf[index++];

    uint16_t index2 = 0;
    uint8_t escape = 0;
    //Leave the last 3 bytes for checksum and EOF
    for (; index < sizeof(buf) - 3; index++)
    {

        if (buf[index] == ESP)
        {
            escape = 1;
            continue;
        }

        if (escape)
        {
            resp->data[index2++] = ESP ^ buf[index];
            escape = 0;
        }
        else
        {
            resp->data[index2++] = buf[index];
        }
    }

    spp_packet_directive_trailer* trailer = (spp_packet_directive_trailer*) (buf + index);
    assert(trailer->eof == EOP);
    //correct checksum (to big endian)
    trailer->checksum = (buf[index] << 8) | (buf[index+1]);

    printf("packet checksum = %d, trailer = %d\n", trailer->checksum, trailer->eof);

    uint16_t checksum = compute_checksum(resp, index2);
    printf("checksum:%d\n, in hex: 0x%02x 0x%02x\n", checksum, (0xff & (checksum >> 8)), (checksum & 0xff));
    assert(trailer->checksum == checksum);
}
