#[==[
Add googletest to the project.

The script tries to find a compatible system installation of googletest first. If it can't be found, googletest is
fetched from the GitHub repository and added as external project. In this case the minimal required version will be
used.
#]==]


if(MJOLNIR_CORE_ENABLE_TESTS)
    enable_testing()
    find_package(GTest ${GTEST_MINIMAL_VERSION} QUIET)

    if(NOT ${GTEST_FOUND})
        message(STATUS
            "GTest ${GTEST_MINIMAL_VERSION} (or higher) installation could not be located. "
            "It will be fetched from the GitHub repository"
            )

        FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-${GTEST_MINIMAL_VERSION}
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
        message(STATUS "GTest ${GTEST_VERSION} installation found.")
    endif()
endif()
