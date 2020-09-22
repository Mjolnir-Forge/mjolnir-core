#[==[
Set all compiler related cmake variables, if they are not already set.
#]==]

# options ------------------------------------------------------------------------------------------------------------

option(MJOLNIR_CORE_ENABLE_LTO "Enables link time optimizations" FALSE)
option(MJOLNIR_CORE_ENABLE_COMPILER_EXTENSIONS "Enables compiler extensions" FALSE)


# Set compile features ------------------------------------------------------------------------------------------------

if(NOT DEFINED MJOLNIR_COMPILE_FEATURES)
    set(MJOLNIR_COMPILE_FEATURES cxx_std_17)
endif()

if(NOT DEFINED MJOLNIR_CORE_COMPILE_FEATURES)
    set(MJOLNIR_CORE_COMPILE_FEATURES ${MJOLNIR_COMPILE_FEATURES})
endif()

set(MJOLNIR_COMPILE_FEATURES
    ${MJOLNIR_COMPILE_FEATURES}
    ${MJOLNIR_ADDITIONAL_COMPILE_FEATURES})

set(MJOLNIR_CORE_COMPILE_FEATURES
    ${MJOLNIR_CORE_COMPILE_FEATURES}
    ${MJOLNIR_CORE_ADDITIONAL_COMPILE_FEATURES})

# Set compile options -------------------------------------------------------------------------------------------------

if(NOT DEFINED MJOLNIR_COMPILE_OPTIONS)
    if (MSVC)
        set(MJOLNIR_COMPILE_OPTIONS /W4)
    else()
        set(MJOLNIR_COMPILE_OPTIONS -Wall -Wextra -pedantic)
    endif()
endif()

if(NOT DEFINED MJOLNIR_CORE_COMPILE_OPTIONS)
    set(MJOLNIR_CORE_COMPILE_OPTIONS ${MJOLNIR_COMPILE_OPTIONS})
endif()

set(MJOLNIR_COMPILE_OPTIONS
    ${MJOLNIR_COMPILE_OPTIONS}
    ${MJOLNIR_ADDITIONAL_COMPILE_OPTIONS})

set(MJOLNIR_CORE_COMPILE_OPTIONS
    ${MJOLNIR_CORE_COMPILE_OPTIONS}
    ${MJOLNIR_CORE_ADDITIONAL_COMPILE_OPTIONS})


#if(DEFINED TREAT_WARNING_AS_ERROR)
#    if (MSVC)
#        set(MJOLNIR_COMPILE_OPTIONS ${MJOLNIR_COMPILE_OPTIONS} /WX)
#    else()
#        set(MJOLNIR_COMPILE_OPTIONS ${MJOLNIR_COMPILE_OPTIONS} -Werror)
#    endif()
#endif()

# Compiler extensions -------------------------------------------------------------------------------------------------


if(${MJOLNIR_CORE_ENABLE_COMPILER_EXTENSIONS})
    set(MJOLNIR_CORE_TARGET_PROPERTIES ${MJOLNIR_CORE_TARGET_PROPERTIES} CXX_EXTENSIONS ON)
else()
    set(MJOLNIR_CORE_TARGET_PROPERTIES ${MJOLNIR_CORE_TARGET_PROPERTIES} CXX_EXTENSIONS OFF)
endif()


# Link time optimizations ---------------------------------------------------------------------------------------------

include(CheckIPOSupported)
check_ipo_supported(RESULT lto_supported)
if(lto_supported)
    set(MJOLNIR_CORE_TARGET_PROPERTIES
        ${MJOLNIR_CORE_TARGET_PROPERTIES}
        INTERPROCEDURAL_OPTIMIZATION
        ${MJOLNIR_CORE_ENABLE_LTO})
endif()
