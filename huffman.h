#ifndef HUFFMAN_HUFFMAN_H
#define HUFFMAN_HUFFMAN_H

#include <stdint.h>
#include <stdio.h>

int huffman_encode_file(FILE* in, FILE* out);

int huffman_decode_file(FILE* in, FILE* out);

int huffman_encode_memory(const unsigned char* bufin,
			  uint32_t bufinlen,
			  unsigned char** pbufout,
			  uint32_t* pbufoutlen);

int huffman_decode_memory(const unsigned char* bufin,
			  uint32_t bufinlen,
			  unsigned char** bufout,
			  uint32_t* pbufoutlen);

#endif
