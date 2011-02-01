#!/bin/bash

MY_ALIMV_TMP_DIR="/tmp/alimv"
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

export MY_WORK_DIR="$MY_ALIMV_TMP_DIR/$MY_MV_NUM_FORMATED"
if [ ! -d $MY_ALIMV_TMP_DIR ]; then
  mkdir -p $MY_ALIMV_TMP_DIR
fi

rm -f $MY_ALIMV_TMP_DIR/last
ln -s $MY_WORK_DIR $MY_ALIMV_TMP_DIR/last

export MY_SOURCE_DIR="/home/mvala/ALICE/alimv"
if [ ! -z $ALIMV ];then
  export MY_SOURCE_DIR="$ALIMV"
fi

export MY_ROOT_DEFAULT_OPTIONS=""
# export MY_ROOT_DEFAULT_OPTIONS="-q"
export MY_ANALYSIS_SOURCE="proof"
# export MY_ANALYSIS_SOURCE="grid"
# export MY_ANALYSIS_SOURCE="local"
export MY_ANALYSIS_MODE="test"
# export MY_ANALYSIS_MODE="full"
# export MY_ANALYSIS_MODE="submit"

# Valgrind options
export MY_VALGRIND=""
# export MY_VALGRIND="valgrind"
# export MY_VALGRIND="$MY_VALGRIND --suppressions=$ROOTSYS/etc/valgrind-root.supp"
# export MY_VALGRIND="$MY_VALGRIND --leak-check=full"
# export MY_VALGRIND="$MY_VALGRIND --show-reachable=yes  "

# custom parfiles
export MY_PARFILES="ANALYSISaliceMV EventMixing PWG2resonances RESONANCESMV TASKSMV"

MY_TASKS="AliAnalysisTaskCustomMix AliAnalysisTaskEvil"

for MY_TASK in $MY_TASKS; do
	rm -f $MY_SOURCE_DIR/macros/aaf-tasks/$MY_TASK.cxx
	ln -s $MY_SOURCE_DIR/TASKSMV/$MY_TASK.cxx $MY_SOURCE_DIR/macros/aaf-tasks/$MY_TASK.cxx
	rm -f $MY_SOURCE_DIR/macros/aaf-tasks/$MY_TASK.h
	ln -s $MY_SOURCE_DIR/TASKSMV/$MY_TASK.h $MY_SOURCE_DIR/macros/aaf-tasks/$MY_TASK.h
done

# recreating work dir
rm -Rf $MY_WORK_DIR
mkdir -p $MY_WORK_DIR

echo "Deleting backup files ..."
for i in `find $MY_SOURCE_DIR/ -name "*~"`; do rm -f $i ;done

rm -Rf $MY_SOURCE_DIR/pars &> /dev/null
mkdir -p $MY_SOURCE_DIR/pars &> /dev/null
for MY_PAR in $MY_PARFILES ; do
	$MY_SOURCE_DIR/test/scripts/MakeParFiles.sh $MY_SOURCE_DIR $MY_PAR
done

echo "Copying macros ..."
cd $MY_WORK_DIR

cat $MY_SOURCE_DIR/macros/template/runALICE_01.C > $MY_WORK_DIR/runALICE.C
cat $MY_SOURCE_DIR/macros/template/runALICE_02.C | grep -v '//' >> $MY_WORK_DIR/runALICE.C
cat $MY_SOURCE_DIR/macros/template/runALICE_03.C >> $MY_WORK_DIR/runALICE.C

$MY_SOURCE_DIR/scripts/alimv-generate-from-run-alice.sh $MY_WORK_DIR/runALICE.C

cp $MY_SOURCE_DIR/macros/datasets/* . &>/dev/null
cp $MY_SOURCE_DIR/pars/* . &>/dev/null

alien-token-info
if [ "$?" != "0" ]; then
  alien-token-init
fi

echo "Running ROOT ..."
# $MY_VALGRIND root.exe $MY_ROOT_DEFAULT_OPTIONS runALICE.C\(\"$MY_ANALYSIS_SOURCE\",\"$MY_ANALYSIS_MODE\"\)
$MY_VALGRIND aliroot $MY_ROOT_DEFAULT_OPTIONS runALICE.C\(\"$MY_ANALYSIS_SOURCE\",\"$MY_ANALYSIS_MODE\"\)
pwd
ls -al *.root

unset MY_WORK_DIR MY_SOURCE_DIR MY_ROOT_DEFAULT_OPTIONS MY_ANALYSIS_SOURCE MY_ANALYSIS_MODE MY_VALGRIND
