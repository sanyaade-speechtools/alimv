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

# ROOT format
# MY_ASTYLE_OPTS="--indent=spaces=3 --convert-tabs --brackets=stroustrup --indent-switches --indent-namespaces --indent-preprocessor"
# MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --max-instatement-indent=60 --min-conditional-indent=0"
# MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --pad-oper --pad-header --unpad-paren"
# MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --suffix=none --recursive"

for MY_DIR in $MY_DIRS; do
	if [ -d $MY_WKDIR/$MY_DIR ];then
		echo "Reformating direcotry $MY_WKDIR/$MY_DIR ..."
		$MY_ASTYLE_CMD  $MY_ASTYLE_OPTS $MY_WKDIR/$MY_DIR/*.cxx $MY_WKDIR/$MY_DIR/*/*.h $MY_WKDIR/$MY_DIR/*.h
		for i in `find $MY_WKDIR/$MY_DIR -name "*.orig"`; do rm -f $i ;done
		for i in `find $MY_WKDIR/$MY_DIR -name "*~"`; do rm -f $i ;done
	fi 
done

