#!/bin/sh
set -e

EXEC_PATH=$(cd "$(dirname "$0")"&&pwd)
OUTPUT_DST_DIR=$1

export OUTPUT_DST_DIR

beken_chip_type=bk7235

echo "begin sdk build..."
rm -rf $EXEC_PATH/build
mkdir -p $EXEC_PATH/build
make -C $EXEC_PATH -f Makefile all

if [ -f build/${beken_chip_type}_sdk_crc.bin ]; then
	cp build/${beken_chip_type}_sdk_crc.bin $OUTPUT_DST_DIR/${beken_chip_type}_sdk_crc.bin
fi
if [ -f build/all_2M.1220.bin ]; then
	cp build/all_2M.1220.bin $OUTPUT_DST_DIR/${beken_chip_type}_sdk_ota.bin
fi

echo "finsh sdk build."
