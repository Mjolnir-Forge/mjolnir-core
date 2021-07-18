#include "mjolnir/core/test_header.h"
#include "mjolnir/core/test_header_2.h"
#include <gtest/gtest.h>

#include <iostream>
#include <mutex>
#include <thread>


auto test(int z) -> int
{
    // if (z == 0)
    //    return 1 / z; // warn
    return z;
}


auto add(int lhs, int rhs) -> int
{
    return lhs + rhs;
}

int        global_var = 0;
std::mutex global_mutex;


auto print() -> void
{
    const std::lock_guard<std::mutex> lock(global_mutex);
    global_var += 2;
    std::cout << global_var << std::endl;
}


TEST(test, add) // NOLINT cert-err58-cpp
{
    auto a = std::thread(print);
    auto b = std::thread(print);
    int* e = new int;

    // char a[10];
    // a[10] = 0;auto a = std::thread(print);
    // int k = 0x7fffffff;
    // k += 2000;
    // AnotherClass::cpp_check_error();

    // address sanitizer
    // int* array = new int[100];
    // delete[] array;
    // EXPECT_EQ(add(array[2], 2), 3);

    // leak sanitizer
    // void* p = malloc(7);
    // p       = 0;

    test(0);

    a.join();
    b.join();
    EXPECT_EQ(add(1, 2), 3);
}
