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
class TypedTest : public ::testing::Test
{
};


using VectorRegisterTestTypes = ::testing::Types<__m128, __m128d, __m256, __m256d>; // NOLINT


// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(TypedTest, VectorRegisterTestTypes, );


// ====================================================================================================================
// Tests
// ====================================================================================================================

// --------------------------------------------------------------------------------------------------------------------


TYPED_TEST(TypedTest, test_get_dynamic_version) // NOLINT
{
    VectorDataArray<TypeParam> data = {{{0}}};

    for (UST i = 0; i < data.size(); ++i)
        data.at(i) = static_cast<ElementType<TypeParam>>(i);
    auto a = mm_load<TypeParam>(data.data());

    for (UST i = 0; i < num_elements<TypeParam>; ++i)
        EXPECT_DOUBLE_EQ(get(a, i), static_cast<F64>(i));
}


// --------------------------------------------------------------------------------------------------------------------

TYPED_TEST(TypedTest, test_get_static_version) // NOLINT
{
    VectorDataArray<TypeParam> data = {{{0}}};

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


// --------------------------------------------------------------------------------------------------------------------

TYPED_TEST(TypedTest, test_set_dynamic_version) // NOLINT
{
    auto a = mm_set1<TypeParam>(-1.);

    for (UST i = 0; i < num_elements<TypeParam>; ++i)
        set(a, i, static_cast<ElementType<TypeParam>>(i));

    VectorDataArray<TypeParam> data = {{{0}}};
    mm_store(data.data(), a);

    for (UST i = 0; i < data.size(); ++i)
        EXPECT_DOUBLE_EQ(data.at(i), static_cast<F64>(i));
}


// --------------------------------------------------------------------------------------------------------------------

TYPED_TEST(TypedTest, test_set_static_version) // NOLINT
{
    auto a = mm_set1<TypeParam>(-1.);

    set<0>(a, 0.); // NOLINT
    set<1>(a, 1.); // NOLINT

    if constexpr (not is_m128d<TypeParam>)
    {
        set<2>(a, 2.); // NOLINT
        set<3>(a, 3.); // NOLINT
        if constexpr (is_m256<TypeParam>)
        {
            set<4>(a, 4.); // NOLINT
            set<5>(a, 5.); // NOLINT
            set<6>(a, 6.); // NOLINT
            set<7>(a, 7.); // NOLINT
        }
    }

    VectorDataArray<TypeParam> data = {{{0}}}; // NOLINT(readability-misleading-indentation)
    mm_store(data.data(), a);

    for (UST i = 0; i < data.size(); ++i)
        EXPECT_DOUBLE_EQ(data.at(i), static_cast<F64>(i));
}
