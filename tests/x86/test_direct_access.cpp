#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/core/x86/x86.h"
#include <gtest/gtest.h>


using namespace mjolnir;
using namespace mjolnir::x86;

// ====================================================================================================================
// Setup
// ====================================================================================================================

// create test suites -------------------------------------------------------------------------------------------------

template <typename T_Type>
class GetTemplateTester : public ::testing::Test
{
};


using get_template_testcases = ::testing::Types<__m128, __m128d, __m256, __m256d>; // NOLINT


// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(GetTemplateTester, get_template_testcases, );


// ====================================================================================================================
// Tests
// ====================================================================================================================


// --------------------------------------------------------------------------------------------------------------------

TYPED_TEST(GetTemplateTester, template) // NOLINT
{
    TypeParam a;
    if constexpr (is_m128d<TypeParam>)
        a = mm_setr<TypeParam>(0.0, 1.0); // NOLINT
    else if constexpr (is_m128<TypeParam>)
        a = mm_setr<TypeParam>(0.F, 1.F, 2.F, 3.F); // NOLINT
    else if constexpr (is_m256d<TypeParam>)
        a = mm_setr<TypeParam>(0.0, 1.0, 2.0, 3.0); // NOLINT
    else
        a = mm_setr<TypeParam>(0.F, 1.F, 2.F, 3.F, 4.F, 5.F, 6.F, 7.F); // NOLINT


    EXPECT_FLOAT_EQ(get<0>(a), 0.F);
    EXPECT_FLOAT_EQ(get<1>(a), 1.F);
    if constexpr (not is_m128d<TypeParam>)
    {
        EXPECT_FLOAT_EQ(get<2>(a), 2.F);
        EXPECT_FLOAT_EQ(get<3>(a), 3.F);
        if constexpr (is_m256<TypeParam>)
        {
            EXPECT_FLOAT_EQ(get<4>(a), 4.F);
            EXPECT_FLOAT_EQ(get<5>(a), 5.F);
            EXPECT_FLOAT_EQ(get<6>(a), 6.F);
            EXPECT_FLOAT_EQ(get<7>(a), 7.F);
        }
    }
}
