/*
 *  huffman_coder - Encode/Decode files using Huffman encoding.
 *  Copyright (C) 2003  Douglas Ryan Richardson
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "huffman.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

static void
version(FILE *out)
{
	fputs("huffman 0.1\n"
		  "Copyright (C) 2003 Douglas Ryan Richardson\n",
		  out);
}

static void
usage(FILE* out)
{
	fputs("Usage: huffmane_coder [-i<input file>] [-o<output file>] [-d|-c]\n"
		  "-i - input file (default is standard input)\n"
		  "-o - output file (default is standard output)\n"
		  "-d - decompress\n"
		  "-c - compress (default)\n",
		  out);
}

int
main(int argc, char** argv)
{
	char compress = 1;
	int opt;
	const char *file_in = NULL, *file_out = NULL;
	FILE *in = stdin;
	FILE *out = stdout;

	/* Get the command line arguments. */
	while((opt = getopt(argc, argv, "i:o:cdhv")) != -1)
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
	}

	return compress ?
		huffman_encode_file(in, out) : huffman_decode_file(in, out);
}
