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
