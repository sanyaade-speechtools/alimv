#!/bin/bash

MY_ASTYLE_CMD=`which astyle`
if [ "$?" != "0" ];then
  echo "'astyle' not found !!!"
  exit 1
fi

cd `dirname $0`
cd ../
MY_WKDIR=`pwd`

# ROOT format
MY_ASTYLE_OPTS="--indent=spaces=3 --convert-tabs --brackets=stroustrup --indent-switches --indent-namespaces --indent-preprocessor"
MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --max-instatement-indent=60 --min-conditional-indent=0"
MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --pad-oper --pad-header --unpad-paren"
MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --keep-one-line-blocks --keep-one-line-statements"
MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --convert-tabs"
MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --suffix=none --recursive"

MY_DIRS="ANALYSISaliceMV EventMixing/EventMixing TASKSMV"

for MY_DIR in $MY_DIRS; do
  if [ -d $MY_WKDIR/$MY_DIR ];then
    echo "Reformating direcotry $MY_WKDIR/$MY_DIR ..."
    $MY_ASTYLE_CMD  $MY_ASTYLE_OPTS $MY_WKDIR/$MY_DIR/*.h $MY_WKDIR/$MY_DIR/*.cxx
  fi 
done

MY_DIRS="macros macros/* ANALYSISaliceMV/macros"
for MY_DIR in $MY_DIRS; do
  if [ -d $MY_WKDIR/$MY_DIR ];then
    echo "Reformating direcotry $MY_WKDIR/$MY_DIR ..."
    $MY_ASTYLE_CMD  $MY_ASTYLE_OPTS $MY_WKDIR/$MY_DIR/*.C
  fi 
done

echo "Cleaning $MY_WKDIR ..."

# find $MY_WKDIR -name "*.orig" -exec rm -f {} \;
find $MY_WKDIR -name "*~" -exec rm -f {} \;
