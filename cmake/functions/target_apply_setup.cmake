#[==[
Perform the setup for a given target using a keyword list.

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
function(target_apply_setup target)

    set(scope_keywords PUBLIC PRIVATE INTERFACE)


    # extract arguments -----------------------------------------------------------------------------------------------

    set(options "")
    set(oneValueArgs SOURCE_DIRECTORY)
    set(multiValueArgs DEFINITIONS
                       SOURCES
                       LIBRARIES
                       INCLUDE_DIRECTORIES
                       LINK_DIRECTORIES
                       COMPILE_FEATURES
                       COMPILE_OPTIONS
                       )
    cmake_parse_arguments(ARG
                          "${options}"
                          "${oneValueArgs}"
                          "${multiValueArgs}"
                          ${ARGN}
                          )


    # handle sources --------------------------------------------------------------------------------------------------

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


    # handle definitions ----------------------------------------------------------------------------------------------

    if(DEFINED ARG_DEFINITIONS)
        process_scopes(PRIVATE tmp_definitions ${ARG_DEFINITIONS})

        foreach(item ${tmp_definitions})
            if(NOT item IN_LIST scope_keywords)
                set(definitions ${definitions} "-D${item}")
            else()
                set(definitions ${definitions} ${item})
            endif()
        endforeach()

        target_compile_definitions(${target} ${definitions})
    endif()


    # include directories ---------------------------------------------------------------------------------------------

    if(DEFINED ARG_INCLUDE_DIRECTORIES)
        process_scopes(PRIVATE directories ${ARG_INCLUDE_DIRECTORIES})
        target_include_directories(${target} ${directories})
    endif()


    # link directories ------------------------------------------------------------------------------------------------

    if(DEFINED ARG_LINK_DIRECTORIES)
        process_scopes(PRIVATE directories ${ARG_LINK_DIRECTORIES})
        target_link_directories(${target} ${directories})
    endif()


    # compile features ------------------------------------------------------------------------------------------------

    if(DEFINED ARG_COMPILE_FEATURES)
        process_scopes(PRIVATE features ${ARG_COMPILE_FEATURES})
        target_compile_features(${target} ${features})
    endif()


    # compile options -------------------------------------------------------------------------------------------------

    if(DEFINED ARG_COMPILE_OPTIONS)
        process_scopes(PRIVATE options ${ARG_COMPILE_OPTIONS})
        target_compile_options(${target} ${options})
    endif()

    # link libraries --------------------------------------------------------------------------------------------------

    if(DEFINED ARG_LIBRARIES)
        process_scopes(PRIVATE libraries ${ARG_LIBRARIES})
        target_link_libraries(${target} ${libraries})
    endif()

endfunction()
