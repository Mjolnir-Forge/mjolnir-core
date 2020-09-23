cmake_minimum_required(VERSION 3.15...3.18)

# read version --------------------------------------------------------------------------------------------------------

set(VERSION_REGEX "#define MJOLNIR_CORE_VERSION_(MAJOR|MINOR|PATCH|TWEAK)")

file(STRINGS "${CMAKE_CURRENT_SOURCE_DIR}/mjolnir/core/version.h"
        ver REGEX ${VERSION_REGEX})

string(REGEX MATCH "VERSION_MAJOR ([0-9]*)" _ ${ver})
set(ver_major ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_MINOR ([0-9]*)" _ ${ver})
set(ver_minor ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_PATCH ([0-9]*)" _ ${ver})
set(ver_patch ${CMAKE_MATCH_1})

string(REGEX MATCH "VERSION_TWEAK ([0-9]*)" _ ${ver})
set(ver_tweak ${CMAKE_MATCH_1})

set(version_number "${ver_major}.${ver_minor}.${ver_patch}")
if (NOT ${ver_tweak} EQUAL 0)
    set(version_number "${version_number}.${ver_tweak}")
endif()




# project setting -----------------------------------------------------------------------------------------------------

project(Mjolnir-Core VERSION ${version_number}
    DESCRIPTION "Core module of the Mjolnir Game Development Kit"
    LANGUAGES CXX)


set(CMAKE_VERBOSE_MAKEFILE OFF)


# Set directories -----------------------------------------------------------------------------------------------------

if(NOT DEFINED MJOLNIR_ROOT_DIR)
    set(MJOLNIR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(NOT DEFINED MJOLNIR_CORE_ROOT_DIR)
    set(MJOLNIR_CORE_ROOT_DIR ${MJOLNIR_ROOT_DIR})
endif()
set(MJOLNIR_CORE_SOURCE_DIR ${MJOLNIR_CORE_ROOT_DIR}/mjolnir/core)


# Perform setup -------------------------------------------------------------------------------------------------------

set(CMAKE_MODULE_PATH
    ${MJOLNIR_CORE_ROOT_DIR}/cmake
    ${CMAKE_MODULE_PATH})

include(compiler_setup)
include(include_cmake_functions)
include(resolve_dependencies)


# Add subdirectories --------------------------------------------------------------------------------------------------

add_subdirectory(mjolnir/core)
add_subdirectory(tests)
