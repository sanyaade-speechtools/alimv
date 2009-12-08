#!/bin/bash
cd ../
# PARS="SKAF_ALIROOT STEERBase ESD AOD ANALYSIS ANALYSISalice ANALYSISTest CORRFW PWG2resonances PWG2resonancesTest PWG2resonancesGui"
# PARS="PWG2resonances PWG2resonancesMV PWG2resonancesTEST"
PARS="PWG2resonancesTest"
# PARS=""
# rm -Rf pars
# mkdir pars

if [ ! -d $ALIMV/pars ]; then
  mkdir -p $ALIMV/pars
fi
cd $ALIMV
for i in $PARS; do
echo "Creating $i.par ..."
tar cfz pars/$i.par $i
done
