# -*- mode: cmake -*-
# Create a library called "lib<name>" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${CMAKE_SOURCE_DIR}/TENDER
${CMAKE_SOURCE_DIR}/TENDERSupplies
${ALIROOT_INCLUDE_DIR}
${ALIROOT_INCLUDE_DIR}/../TOF/
${ALIROOT_INCLUDE_DIR}/../TPC/
${ALIROOT_INCLUDE_DIR}/../T0/
${ALIROOT_INCLUDE_DIR}/../TRD/
${ALIROOT_INCLUDE_DIR}/../TRD/Cal
${ALIROOT_INCLUDE_DIR}/../VZERO/
${ROOT_INCLUDE_DIR}
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

include(CMake_libTENDERSupplies.txt)
