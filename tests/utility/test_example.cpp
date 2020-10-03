#include <gtest/gtest.h>


const int global_constant = 0;                // GlobalConstant
int* const global_constant_pointer = nullptr; // GlobalConstantPointer
int* global_pointer = nullptr;                // GlobalPointer
int global_variable = 0;                      // GlobalVariable

enum AnEnum // Enum
{
    ONE,  // EnumConstantCase
    TWO,  // EnumConstantCase
    THREE // EnumConstantCase
};

class AnAbstractClass // AbstractClass
{
    virtual int private_method() = 0; // PrivateMethod + Method
};

class RegularClass // ClassCase
{
    static const int class_constant = 0; // ClassConstant
    int m_private_member = 0;            // PrivateMember + Member
    int private_method()                 // PrivateMethod + Method
    {
        return 0;
    }

protected:
    int m_protected_member = 0; // ProtectedMember + Member
    int protected_method()      // ProtectedMethod + Method
    {
        return 0;
    }

public:
    int public_member = 42; // PublicMember + Member

    const int constant_member = 0; // ConstantMember
    static int class_member;       // ClassMember

    int public_method(int* const constant_pointer_parameter) // PublicMember + Method --- ConstantPointerParameter
    {
        constexpr int constant_expression_variable = 0; // ConstexprVariable

        return m_private_member + constant_member + *constant_pointer_parameter + constant_expression_variable
               + global_constant;
    }


    static bool class_method(const bool constant_parameter) // ClassMethod --- ConstantParameter
    {
        return constant_parameter;
    }

    constexpr int constant_expression_method(int method_parameter,   // ConstexprMethod --- Parameter
                                             int* pointer_parameter) // PointerParameter
    {
        const int local_constant = 0;                         // LocalConstant + Constant
        int* const local_constant_pointer = &global_variable; // LocalConstantPointer
        int* local_pointer = pointer_parameter;               // LocalPointer
        int local_variable = 2;                               // LocalVariable

        return method_parameter + local_constant + *local_constant_pointer + *local_pointer + local_variable;
    }
};


constexpr int constant_expression_function(int function_parameter) // ConstexprFunction --- Parameter
{
    return function_parameter + 4;
}


int* global_function() // Function + GlobalFunction
{
    return global_constant_pointer;
}


template <typename... TYPE_parameters>
bool template_function([[maybe_unused]] TYPE_parameters... parameter_pack) // Function --- ParameterPack
{
    return true;
};


namespace some_namespace // Namespace
{
inline namespace some_inline_namespace // InlineNamespace
{
}
} // namespace some_namespace


int add(int lhs, int rhs)
{
    return lhs + rhs;
}

TEST(test, add)
{
    EXPECT_EQ(add(1, 2), 3);
}
