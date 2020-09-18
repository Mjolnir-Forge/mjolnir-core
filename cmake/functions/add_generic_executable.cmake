#[==[
Create an executable and add all sources, definitions and dependencies passed as a keyword list of items.

PARAMETERS:
-----------

    target:
        Name of the CMake target that should be used

    ARGN:
        A list containing all necessary data separated by keywords.

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
function(add_generic_executable target)
    add_executable(${target})
    target_apply_setup(${target} ${ARGN})
endfunction()
