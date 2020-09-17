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


#]==]
function(target_apply_setup target)

    # extract arguments -----------------------------------------------------------------------------------------------

    set(options "")
    set(oneValueArgs SOURCE_DIRECTORY)
    set(multiValueArgs DEFINITIONS
                       SOURCES
                       LIBRARIES
                       INCLUDE_DIRECTORIES
                       LINK_DIRECTORIES
                       )
    cmake_parse_arguments(ARG
                          "${options}"
                          "${oneValueArgs}"
                          "${multiValueArgs}"
                          ${ARGN}
                          )


    # handle sources --------------------------------------------------------------------------------------------------

    set(scope_keywords PUBLIC PRIVATE INTERFACE)
    cmake_parse_arguments(SOURCE_SCOPE
                          ""
                          ""
                          "${scope_keywords}"
                          ${ARG_SOURCES}
                          )
    if(DEFINED ARG_UNPARSED_ARGUMENTS OR DEFINED SOURCE_SCOPE_PRIVATE)
        set(tmp_sources PRIVATE ${ARG_UNPARSED_ARGUMENTS} ${SOURCE_SCOPE_PRIVATE})
    endif()

    if(DEFINED SOURCE_SCOPE_PUBLIC)
        set(tmp_sources ${tmp_sources} PUBLIC ${SOURCE_SCOPE_PUBLIC})
    endif()

    if(DEFINED SOURCE_SCOPE_INTERFACE)
        set(tmp_sources ${tmp_sources} INTERFACE ${SOURCE_SCOPE_INTERFACE})
    endif()

    if(DEFINED ARG_SOURCE_DIRECTORY)
        foreach(source ${tmp_sources})
            if(NOT source IN_LIST scope_keywords)
                set(sources ${sources} "${ARG_SOURCE_DIRECTORY}/${source}")
            else()
                set(sources ${sources} ${source})
            endif()
        endforeach()
    endif()


    # handle definitions ----------------------------------------------------------------------------------------------

    if(DEFINED ARG_DEFINITIONS)
        foreach(item ${ARG_DEFINITIONS})
            if(NOT item IN_LIST scope_keywords)
                set(definitions ${definitions} "-D${item}")
            else()
                set(definitions ${definitions} ${item})
            endif()
        endforeach()
    endif()


    # apply setup -----------------------------------------------------------------------------------------------------

    target_sources(${target} ${sources})

    if(DEFINED definitions)
        target_compile_definitions(${target} ${definitions})
    endif()

    if(DEFINED ARG_INCLUDE_DIRECTORIES)
        target_include_directories(${target} ${ARG_INCLUDE_DIRECTORIES})
    endif()

    if(DEFINED ARG_LINK_DIRECTORIES)
        target_link_directories(${target} ${ARG_LINK_DIRECTORIES})
    endif()

    target_link_libraries(${target} ${ARG_LIBRARIES})
endfunction()
