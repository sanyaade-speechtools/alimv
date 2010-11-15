#!/bin/bash

export MY_WORK_DIR="/tmp/test1"
export MY_SOURCE_DIR="/home/mvala/WORK/SVN/ali-analysis-mix"
export MY_ROOT_DEFAULT_OPTIONS=""
# MY_ROOT_DEFAULT_OPTIONS="-q"
export MY_ANALYSIS_SOURCE="proof"
# MY_ANALYSIS_SOURCE="local"
export MY_ANALYSIS_MODE="test"
# MY_ANALYSIS_MODE="full" 
export MY_VALGRIND=""
# MY_VALGRIND="valgrind"
# MY_VALGRIND="$MY_VALGRIND --suppressions=$ROOTSYS/etc/valgrind-root.supp"
# MY_VALGRIND="$MY_VALGRIND --leak-check=full"
# MY_VALGRIND="$MY_VALGRIND --show-reachable=yes  "



# recreating work dir
rm -Rf $MY_WORK_DIR &>/dev/null
mkdir -p $MY_WORK_DIR

echo "Deleting backup files ..."
for i in `find $MY_SOURCE_DIR -name "*~"`; do rm -f $i ;done

$MY_SOURCE_DIR/test/scripts/MakeParFiles.sh $MY_SOURCE_DIR EventMixing

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
