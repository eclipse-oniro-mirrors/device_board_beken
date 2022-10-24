#!/usr/bin/sh
if [ $# != 1 ]; then
	echo "no dir param"
	exit 1
fi
SDK_LIBS_DIR=$1
SDK_LIBS=$(ls $SDK_LIBS_DIR)
#SDK_LIBS=$(basename $SDK_LIBS)
RESULT=$(echo $SDK_LIBS | sed 's/libbk7236_sdk.a//' | sed 's/\.a//g' | sed 's/lib/-l/g')
echo $RESULT
