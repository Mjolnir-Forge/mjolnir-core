#[==[
Create a test for the mjolnir libraries and add all sources, definitions and dependencies passed as a list of items.


PARAMETERS:
-----------

    target:
        Name of the CMake target that should be used. The prefix "test_" is added automatically.

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

    append_keyword_list("${ARGN}" LIBRARIES gtest_main)

    append_keyword_list("${keyword_list}" COMPILE_FEATURES PRIVATE ${MJOLNIR_COMPILE_FEATURES})
    append_keyword_list("${keyword_list}" COMPILE_OPTIONS PRIVATE ${MJOLNIR_COMPILE_OPTIONS})

    add_generic_executable(${target}
                           ${test_source}
                           SOURCE_DIRECTORY
                               ${MJOLNIR_CORE_ROOT_DIR}
                           ${keyword_list}
                           )

    add_test(${ctest_test_name} ${target})
endfunction()
