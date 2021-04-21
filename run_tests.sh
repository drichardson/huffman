#!/bin/bash
set -euo pipefail
shopt -s inherit_errexit

USE_VALGRIND=0
HUFFCODE="./huffcode"

usage() {
	echo "Usage: run_tests.sh [USE_VALGRIND]"
}

while [[ $# -gt 0 ]]; do
	case "$1" in
		--use-valgrind)
			USE_VALGRIND=1
			shift
			;;
		--huffcode)
			shift
			if [[ $# -eq 0 ]]; then
				echo "Missing argument for --huffcode"
				exit 1
			fi
			HUFFCODE=$1
			shift
			;;
		*)
			echo "Unexpected parameter $1"
			exit 1
			;;
	esac
done

SCRATCH=$(mktemp --tmpdir -d huffman.XXXXXXXXXX)
# echo "Using temporary directory $SCRATCH"
cleanup() {
	# echo "Cleaning up $SCRATCH"
	rm -r "$SCRATCH"
}
trap cleanup EXIT

if [[ $USE_VALGRIND == 1 ]]; then
	VALGRIND="valgrind \
		--tool=memcheck \
		--memcheck:leak-check=yes \
		--leak-check=full \
		--errors-for-leak-kinds=all \
		--show-leak-kinds=all \
		--track-origins=yes \
		--error-exitcode=1"
	HUFFCODE="$VALGRIND $HUFFCODE"
fi

compress_test() {
	local IN=$1
	local OUT="$SCRATCH/$(basename $IN)"
	echo "compress_test $IN"
	set +e
	$HUFFCODE -i "$IN" -o "$OUT.compressed"
	if [ "$?" != 0 ]; then
		echo "FAILURE: huffcode encode failed on $IN"
		exit 1
	fi
	$HUFFCODE -d -i "$OUT.compressed" -o "$OUT.decompressed"
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
	local OUT="$SCRATCH/$(basename $IN)"
	echo "decompress_test $IN"
	set +e
	$HUFFCODE -d -i "$IN" -o "$OUT.decompressed"
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

