#[==[
Perform the setup for a given target using a keyword list.

A list of accepted keywords and their purpose is given below. Always use scope keywords
(PUBLIC, PRIVATE, INTERFACE) directly after one of these keywords, otherwise the scope
of the new items is undefined.


PARAMETERS:
-----------

    target:
        Name of the CMake target that should be used

    ARGN:
        A list containing all necessary data separated by keywords.


KEYWORDS
--------

    COMPILE_DEFINITIONS:
        Definitions that should be added

    COMPILE_FEATURES:
        Compile features that should be added (target_compile_features)

    COMPILE_OPTIONS:
        Compile options that should be added (target_compile_options)

    INCLUDE_DIRECTORIES:
        Additional directories that should be searched for the included header files
        (uses target_include_directories)

    LINK_DIRECTORIES:
        Additional directories that should be searched for the linked libraries (uses
        target_link_libraries)

    LINK_LIBRARIES:
        Libraries that should be linked

    LINK_OPTIONS:
        Linker options that should be added (target_link_options)

    PROPERTIES:
    List of properties (set_target_properties).

    SOURCES:
        List of source files. The filepath must be given in relation to the specified
        root directory

    SOURCE_DIRECTORY:
        The root directory of the source files. If none is specified, the current CMake
        source directory is taken.

#]==]
function(target_apply_setup target)

    set(scope_keywords PUBLIC PRIVATE INTERFACE)

    # ----------------------------------------------------------------------------------
    # extract arguments
    # ----------------------------------------------------------------------------------

    set(options "")
    set(oneValueArgs SOURCE_DIRECTORY)
    set(multiValueArgs
        COMPILE_DEFINITIONS
        COMPILE_FEATURES
        COMPILE_OPTIONS
        INCLUDE_DIRECTORIES
        LINK_DIRECTORIES
        LINK_LIBRARIES
        LINK_OPTIONS
        PROPERTIES
        SOURCES)

    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}"
                          "${multiValueArgs}" ${ARGN})

    # ----------------------------------------------------------------------------------
    # sources
    # ----------------------------------------------------------------------------------

    process_scopes(PRIVATE tmp_sources ${ARG_UNPARSED_ARGUMENTS} ${ARG_SOURCES})

    if(DEFINED ARG_SOURCE_DIRECTORY)
        foreach(source ${tmp_sources})
            if(NOT source IN_LIST scope_keywords)
                set(sources ${sources} "${ARG_SOURCE_DIRECTORY}/${source}")
            else()
                set(sources ${sources} ${source})
            endif()
        endforeach()
    endif()

    target_sources(${target} ${sources})

    # ----------------------------------------------------------------------------------
    # compile definitions
    # ----------------------------------------------------------------------------------

    if(DEFINED ARG_COMPILE_DEFINITIONS)
        process_scopes(PRIVATE tmp_definitions ${ARG_COMPILE_DEFINITIONS})

        foreach(item ${tmp_definitions})
            string(FIND ${item} "-D" find_pos)
            if(NOT item IN_LIST scope_keywords AND NOT ${find_pos} EQUAL 0)
                set(definitions ${definitions} "-D${item}")
            else()
                set(definitions ${definitions} ${item})
            endif()
        endforeach()

        target_compile_definitions(${target} ${definitions})
    endif()

    # ----------------------------------------------------------------------------------
    # compile features
    # ----------------------------------------------------------------------------------

    if(DEFINED ARG_COMPILE_FEATURES)
        process_scopes(PRIVATE features ${ARG_COMPILE_FEATURES})
        target_compile_features(${target} ${features})
    endif()

    # ----------------------------------------------------------------------------------
    # compile options
    # ----------------------------------------------------------------------------------

    if(DEFINED ARG_COMPILE_OPTIONS)
        process_scopes(PRIVATE options ${ARG_COMPILE_OPTIONS})
        target_compile_options(${target} ${options})
    endif()

    # ----------------------------------------------------------------------------------
    # include directories
    # ----------------------------------------------------------------------------------

    if(DEFINED ARG_INCLUDE_DIRECTORIES)
        process_scopes(PRIVATE directories ${ARG_INCLUDE_DIRECTORIES})
        target_include_directories(${target} ${directories})
    endif()

    # ----------------------------------------------------------------------------------
    # link directories
    # ----------------------------------------------------------------------------------

    if(DEFINED ARG_LINK_DIRECTORIES)
        process_scopes(PRIVATE directories ${ARG_LINK_DIRECTORIES})
        target_link_directories(${target} ${directories})
    endif()

    # ----------------------------------------------------------------------------------
    # link libraries
    # ----------------------------------------------------------------------------------

    if(DEFINED ARG_LINK_LIBRARIES)
        process_scopes(PRIVATE libraries ${ARG_LINK_LIBRARIES})
        target_link_libraries(${target} ${libraries})
    endif()

    # ----------------------------------------------------------------------------------
    # link options
    # ----------------------------------------------------------------------------------

    if(DEFINED ARG_LINK_OPTIONS)
        process_scopes(PRIVATE options ${ARG_LINK_OPTIONS})
        target_link_options(${target} ${options})
    endif()

    # ----------------------------------------------------------------------------------
    # properties
    # ----------------------------------------------------------------------------------

    if(DEFINED ARG_PROPERTIES)
        set_target_properties(${target} PROPERTIES ${ARG_PROPERTIES})
    endif()

endfunction()
