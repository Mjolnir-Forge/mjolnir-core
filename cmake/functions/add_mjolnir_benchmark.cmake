#[[
Create a benchmark for mjolnir libs and perform the setup using a keyword list.

A list of accepted keywords and their purpose is given below. Always use scope
keywords (PUBLIC, PRIVATE, INTERFACE) directly after one of these keywords,
otherwise the scope of the new items is undefined.


PARAMETERS:
-----------

    target:
        Name of the CMake target that should be used.

    ARGN:
        A list containing all necessary data of arbitrary length separated by
        keywords.


KEYWORDS
--------

    COMPILE_DEFINITIONS:
        Definitions that should be added

    COMPILE_FEATURES:
        Compile features that should be added (target_compile_features)

    COMPILE_OPTIONS:
        Compile options that should be added (target_compile_options)

    INCLUDE_DIRECTORIES:
        Additional directories that should be searched for the included header
        files (uses target_include_directories)

    LINK_DIRECTORIES:
        Additional directories that should be searched for the linked libraries
        (uses target_link_libraries)

    LINK_LIBRARIES:
        Libraries that should be linked

    LINK_OPTIONS:
        Linker options that should be added (target_link_options)

    PROPERTIES:
        List of properties (set_target_properties).

    SOURCES:
        List of source files. The filepath must be given in relation to the
        specified root directory

    SOURCE_DIRECTORY:
        The root directory of the source files. If none is specified, the
        current CMake source directory is taken.

]]
# silence [C0112]
function(add_mjolnir_benchmark target module)
    if(${MJOLNIR_${module}_ENABLE_TESTS})
        set(target "benchmark_${target}")

        string(REPLACE "${MJOLNIR_${module}_ROOT_DIR}/benchmarks/" ""
                       relative_path ${CMAKE_CURRENT_SOURCE_DIR})

        set(benchmark_source "/benchmarks/${relative_path}/${target}.cpp")

        add_to_list_after_keyword("${ARGN}" arguments LINK_LIBRARIES PRIVATE
                                  benchmark::benchmark)

        if(NOT "${MJOLNIR_${module}_COMPILE_DEFINITIONS}" STREQUAL "")
            add_to_list_after_keyword(
                "${arguments}" arguments COMPILE_DEFINITIONS PRIVATE
                ${MJOLNIR_${module}_COMPILE_DEFINITIONS})
        endif()

        add_to_list_after_keyword("${arguments}" arguments COMPILE_FEATURES
                                  PRIVATE ${MJOLNIR_${module}_COMPILE_FEATURES})

        add_to_list_after_keyword("${arguments}" arguments COMPILE_OPTIONS
                                  PRIVATE ${MJOLNIR_${module}_COMPILE_OPTIONS})

        add_to_list_after_keyword("${arguments}" arguments INCLUDE_DIRECTORIES
                                  ${MJOLNIR_${module}_SOURCE_DIR})

        if(NOT "${MJOLNIR_${module}_LINK_OPTIONS}" STREQUAL "")
            add_to_list_after_keyword("${arguments}" arguments LINK_OPTIONS
                                      PRIVATE ${MJOLNIR_${module}_LINK_OPTIONS})
        endif()

        add_to_list_after_keyword("${arguments}" arguments PROPERTIES
                                  ${MJOLNIR_${module}_TARGET_PROPERTIES})

        #if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        #    add_to_list_after_keyword("${arguments}" arguments LINK_OPTIONS
        #                              -lshlwapi)
        #endif()

        add_generic_executable(
            ${target} ${benchmark_source}
            SOURCE_DIRECTORY ${MJOLNIR_${module}_ROOT_DIR} ${arguments})

    endif()
endfunction()
