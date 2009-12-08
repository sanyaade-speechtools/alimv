#!/bin/bash
cd ..
TESTDIR="$1"
if [ "$2" == "--reset" ]; then
  echo "Deleting directory $ALIMV/$TESTDIR ..."
  rm -Rf $ALIMV/$TESTDIR
  mkdir $ALIMV/$TESTDIR
fi

if [ ! -d $ALIMV/$TESTDIR ]; then
  mkdir -p $ALIMV/$TESTDIR
fi

printf "Copying macros files to $TESTDIR ..."
cp -f $ALIMV/macros/RUN/* $ALIMV/$TESTDIR/ &> /dev/null
cp -f $ALIMV/macros/RUN_MV/* $ALIMV/$TESTDIR/ &> /dev/null
cp -f $ALIMV/macros/RUN_TRAIN/* $ALIMV/$TESTDIR/ &> /dev/null
cp -f $ALIMV/macros/TXT/*.txt $ALIMV/$TESTDIR/ &> /dev/null
cp -f $ALIMV/pars/* $ALIMV/$TESTDIR/ &> /dev/null
# sleep 1
printf "\rCopying macros files to $TESTDIR was OK.\n"