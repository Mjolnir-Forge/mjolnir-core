#include <gtest/gtest.h>


int add(int lhs, int rhs)
{
    int unused;
    return lhs + rhs;
}

TEST(test, add)
{
    EXPECT_EQ(add(1, 2), 3);
}
