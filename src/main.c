/*
 * Copyright (C) 2013 Freie Universität Berlin
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * General Header Compression Test Case
 * Defined in http://tools.ietf.org/html/draft-bormann-6lowpan-ghc-06
 *
 * Software Project in Telematics
 * SS13 Freie Universität Berlin
 *
 * @author Fares Mokrani
 * @author Christoffer Hamberg
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ghc.h"

void compareBuffer(uint8_t *buffer1, uint8_t *buffer2, int buffer_len, int offset)
{
    for (int i = 0; i < buffer_len; i++) {
        if (buffer1[i + offset] != buffer2[i]) {
            printf("Failed: i: %d, Got: %02x, Expected: %02x\n", i + offset, buffer1[i + offset], buffer2[i]);
            return;
        }
    }
    printf("Passed\n");
}

void compareDictionary(uint8_t *buffer1, uint8_t *buffer2, int buffer_len)
{
    for (int i = 0; i < buffer_len; i++) {
        if (buffer1[i] != buffer2[i]) {
            printf("Failed: i: %d, Got: %02x, Expected: %02x\n", i, buffer1[i], buffer2[i]);
            return;
        }
    }
    printf("Passed\n");
}

int main(int argc, const char * argv[])
{
    uint8_t buffer[BUFFERSIZE];
    uint8_t buffer2[BUFFERSIZE];
        
    uint8_t hdr0[] = {
        0x60, 0x00, 0x00, 0x00, 0x00, 0x08, 0x3a, 0xff, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x20, 0x24, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a };
    
    uint8_t payload0[] = {
        0x9b, 0x00, 0x6b, 0xde, 0x00, 0x00, 0x00, 0x00 };
    
    uint8_t dictionary0[] = {
        0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x20, 0x24,
        0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed0[] = {0x04, 0x9b, 0x00, 0x6b, 0xde, 0x82 };
    
    printf("Testcase: 0\n");
    dictionary_buffer_init(buffer, hdr0);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary0, sizeof(dictionary0));
    
    compress(buffer, hdr0, payload0, sizeof(payload0));
    printf("Compress: ");
    compareBuffer(buffer, compressed0, sizeof(compressed0), 0);
    
    decompress(buffer2, hdr0, buffer, sizeof(compressed0));
    printf("Decompress: ");
    compareBuffer(buffer2, payload0, sizeof(payload0), 48);
    printf("______\n");
    
    uint8_t hdr1[] = {
        0x60, 0x00, 0x00, 0x00, 0x00, 0x5c, 0x3a, 0xff, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a };
    
    uint8_t payload1[] = {
        0x9b, 0x01, 0x7a, 0x5f, 0x00, 0xf0, 0x01, 0x00, 0x88, 0x00, 0x00, 0x00, 0x20, 0x02, 0x0d, 0xb8,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0xfa, 0xce, 0x04, 0x0e, 0x00, 0x14,
        0x09, 0xff, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x1e, 0x80, 0x20,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x0d, 0xb8,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0xfa, 0xce, 0x03, 0x0e, 0x40, 0x00,
        0xff, 0xff, 0xff, 0xff, 0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00 };
    
    uint8_t dictionary1[] = {
        0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23,
        0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1a,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed1[] = {
        0x06, 0x9b, 0x01, 0x7a, 0x5f, 0x00, 0xf0, 0xc7, 0x01, 0x88, 0x81, 0x04, 0x20, 0x02, 0x0d, 0xb8,
        0x85, 0xa7, 0xc9, 0x08, 0xfa, 0xce, 0x04, 0x0e, 0x00, 0x14, 0x09, 0xff, 0xa4, 0xda, 0x83, 0x06,
        0x08, 0x1e, 0x80, 0x20, 0xff, 0xff, 0xc0, 0xd0, 0x82, 0xb4, 0xf0, 0x03, 0x03, 0x0e, 0x40, 0xc7,
        0xa3, 0xc9, 0xa2, 0xf0 };
    
    printf("Testcase: 1\n");
    dictionary_buffer_init(buffer, hdr1);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary1, sizeof(dictionary1));
    
    compress(buffer, hdr1, payload1, sizeof(payload1));
    printf("Compress: ");
    compareBuffer(buffer, compressed1, sizeof(compressed1), 0);
    
    decompress(buffer2, hdr1, buffer, sizeof(compressed1));
    printf("Decompress: ");
    compareBuffer(buffer2, payload1, sizeof(payload1), 48);
    printf("______\n");
 
    uint8_t hdr2[] = {
        0x60, 0x00, 0x00, 0x00, 0x00, 0x32, 0x3a, 0xff, 0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x33, 0x44, 0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x11, 0x22 };
    
    uint8_t payload2[] = {
        0x9b, 0x02, 0x58, 0x7d, 0x01, 0x80, 0x00, 0xf1, 0x05, 0x12, 0x00, 0x80, 0x20, 0x02, 0x0d, 0xb8,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x33, 0x44, 0x06, 0x14, 0x00, 0x80,
        0xf1, 0x00, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00,
        0x11, 0x22 };
    
    uint8_t dictionary2[] = {
        0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x33, 0x44,
        0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x11, 0x22,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed2[] = {
        0x0c, 0x9b, 0x02, 0x58, 0x7d, 0x01, 0x80, 0x00, 0xf1, 0x05, 0x12, 0x00, 0x80, 0xb5, 0xf4, 0x08,
        0x06, 0x14, 0x00, 0x80, 0xf1, 0x00, 0xfe, 0x80, 0x87, 0xa7, 0xdd };
    
    printf("Testcase: 2\n");
    dictionary_buffer_init(buffer, hdr2);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary2, sizeof(dictionary2));
    
    compress(buffer, hdr2, payload2, sizeof(payload2));
    printf("Compress: ");
    compareBuffer(buffer, compressed2, sizeof(compressed2), 0);
    
    decompress(buffer2, hdr2, buffer, sizeof(compressed2));
    printf("Decompress: ");
    compareBuffer(buffer2, payload2, sizeof(payload2), 48);
    printf("______\n");
    
    uint8_t hdr3[] = {
        0x60, 0x00, 0x00, 0x00, 0x00, 0x30, 0x3a, 0xff, 0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x3b, 0xd3, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23 };
    
    uint8_t payload3[] = {
        0x87, 0x00, 0xa7, 0x68, 0x00, 0x00, 0x00, 0x00, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23, 0x01, 0x01, 0x3b, 0xd3, 0x00, 0x00, 0x00, 0x00,
        0x1f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x20, 0x24
    };
    
    uint8_t dictionary3[] = {
        0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x3b, 0xd3,
        0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed3[] = {
        0x04, 0x87, 0x00, 0xa7, 0x68, 0x82, 0xb3, 0xf0, 0x04, 0x01, 0x01, 0x3b, 0xd3, 0x82, 0x02, 0x1f,
        0x02, 0x83, 0x02, 0x06, 0x00, 0xa2, 0xdb, 0x02, 0x20, 0x24 };
    
    printf("Testcase: 3\n");
    dictionary_buffer_init(buffer, hdr3);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary3, sizeof(dictionary3));
    
    compress(buffer, hdr3, payload3, sizeof(payload3));
    printf("Compress: ");
    compareBuffer(buffer, compressed3, sizeof(compressed3), 0);
    
    decompress(buffer2, hdr3, buffer, sizeof(compressed3));
    printf("Decompress: ");
    compareBuffer(buffer2, payload3, sizeof(payload3), 48);
    printf("______\n");
    
    uint8_t hdr4[] = {
        0x60, 0x00, 0x00, 0x00, 0x00, 0x30, 0x3a, 0xfe, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23, 0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x3b, 0xd3 };
    
    uint8_t payload4[] = {
        0x88, 0x00, 0x26, 0x6c, 0xc0, 0x00, 0x00, 0x00, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23, 0x02, 0x01, 0xfa, 0xce, 0x00, 0x00, 0x00, 0x00,
        0x1f, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x20, 0x24
    };
    
    uint8_t dictionary4[] = {
        0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x1c, 0xda, 0xff, 0xfe, 0x00, 0x30, 0x23,
        0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x3b, 0xd3,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed4[] = {
        0x05, 0x88, 0x00, 0x26, 0x6c, 0xc0, 0x81, 0xb5, 0xf0, 0x04, 0x02, 0x01, 0xfa, 0xce, 0x82, 0x02,
        0x1f, 0x02, 0x83, 0x02, 0x06, 0x00, 0xa2, 0xdb, 0x02, 0x20, 0x24 };
    
    printf("Testcase: 4\n");
    dictionary_buffer_init(buffer, hdr4);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary4, sizeof(dictionary4));
    
    compress(buffer, hdr4, payload4, sizeof(payload4));
    printf("Compress: ");
    compareBuffer(buffer, compressed4, sizeof(compressed4), 0);
    
    decompress(buffer2, hdr4, buffer, sizeof(compressed4));
    printf("Decompress: ");
    compareBuffer(buffer2, payload4, sizeof(payload4), 48);
    printf("______\n");
  
    uint8_t hdr5[] = {
        0x60, 0x00, 0x00, 0x00, 0x00, 0x18, 0x3a, 0xff, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xae, 0xde, 0x48, 0x00, 0x00, 0x00, 0x00, 0x01, 0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02 };
    
    uint8_t payload5[] = {
        0x85, 0x00, 0x90, 0x65, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0xac, 0xde, 0x48, 0x00, 0x00, 0x00,
        0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    uint8_t dictionary5[] = {
        0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xae, 0xde, 0x48, 0x00, 0x00, 0x00, 0x00, 0x01,
        0xff, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed5[] = {
        0x04, 0x85, 0x00, 0x90, 0x65, 0xde, 0x02, 0x02, 0xac, 0xa5, 0xeb, 0x84 };
    
    printf("Testcase: 5\n");
    dictionary_buffer_init(buffer, hdr5);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary5, sizeof(dictionary5));
    
    compress(buffer, hdr5, payload5, sizeof(payload5));
    printf("Compress: ");
    compareBuffer(buffer, compressed5, sizeof(compressed5), 0);
    
    decompress(buffer2, hdr5, buffer, sizeof(compressed5));
    printf("Decompress: ");
    compareBuffer(buffer2, payload5, sizeof(payload5), 48);
    printf("______\n");
    
    uint8_t hdr6[] = {
        0x60, 0x00, 0x00, 0x00, 0x00, 0x60, 0x3a, 0xff, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x10, 0x34, 0x00, 0xff, 0xfe, 0x00, 0x11, 0x22, 0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0xae, 0xde, 0x48, 0x00, 0x00, 0x00, 0x00, 0x01 };
    
    uint8_t payload6[] = {
        0x86, 0x00, 0x55, 0xc9, 0x40, 0x00, 0x0f, 0xa0, 0x1c, 0x5a, 0x38, 0x17, 0x00, 0x00, 0x07, 0xd0,
        0x01, 0x01, 0x11, 0x22, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x40, 0x40, 0xff, 0xff, 0xff, 0xff,
        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x02, 0x40, 0x10, 0x00, 0x00, 0x03, 0xe8,
        0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x21, 0x03, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00,
        0x20, 0x02, 0x0d, 0xb8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xfe, 0x00, 0x11, 0x22
    };
    
    uint8_t dictionary6[] = {
        0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x34, 0x00, 0xff, 0xfe, 0x00, 0x11, 0x22,
        0xfe, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xae, 0xde, 0x48, 0x00, 0x00, 0x00, 0x00, 0x01,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed6[] = {
        0x0c, 0x86, 0x00, 0x55, 0xc9, 0x40, 0x00, 0x0f, 0xa0, 0x1c, 0x5a, 0x38, 0x17, 0x80, 0x06, 0x07,
        0xd0, 0x01, 0x01, 0x11, 0x22, 0x82, 0x06, 0x03, 0x04, 0x40, 0x40, 0xff, 0xff, 0xc0, 0xd0, 0x82,
        0x04, 0x20, 0x02, 0x0d, 0xb8, 0x8a, 0x04, 0x20, 0x02, 0x40, 0x10, 0xa4, 0xcb, 0x01, 0xe8, 0xa2,
        0xf0, 0x02, 0x21, 0x03, 0xa9, 0xe6, 0xb3, 0xcd, 0xaf, 0xdb };
    
    printf("Testcase: 6\n");
    dictionary_buffer_init(buffer, hdr6);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary6, sizeof(dictionary6));
    
    compress(buffer, hdr6, payload6, sizeof(payload6));
    printf("Compress: ");
    compareBuffer(buffer, compressed6, sizeof(compressed6), 0);
    
    decompress(buffer2, hdr6, buffer, sizeof(compressed6));
    printf("Decompress: ");
    compareBuffer(buffer2, payload6, sizeof(payload6), 48);
    printf("______\n");
    
    uint8_t hdr7[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    uint8_t payload7[] = {
        0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x1d, 0x00, 0x01, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x01, 0x09, 0xb2, 0x0e, 0x82, 0xc1, 0x6e, 0xb6, 0x96, 0xc5, 0x1f, 0x36,
        0x8d, 0x17, 0x61, 0xe2, 0xb5, 0xd4, 0x22, 0xd4, 0xed, 0x2b };
    
    uint8_t dictionary7[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed7[] = {
        0xb0, 0xd1, 0x01, 0x1d, 0xf2, 0x15, 0x09, 0xb2, 0x0e, 0x82, 0xc1, 0x6e, 0xb6, 0x96, 0xc5, 0x1f,
        0x36, 0x8d, 0x17, 0x61, 0xe2, 0xb5, 0xd4, 0x22, 0xd4, 0xed, 0x2b };
    
    printf("Testcase: 7\n");
    dictionary_buffer_init(buffer, hdr7);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary7, sizeof(dictionary7));
    
    compress(buffer, hdr7, payload7, sizeof(payload7));
    printf("Compress: ");
    compareBuffer(buffer, compressed7, sizeof(compressed7), 0);
    
    decompress(buffer2, hdr7, buffer, sizeof(compressed7));
    printf("Decompress: ");
    compareBuffer(buffer2, payload7, sizeof(payload7), 48);
    printf("______\n");
    
    uint8_t hdr8[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    uint8_t payload8[] = {
        0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x16, 0x00, 0x01, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x05, 0xae, 0xa0, 0x15, 0x56, 0x67, 0x92, 0x4d, 0xff, 0x8a, 0x24, 0xe4,
        0xcb, 0x35, 0xb9 };
    
    uint8_t dictionary8[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed8[] = {
        0xb0, 0xc3, 0x03, 0x05, 0x00, 0x16, 0xf2, 0x0e, 0xae, 0xa0, 0x15, 0x56, 0x67, 0x92, 0x4d, 0xff,
        0x8a, 0x24, 0xe4, 0xcb, 0x35, 0xb9 };
    
    printf("Testcase: 8\n");
    dictionary_buffer_init(buffer, hdr8);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary8, sizeof(dictionary8));
    
    compress(buffer, hdr8, payload8, sizeof(payload8));
    printf("Compress: ");
    compareBuffer(buffer, compressed8, sizeof(compressed8), 0);
    
    decompress(buffer2, hdr8, buffer, sizeof(compressed8));
    printf("Decompress: ");
    compareBuffer(buffer2, payload8, sizeof(payload8), 48);
    printf("______\n");
    
    uint8_t hdr9[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
    
    uint8_t payload9[] = {
        0x16, 0xfe, 0xfd, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x36, 0x01, 0x00, 0x00,
        0x2a, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2a, 0xfe, 0xfd, 0x51, 0x52, 0xed, 0x79, 0xa4,
        0x20, 0xc9, 0x62, 0x56, 0x11, 0x47, 0xc9, 0x39, 0xee, 0x6c, 0xc0, 0xa4, 0xfe, 0xc6, 0x89, 0x2f,
        0x32, 0x26, 0x9a, 0x16, 0x4e, 0x31, 0x7e, 0x9f, 0x20, 0x92, 0x92, 0x00, 0x00, 0x00, 0x02, 0xc0,
        0xa8, 0x01, 0x00 };
    
    uint8_t dictionary9[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    uint8_t compressed9[] = {
        0xa1, 0xcd, 0x87, 0x01, 0x36, 0xa1, 0xcd, 0x01, 0x2a, 0x85, 0x23, 0x2a, 0xfe, 0xfd, 0x51, 0x52,
        0xed, 0x79, 0xa4, 0x20, 0xc9, 0x62, 0x56, 0x11, 0x47, 0xc9, 0x39, 0xee, 0x6c, 0xc0, 0xa4, 0xfe,
        0xc6, 0x89, 0x2f, 0x32, 0x26, 0x9a, 0x16, 0x4e, 0x31, 0x7e, 0x9f, 0x20, 0x92, 0x92, 0x81, 0x05,
        0x02, 0xc0, 0xa8, 0x01, 0x00 };
    
    printf("Testcase: 9\n");
    dictionary_buffer_init(buffer, hdr9);
    printf("Dictionary: ");
    compareDictionary(buffer, dictionary9, sizeof(dictionary9));
    
    compress(buffer, hdr9, payload9, sizeof(payload9));
    printf("Compress: ");
    compareBuffer(buffer, compressed9, sizeof(compressed9), 0);
    
    decompress(buffer2, hdr9, buffer, sizeof(compressed9));
    printf("Decompress: ");
    compareBuffer(buffer2, payload9, sizeof(payload9), 48);
    printf("______\n");
    
    return 0;
}
