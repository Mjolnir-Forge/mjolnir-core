#include <gtest/gtest.h>

enum AnEnum // Enum
{
    ONE,  // EnumConstantCase
    TWO,  // EnumConstantCase
    THREE // EnumConstantCase
};

class AnAbstractClass // AbstractClass
{
    virtual int a_method() = 0;
};

class RegularClass // ClassCase
{
    static const int class_constant = 0; // ClassConstant
    int m_haha = 0;

public:
    const int constant_member = 0; // ConstantMember
    static int ne_ne;              // ClassMember

    int get_haha(int* const constant_pointer_parameter) // ConstantPointerParameter
    {
        constexpr int constant_expression_variable = 0; // ConstexprVariable
        return m_haha + constant_member + *constant_pointer_parameter + constant_expression_variable;
    }


    static bool class_method(const bool constant_parameter) // ClassMethod + ConstantParameter
    {
        const bool return_me = constant_parameter; // Constant
        return return_me;
    }

    constexpr int constant_expression_method(int a) // ConstexprMethod
    {
        return a + 4;
    }
};


constexpr int constant_expression_function(int a) // ConstexprFunction
{
    return a + 4;
}

int global_function()
{
    return 5;
}


int add(int lhs, int rhs)
{
    RegularClass test_class;
    return lhs + rhs + test_class.constant_member;
}

TEST(test, add)
{
    EXPECT_EQ(add(1, 2), 3);
}
