#!/bin/bash

MY_MV_FILE="/tmp/.alimvid"
MY_MV_NUM="1"
if [ ! -e $MY_MV_FILE ];then
  echo "$MY_MV_NUM" > $MY_MV_FILE
else
  MY_MV_NUM=`cat $MY_MV_FILE`
  MY_MV_NUM=`expr $MY_MV_NUM + 1`
  echo "$MY_MV_NUM" > $MY_MV_FILE
fi

MY_MV_NUM_FORMATED=`printf "%04.0f" $MY_MV_NUM`
export MY_WORK_DIR="/tmp/alimv/test_$MY_MV_NUM_FORMATED"
export MY_SOURCE_DIR="/home/mvala/ALICE/alimv"
if [ ! -z $ALIMV ];then
  export MY_SOURCE_DIR="$ALIMV"
fi
export MY_ROOT_DEFAULT_OPTIONS=""
# export MY_ROOT_DEFAULT_OPTIONS="-q"
export MY_ANALYSIS_SOURCE="proof"
export MY_ANALYSIS_SOURCE="grid"
# export MY_ANALYSIS_SOURCE="local"
export MY_ANALYSIS_MODE="test"
# export MY_ANALYSIS_MODE="full"

# Valgrind options
export MY_VALGRIND=""
# export MY_VALGRIND="valgrind"
# export MY_VALGRIND="$MY_VALGRIND --suppressions=$ROOTSYS/etc/valgrind-root.supp"
# export MY_VALGRIND="$MY_VALGRIND --leak-check=full"
# export MY_VALGRIND="$MY_VALGRIND --show-reachable=yes  "

# custom parfiles
export MY_PARFILES="ANALYSISaliceMV EventMixing RESONANCESMV TASKSMV"

# recreating work dir
rm -Rf $MY_WORK_DIR &>/dev/null
mkdir -p $MY_WORK_DIR

echo "Deleting backup files ..."
for i in `find $MY_SOURCE_DIR/ -name "*~"`; do rm -f $i ;done

rm -Rf $MY_SOURCE_DIR/pars &> /dev/null
mkdir -p $MY_SOURCE_DIR/pars &> /dev/null
for MY_PAR in $MY_PARFILES ; do
	echo "Creating $MY_PAR.par ..."
	$MY_SOURCE_DIR/test/scripts/MakeParFiles.sh $MY_SOURCE_DIR $MY_PAR
done

echo "Copying macros ..."
cd $MY_WORK_DIR

cat $MY_SOURCE_DIR/macros/template/runALICE_01.C > runALICE.C
cat $MY_SOURCE_DIR/macros/template/runALICE_02.C | grep -v '//' >> runALICE.C
cat $MY_SOURCE_DIR/macros/template/runALICE_03.C >> runALICE.C

$MY_SOURCE_DIR/scripts/ali-generate-from-run-alice.sh $MY_WORK_DIR/runALICE.C

cp $MY_SOURCE_DIR/macros/datasets/* . &>/dev/null

echo "Running ROOT ..."
$MY_VALGRIND root.exe $MY_ROOT_DEFAULT_OPTIONS runALICE.C\(\"$MY_ANALYSIS_SOURCE\",\"$MY_ANALYSIS_MODE\"\)
pwd
ls -al *.root

unset MY_WORK_DIR MY_SOURCE_DIR MY_ROOT_DEFAULT_OPTIONS MY_ANALYSIS_SOURCE MY_ANALYSIS_MODE MY_VALGRIND
