#!/bin/bash

echo "Creating parfiles <$*> ..."
MY_COMPILE_DIR="`pwd`/Testing/pars_compile_tmp"
rm -Rf $MY_COMPILE_DIR &> /dev/null
mkdir -p $MY_COMPILE_DIR &> /dev/null
for PAR in "$*";do
  echo "Compiling par/$PAR.par ..."
  cp ../pars/$PAR.par $MY_COMPILE_DIR/
  cd $MY_COMPILE_DIR/
  tar xfz $PAR.par
  cd $PAR
  PROOF-INF/BUILD.sh
	pwd
	ls -al
  root -b -q PROOF-INF/SETUP.C
	RET=$?
  cd ../../
	if [ "$RET" != "0" ];then
		exit 1
	fi
done
# rm -Rf $MY_COMPILE_DIR &> /dev/null
echo "Done."
