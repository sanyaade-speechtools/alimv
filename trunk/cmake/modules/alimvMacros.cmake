# -*- mode: cmake -*-

#_______________________________________________________________________________
Macro (CHECK_OUT_OF_SOURCE_BUILD)

# Checks that the binary is built outside the source

   String(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_BINARY_DIR}" insource)
   If(insource)
#       File(REMOVE_RECURSE ${CMAKE_SOURCE_DIR}/Testing)
#       File(REMOVE ${CMAKE_SOURCE_DIR}/DartConfiguration.tcl)
      Message(FATAL_ERROR "ALIMV should be installed as an out of source build, to keep the source directory clean. Please create a extra build directory and run the command 'cmake path_to_source_dir' in this newly created directory. You have also to delete the directory CMakeFiles and the file CMakeCache.txt in the source directory. Otherwise cmake will complain even if you run it from an out-of-source directory.") 
   Endif(insource)

EndMacro (CHECK_OUT_OF_SOURCE_BUILD)

#_______________________________________________________________________________
Function (AddLibrary LIB SRCS DHDRS)

# Adds an library as a target

  Set(_path)
  List(LENGTH SRCS _len)
  If(_len GREATER 0)
    List(GET SRCS 0 _file)
    Get_filename_component(_path ${_file} PATH)
  Endif(_len GREATER 0)

  Set(LDEF)
  If(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIB}LinkDef.h)
    Set(LDEF "${LIB}LinkDef.h")
  Endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${LIB}LinkDef.h)

  If(_path)
    If(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_path}/${LIB}LinkDef.h)
      Set(LDEF "${_path}/${LIB}LinkDef.h")
    Endif(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${_path}/${LIB}LinkDef.h)
  Endif(_path)

  Set(DICT)
  If(LDEF)
    Set(DICT "G__${LIB}.cxx")
    Set(ASRCS ${SRCS} ${DICT})
    Root_Generate_Dictionary("${DHDRS}" "${LDEF}" "${DICT}" "${INCLUDE_DIRECTORIES}")
  Else(LDEF)
    Message(STATUS "No ${LIB}LinkDef.h found... building lib with no ROOT dict")
    Set(ASRCS ${SRCS})
  Endif(LDEF)

  Add_Library(${LIB} SHARED ${ASRCS})
  Target_Link_Libraries(${LIB} ${ALIMV_LIBRARIES})
  Set_Target_Properties(${LIB} PROPERTIES ${ALIMV_LIBRARY_PROPERTIES})
  
  Install(TARGETS ${LIB} DESTINATION ${ALIMV_INSTALL_DIR}/lib
    COMPONENT shared)

EndFunction (AddLibrary)


#_______________________________________________________________________________
Function (AddExecutable BIN SRCS LIBS)

# Adds an AliRoot executable as a target

  Add_Executable(${BIN} ${SRCS})
  Target_Link_Libraries(${BIN} ${ROOT_LIBRARIES} ${LIBS})
  Install(TARGETS ${BIN} DESTINATION ${ALIMV_INSTALL_DIR}/bin)

  If(ALICE_STATIC_BUILD)
    Add_Executable(${BIN}_a ${SRCS})
    Set(_ar_libs)
    Foreach(_lib ${LIBS})
      Set(_ar_libs ${_ar_libs} ${_lib}_a)
    EndForeach(_lib ${LIBS})
    Foreach(_lib ${LIBS})
      Set(_ar_libs ${_ar_libs} ${_lib}_a)
    EndForeach(_lib ${LIBS})
    Target_Link_Libraries(${BIN}_a ${ROOT_LIBRARIES} ${_ar_libs})
    Install(TARGETS ${BIN}_a DESTINATION ${ALIMV_INSTALL_DIR}/bin)
  EndIf(ALICE_STATIC_BUILD)

#   CheckViols(${BIN} "${SRCS}")

EndFunction (AddExecutable)

#_______________________________________________________________________________
Macro (SetupSystem)

# Set up all system dependencies of AliRoot

# check for ALIMV
set(ALIMV $ENV{ALIMV})
if(NOT ALIMV)
  set(ALIMV ${CMAKE_CURRENT_SOURCE_DIR})
  message(STATUS "Setting ${CMAKE_CURRENT_SOURCE_DIR} to ALIMV ... ")
endif(NOT ALIMV)

set(ALIMV_TARGET $ENV{ALIMV_TARGET})
if(NOT ALIMV_TARGET)
  execute_process(COMMAND root-config --arch OUTPUT_VARIABLE ALIMV_TARGET OUTPUT_STRIP_TRAILING_WHITESPACE)
  message(STATUS "Setting ${ALIMV_TARGET} to ALIMV_TARGET ... ")
endif(NOT ALIMV_TARGET)

set(ALIMV_COMPILE $ENV{ALIMV_COMPILE})

Message(STATUS "Setting up system dependent parameters for ${ALIMV_TARGET}" )

# macosx64
If(ALIMV_TARGET STREQUAL macosx64) 

  Execute_process(
    COMMAND sw_vers -productVersion
    OUTPUT_VARIABLE MACOSX
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  String(REGEX REPLACE "^(.*)[.](.*)[.](.*)$" "\\1" MACOSX_MAJOR "${MACOSX}")
  String(REGEX REPLACE "^(.*)[.](.*)[.](.*)$" "\\2" MACOSX_MINOR "${MACOSX}")

  Message(STATUS "Mac OS X ${MACOSX_MAJOR}.${MACOSX_MINOR}")

  Find_Package(fink)
  
#  Set(CMAKE_CXX_COMPILER g++)
#  Set(CMAKE_C_COMPILER gcc)
# I found no way to make this work...
#  Set(CMAKE_CXX_LINK_EXECUTABLE 
#    "MACOSX_DEPLOYMENT_TARGET=${MACOSX_MAJOR}.${MACOSX_MINOR} ${CMAKE_CXX_LINK_EXECUTABLE}")

  Set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -O -g -flat_namespace -single_module -undefined dynamic_lookup -m64")

  Set(CMAKE_Fortran_FLAGS "-O -g -fno-second-underscore -m64")

  Set(CLIBDEFS "-DCERNLIB_LXIA64 -DCERNLIB_BLDLIB -DCERNLIB_CZ -DCERNLIB_PPC")

  Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g -m64 -pipe -Wall -W -pipe -fbounds-check -fsigned-char -fno-common -fmessage-length=0 -Woverloaded-virtual -Weffc++ -Wconversion -Wshadow -fno-default-inline -fno-inline -I/usr/X11R6/include -I${FINK_ROOT}/include")
  Set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O -g -m64 -Wall -W -fno-common -pipe -I${FINK_ROOT}/include")

  If(CMAKE_Fortran_COMPILER MATCHES g95) 
    Set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fbounds-check -ftrace=full -DFORTRAN_G95")
  Else(CMAKE_Fortran_COMPILER MATCHES g95)
    Set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -DFORTRAN_GFORTRAN")
    Execute_process(
      COMMAND gfortran -m64 -print-file-name=libgfortran.dylib
      OUTPUT_VARIABLE FLIB
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ROOT_LIBRARIES "${ROOT_LIBRARIES} ${FLIB}")
    Execute_process(
      COMMAND gfortran -m64 -print-file-name=libgfortranbegin.a
      OUTPUT_VARIABLE FLIB
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ROOT_LIBRARIES "${ROOT_LIBRARIES} ${FLIB}")
  Endif(CMAKE_Fortran_COMPILER MATCHES g95) 

# I think this is useless
#  Set(ALIMV_LIBRARIES "${ROOT_LIBRARIES} -L/usr/X11R6/lib -lX11")
  Set(ALIMV_LIBRARIES "${ROOT_LIBRARIES}")

# Would like to use this, but did not manage on the Mac
#Include(FindOpenGL)
#Set(ROOT_LIBRARIES ${ROOT_LIBRARIES} ${OPENGL_LIBRARIES})
#Set(ROOT_INCLUDE_DIR ${ROOT_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
# Poor man's version of the above
  Set(ALIMV_INCLUDE_DIR ${ROOT_INCLUDE_DIR} /usr/X11/include)

# 
# LD            = export MACOSX_DEPLOYMENT_TARGET=$(MACOSX_MAJOR).$(MACOSX_MINOR) ; \
# 		unset LD_PREBIND ; \
# 		g++
# ALLD	      = ar
# ALFLAGS       = cr
# ALLIB         = 
# AEXT 	      = a
# 

Endif(ALIMV_TARGET STREQUAL macosx64)

# linuxx8664gcc
If(ALIMV_TARGET STREQUAL linuxx8664gcc) 


  Set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -O -g -shared -Wl")

  Set(CMAKE_Fortran_FLAGS "-O -g -fno-second-underscore -m64")

  Set(CLIBDEFS "-DCERNLIB_LXIA64 -DCERNLIB_BLDLIB -DCERNLIB_CZ")

#    If(ALIMV_ALIROOT STREQUAL "SILENT") 
#      Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -m64 -pipe -W -Wall -Dlinux -I/usr/X11R6/include")
#    Endif(ALIMV_ALIROOT STREQUAL "SILENT") 

 If(ALIMV_COMPILE STREQUAL "MORE") 
  Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g -m64 -pipe -W -Wall -Weffc++ -Woverloaded-virtual -fmessage-length=0 -Wno-long-long -Dlinux -I/usr/X11R6/include")
 Else(ALIMV_COMPILE STREQUAL "MORE") 
  Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g -m64 -pipe -W -Wall -Dlinux -I/usr/X11R6/include")
 Endif(ALIMV_COMPILE STREQUAL "MORE") 

  Set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O -g -m64 -Wall -W -pipe")

  If(CMAKE_Fortran_COMPILER MATCHES g95) 
    Set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fbounds-check -ftrace=full -DFORTRAN_G95")
  Else(CMAKE_Fortran_COMPILER MATCHES g95)
    Set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -DFORTRAN_GFORTRAN")
    Execute_process(
      COMMAND gfortran -m64 -print-file-name=libgfortran.so
      OUTPUT_VARIABLE FLIB
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ROOT_LIBRARIES "${ROOT_LIBRARIES} ${FLIB}")
    Execute_process(
      COMMAND gfortran -m64 -print-file-name=libgfortranbegin.a
      OUTPUT_VARIABLE FLIB
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ROOT_LIBRARIES "${ROOT_LIBRARIES} ${FLIB}")
  Endif(CMAKE_Fortran_COMPILER MATCHES g95) 

  Set(ALIMV_LIBRARIES "${ROOT_LIBRARIES}")

  Set(ALIMV_INCLUDE_DIR ${ROOT_INCLUDE_DIR} /usr/X11/include)

Endif(ALIMV_TARGET STREQUAL linuxx8664gcc)

# linux
If(ALIMV_TARGET STREQUAL linux) 


  Set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -O -g -shared -Wl")

  Set(CMAKE_Fortran_FLAGS "-O -g -fno-second-underscore -m32")

  Set(CLIBDEFS "-DCERNLIB_LINUX -DCERNLIB_BLDLIB -DCERNLIB_CZ")

  Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g -m32 -pipe -W -Wall -Weffc++ -Woverloaded-virtual -fmessage-length=0 -Wno-long-long -Dlinux -I/usr/X11R6/include")
  Set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O -g -m32 -Wall -W -pipe")

  If(CMAKE_Fortran_COMPILER MATCHES g95) 
    Set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fbounds-check -ftrace=full -DFORTRAN_G95")
  Else(CMAKE_Fortran_COMPILER MATCHES g95)
    Set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -DFORTRAN_GFORTRAN")
    Execute_process(
      COMMAND gfortran -m32 -print-file-name=libgfortran.so
      OUTPUT_VARIABLE FLIB
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ROOT_LIBRARIES "${ROOT_LIBRARIES} ${FLIB}")
    Execute_process(
      COMMAND gfortran -m32 -print-file-name=libgfortranbegin.a
      OUTPUT_VARIABLE FLIB
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ROOT_LIBRARIES "${ROOT_LIBRARIES} ${FLIB}")
  Endif(CMAKE_Fortran_COMPILER MATCHES g95) 

  Set(ALIMV_LIBRARIES "${ROOT_LIBRARIES}")

  Set(ALIMV_INCLUDE_DIR ${ROOT_INCLUDE_DIR} /usr/X11/include)

Endif(ALIMV_TARGET STREQUAL linux)

# macosx
If(ALIMV_TARGET STREQUAL macosx) 

  Execute_process(
    COMMAND sw_vers -productVersion
    OUTPUT_VARIABLE MACOSX
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  String(REGEX REPLACE "^(.*)[.](.*)[.](.*)$" "\\1" MACOSX_MAJOR "${MACOSX}")
  String(REGEX REPLACE "^(.*)[.](.*)[.](.*)$" "\\2" MACOSX_MINOR "${MACOSX}")

  Message(STATUS "Mac OS X ${MACOSX_MAJOR}.${MACOSX_MINOR}")

  Find_Package(fink)
  
  Set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -O -g -flat_namespace -single_module -undefined dynamic_lookup -m32")

  Set(CMAKE_Fortran_FLAGS "-O -g -fno-second-underscore -m32")

  Set(CLIBDEFS "-DCERNLIB_LINUX -DCERNLIB_BLDLIB -DCERNLIB_CZ -DCERNLIB_PPC")

  Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g -m32 -pipe -Wall -W -pipe -fbounds-check -fsigned-char -fno-common -fmessage-length=0 -Woverloaded-virtual -Weffc++ -Wconversion -Wshadow -fno-default-inline -fno-inline -I/usr/X11R6/include -I${FINK_ROOT}/include")
  Set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O -g -m32 -Wall -W -fno-common -pipe -I${FINK_ROOT}/include")

  If(CMAKE_Fortran_COMPILER MATCHES g95) 
    Set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -fbounds-check -ftrace=full -DFORTRAN_G95 -single_module")
  Else(CMAKE_Fortran_COMPILER MATCHES g95)
    Set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS}  -single_module -DFORTRAN_GFORTRAN")
    Execute_process(
      COMMAND gfortran -m32 -print-file-name=libgfortran.dylib
      OUTPUT_VARIABLE FLIB
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ROOT_LIBRARIES "${ROOT_LIBRARIES} ${FLIB}")
    Execute_process(
      COMMAND gfortran -m32 -print-file-name=libgfortranbegin.a
      OUTPUT_VARIABLE FLIB
      OUTPUT_STRIP_TRAILING_WHITESPACE)
    Set(ROOT_LIBRARIES "${ROOT_LIBRARIES} ${FLIB}")
  Endif(CMAKE_Fortran_COMPILER MATCHES g95) 

  Set(ALIMV_LIBRARIES "${ROOT_LIBRARIES}")

  Set(ALIMV_INCLUDE_DIR ${ROOT_INCLUDE_DIR} /usr/X11/include)

Endif(ALIMV_TARGET STREQUAL macosx)

# solarisCC5
If(ALIMV_TARGET STREQUAL solarisCC5) 

  Set(CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS "${CMAKE_SHARED_LIBRARY_CREATE_CXX_FLAGS} -O -g")

  Set(CLIBDEFS "-DCERNLIB_SUN -DCERNLIB_BLDLIB -DCERNLIB_CZ")

  Set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O -g +w -KPIC -template=no%extdef")
  Set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -O -g -KPIC -erroff=%none")

  Set(CMAKE_Fortran_FLAGS "${CMAKE_Fortran_FLAGS} -O -g -KPIC -erroff=%none")

  Set(ALIMV_LIBRARIES "${ROOT_LIBRARIES}")

  Set(ALIMV_INCLUDE_DIR ${ROOT_INCLUDE_DIR} /usr/X11/include)

  Set(CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -L/usr/dt/lib -L/usr/openwin/lib -L/usr/ccs/lib -lXm -lXt -lX11 -lm -lgen -ldl -lsocket -lsunmath -lfsu -lfui -lnsl")

Endif(ALIMV_TARGET STREQUAL solarisCC5)

EndMacro (SetupSystem)
