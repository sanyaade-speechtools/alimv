#!/bin/bash

export MY_SOURCE_DIR="`pwd`"
if [ ! -z $ALIMV ];then
  export MY_SOURCE_DIR="$ALIMV"
fi

mkdir -p $MY_SOURCE_DIR/build

cd $MY_SOURCE_DIR/build

cmake ../

make -j5 install

cd ../

echo "You can start by running :"
echo "\t $MY_SOURCE_DIR/scripts/alimv-main.sh"
