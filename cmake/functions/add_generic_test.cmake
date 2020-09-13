#[==[
Create a test and add all sources, definitions and dependencies passed as a list of items.

The tests cpp-file doesn't need to be passed. It is added automatically.

PARAMETERS:
-----------

    target:
        Name of the CMake target that should be used. The prefix "test_" is added automatically.

    root_directory:
        The root directory of the source files

    test_name:
        The name of the test that is shown when running ctest

    ARGN:
        Additional parameters can be a list of sources, definitions and dependencies
        - Every item containing ".cpp" is considered as a source
        - Every item containing "-D" is considered as a definition
        - All other items are considered as dependencies

#]==]
function(add_generic_test target root_directory test_name)
    add_generic_executable(${target}
                           ${root_directory}
                           gtest_main
                           ${ARGN}
                           )
    add_test(${test_name} ${target})
endfunction()
