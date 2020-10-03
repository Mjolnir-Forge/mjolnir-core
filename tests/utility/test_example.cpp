#include <gtest/gtest.h>


int add(int lhs, int rhs)
{
    return lhs + rhs;
}

TEST(test, add) // NOLINT cert-err58-cpp
{
    EXPECT_EQ(add(1, 2), 3);
}
