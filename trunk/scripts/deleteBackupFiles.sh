#!/bin/bash
echo "Deleting backup files ..."
DIR=`pwd`
cd ..
for i in `find . -name "*~"`; do rm -f $i ;done
# for i in `find . -name ".svn"`; do rm -fR $i ;done
cd $DIR