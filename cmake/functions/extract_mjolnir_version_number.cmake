#[==[
Extract the version number of a mjolnir module from its c++ header.

The returned format is MAJOR.MINOR.PATCH.TWEAK. The tweak version number is omitted
if it is 0.

PARAMETERS:
-----------

    version_header_path:
        Path to the version header

    return_variable_name:
        Name of the returned variable

RETURNS
-------

    ${return_variable_name}:
        Version number

#]==]
function(
    extract_mjolnir_version_number
    version_header_path
    module
    return_variable_name)
    set(VERSION_REGEX
        "#define MJOLNIR_${module}_VERSION_(MAJOR|MINOR|PATCH|TWEAK)")

    file(STRINGS ${version_header_path} version_data REGEX ${VERSION_REGEX})

    string(
        REGEX MATCH
              "VERSION_MAJOR ([0-9]*)"
              _
              ${version_data})
    set(version_major ${CMAKE_MATCH_1})

    string(
        REGEX MATCH
              "VERSION_MINOR ([0-9]*)"
              _
              ${version_data})
    set(version_minor ${CMAKE_MATCH_1})

    string(
        REGEX MATCH
              "VERSION_PATCH ([0-9]*)"
              _
              ${version_data})
    set(version_patch ${CMAKE_MATCH_1})

    string(
        REGEX MATCH
              "VERSION_TWEAK ([0-9]*)"
              _
              ${version_data})
    set(version_tweak ${CMAKE_MATCH_1})

    set(${return_variable_name}
        "${version_major}.${version_minor}.${version_patch}")
    if(NOT
       ${version_tweak}
       EQUAL
       0)
        set(${return_variable_name} "${version_number}.${version_tweak}")
    endif()

    set(${return_variable_name}
        ${version_number}
        PARENT_SCOPE)
endfunction()
