add_generic_executable(
    hello_world hello_world.cpp
    SOURCE_DIRECTORY ${MJOLNIR_CORE_ROOT_DIR}/src/mjolnir/core)
target_compile_features(hello_world cxx_std_17)
