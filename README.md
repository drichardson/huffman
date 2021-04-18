# huffman

[![Build](https://github.com/drichardson/huffman/actions/workflows/build.yml/badge.svg)](https://github.com/drichardson/huffman/actions/workflows/build.yml)

A huffman coding library and command line interface to the library. The encoder is a 2 pass encoder. The first pass generates a huffman tree and the second pass encodes the data. The decoder is one pass and uses a huffman code table at the beginning of the compressed file to decode the data.

libhuffman has functions for encoding and decoding both files and memory.

To build, run:

    make

To run unit tests, run:

    make check

To run unit tests under valgrind, run:

    make valgrind_check

