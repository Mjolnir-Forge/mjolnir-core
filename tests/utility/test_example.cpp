#include <gtest/gtest.h>


class TestClass
{
    static const int class_constat = 0;
    int m_haha = 0;


public:
    int get_haha()
    {
        return m_haha;
    }


    static bool is_it_correct()
    {
        return false;
    }

    static int ne_ne;
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
