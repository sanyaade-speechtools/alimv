#!/bin/bash

MY_ASTYLE_CMD=`which astyle`
if [ "$?" != "0" ];then
	echo "'astyle' not found !!!"
	exit 1
fi

cd `dirname $0`
cd ../
MY_WKDIR=`pwd`

MY_DIRS="ANALYSISaliceMV EventMixing TASKSMV macros"
MY_ASTYLE_OPTS="--style=k&r --align-pointer=name --keep-one-line-blocks --keep-one-line-statements --unpad-paren --suffix=none"
# MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --delete-empty-lines"


for MY_DIR in $MY_DIRS; do
	if [ -d $MY_WKDIR/$MY_DIR ];then
		echo "Reformating direcotry $MY_WKDIR/$MY_DIR ..."
		$MY_ASTYLE_CMD  $MY_ASTYLE_OPTS --recursive $MY_WKDIR/$MY_DIR/*.cxx $MY_WKDIR/$MY_DIR/*/*.h $MY_WKDIR/$MY_DIR/*.h
		for i in `find $MY_WKDIR/$MY_DIR -name "*.orig"`; do rm -f $i ;done
		for i in `find $MY_WKDIR/$MY_DIR -name "*~"`; do rm -f $i ;done
	fi 
done

