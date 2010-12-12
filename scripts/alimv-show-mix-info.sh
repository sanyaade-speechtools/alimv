#!/bin/bash
export MY_SOURCE_DIR="/home/mvala/ALICE/alimv/"
if [ ! -z $ALIMV ];then
  export MY_SOURCE_DIR="$ALIMV"
fi

if [ ! `which root-config` ]; then
	echo "root-config not found !!!"
	exit 1
fi

MY_MV_FILE="/tmp/.alimvid"
MY_MV_NUM="1"
if [ ! -e $MY_MV_FILE ];then
  echo "File $MY_MV_FILE was not found !!!"
  exit 1
else
  MY_MV_NUM=`cat $MY_MV_FILE`
# 	MY_MV_NUM=`expr $MY_MV_NUM - 1`
fi
MY_MV_NUM_FORMATED=`printf "%04.0f" $MY_MV_NUM`
export MY_WORK_DIR="/tmp/alimv/$MY_MV_NUM_FORMATED"

$MY_SOURCE_DIR/bin/tgt_`root-config --arch`/alimv $MY_SOURCE_DIR/macros/mixinfo/ShowMixInfo.C\(\"$MY_WORK_DIR/MixInfo.root\"\)

unset MY_WORK_DIR ALIMV
