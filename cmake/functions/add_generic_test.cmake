#[==[
Create a test and add all sources, definitions and dependencies passed as a list of items.


PARAMETERS:
-----------

    target:
        Name of the CMake target that should be used.

    test_name:
        The name of the test that is shown when running ctest

    ARGN:
        A list containing all necessary data of arbitrary length separated by keywords.

KEYWORDS
--------

    SOURCES:
        List of source files. The filepath must be given in relation to the specified root directory

    DEFINITIONS:
        Definitions that should be added

    LIBRARIES:
        Libraries that should be linked

    SOURCE_DIRECTORY:
        The root directory of the source files. If none is specified, the current CMake source directory is taken.

    INCLUDE_DIRECTORIES:
        Additional directories that should be searched for the included header files (uses target_include_directories)

    LINK_DIRECTORIES:
        Additional directories that should be searched for the linked libraries (uses target_link_libraries)

    COMPILE_FEATURES:
        Compile features that should be added (target_compile_features)

    COMPILE_OPTIONS:
        Compile options that should be added (target_compile_options)

#]==]
function(add_generic_test target test_name)
    add_to_list_after_keyword("${ARGN}" arguments LIBRARIES  gtest_main)
    add_generic_executable(${target}
                           ${arguments}
                           )
    add_test(${test_name} ${target})
endfunction()
