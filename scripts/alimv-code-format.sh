#!/bin/bash

MY_ASTYLE_CMD=`which astyle`
if [ "$?" != "0" ];then
  echo "'astyle' not found !!!"
  exit 1
fi

cd `dirname $0`
cd ../
MY_WKDIR=`pwd`

# # ROOT format
# MY_ASTYLE_OPTS="--indent=spaces=3 --convert-tabs --brackets=stroustrup --indent-switches --indent-namespaces --indent-preprocessor"
# MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --max-instatement-indent=60 --min-conditional-indent=0"
# MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --pad-oper --pad-header --unpad-paren"
# MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --keep-one-line-blocks --keep-one-line-statements"
# MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --convert-tabs"
# MY_ASTYLE_OPTS="$MY_ASTYLE_OPTS --suffix=none --recursive"
MY_ASTYLE_OPTS=""

$MY_ASTYLE_CMD $MY_ASTYLE_OPTS $MY_WKDIR/'*.h' $MY_WKDIR/'*.cxx' $MY_WKDIR/'*.C'

echo "Cleaning $MY_WKDIR ..."
cd $MY_WKDIR
find . -name "*~" -exec rm -f {} \;
