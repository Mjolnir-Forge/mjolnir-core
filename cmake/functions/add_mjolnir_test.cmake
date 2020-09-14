#[==[
Create a test for this library and add all sources, definitions and dependencies passed as a list of items.

The tests cpp-file doesn't need to be passed. It is added automatically.

PARAMETERS:
-----------

    target:
        Name of the CMake target that should be used. The prefix "test_" is added automatically.

    root_directory:
        The root directory of the source files

    ARGN:
        Additional parameters can be a list of sources, definitions and dependencies
        - Every item containing ".cpp" is considered as a source
        - Every item containing "-D" is considered as a definition
        - All other items are considered as dependencies

#]==]
function(add_mjolnir_test target)
    set(test_name ${target})
    set(target "test_${target}")

    string(REPLACE "${MJOLNIR_CORE_ROOT_DIR}/tests/"
           ""
           relative_path
           ${CMAKE_CURRENT_SOURCE_DIR}
           )

    string(REPLACE "/"
           "::"
           test_prefix
           ${relative_path}
           )

    set(test_source "/tests/${relative_path}/${target}.cpp")
    set(ctest_test_name "${test_prefix}::${test_name}")

    add_generic_test(${target}
                     ${MJOLNIR_CORE_ROOT_DIR}
                     ${ctest_test_name}
                     ${test_source}
                     ${ARGN}
                     )
endfunction()
