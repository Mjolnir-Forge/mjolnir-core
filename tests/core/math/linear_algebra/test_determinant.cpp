#include "mjolnir/core/math/linear_algebra/determinant.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/testing/x86/floating_point_vector_register_test_suite.h"
#include <gtest/gtest.h>

// ====================================================================================================================
// Setup
// ====================================================================================================================

using namespace mjolnir;
using namespace mjolnir::x86;


// --- test suite -----------------------------------------------------------------------------------------------------

template <mjolnir::Number T_RegisterType>
class FloatingPointTypeTestSuite : public ::testing::Test
{
};


using FloatingPointTestTypes = ::testing::Types<F32, F64>; // NOLINT

TYPED_TEST_SUITE(FloatingPointTypeTestSuite, FloatingPointTestTypes, );


// --- helper functions -----------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType, UST t_size, typename... T_Args>
[[nodiscard]] constexpr auto create_register_array(T_Args... args) -> std::array<T_RegisterType, t_size>
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


// ====================================================================================================================
// Tests
// ====================================================================================================================


// --- test determinant_2x2 (serial version) --------------------------------------------------------------------------

TYPED_TEST(FloatingPointTypeTestSuite, determinant_2x2) // NOLINT
{
    std::array<TypeParam, 4> a = {{1., 0., 0., 1.}};
    EXPECT_DOUBLE_EQ(determinant_2x2(a), 1.);

    std::array<TypeParam, 4> b = {{4., 2., -3., 5.}}; // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(b), 26.);

    std::array<TypeParam, 4> c = {{-1., 2., 4., 3.}}; // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(c), -11.);

    std::array<TypeParam, 4> d = {{4., 2., 6., 3.}}; // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(d), 0.);
}


// --- test determinant_2x2 (x86 version) --------------------------------------------------------------------------

TYPED_TEST(FloatingPointVectorRegisterTestSuite, determinant_2x2) // NOLINT
{
    auto a = create_register_array<TypeParam, 2>(1., 0., 0., 1.);
    EXPECT_DOUBLE_EQ(determinant_2x2(a), 1.);

    auto b = create_register_array<TypeParam, 2>(4., 2., -3., 5.); // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(b), 26.);

    auto c = create_register_array<TypeParam, 2>(-1., 2., 4., 3.); // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(c), -11.);

    auto d = create_register_array<TypeParam, 2>(4., 2., 6., 3.); // NOLINT(readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(d), 0.);
}
