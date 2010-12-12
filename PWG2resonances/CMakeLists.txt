# -*- mode: cmake -*-
# Create a library called "lib<name>" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${CMAKE_SOURCE_DIR}/STEERBase
${CMAKE_SOURCE_DIR}/ESD
${CMAKE_SOURCE_DIR}/AOD
${CMAKE_SOURCE_DIR}/ANALYSIS
${CMAKE_SOURCE_DIR}/ANALYSISalice
${CMAKE_SOURCE_DIR}/CORRFW
${CMAKE_SOURCE_DIR}/PWG2resonances
${CMAKE_SOURCE_DIR}/PWG2resonances/RESONANCES
${ALIROOT_INCLUDE_DIR}
${ALIROOT_INCLUDE_DIR}/../TOF
${ROOT_INCLUDE_DIR}
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

# SetModule()

include(CMake_libPWG2resonances.txt)

