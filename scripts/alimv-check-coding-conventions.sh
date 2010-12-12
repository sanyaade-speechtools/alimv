#!/bin/sh

makexml()
{
    filecxx=${1}.cxx 
    fileh=${1}.h 
    filecxxxml=check/${filecxx}.xml
    filehxml=check/${fileh}.xml

    dirv=`dirname ${filehxml}` 
    do=n

    [ -d ${dirv} ] || mkdir -p ${dirv} 

    if [ -r ${filecxx} ] ; then
	if [ ${filecxx} -nt ${filecxxxml} ] ; then
	    src2srcml ${filecxx} ${filecxxxml} 
	    do=y
	fi
    fi
	
    if [ -r ${fileh} ] ; then
	if [ ${fileh} -nt ${filehxml} ] ; then
	    src2srcml ${fileh} ${filehxml} 
	    do=y
	fi
    fi
    
    if [ "$do"=="y" ] ; then 
#	echo ${filecxxxml} ${filehxml}
    touch $HERE/reFact
    fi
}

makeviol()
{
    viols=check/${1}.viol
    filecxxxml=check/${1}.cxx.xml
    filehxml=check/${1}.h.xml

    if [ ${filehxml}   -nt ${viols} ] || [ ${filecxxxml} -nt ${viols} ] ; then
	echo ${viols}
	java -Xmx1024M -jar $irst/NewRuleChecker/NewRuleChecker.jar ${filecxxxml} ${filehxml} \
	$HERE/factFile.xml $irst/NewRuleChecker/config/AliceCodingConventions.xml > ${viols}
    fi
}

makesmell()
{
    smells=check/${1}.smell
    filecxxxml=check/${1}.cxx.xml
    filehxml=check/${1}.h.xml

    if [ ${filehxml}   -nt ${smells} ] || [ ${filecxxxml} -nt ${smells} ] ; then
	echo ${smells}
	java -Xmx1024M -jar $irst/SmellDetector.jar ${filecxxxml} ${filehxml} > ${smells}
    fi
}

srcdirs()
{
    find . -mindepth 1 -maxdepth 1 -type d |\
                               grep -v bin |\
                               grep -v doc |\
                               grep -v etc |\
                               grep -v lib |\
                               grep -v man |\
                              grep -v data |\
                              grep -v html |\
                              grep -v test |\
                             grep -v build |\
                             grep -v cmake |\
                             grep -v picts |\
                             grep -v share |\
                            grep -v macros |\
                           grep -v include |\
                          grep -v roothxml |\
                      grep -v linuxx8664gcc|\
                             grep -v linux |\
                              grep -v pars |\
                           grep -v scripts |\
                              grep -v test |\
                      grep -v local

}
    
getstubs()
{
    find . -name '*.cxx' -or -name '*.h' |\
                      sed -e 's/\.cxx//' |\
                        sed -e 's/\.h//' |\
                       sed -e 's?^\./??' |\
                             grep -v G__ |\
                         grep -v LinkDef |\
                              grep -v \# |\
                                 sort -u
}


HERE=`cd ../;pwd`
irst=$HERE/local/ALICENewRuleChecker
export PATH="$HERE/local/srcml/:$PATH"
cd $HERE

echo ========================================= Creating xml files =========================================
for i in `srcdirs` ;\
   do cd $i \;
      for j in `getstubs` ; \
         do makexml $j ;\
            done ;\
      cd $HERE ;\
done

echo ================================== Creating files for root includes ==================================
cd $HERE
[ -d roothxml ] || mkdir roothxml
cd roothxml
ln -s $ROOTSYS/include/* . 2>/dev/null
for i in `ls *.h | grep -v G__` ; do makexml `echo $i | sed -e 's/\.h//'` ; done
cd $HERE

if [ reFact -nt factFile.xml ] ; then 
    echo ======================================== Rebuilding fact file ========================================
    java -Xmx1024M -jar $irst/FactExtractor/FactExtractor.jar ./ ./
fi

echo ========================================= Creating viol files ========================================
for i in `srcdirs` ;\
   do cd $i \;
      for j in `getstubs` ; \
      do makeviol $j ;\
         done ;\
      cd $HERE ;\
done

echo ========================================= Creating smell files ========================================
for i in `srcdirs` ;\
   do cd $i \;
      for j in `getstubs` ; \
      do makesmell $j ;\
         done ;\
      cd $HERE ;\
done


