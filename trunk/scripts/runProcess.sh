#!/bin/bash
echo "Running process ..."
cd ../$1
if [ "$2" != "" ]; then
  CMD_TMP="runProcess.C($2)"
  root.exe -l -q $CMD_TMP
else
#  root.exe -l runProcess.C
#   echo "Running \"aliroot -l runProcess.C\" ... "
  aliroot -l runProcess.C
fi
echo "Running process done..."
