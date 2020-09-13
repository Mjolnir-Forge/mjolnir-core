#[==[
Separate a list of items into sources, definitions and dependencies.

- Every item containing ".cpp" is considered as a source
- Every item containing "-D" is considered as a definition
- All other items are considered as dependencies

PARAMETERS:
-----------

    items:
        A list of items that should be separated

    root_directory:
        A directory name that should be added to the sources

RETURNS
-------

    sources:
        List of sources

    definitions:
        List of definitions

    dependencies:
        List of dependencies

#]==]
function(extract_sources_definitions_dependencies items root_directory)
    # set(definitions "PUBLIC")

    # loop over all items
    foreach(item ${items})
        string(FIND "${item}" ".cpp" is_cpp)

        if(NOT ${is_cpp} EQUAL "-1")
            set(sources "${sources};${root_directory}/${item}")
        else()
            string(FIND ${item} "-D" is_definition)
            if(NOT ${is_definition} EQUAL "-1")
                set(definitions "${definitions};${item}")
            else()
                set(dependencies "${dependencies};${item}")
            endif()
        endif()
    endforeach()

    set(sources ${sources} PARENT_SCOPE)
    set(definitions ${definitions} PARENT_SCOPE)
    set(dependencies ${dependencies} PARENT_SCOPE)
endfunction()
