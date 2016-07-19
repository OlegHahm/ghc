/**
 * General Header Compression
 * Defined in http://tools.ietf.org/html/draft-bormann-6lowpan-ghc-06
 *
 * Software Project in Telematics
 * SS13 Freie Universität Berlin
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ghc.h"

/*
 * Initiates the dictionary buffer composed out of the pseudo header and a static dictionary
 *
 * @param [out] comp_buf  Buffer where to put the dictionary
 * @param [in]  hdr       48-byte long header
 *
 */
void dictionary_buffer_init(uint8_t* comp_buf, uint8_t* hdr)
{
    const uint8_t static_dictionary[] = {
        0x16, 0xfe, 0xfd, 0x17, 0xfe, 0xfd, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00 };
    
    /* Copy src address */
    memcpy(&comp_buf[0], &hdr[8], 16);
    /* Copy dst address */
    memcpy(&comp_buf[16], &hdr[24], 16);
    /* Copy 16 byte static dictionary */
    memcpy(&comp_buf[32], static_dictionary, 16);
}

/*
 * Decompresses the payload
 *
 * @param [out] decomp_buf    Buffer where to put the decompressed result
 * @param [in]  hdr           48-byte long header
 * @param [in]  comp_buf      Buffer to decompress
 * @param [in]  comp_buf_len  Length of comp_buf
 *
 */
void decompress(uint8_t *decomp_buf, uint8_t *hdr, uint8_t *comp_buf, int comp_buf_len)
{
    int decomp_buf_index = 48;
    dictionary_buffer_init(decomp_buf, hdr);
    
    int na = 0x00, sa = 0x00;
    int n = 0, s = 0;
    
    for (int i = 0; i < comp_buf_len; i++) {
        if ((comp_buf[i] & 0x80) == 0x00) {
            /* Append k bytes of data */
            memcpy(&decomp_buf[decomp_buf_index], &comp_buf[i+1], comp_buf[i]);
            decomp_buf_index += comp_buf[i];
            i += comp_buf[i];
            continue;
        } else if ((comp_buf[i] & 0xF0) == 0x80) {
            /* Append n + 2 bytes of zeroes */
            memset(&decomp_buf[decomp_buf_index], 0x00, (comp_buf[i] & 0x0F) + 2);
            decomp_buf_index += (comp_buf[i] & 0x0F) + 2;
            continue;
        } else if ((comp_buf[i] & 0xE0) == 0xA0) {
            /* Set up back refernece */
            na += (comp_buf[i] & 0x10) >> 1;
            sa += (comp_buf[i] & 0x0F) << 3;
            continue;
        } else if ((comp_buf[i] & 0xC0) == 0xC0) {
            /* Back reference */
            n = na + ((comp_buf[i] & 0x38) >> 3) + 2;
            s = (comp_buf[i] & 0x07) + sa + n;
            memcpy(&decomp_buf[decomp_buf_index], &decomp_buf[decomp_buf_index - s], n);
            decomp_buf_index += n;
            na = 0x00;
            sa = 0x00;
            continue;
        } else if ((comp_buf[i] & 0xFF) == 0x90) {
            /* STOP code, not implemented yet */
            continue;
        }
    }

    if (DEBUG) {
        printf("--------\n");
        for (int x = 48; x < decomp_buf_index; x++) {
            printf("%02x ", (unsigned char)decomp_buf[x]);
        }
        printf("\n--------\n");
    }
}

/*
 * Compresses the payload
 *  
 * @param [out] comp_buf          Buffer where to put the compressed result
 * @param [in]  hdr               48-byte long header
 * @param [in]  payload           Buffer to compress
 * @param [in]  payload_buf_len   Length of payload_buf
 *
 */
void compress(uint8_t *comp_buf, uint8_t *hdr, uint8_t *payload_buf, int payload_buf_len)
{
    uint8_t buffer[48+payload_buf_len];
    dictionary_buffer_init(buffer, hdr);
    memcpy(&buffer[48], payload_buf, payload_buf_len);
    
    int buffer_index = 0;
    int copy_buffer = 0;
    int payload_index = 0;
    
    for (int i = 0; i < payload_buf_len; i++) {
        /* Count zero sequence */
        int zero_sequence = 0;
        for (int z = i; z<i+17; z++) {
            if (payload_buf[z] == 0x00) {
                zero_sequence++;
            } else {
                break;
            }
        }
        
        /* Dictionary search */
        int index = 0, index_best = 0;
        int append = 0, append_best = 0;

        for (int dictionary_index = 0; dictionary_index < i+48;) {
                /* First match and not the last byte in payload */
            if ((payload_index < payload_buf_len) && (append == 0) && (((i+48) - dictionary_index) > 1) &&
                /* Matching pair */
                ((payload_buf[payload_index] == buffer[dictionary_index]) && (payload_buf[payload_index+1] == buffer[dictionary_index+1]))) {
                /* Found first matching pair */
                index = dictionary_index;
                append = 2;
                
                if (append >= append_best) {
                    append_best = append;
                    index_best = index;
                }
                
                dictionary_index += 2;
                payload_index += 2;

                continue;
            } else if ((payload_index < payload_buf_len) && (append > 0) && ((payload_buf[payload_index] == buffer[dictionary_index]))) {
                /* Found 2n+1 match */
                append++;
                
                if (append >= append_best) {
                    append_best = append;
                    index_best = index;
                }
                
                dictionary_index++;
                payload_index++;

                continue;
            } else if ((append > 0) && (payload_buf[payload_index] != buffer[dictionary_index])) {
                /* Set best match */
                if (append >= append_best) {
                    append_best = append;
                    index_best = index;
                }
                payload_index = i;
                dictionary_index -= append - 1;
                append = 0, index = 0;
                
                continue;
            }
            /* No match */
            dictionary_index++;
        }
        /* Set back payload point */
        payload_index = i;
        if ((append_best > zero_sequence) && ((append_best < 3 && (i + 48 - index_best) < 10) || (append_best > 2))) {
            /* Assuming that zeros are in static dic */
           
            /* Stop copy run */
            if (copy_buffer > 0) {
                if (DEBUG) {
                    printf("copy: ");
                    
                    for (int copy_i = buffer_index-copy_buffer -1; copy_i < buffer_index; copy_i++) {
                        printf("%02x ", (unsigned char)comp_buf[copy_i]);
                    }
                    printf("\n");
                }
                copy_buffer = 0;
            }

            /* Only append 7+2 bytes and offeset 7+9 (n) without extending the reference */
            uint8_t backref = 0xc0;
            uint8_t extended_backref = 0xa0;
            
            int n = append_best - 2;
            int s = i + 48 - index_best - n;
            int times_n = 0;
            int times_s = 0;
            int rest_n = 0;
            int rest_s = 0;

            if (DEBUG) {
                printf("ref(%d) - Append: %d\n", s + n, n + 2);
            }
            
            /* Setup extended back reference */
            if (n > 7 || s > 9) {
                times_n = n / 8;
                rest_n = n % 8;
                times_s = s / 120;
                rest_s = s % 120;
                
                /* Send */
                while (times_s > 0) {
                    extended_backref = 0xa0;
                    if (times_n > 0) {
                        extended_backref += 0x10;
                        times_n--;
                    }
                    /* Set all s's to 1s */
                    extended_backref +=  0xf;
                    times_s--;
                    memset(&comp_buf[buffer_index], extended_backref, 1);
                    buffer_index++;
                }

                extended_backref = 0xa0;
                if (times_n > 0) {
                    extended_backref += 0x10;
                    times_n--;
                }
                
                /* Mask rest_s */
                extended_backref += ((rest_s - 2) & 0x78) >> 3;
                memset(&comp_buf[buffer_index], extended_backref, 1);
                buffer_index++;
                
                while (times_n > 0) {
                    extended_backref = 0xb0;
                    memset(&comp_buf[buffer_index], extended_backref, 1);
                    buffer_index++;
                }
                
                backref += (rest_n << 3) + ((rest_s - 2) & 0x07);
                
                memset(&comp_buf[buffer_index], backref, 1);
                buffer_index++;
            } else {
                backref += (n << 3) + s - 2;
                memset(&comp_buf[buffer_index], backref, 1);
            
                buffer_index++;
            }
            /* Move pointers forward */
            i += append_best - 1;
            payload_index += append_best;
            

        } else if (zero_sequence > 1) {
            /* Zero sequence */
            if (DEBUG) {
                printf("%d nulls \n", zero_sequence);
            }
            memset(&comp_buf[buffer_index], 0x80 + zero_sequence - 2, 1);
            buffer_index++;
            
            /* Move pointers forward */
            i += zero_sequence - 1;
            payload_index += zero_sequence;
            
            copy_buffer = 0;
    
        } else {
            /* No dictionary match or zero sequence found, copy instead */
            if (copy_buffer == 0) {
                /* Set copy byte code */
                memset(&comp_buf[buffer_index], 0x00 + copy_buffer + 1, 1);
                buffer_index++;
                memcpy(&comp_buf[buffer_index], &payload_buf[payload_index], 1);
                
                copy_buffer++;
                buffer_index++;
                payload_index++;
            } else {
                /* Update copy byte code */
                memset(&comp_buf[buffer_index - copy_buffer - 1], 0x00 + copy_buffer + 1, 1);
                memcpy(&comp_buf[buffer_index], &payload_buf[payload_index], 1);
                
                copy_buffer++;
                buffer_index++;
                payload_index++;
            }
            
        }
    }
    if (DEBUG) {
        printf("--------\n");
        for (int x = 0; x < buffer_index; x++) {
            printf("%02x ", (unsigned char)comp_buf[x]);
        }
        printf("\n--------\n");
    }
}
