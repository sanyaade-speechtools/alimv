#!/bin/bash
# export XrdSecGSISRVNAMES=lxfsrd0506.cern.ch
#export XrdSecGSISRVNAMES=lxfsrd0506.cern.ch

TESTDIR=$1
if [ "$TESTDIR" == "" ]; then
  TESTDIR="test"
fi

if [ "x$ALIMV" == "x" ];then
  export ALIMV=`dirname \`pwd\``
fi


cd $ALIMV/scripts/
sh deleteBackupFiles.sh
source compileAllPars.sh
if [ "$?" != "0" ]; 
then
  echo "Error in compilation!!!"
  exit 2;
fi
cd $ALIMV/scripts/
sh makeAllPars.sh
sh createRunDirectory.sh $TESTDIR $*
sh runProcess.sh $TESTDIR $2
