#include "mjolnir/core/test_header.h"
#include "mjolnir/core/test_header_2.h"
#include <gtest/gtest.h>


auto add(int lhs, int rhs) -> int
{
    return lhs + rhs;
}


TEST(test, add) // NOLINT cert-err58-cpp
{
    // char a[10];
    // a[10] = 0;
    // int k = 0x7fffffff;
    // k += 2000;
    // AnotherClass::cpp_check_error();

    // address sanitizer
    int* array = new int[100];
    delete[] array;

    EXPECT_EQ(add(array[2], 2), 3);

    EXPECT_EQ(add(1, 2), 3);
}
