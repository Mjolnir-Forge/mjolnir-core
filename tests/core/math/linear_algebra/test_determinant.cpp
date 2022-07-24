#include "mjolnir/core/math/linear_algebra/determinant.h"
#include "mjolnir/core/x86/direct_access.h"
#include <gtest/gtest.h>

// ====================================================================================================================
// Setup
// ====================================================================================================================

using namespace mjolnir;
using namespace mjolnir::x86;


// --- test suite -----------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
class DeterminantTestSuite : public ::testing::Test
{
};

using DeterminantTestTypes = ::testing::Types<F32, F64, __m128, __m128d, __m256, __m256d>; // NOLINT

// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(DeterminantTestSuite, DeterminantTestTypes, );


// --- helper functions -----------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType, UST t_size, typename... T_Args>
[[nodiscard]] constexpr auto create_matrix_data(T_Args... args) -> std::array<T_RegisterType, t_size>
{
    constexpr UST n_values      = sizeof...(args);
    constexpr UST n_val_per_reg = n_values / t_size;

    static_assert(n_values % t_size == 0, "Values can't be distributed equally.");

    std::array<ElementType<T_RegisterType>, n_values> data = {{static_cast<ElementType<T_RegisterType>>(args)...}};

    std::array<T_RegisterType, t_size> a = {{{0}}};
    for (UST i = 0; i < t_size; ++i)
        for (UST j = 0; j < n_val_per_reg; j++)
            set(a.at(i), j, data.at(i * n_val_per_reg + j));

    return a;
}


template <Number T_Type, UST t_size, typename... T_Args>
[[nodiscard]] constexpr auto create_matrix_data(T_Args... args) -> std::array<T_Type, sizeof...(T_Args)>
{
    std::array<T_Type, sizeof...(T_Args)> mat = {{static_cast<T_Type>(args)...}};

    return mat;
}


// ====================================================================================================================
// Tests
// ====================================================================================================================


// --- test determinant_2x2 -------------------------------------------------------------------------------------------

TYPED_TEST(DeterminantTestSuite, determinant_2x2) // NOLINT
{
    auto a = create_matrix_data<TypeParam, 2>(1., 0., 0., 1.);
    EXPECT_DOUBLE_EQ(determinant_2x2(a), 1.);

    auto b = create_matrix_data<TypeParam, 2>(4., 2., -3., 5.); // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(b), 26.);

    auto c = create_matrix_data<TypeParam, 2>(-1., 2., 4., 3.); // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(c), -11.);

    auto d = create_matrix_data<TypeParam, 2>(4., 2., 6., 3.); // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(d), 0.);
}


// --- test determinant_3x3 -------------------------------------------------------------------------------------------

TYPED_TEST(DeterminantTestSuite, determinant_3x3) // NOLINT
{
    if constexpr (! is_m128d<TypeParam>)
    {
        auto a = create_matrix_data<TypeParam, 3>(1., 0., 0., 0., 1., 0., 0., 0., 1.);
        EXPECT_DOUBLE_EQ(determinant_3x3(a), 1.);

        a = create_matrix_data<TypeParam, 3>(1., 2., 3., 4., 5., 6., 7., 8., 9.); // NOLINT(readability-magic-numbers)
        EXPECT_DOUBLE_EQ(determinant_3x3(a), 0.);

        a = create_matrix_data<TypeParam, 3>(-2., 4., 1., 3., -5., 2., 5., 1., 3.); // NOLINT(readability-magic-numbers)
        EXPECT_DOUBLE_EQ(determinant_3x3(a), 66.);

        a = create_matrix_data<TypeParam, 3>(-2., 3., 5., 4., -5., 1., 1., 2., 3.); // NOLINT(readability-magic-numbers)
        EXPECT_DOUBLE_EQ(determinant_3x3(a), 66.);
    }
}


// --- test determinant_4x4 -------------------------------------------------------------------------------------------

TYPED_TEST(DeterminantTestSuite, determinant_4x4) // NOLINT
{
    if constexpr (! is_m128d<TypeParam> && ! is_m256d<TypeParam>)
    {
        auto a = create_matrix_data<TypeParam, 4>(1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1.);
        EXPECT_DOUBLE_EQ(determinant_4x4(a), 1.);

        // NOLINTNEXTLINE(readability-magic-numbers)
        a = create_matrix_data<TypeParam, 4>(1., 2., 3., 4., 5., 6., 7., 8., 9., 10., 11., 12., 13., 14., 15., 16.);
        EXPECT_DOUBLE_EQ(determinant_4x4(a), 0.);

        // NOLINTNEXTLINE(readability-magic-numbers)
        a = create_matrix_data<TypeParam, 4>(4., -2., 7., 3., 1., 3., -6., 2., 5., -1., 6., 5., 2., 4., 9., -2.);
        EXPECT_DOUBLE_EQ(determinant_4x4(a), -208.);

        // NOLINTNEXTLINE(readability-magic-numbers)
        a = create_matrix_data<TypeParam, 4>(4., 1., 5., 2., -2., 3., -1., 4., 7., -6., 6., 9., 3., 2., 5., -2.);
        EXPECT_DOUBLE_EQ(determinant_4x4(a), -208.);
    }
}
