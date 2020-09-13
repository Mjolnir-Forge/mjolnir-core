#[==[
Create an executable and add all sources, definitions and dependencies passed as a list of items.

PARAMETERS:
-----------

    target:
        Name of the CMake target that should be used

    root_directory:
        The root directory of the source files

    ARGN:
        Additional parameters can be a list of sources, definitions and dependencies
        - Every item containing ".cpp" is considered as a source
        - Every item containing "-D" is considered as a definition
        - All other items are considered as dependencies

#]==]
function(add_generic_executable target root_directory)

    extract_sources_definitions_dependencies("${ARGN}" ${root_directory})

    add_executable(${target} ${sources})

    target_compile_definitions(${target} PRIVATE ${definitions})

    target_link_libraries(${target} ${dependencies})

endfunction()
