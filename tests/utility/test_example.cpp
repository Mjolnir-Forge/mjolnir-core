#include <gtest/gtest.h>


auto add(int lhs, int rhs) -> int
{
    return lhs + rhs;
}

TEST(test, add) // NOLINT cert-err58-cpp
{
    EXPECT_EQ(add(1, 2), 3);
}
