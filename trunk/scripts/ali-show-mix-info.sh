#!/bin/bash

export ALIMV="/home/mvala/ALICE/alimv/"
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

$ALIMV/bin/tgt_`root-config --arch`/alimv $ALIMV/macros/mixinfo/ShowMixInfo.C\(\"$MY_WORK_DIR/MixInfo.root\"\)

unset MY_WORK_DIR ALIMV
