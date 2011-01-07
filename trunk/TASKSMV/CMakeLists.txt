# -*- mode: cmake -*-
# Create a library called "lib<name>" which includes the source files given in
# the array .
# The extension is already found.  Any number of sources could be listed here.

set(INCLUDE_DIRECTORIES
${CMAKE_SOURCE_DIR}/TASKSMV
${CMAKE_SOURCE_DIR}/ANALYSISaliceMV
${CMAKE_SOURCE_DIR}/EventMixing
${CMAKE_SOURCE_DIR}/EventMixing/EventMixing
${CMAKE_SOURCE_DIR}/RESONANCESMV
${CMAKE_SOURCE_DIR}/PWG2resonances
${CMAKE_SOURCE_DIR}/PWG2resonances/RESONANCES
${ALIROOT_INCLUDE_DIR}
${ROOT_INCLUDE_DIR}
)

include_directories( ${INCLUDE_DIRECTORIES})

set(LINK_DIRECTORIES
${ROOT_LIBRARY_DIR}
) 

link_directories( ${LINK_DIRECTORIES})

include(CMake_libTASKSMV.txt)
