#!/bin/bash
MY_WORK_DIR=$1
shift
echo "Creating parfiles <$*> ..."
cd $MY_WORK_DIR
rm -Rf pars &> /dev/null
mkdir -p pars &> /dev/null
for PAR in "$*";do
  echo "Creating $PAR.par ..."
  tar cfz pars/$PAR.par $PAR
done

echo "Done."
