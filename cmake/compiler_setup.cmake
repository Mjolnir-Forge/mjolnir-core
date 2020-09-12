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

