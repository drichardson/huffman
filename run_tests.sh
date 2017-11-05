#!/bin/bash

set -e
set -x

rm -rf scratch
mkdir scratch

compress_test() {
    local IN=$1
    local OUT=scratch/$(basename $IN)
    echo "compress_test $IN"
    ./huffcode -i "$IN" -o "$OUT.compressed"
    ./huffcode -d -i "$OUT.compressed" -o "$OUT.decompressed"
    diff "$OUT.decompressed" "$IN"
}

compress_test "test_data/3_line_file"
compress_test "test_data/empty_file"

echo "All Tests Passed"

