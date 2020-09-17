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


    # handle source directory -----------------------------------------------------------------------------------------

    if(NOT DEFINED ARG_SOURCE_DIRECTORY)
        set(ARG_SOURCE_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    endif()


    # handle sources --------------------------------------------------------------------------------------------------

    set(scope_keywords PUBLIC PRIVATE INTERFACE)

    set(ARG_SOURCES ${ARG_UNPARSED_ARGUMENTS} ${ARG_SOURCES})
    foreach(source ${ARG_SOURCES})
        set(sources "${ARG_SOURCE_DIRECTORY}/${source}" ${sources})
    endforeach()


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


    # create executable -----------------------------------------------------------------------------------------------

    target_sources(${target} PRIVATE ${sources})

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
