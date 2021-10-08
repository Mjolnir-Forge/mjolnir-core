#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/is_close.h"
#include <gtest/gtest.h>

#include <array>


using namespace mjolnir;

// ====================================================================================================================
// Setup
// ====================================================================================================================

// create test suites -------------------------------------------------------------------------------------------------

template <typename T_Type>
class IsCloseTemplateTester : public ::testing::Test
{
};


using is_close_template_testcases = ::testing::Types<I32, F32, F64>; // NOLINT


// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(IsCloseTemplateTester, is_close_template_testcases, );


// ====================================================================================================================
// Tests
// ====================================================================================================================

TYPED_TEST(IsCloseTemplateTester, is_close_abs) // NOLINT
{
    const TypeParam a              = static_cast<TypeParam>(20.56);
    const TypeParam tolerance      = static_cast<TypeParam>(5.1);
    const U32       tolerance_uint = static_cast<U32>(tolerance);

    for (U32 i = 0; i <= tolerance_uint; ++i)
    {
        TypeParam b = a + static_cast<TypeParam>(i);
        EXPECT_TRUE(is_close_abs<TypeParam>(a, b, tolerance));

        TypeParam c = a - static_cast<TypeParam>(i);
        EXPECT_TRUE(is_close_abs<TypeParam>(a, c, tolerance));
    }

    for (U32 i = tolerance_uint + 1; i <= tolerance_uint * 2; ++i)
    {
        TypeParam b = a + static_cast<TypeParam>(i);
        EXPECT_FALSE(is_close_abs<TypeParam>(a, b, tolerance));

        TypeParam c = a - static_cast<TypeParam>(i);
        EXPECT_FALSE(is_close_abs<TypeParam>(a, c, tolerance));
    }
}
