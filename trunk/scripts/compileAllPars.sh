#!/bin/bash
echo "Compiling ..."
cd ../build
NUMBER_OF_CORES="`cat /proc/cpuinfo | grep "processor" | wc -l`"
let NUMBER_OF_CORES+=1
echo "Doing make -j$NUMBER_OF_CORES"
# make
make install -j$NUMBER_OF_CORES
return $?
echo "Compiling DONE..."
