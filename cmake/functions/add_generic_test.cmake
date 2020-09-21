#[==[
Create a test and perform the necessary setup using a keyword list.

A list of accepted keywords and their purpose is given below. Always use scope keywords (PUBLIC, PRIVATE, INTERFACE)
directly after one of these keywords, otherwise the scope of the new items is undefined.


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

    COMPILE_DEFINITIONS:
        Definitions that should be added

    COMPILE_FEATURES:
        Compile features that should be added (target_compile_features)

    COMPILE_OPTIONS:
        Compile options that should be added (target_compile_options)

    INCLUDE_DIRECTORIES:
        Additional directories that should be searched for the included header files (uses target_include_directories)

    LINK_DIRECTORIES:
        Additional directories that should be searched for the linked libraries (uses target_link_libraries)

    LINK_LIBRARIES:
        Libraries that should be linked

    PROPERTIES:
        List of properties (set_target_properties).

    SOURCES:
        List of source files. The filepath must be given in relation to the specified root directory

    SOURCE_DIRECTORY:
        The root directory of the source files. If none is specified, the current CMake source directory is taken.

#]==]
function(add_generic_test target test_name)
    add_to_list_after_keyword("${ARGN}" arguments LINK_LIBRARIES  PRIVATE gtest_main)
    add_generic_executable(${target}
                           ${arguments}
                           )
    add_test(${test_name} ${target})
endfunction()
