cmake_minimum_required(VERSION 3.13...3.18)

project(Mjolnir-Core VERSION 0.0.1
    DESCRIPTION "Core module of the Mjolnir Game Development Kit"
    LANGUAGES CXX)


set(CMAKE_VERBOSE_MAKEFILE OFF)


# Set directories -----------------------------------------------------------------------------------------------------

if(NOT DEFINED MJOLNIR_CORE_ROOT_DIR)
    set(MJOLNIR_CORE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()
set(MJOLNIR_CORE_SOURCE_DIR ${MJOLNIR_CORE_ROOT_DIR}/core)


# Perform setup -------------------------------------------------------------------------------------------------------

set(CMAKE_MODULE_PATH
    ${MJOLNIR_CORE_ROOT_DIR}/cmake
    ${CMAKE_MODULE_PATH})

include(compiler_setup)
include(include_cmake_functions)
include(resolve_dependencies)


# Add subdirectories --------------------------------------------------------------------------------------------------

add_subdirectory(core)
add_subdirectory(tests)
