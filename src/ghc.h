/**
 * General Header Compression
 * Defined in http://tools.ietf.org/html/draft-bormann-6lowpan-ghc-06
 *
 * Software Project in Telematics
 * SS13 Freie Universität Berlin
 *
 */

#ifndef GHC_ghc_h
#define GHC_ghc_h
typedef __int8_t uint8_t;

#define COPY        0x00
#define ZERO        0x80
#define STOP        0x90
#define SET_BACKREF 0xA0
#define BACKREF     0xC0

#define BUFFERSIZE  1000
#define DEBUG       0

void dictionary_buffer_init(uint8_t *comp_buffer, uint8_t* hdr);
void decompress(uint8_t *decomp_buf, uint8_t *hdr, uint8_t *comp_buf, int comp_buf_length);
void compress(uint8_t *comp_buf, uint8_t *hdr, uint8_t *payload_buf, int payload_buf_len);

#endif
