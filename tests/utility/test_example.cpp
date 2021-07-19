#include "mjolnir/core/test_header.h"
#include "mjolnir/core/test_header_2.h"
#include <gtest/gtest.h>

#include <iostream>
#include <mutex>
#include <thread>


auto test(int z) -> int
{
    // triggers clang-static-analyzer
    // if (z == 0)
    //    return 1 / z; // warn
    return z;
}


auto add(int lhs, int rhs) -> int
{
    return lhs + rhs;
}

int        global_var = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
std::mutex global_mutex;   // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)


auto print() -> void
{
    const std::lock_guard<std::mutex> lock(global_mutex);
    global_var += 2;
    std::cout << global_var << std::endl;
}

