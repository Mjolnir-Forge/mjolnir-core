#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/type.h"
#include <gtest/gtest.h>


using namespace mjolnir;


// ====================================================================================================================
// Setup
// ====================================================================================================================


// --- Test suites ----------------------------------------------------------------------------------------------------

template <class T_Type>
class IsAnyOfTestSuite : public ::testing::Test
{
};
using IsAnyOfTestTypes = ::testing::Types<I8, I32, I64, U32, U64, UST, F32, F64>;
// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(IsAnyOfTestSuite, IsAnyOfTestTypes, ); // NOLINT


template <std::signed_integral T_Type>
class SignedTestSuite : public ::testing::Test
{
};
using SignedTestTypes = ::testing::Types<I8, I16, I32, I64, IPT, int>; // NOLINT
TYPED_TEST_SUITE(SignedTestSuite, SignedTestTypes, );


template <std::unsigned_integral T_Type>
class UnsignedTestSuite : public ::testing::Test
{
};
using UnsignedTestTypes = ::testing::Types<U8, U16, U32, U64, UPT, unsigned int>; // NOLINT
TYPED_TEST_SUITE(UnsignedTestSuite, UnsignedTestTypes, );


// ====================================================================================================================
// Tests
// ====================================================================================================================


// test_is_any_of -----------------------------------------------------------------------------------------------------

TYPED_TEST(IsAnyOfTestSuite, testcase) // NOLINT
{
    constexpr bool result     = mjolnir::is_any_of<TypeParam, I32, UST, F64>();
    constexpr bool exp_result = std::is_same<TypeParam, I32>::value || std::is_same<TypeParam, UST>::value
                                || std::is_same<TypeParam, F64>::value;
    EXPECT_EQ(result, exp_result);
}


// test signed_to_unsigned --------------------------------------------------------------------------------------------

TYPED_TEST(SignedTestSuite, is_unsigned_with_correct_size_and_value) // NOLINT
{
    TypeParam i = 2;
    auto      u = signed_to_unsigned(i);

    EXPECT_TRUE(sizeof(i) == sizeof(u));
    EXPECT_TRUE(std::is_unsigned_v<decltype(u)>);
    EXPECT_EQ(static_cast<TypeParam>(u), i);
}


TYPED_TEST(UnsignedTestSuite, is_type_and_value_identical) // NOLINT
{
    TypeParam u1 = 2;
    auto      u2 = signed_to_unsigned(u1);

    EXPECT_TRUE((std::is_same_v<decltype(u2), TypeParam>) );
    EXPECT_EQ(u2, u1);
}
