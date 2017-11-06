#!/bin/bash

set -e
#set -x

rm -rf scratch
mkdir scratch

compress_test() {
    local IN=$1
    local OUT=scratch/$(basename $IN)
    echo "compress_test $IN"
    set +e
    ./huffcode -i "$IN" -o "$OUT.compressed"
    if [ "$?" != 0 ]; then
        echo "FAILURE: huffcode encode failed on $IN"
        exit 1
    fi
    ./huffcode -d -i "$OUT.compressed" -o "$OUT.decompressed"
    if [ "$?" != 0 ]; then
        echo "FAILURE: huffcode decode failed on $OUT.compressed"
        exit 1
    fi
    diff "$OUT.decompressed" "$IN"
    if [ "$?" != 0 ]; then
        echo "FAILURE: huffcode decode didn't match original on $IN"
        exit 1
    fi
    set -e
}

# Only decompress. Does not verify against originally compressed file,
# but is used to make sure invalid code tables do not crash the decompressor.
decompress_test() {
    local IN=$1
    local OUT=scratch/$(basename $IN)
    echo "decompress_test $IN"
    set +e
    ./huffcode -d -i "$IN" -o "$OUT.decompressed"
    local STATUS=$?
    if [ "$STATUS" != 1 ]; then
        echo "Expected test to fail with 1 but got $STATUS."
        exit 1
    fi
    set -e
}

echo "Basic Checks..."
compress_test "test_data/3_line"
compress_test "test_data/empty"
compress_test "test_data/1_byte"
compress_test "test_data/2_byte_2_symbol"
compress_test "test_data/2_byte_1_symbol"
compress_test "test_data/3_byte_1_symbol"
compress_test "test_data/3_byte_3_symbol"
compress_test "test_data/128_byte_1_symbol"
compress_test "test_data/128_byte_2_symbol"

echo "Issue 2 Regression Checks..."
for file in "test_data/reported_issues/issue2/"*; do
    decompress_test "$file"
done


echo "All Tests Passed"

