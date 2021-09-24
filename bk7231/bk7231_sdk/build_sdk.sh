#!/bin/sh
set -e

OUTPUT_DST_DIR=$1
export OUTPUT_DST_DIR

echo "begin sdk build..."

#cp empty_bk7231_sdk.bin $OUTPUT_DST_DIR/bk7231_sdk.bin
if [ ! -d build ]; then
	mkdir build
fi
make -f Makefile all
cp build/bk7231_sdk_crc.bin $OUTPUT_DST_DIR/bk7231_sdk_crc.bin
if [ -f build/all_2M.1220.bin ]; then
	cp build/all_2M.1220.bin $OUTPUT_DST_DIR/bk7231_sdk_ota.bin
fi

echo "finsh sdk build."
