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

# define minimal compiler versions

set(MINIMAL_CLANG_CXX_VERSION "12.0")
set(MINIMAL_GNU_CXX_VERSION "10.0")
set(MINIMAL_MSVC_CXX_VERSION "19.28")

# Set minimal version for detected compiler
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(MINIMAL_CXX_COMPILER_VERSION ${MINIMAL_GNU_CXX_VERSION})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(MINIMAL_CXX_COMPILER_VERSION ${MINIMAL_CLANG_CXX_VERSION})
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(MINIMAL_CXX_COMPILER_VERSION ${MINIMAL_MSVC_CXX_VERSION})
else()
    message(WARNING "Compiler is not supported. Builds might fail!")
endif()

# Check compiler version
if(${CMAKE_CXX_COMPILER_VERSION} VERSION_LESS ${MINIMAL_CXX_COMPILER_VERSION})
    if(NOT DEFINED IGNORE_CXX_COMPILER_COMPATIBILITY
       OR NOT ${IGNORE_CXX_COMPILER_COMPATIBILITY})
        message(
            FATAL_ERROR
                "The required minimal version for compiler "
                "'${CMAKE_CXX_COMPILER_ID}' is "
                "${MINIMAL_CXX_COMPILER_VERSION}."
                " Your version is ${CMAKE_CXX_COMPILER_VERSION}")
    else()
        message(
            WARNING
                "You are using an unsupported compiler version. Builds might "
                "fail.")
    endif()
endif()

# Set compile definitions
set(MJOLNIR_CORE_COMPILE_DEFINITIONS
    ${MJOLNIR_CORE_ADDITIONAL_COMPILE_DEFINITIONS})

# Set compile features
set(MJOLNIR_CORE_COMPILE_FEATURES cxx_std_20
                                  ${MJOLNIR_CORE_ADDITIONAL_COMPILE_FEATURES})

# Set compile options
if(MSVC)
    set(MJOLNIR_CORE_COMPILE_OPTIONS /W4 /arch:AVX2 /w14242 /w14254 /w14263 /w14265 /w14287 /w14296 /w14640
                                     ${MJOLNIR_CORE_ADDITIONAL_COMPILE_OPTIONS})
    if(${MJOLNIR_CORE_ENABLE_TESTS})
        set(MJOLNIR_CORE_COMPILE_OPTIONS ${MJOLNIR_CORE_COMPILE_OPTIONS}
                                         /bigobj)
    endif()
else()
    set(MJOLNIR_CORE_COMPILE_OPTIONS
        -Wall
        -Wextra
        -Wpedantic
        -Wmissing-braces
        -Wshadow
        -Wnon-virtual-dtor
        -Wold-style-cast
        -Wcast-align
        -Wunused
        -Woverloaded-virtual
        -Wconversion
        -Wsign-conversion
        -Wmisleading-indentation
        # -Wduplicated-cond -Wduplicated-branches -Wlogical-op
        -Wnull-dereference
        # -Wuseless-cast
        -Wdouble-promotion
        # -Wlifetime -Weverything
        -pedantic
        -pthread
        -march=native
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

set(MJOLNIR_CORE_LINK_OPTIONS ${MJOLNIR_CORE_ADDITIONAL_LINK_OPTIONS})

# Link time optimizations
include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported)
if(lto_supported)
    set(MJOLNIR_CORE_TARGET_PROPERTIES
        ${MJOLNIR_CORE_TARGET_PROPERTIES} INTERPROCEDURAL_OPTIMIZATION
        ${MJOLNIR_CORE_ENABLE_LTO})
endif()
