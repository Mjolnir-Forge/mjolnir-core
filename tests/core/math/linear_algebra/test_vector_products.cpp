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


template <UST t_size, Number T_Type>
void compare_vectors_equal(const std::array<T_Type, t_size>& lhs, const std::array<T_Type, t_size>& rhs) noexcept
{
    for (UST i = 0; i < t_size; ++i)
        EXPECT_DOUBLE_EQ(lhs.at(i), rhs.at(i));
}


template <UST t_size, FloatVectorRegister T_RegisterType>
void compare_vectors_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept
{
    for (UST i = 0; i < t_size; ++i)
        EXPECT_DOUBLE_EQ(get(lhs, i), get(rhs, i));
}


// ====================================================================================================================
// Tests
// ====================================================================================================================


// --- test cross_product  --------------------------------------------------------------------------------------------

TYPED_TEST(VectorProductTestSuite, cross_product) // NOLINT
{
    if constexpr (! is_m128d<TypeParam>)
    {
        auto a   = set_vector<TypeParam>(1., 0., 0.);
        auto b   = set_vector<TypeParam>(0., 1., 0.); // NOLINT(readability-magic-numbers)
        auto exp = set_vector<TypeParam>(0., 0., 1.);

        compare_vectors_equal<3>(cross_product(a, b), exp);


        a   = set_vector<TypeParam>(3., 2., 1.);  // NOLINT(readability-magic-numbers)
        b   = set_vector<TypeParam>(1., 2., 3.);  // NOLINT(readability-magic-numbers)
        exp = set_vector<TypeParam>(4., -8., 4.); // NOLINT(readability-magic-numbers)

        compare_vectors_equal<3>(cross_product(a, b), exp);


        a   = set_vector<TypeParam>(-2., 4., 3.);  // NOLINT(readability-magic-numbers)
        b   = set_vector<TypeParam>(-1., -3., 2.); // NOLINT(readability-magic-numbers)
        exp = set_vector<TypeParam>(17., 1., 10.); // NOLINT(readability-magic-numbers)

        compare_vectors_equal<3>(cross_product(a, b), exp);
    }
}


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
