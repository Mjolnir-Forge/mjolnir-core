#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/is_close.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/core/x86/math.h"
#include <gtest/gtest.h>

#include <array>

using namespace mjolnir;
using namespace mjolnir::x86;

// ====================================================================================================================
// Setup
// ====================================================================================================================

// create test suites -------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
class TestFloatingPointVectorRegisterTypes : public ::testing::Test
{
};


using VectorRegisterTestTypes = ::testing::Types<__m128, __m128d, __m256, __m256d>; // NOLINT


// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(TestFloatingPointVectorRegisterTypes, VectorRegisterTestTypes, );


// ====================================================================================================================
// Tests
// ====================================================================================================================


// test abs -----------------------------------------------------------------------------------------------------------

TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_abs) // NOLINT
{
    constexpr UST n_e = num_elements<TypeParam>;
    using EType       = ElementType<TypeParam>;

    constexpr std::array<std::array<EType, 8>, 4> test_values = {{{{-1, 2, -3, -4, 5, 6, 7, 8}},
                                                                  {{3, -0, 6, 2, -1, 9, -3, -5}},
                                                                  {{-3, -1, -3, -5, -6, -2, -0, -1}},
                                                                  {{6, 2, 5, 5, 1, 0, 6, 3}}}};

    constexpr UST n_tests = test_values.size();

    for (UST i = 0; i < n_tests; ++i)
    {
        auto test_case_values = test_values.at(i);
        auto a                = mm_setzero<TypeParam>();

        for (UST j = 0; j < n_e; j++)
            set(a, j, test_case_values.at(j));

        auto b = abs(a);

        for (UST j = 0; j < n_e; j++)
            EXPECT_EQ(get(b, j), std::abs(test_case_values.at(j)));
    }
}


// test copy_sign -----------------------------------------------------------------------------------------------------

TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_copy_sign) // NOLINT
{
    constexpr UST n_e = num_elements<TypeParam>;
    using EType       = ElementType<TypeParam>;

    constexpr std::array<std::array<EType, 8>, 4> test_values = {{{{-1, 2, -3, -4, 5, 6, 7, 8}},
                                                                  {{3, -0, 6, 2, -1, 9, -3, -5}},
                                                                  {{-3, -1, -3, -5, -6, -2, -0, -1}},
                                                                  {{6, 2, 5, 5, 1, 0, 6, 3}}}};

    constexpr UST n_tests = test_values.size();

    for (UST i = 0; i < n_tests; ++i)
        for (UST j = 0; j < n_tests; ++j)
        {
            auto values_mag  = test_values.at(i);
            auto values_sign = test_values.at(j);

            auto mag     = mm_setzero<TypeParam>();
            auto mag_abs = mm_setzero<TypeParam>();
            auto sign    = mm_setzero<TypeParam>();

            for (UST k = 0; k < n_e; k++)
            {
                set(mag, k, values_mag.at(k));
                set(mag_abs, k, std::abs(values_mag.at(k)));
                set(sign, k, values_sign.at(k));
            }

            auto res     = copy_sign(mag, sign);
            auto res_abs = copy_sign<true>(mag_abs, sign);

            for (UST k = 0; k < n_e; k++)
            {
                auto exp = std::copysign(values_mag.at(k), values_sign.at(k));
                EXPECT_EQ(get(res, k), exp);
                EXPECT_EQ(get(res_abs, k), exp);
            }
        }
}
