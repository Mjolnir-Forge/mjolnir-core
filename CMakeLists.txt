cmake_minimum_required(VERSION 3.15...3.18)


# Set directories -----------------------------------------------------------------------------------------------------

if(NOT DEFINED MJOLNIR_ROOT_DIR)
    set(MJOLNIR_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
endif()

if(NOT DEFINED MJOLNIR_CORE_ROOT_DIR)
    set(MJOLNIR_CORE_ROOT_DIR ${MJOLNIR_ROOT_DIR})
endif()

set(MJOLNIR_CORE_SOURCE_DIR ${MJOLNIR_CORE_ROOT_DIR}/mjolnir/core)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${MJOLNIR_CORE_ROOT_DIR}/cmake)


# project setting -----------------------------------------------------------------------------------------------------

include(include_cmake_functions)

extract_mjolnir_version_number("${MJOLNIR_CORE_ROOT_DIR}/mjolnir/core/version.h"
    CORE
    version_number
    )

project(Mjolnir-Core
    VERSION ${version_number}
    DESCRIPTION "Core module of the Mjolnir Game Development Kit"
    LANGUAGES CXX
    )

message("Mjolnir-Core version ${version_number}")



# Perform setup -------------------------------------------------------------------------------------------------------

include(configure_core_module)
include(resolve_dependencies)


# Add subdirectories --------------------------------------------------------------------------------------------------

add_subdirectory(mjolnir/core)
add_subdirectory(tests)
