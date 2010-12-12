#!/bin/bash

export MY_SOURCE_DIR="`pwd`"
if [ ! -z $ALIMV ];then
  export MY_SOURCE_DIR="$ALIMV"
fi

if [ -d $MY_SOURCE_DIR/build ]; then
	rm -Rf $MY_SOURCE_DIR/build
fi

mkdir -p $MY_SOURCE_DIR/build

cd $MY_SOURCE_DIR/build

cmake ../

make -j5 install

cd ../

echo ""
echo "You can start by running :"
echo "  $MY_SOURCE_DIR/scripts/alimv-main.sh"
echo ""
