#[==[
Add googletest to the project.

The script tries to find a system installation of googletest first. If it can't be found, googletest is pulled from the
GitHub repository and added as external project.
#]==]


if(MJOLNIR_CORE_ENABLE_TESTS)
    enable_testing()
    find_package(GTest 1.10.0 QUIET)

    if(NOT ${GTEST_FOUND})
        message(STATUS "GTest installation could not be located. It will be fetched from the GitHub repository")

        FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-1.10.0
            )

        FetchContent_MakeAvailable(googletest)

        FetchContent_GetProperties(googletest)
        if(NOT googletest_POPULATED)
            message(STATUS "Populating GTest...")

            FetchContent_Populate(googletest
                FETCHCONTENT_QUIET ON
                )
            add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
            message(STATUS "Finished populating GTest")
        else()
            message(STATUS "GTest already populated.")
        endif()
    else()
        message(STATUS "GTest installation found.")
    endif()
endif()
