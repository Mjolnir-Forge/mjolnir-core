#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/abs.h"
#include <gtest/gtest.h>

using namespace mjolnir;


// ====================================================================================================================
// Setup
// ====================================================================================================================

// create test suites -------------------------------------------------------------------------------------------------

class AbsTester : public ::testing::TestWithParam<UST>
{
};

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(abs, AbsTester, ::testing::Values(0, 1, 2, 3, 4, 5, 6, 7));


// ====================================================================================================================
// Tests
// ====================================================================================================================


// test_is_aligned ----------------------------------------------------------------------------------------------------

TEST_P(AbsTester, test_abs) // NOLINT cert-err58-cpp
{
    const UST pointer_offset = GetParam();

    __m128 a = _mm_set1_ps(pointer_offset);
    __m128 b = abs(a);
    std::cout << alignment_bytes<__m128> << std::endl;
}
