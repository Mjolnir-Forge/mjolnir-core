#include "mjolnir/core/math/linear_algebra/vector_products.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/testing/x86/floating_point_vector_register_test_suite.h"
#include <gtest/gtest.h>

// ====================================================================================================================
// Setup
// ====================================================================================================================

using namespace mjolnir;
using namespace mjolnir::x86;


// --- test suite -----------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
class VectorProductTestSuite : public ::testing::Test
{
};


using VectorProductTestTypes = ::testing::Types<F32, F64, __m128, __m128d, __m256, __m256d>; // NOLINT

TYPED_TEST_SUITE(VectorProductTestSuite, VectorProductTestTypes, );


// --- helper functions -----------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType, Number... T_Args>
[[nodiscard]] auto set_vector(T_Args... args) -> T_RegisterType
{
    using EType        = ElementType<T_RegisterType>;
    constexpr UST size = sizeof...(args);

    std::array<EType, size> data = {{static_cast<EType>(args)...}};

    auto reg = mm_setzero<T_RegisterType>();
    for (UST i = 0; i < size; ++i)
        set(reg, i, data.at(i));

    return reg;
}


template <Number T_Type, Number... T_Args>
[[nodiscard]] auto set_vector(T_Args... args) -> std::array<T_Type, sizeof...(T_Args)>
{
    std::array<T_Type, sizeof...(T_Args)> data = {{static_cast<T_Type>(args)...}};
    return data;
}


// ====================================================================================================================
// Tests
// ====================================================================================================================


// --- test dot_product with vector size 2 ----------------------------------------------------------------------------

TYPED_TEST(VectorProductTestSuite, dot_product_vec_2) // NOLINT
{
    auto a = set_vector<TypeParam>(0., 0.);
    auto b = set_vector<TypeParam>(2., 5.); // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(dot_product<2>(a, b), 0.);


    a = set_vector<TypeParam>(2., 3.); // NOLINT(readability-magic-numbers)
    b = set_vector<TypeParam>(4., 4.); // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(dot_product<2>(a, b), 20.);


    a = set_vector<TypeParam>(-4., 3.); // NOLINT(readability-magic-numbers)
    b = set_vector<TypeParam>(6., 2.);  // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(dot_product<2>(a, b), -18.);
}


// --- test dot_product with vector size 3 ----------------------------------------------------------------------------

TYPED_TEST(VectorProductTestSuite, dot_product_vec_3) // NOLINT
{
    if constexpr (! is_m128d<TypeParam>)
    {
        auto a = set_vector<TypeParam>(0., 0., 0.);
        auto b = set_vector<TypeParam>(2., 5., -2.); // NOLINT(readability-magic-numbers)
        EXPECT_DOUBLE_EQ(dot_product<3>(a, b), 0.);


        a = set_vector<TypeParam>(2., 3., 1.); // NOLINT(readability-magic-numbers)
        b = set_vector<TypeParam>(4., 4., 2.); // NOLINT(readability-magic-numbers)
        EXPECT_DOUBLE_EQ(dot_product<3>(a, b), 22.);


        a = set_vector<TypeParam>(-4., 3., 4.); // NOLINT(readability-magic-numbers)
        b = set_vector<TypeParam>(6., 2., -3.); // NOLINT(readability-magic-numbers)
        EXPECT_DOUBLE_EQ(dot_product<3>(a, b), -30.);
    }
}


// --- test dot_product with vector size 4 ----------------------------------------------------------------------------

TYPED_TEST(VectorProductTestSuite, dot_product_vec_4) // NOLINT
{
    if constexpr (! is_m128d<TypeParam>)
    {
        auto a = set_vector<TypeParam>(0., 0., 0., 0.);
        auto b = set_vector<TypeParam>(2., 5., -2., 7.); // NOLINT(readability-magic-numbers)
        EXPECT_DOUBLE_EQ(dot_product<4>(a, b), 0.);


        a = set_vector<TypeParam>(2., 3., 1., 5.); // NOLINT(readability-magic-numbers)
        b = set_vector<TypeParam>(4., 4., 2., 2.); // NOLINT(readability-magic-numbers)
        EXPECT_DOUBLE_EQ(dot_product<4>(a, b), 32.);


        a = set_vector<TypeParam>(-4., 3., 4., -5.); // NOLINT(readability-magic-numbers)
        b = set_vector<TypeParam>(6., 2., -3., -2.); // NOLINT(readability-magic-numbers)
        EXPECT_DOUBLE_EQ(dot_product<4>(a, b), -20.);
    }
}
