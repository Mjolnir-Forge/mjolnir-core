#include <gtest/gtest.h>


class TestClass
{
    int m_haha = 0;

public:
    int get_haha()
    {
        return m_haha;
    }
};

int add(int lhs, int rhs)
{
    TestClass test_class;
    return lhs + rhs + test_class.get_haha();
}

TEST(test, add)
{
    EXPECT_EQ(add(1, 2), 3);
}
