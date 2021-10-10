#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/x86.h"
#include <gtest/gtest.h>

#include <array>

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

TYPED_TEST(GetTemplateTester, test_get_template_version) // NOLINT
{
    VectorDataArray<TypeParam> data = {{0}};

    for (UST i = 0; i < data.size(); ++i)
        data.at(i) = static_cast<ElementType<TypeParam>>(i);
    auto a = mm_load<TypeParam>(data.data());


    EXPECT_DOUBLE_EQ(get<0>(a), 0.);
    EXPECT_DOUBLE_EQ(get<1>(a), 1.);
    if constexpr (not is_m128d<TypeParam>)
    {
        EXPECT_DOUBLE_EQ(get<2>(a), 2.);
        EXPECT_DOUBLE_EQ(get<3>(a), 3.);
        if constexpr (is_m256<TypeParam>)
        {
            EXPECT_DOUBLE_EQ(get<4>(a), 4.);
            EXPECT_DOUBLE_EQ(get<5>(a), 5.);
            EXPECT_DOUBLE_EQ(get<6>(a), 6.);
            EXPECT_DOUBLE_EQ(get<7>(a), 7.);
        }
    }
}
