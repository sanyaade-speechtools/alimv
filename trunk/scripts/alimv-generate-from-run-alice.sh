#!/bin/bash

# DIRS

function CopyFilesFromManagerMacro() {

	MGR_FILENAME=`find $MY_SOURCE_DIR/macros/ -name "$1"`
	echo "Copying $MGR_FILENAME to $MY_WORK_DIR ..."
# 	cp -f $MGR_FILENAME $MY_WORK_DIR/
cat $MGR_FILENAME | grep -v "//|" > $MY_WORK_DIR/`basename $MGR_FILENAME`
	

	# copy all files which are in needed by gROOT->LoadMacro("...")
	MY_FILES_TO_COPY=`cat $MGR_FILENAME | grep -v '//' | grep 'gROOT->LoadMacro'`
	for MY_TMP_FILE in $MY_FILES_TO_COPY; do
		MY_TMP_FILE_TO_COPY=`echo $MY_TMP_FILE | awk -F '"' '{print $2}' | awk -F '+' '{print $1}'`
		MY_TMP_FILE_TO_COPY=`find $MY_SOURCE_DIR/macros/ -name "$MY_TMP_FILE_TO_COPY"`
		if [ ! -e $MY_WORK_DIR/`basename $MY_TMP_FILE_TO_COPY` ];then
			echo "Copying $MY_TMP_FILE_TO_COPY to $MY_WORK_DIR ..."
			cat $MY_TMP_FILE_TO_COPY | grep -v "//|" > $MY_WORK_DIR/`basename $MY_TMP_FILE_TO_COPY`
		fi

    MY_TMP_FILE_TO_COPY_TMP=`echo $MY_TMP_FILE_TO_COPY | awk -F '.cxx' '{print $1}'`
		if [ "$MY_TMP_FILE_TO_COPY_TMP" != "$MY_TMP_FILE_TO_COPY" ];then
			if [ ! -e $MY_WORK_DIR/`basename ${MY_TMP_FILE_TO_COPY/cxx/h}` ];then
				echo "Copying ${MY_TMP_FILE_TO_COPY/cxx/h} to $MY_WORK_DIR ..."
				cp -f ${MY_TMP_FILE_TO_COPY/cxx/h} $MY_WORK_DIR/	
			fi
		fi
	done

	MGR_PLUGIN_MACRO=`cat $MGR_FILENAME | grep -v '//' | grep 'AliAnalysisAlien::SetupPar('`

	for MY_TMP_FILE in $MGR_PLUGIN_MACRO; do
		MY_TMP_PARFILE=`echo $MY_TMP_FILE | awk -F '"' '{print $2}'`
		MY_TMP_FILE_TO_COPY=`find $MY_SOURCE_DIR/pars/ -name "$MY_TMP_PARFILE.par"`
		if [ ! -e $MY_WORK_DIR/`basename $MY_TMP_FILE_TO_COPY` ];then
			echo "Copying $MY_TMP_FILE_TO_COPY to $MY_WORK_DIR ..."
			cp -f $MY_TMP_FILE_TO_COPY $MY_WORK_DIR/
		fi
	done 

	echo "Copying all rsn config files ..."
	cp -f $MY_SOURCE_DIR/macros/aaf-rsn/Config* $MY_WORK_DIR/
	cp -f $MY_SOURCE_DIR/macros/aaf-rsn/Quality* $MY_WORK_DIR/
}

# function GenerateManagerMacro() {
# 
# }


# MAIN 

if [ -z $1 ];then
	echo "No first argument !!!"
	exit 1
fi

if [ ! -e $1 ];then
	echo "$1 is not file !!!"
	exit 2
fi

# finds out what is AddManagerMacro
MGR_FUNCTION=`cat $1 | grep 'RunAnalysisManager("'`
for MY_TMP_MGR_FUNCTION in $MGR_FUNCTION ; do
# 	echo $MY_TMP_MGR_FUNCTION
	MY_TMP_MGR_FUNCTION2=`echo $MY_TMP_MGR_FUNCTION | awk -F '"' '{ print $2}'`
	CopyFilesFromManagerMacro "$MY_TMP_MGR_FUNCTION2.C"
done

ls -al $MY_WORK_DIR
