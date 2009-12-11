# -*- mode: cmake -*-

cmake_minimum_required(VERSION 2.6 FATAL_ERROR)

project(AliMV)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

find_package(ROOT 5.0.0 REQUIRED)

include(alimvMacros)

SetupSystem()

Check_out_of_Source_Build()

# searches for needed packages
set(ALIMV_INSTALL_DIR ${ALIMV}/${ALIMV_TARGET})
# set(ALIMV_LIBRARIES ${ALIMV}/${ALIMV_TARGET})

set(ALIMV_MAJOR_VERSION 0)
set(ALIMV_MINOR_VERSION 0)
set(ALIMV_PATCH_VERSION 0)
set(ALIMV_VERSION "${ALIMV_MAJOR_VERSION}.${ALIMV_MINOR_VERSION}.${ALIMV_PATCH_VERSION}")
set(ALIMV_LIBRARY_PROPERTIES ${ALIMV_LIBRARY_PROPERTIES}
    VERSION "${ALIMV_VERSION}"
    SOVERSION "${ALIMV_MAJOR_VERSION}"
    SUFFIX ".so"
)

add_subdirectory(PWG2resonances)
add_subdirectory(PWG2resonancesTest)
