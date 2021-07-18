#[==[
Configure the module.
#]==]

# ------------------------------------------------------------------------------
# options
# ------------------------------------------------------------------------------

option(MJOLNIR_CORE_ENABLE_TESTS "Enable the tests" TRUE)
option(MJOLNIR_CORE_ENABLE_COMPILER_EXTENSIONS "Enables compiler extensions"
       FALSE)
option(MJOLNIR_CORE_ENABLE_LTO "Enables link time optimizations" FALSE)

# ------------------------------------------------------------------------------
# CMake setup
# ------------------------------------------------------------------------------

set(CMAKE_VERBOSE_MAKEFILE OFF)

# Require out-of-source builds

file(TO_CMAKE_PATH "${PROJECT_BINARY_DIR}/CMakeLists.txt" LOC_PATH)
if(EXISTS "${LOC_PATH}")
    message(
        FATAL_ERROR
            "You cannot build in a source directory (or any directory with a "
            "CMakeLists.txt file). Create a build subdirectory.")
endif()

# ------------------------------------------------------------------------------
# Dependencies
# ------------------------------------------------------------------------------

set(GTEST_MINIMAL_VERSION 1.10.0)

# ------------------------------------------------------------------------------
# Compiler
# ------------------------------------------------------------------------------

# Set compile features

set(MJOLNIR_CORE_COMPILE_FEATURES cxx_std_17
                                  ${MJOLNIR_CORE_ADDITIONAL_COMPILE_FEATURES})

# Set compile options

if(MSVC)
    set(MJOLNIR_CORE_COMPILE_OPTIONS /W4
                                     ${MJOLNIR_CORE_ADDITIONAL_COMPILE_OPTIONS})
else()
    set(MJOLNIR_CORE_COMPILE_OPTIONS
        -Wall
        -Wextra
        -Wpedantic
        -Wmissing-braces
        -Wshadow
        -pthread
        ${MJOLNIR_CORE_ADDITIONAL_COMPILE_OPTIONS})
endif()

# Compiler extensions

if(${MJOLNIR_CORE_ENABLE_COMPILER_EXTENSIONS})
    set(MJOLNIR_CORE_TARGET_PROPERTIES CXX_EXTENSIONS ON)
else()
    set(MJOLNIR_CORE_TARGET_PROPERTIES CXX_EXTENSIONS OFF)
endif()

# ------------------------------------------------------------------------------
# Linker
# ------------------------------------------------------------------------------

# Link time optimizations
set(MJOLNIR_CORE_LINK_OPTIONS ${MJOLNIR_CORE_ADDITIONAL_LINK_OPTIONS})

include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported)
if(lto_supported)
    set(MJOLNIR_CORE_TARGET_PROPERTIES
        ${MJOLNIR_CORE_TARGET_PROPERTIES} INTERPROCEDURAL_OPTIMIZATION
        ${MJOLNIR_CORE_ENABLE_LTO})
endif()
