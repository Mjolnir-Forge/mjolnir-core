#[==[
Add googletest to the project.

The script tries to find a system installation of googletest first. If it can't be found, googletest is pulled from the
GitHub repository and added as external project.
#]==]


if(MJOLNIR_CORE_ENABLE_TESTS)
    enable_testing()
    find_package(GTest)

    if(NOT ${GTEST_FOUND})
        FetchContent_Declare(googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG        release-1.10.0
            )

        FetchContent_MakeAvailable(googletest)

        FetchContent_GetProperties(googletest)
        if(NOT googletest_POPULATED)
            FetchContent_Populate(googletest
                FETCHCONTENT_QUIET ON
                )
            add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
        endif()
    endif()
endif()
