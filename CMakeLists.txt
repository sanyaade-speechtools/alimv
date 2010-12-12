# -*- mode: cmake -*-

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(alimv)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(ALI_MIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "-ggdb -Wall -Wextra")
# set(CMAKE_CXX_FLAGS "-pedantic")
find_package(ROOT 5.0.0 REQUIRED)
find_package(AliRoot REQUIRED)

include(AliRootBaseMacro)

set(PARS_TO_CREATE ANALYSISaliceMV PWG2resonances EventMixing RESONANCESMV TASKSMV)

foreach (MYPAR ${PARS_TO_CREATE})
  add_subdirectory(${MYPAR})
endforeach(MYPAR)

add_subdirectory(alimv)

# uninstall
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
add_custom_target(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Testing
enable_testing()
add_test(LoadLibs root -l -q ${CMAKE_CURRENT_SOURCE_DIR}/test/macros/base/LoadLibs.C)

add_test(CreateParFiles ${CMAKE_CURRENT_SOURCE_DIR}/test/scripts/MakeParFiles.sh ${CMAKE_CURRENT_SOURCE_DIR} ${PARS_TO_CREATE})
add_test(CompileParFiles ${CMAKE_CURRENT_SOURCE_DIR}/test/scripts/CompileParFiles.sh ${PARS_TO_TEST})
