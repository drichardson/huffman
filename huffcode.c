/*
 *  huffcode - Encode/Decode files using Huffman encoding.
 *  http://huffman.sourceforge.net
 *  Copyright (C) 2003  Douglas Ryan Richardson
 */

#include "huffman.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <assert.h>

#ifdef WIN32
#include <malloc.h>
extern int getopt(int, char**, char*);
extern char* optarg;
#else
#include <unistd.h>
#endif

static int memory_encode_file(FILE *in, FILE *out);
static int memory_decode_file(FILE *in, FILE *out);

static void
version(FILE *out)
{
	fputs("huffcode 0.3\n"
	      "Copyright (C) 2003 Douglas Ryan Richardson"
	      "; Gauss Interprise, Inc\n",
	      out);
}

static void
usage(FILE* out)
{
	fputs("Usage: huffcode [-i<input file>] [-o<output file>] [-d|-c]\n"
		  "-i - input file (default is standard input)\n"
		  "-o - output file (default is standard output)\n"
		  "-d - decompress\n"
		  "-c - compress (default)\n"
		  "-m - read file into memory, compress, then write to file (not default)\n",
		  out);
}

int
main(int argc, char** argv)
{
	char memory = 0;
	char compress = 1;
	int opt;
	const char *file_in = NULL, *file_out = NULL;
	FILE *in = stdin;
	FILE *out = stdout;
	int close_in = 0;
	int close_out = 0;
	int rc = 0;

	/* Get the command line arguments. */
	while((opt = getopt(argc, argv, "i:o:cdhvm")) != -1)
	{
		switch(opt)
		{
		case 'i':
			file_in = optarg;
			break;
		case 'o':
			file_out = optarg;
			break;
		case 'c':
			compress = 1;
			break;
		case 'd':
			compress = 0;
			break;
		case 'h':
			usage(stdout);
			return 0;
		case 'v':
			version(stdout);
			return 0;
		case 'm':
			memory = 1;
			break;
		default:
			usage(stderr);
			return 1;
		}
	}

	/* If an input file is given then open it. */
	if(file_in)
	{
		in = fopen(file_in, "rb");
		if(!in)
		{
			fprintf(stderr,
					"Can't open input file '%s': %s\n",
					file_in, strerror(errno));
			return 1;
		}
		close_in = 1;
	}

	/* If an output file is given then create it. */
	if(file_out)
	{
		out = fopen(file_out, "wb");
		if(!out)
		{
			fprintf(stderr,
					"Can't open output file '%s': %s\n",
					file_out, strerror(errno));
			return 1;
		}
		close_out = 1;
	}

	if(memory)
	{
		return compress ?
			memory_encode_file(in, out) : memory_decode_file(in, out);
	}

	if (compress)
	{
		rc = huffman_encode_file(in, out);
	}
	else
	{
		rc = huffman_decode_file(in, out);
	}

	/* Since exit is about to happen, the fclose calls aren't necessary, but they make valgrind happy. */
	if (close_in)
	{
		fclose(in);
	}

	if (close_out)
	{
		fclose(out);
	}

	return rc;
}

static int
memory_encode_file(FILE *in, FILE *out)
{
	unsigned char *buf = NULL, *bufout = NULL;
	unsigned int len = 0, cur = 0, inc = 1024, bufoutlen = 0;

	assert(in && out);

	/* Read the file into memory. */
	while(!feof(in))
	{
		unsigned char *tmp;
		len += inc;
		tmp = (unsigned char*)realloc(buf, len);
		if(!tmp)
		{
			if(buf)
				free(buf);
			return 1;
		}

		buf = tmp;
		cur += fread(buf + cur, 1, inc, in);
	}

	if(!buf)
		return 1;

	/* Encode the memory. */
	if(huffman_encode_memory(buf, cur, &bufout, &bufoutlen))
	{
		free(buf);
		return 1;
	}

	free(buf);

	/* Write the memory to the file. */
	if(fwrite(bufout, 1, bufoutlen, out) != bufoutlen)
	{
		free(bufout);
		return 1;
	}

	free(bufout);

	return 0;
}

static int
memory_decode_file(FILE *in, FILE *out)
{
	unsigned char *buf = NULL, *bufout = NULL;
	unsigned int len = 0, cur = 0, inc = 1024, bufoutlen = 0;
	assert(in && out);

	/* Read the file into memory. */
	while(!feof(in))
	{
		unsigned char *tmp;
		len += inc;
		tmp = (unsigned char*)realloc(buf, len);
		if(!tmp)
		{
			if(buf)
				free(buf);
			return 1;
		}

		buf = tmp;
		cur += fread(buf + cur, 1, inc, in);
	}

	if(!buf)
		return 1;

	/* Decode the memory. */
	if(huffman_decode_memory(buf, cur, &bufout, &bufoutlen))
	{
		free(buf);
		return 1;
	}

	free(buf);

	/* Write the memory to the file. */
	if(fwrite(bufout, 1, bufoutlen, out) != bufoutlen)
	{
		free(bufout);
		return 1;
	}

	free(bufout);

	return 0;
}
