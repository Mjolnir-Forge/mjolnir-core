#[==[
Set all compiler related cmake variables, if they are not already set.
#]==]


# Set compile features ------------------------------------------------------------------------------------------------

if(NOT DEFINED MJOLNIR_COMPILE_FEATURES)
    set(MJOLNIR_COMPILE_FEATURES cxx_std_17)
endif()

set(MJOLNIR_COMPILE_FEATURES
    ${MJOLNIR_COMPILE_FEATURES}
    ${MJOLNIR_ADDITIONAL_COMPILE_FEATURES})

# Set compile options -------------------------------------------------------------------------------------------------

if(NOT DEFINED MJOLNIR_COMPILE_OPTIONS)
    if (MSVC)
        set(MJOLNIR_COMPILE_OPTIONS /W4)
    else()
        set(MJOLNIR_COMPILE_OPTIONS -Wall -Wextra -pedantic)
    endif()
endif()

if(DEFINED TREAT_WARNING_AS_ERROR)
    if (MSVC)
        set(MJOLNIR_COMPILE_OPTIONS ${MJOLNIR_COMPILE_OPTIONS} /WX)
    else()
        set(MJOLNIR_COMPILE_OPTIONS ${MJOLNIR_COMPILE_OPTIONS} -Werror)
    endif()
endif()

