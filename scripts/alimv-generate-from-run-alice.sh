#!/bin/bash

# DIRS
function CopyFilesFromAddTaskMacro() {

	if [ -z $1 ]; then
		return 1
	fi
	
	if [ ! -e $MY_WORK_DIR/`basename $1` ];then
    
    MY_FILE_TYPE=`echo $1 | awk -F '.' '{print $2}' | awk -F '+' '{print $1}'`
    if [ "$MY_FILE_TYPE" == "cxx" ];then
    echo "Copying $1 to $MY_WORK_DIR ..."
      cp -f $1 $MY_WORK_DIR/
    echo "Copying ${1/cxx/h} to $MY_WORK_DIR ..."
      cp -f ${1/cxx/h} $MY_WORK_DIR/
    elif [ "$MY_FILE_TYPE" == "C" ];then
      echo "Copying Macro $1 to $MY_WORK_DIR ..."
      cat $1 | grep -v "//|" > $MY_WORK_DIR/`basename $1`
    fi
	fi
	# copy all files which are in needed by gROOT->LoadMacro("...")
	MY_FILES_TO_COPY=`cat $1 | grep -v '//' | grep 'gROOT->LoadMacro'| grep -v ALICE_ROOT`
	for MY_TMP_FILE in $MY_FILES_TO_COPY; do
		MY_TMP_FILE_TO_COPY=`echo $MY_TMP_FILE | awk -F '"' '{print $2}' | awk -F '+' '{print $1}'`
		MY_TMP_FILE_TO_COPY=`find $MY_SOURCE_DIR/macros/ -name "$MY_TMP_FILE_TO_COPY"`
		if [ ! -e $MY_WORK_DIR/`basename $MY_TMP_FILE_TO_COPY` ];then
			echo "Copying Macro $MY_TMP_FILE_TO_COPY to $MY_WORK_DIR ..."
			cat $MY_TMP_FILE_TO_COPY | grep -v "//|" > $MY_WORK_DIR/`basename $MY_TMP_FILE_TO_COPY`
		fi
	done

}
function CopyFilesFromManagerMacro() {
	
	if [ -z $1 ]; then
		return 1
	fi

	MGR_FILENAME=`find $MY_SOURCE_DIR/macros/ -name "$1"`
	echo "Copying Manager $MGR_FILENAME to $MY_WORK_DIR ..."
	echo "\$MGR_FILENAME $MGR_FILENAME"
	cat $MGR_FILENAME | grep -v "//|" > $MY_WORK_DIR/`basename $MGR_FILENAME`


	MY_FILES_TO_COPY=`cat $MGR_FILENAME | grep -v '//' | grep 'gROOT->LoadMacro'| grep -v ALICE_ROOT`
	for MY_TMP_FILE in $MY_FILES_TO_COPY; do
		MY_TMP_FILE=`echo $MY_TMP_FILE | awk -F '"' '{print $2}' | awk -F '+' '{print $1}'`
		MY_TMP_FILE=`find $MY_SOURCE_DIR/macros/ -name "$MY_TMP_FILE"`
		CopyFilesFromAddTaskMacro $MY_TMP_FILE
	done

	MGR_PLUGIN_MACRO=`cat $MGR_FILENAME | grep -v '//' | grep 'AliAnalysisAlien::SetupPar('`

	for MY_TMP_FILE in $MGR_PLUGIN_MACRO; do
		MY_TMP_PARFILE=`echo $MY_TMP_FILE | awk -F '"' '{print $2}'`
		MY_TMP_FILE_TO_COPY=`find $MY_SOURCE_DIR/pars/ -name "$MY_TMP_PARFILE.par"`
		if [ ! -z "$MY_TMP_FILE_TO_COPY" ];then
			if [ ! -e $MY_WORK_DIR/`basename $MY_TMP_FILE_TO_COPY` ];then
				echo "Copying $MY_TMP_FILE_TO_COPY to $MY_WORK_DIR ..."
				cp -f $MY_TMP_FILE_TO_COPY $MY_WORK_DIR/
			fi
		fi
	done 
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

echo "\$1 $1"
# finds out what is AddManagerMacro
MGR_FUNCTION=`cat $1 | grep 'RunAnalysisManager("'`
for MY_TMP_MGR_FUNCTION in $MGR_FUNCTION ; do
	MY_TMP_MGR_FUNCTION2=`echo $MY_TMP_MGR_FUNCTION | awk -F '"' '{ print $2}'`
	
	if [ ! -z $MY_TMP_MGR_FUNCTION2 ];then
		CopyFilesFromManagerMacro "$MY_TMP_MGR_FUNCTION2.C"
	fi
done

ls -al $MY_WORK_DIR
echo "Output dir is : $MY_WORK_DIR"