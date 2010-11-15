# -*- mode: cmake -*-

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(ali-analysis-mix)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
set(ALI_MIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_CXX_FLAGS "-ggdb -Wall -Wextra")
# set(CMAKE_CXX_FLAGS "-pedantic")
find_package(ROOT 5.0.0 REQUIRED)
find_package(AliRoot REQUIRED)

include(AliRootBaseMacro)

# add_subdirectory(MIXMV)
# add_subdirectory(EventMixing)

# uninstall
CONFIGURE_FILE("${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in" "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake" IMMEDIATE @ONLY)
ADD_CUSTOM_TARGET(uninstall "${CMAKE_COMMAND}" -P "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Testing
ENABLE_TESTING()
ADD_TEST(LoadLibs root -l -q ${CMAKE_CURRENT_SOURCE_DIR}/test/macros/base/LoadLibs.C)

# set(PARS_TO_TEST EventMixing)
set(PARS_TO_TEST )
ADD_TEST(CreateParFiles ${CMAKE_CURRENT_SOURCE_DIR}/test/scripts/MakeParFiles.sh ${CMAKE_CURRENT_SOURCE_DIR} ${PARS_TO_TEST})
ADD_TEST(CompileParFiles ${CMAKE_CURRENT_SOURCE_DIR}/test/scripts/CompileParFiles.sh ${PARS_TO_TEST})
