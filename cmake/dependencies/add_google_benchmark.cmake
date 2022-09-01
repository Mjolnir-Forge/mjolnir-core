#[==[
Add google benchmark to the project.

The script tries to find a compatible system installation of google benchmark
first. If it can't be found, it is fetched from the GitHub repository and added
as external project. In this case the minimal required version will be used.
#]==]

if(MJOLNIR_CORE_ENABLE_BENCHMARKS)
    find_package(benchmark ${BENCHMARK_MINIMAL_VERSION} QUIET)
    if (NOT ${benchmark_FOUND})
        message(
            STATUS
                "Google benchmark ${BENCHMARK_MINIMAL_VERSION} (or higher)"
                "installation could not be located. It will be fetched from "
                "the GitHub repository")

        FetchContent_Declare(
            benchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG v${BENCHMARK_MINIMAL_VERSION})

        FetchContent_MakeAvailable(benchmark)

        FetchContent_GetProperties(benchmark)
        if(NOT benchmark_POPULATED)
            message(STATUS "Populating google benchmark...")

            FetchContent_Populate(benchmark FETCHCONTENT_QUIET ON)
            add_subdirectory(${benchmark_SOURCE_DIR} ${banchmark_BINARY_DIR})
            message(STATUS "Finished populating google benchmark")
        else()
            message(STATUS "Google benchmark already populated.")
        endif()
    else()
        message(STATUS "Google benchmark ${benchmark_VERSION} installation found.")
    endif()
endif()
