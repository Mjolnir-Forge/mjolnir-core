#include "mjolnir/core/utility/parameter_pack.h"
#include <gtest/gtest.h>


using namespace mjolnir;

// ====================================================================================================================
// Setup
// ====================================================================================================================

// ====================================================================================================================
// Tests
// ====================================================================================================================


TEST(ParameterPackTester, test_pp_all_less) // NOLINT
{
    EXPECT_TRUE((pp_all_less<1>(5)));             // NOLINT - magic number
    EXPECT_TRUE((pp_all_less<1, 4, 3>(5)));       // NOLINT - magic number
    EXPECT_TRUE((pp_all_less<1, 4, 3, 2, 0>(5))); // NOLINT - magic number
    EXPECT_TRUE((pp_all_less<1, 4, 3, 2, 0>(7))); // NOLINT - magic number

    EXPECT_FALSE((pp_all_less<4>(1)));
    EXPECT_FALSE((pp_all_less<1, 4, 3>(3)));
    EXPECT_FALSE((pp_all_less<1, 4, 3, 2, 0>(4)));
    EXPECT_FALSE((pp_all_less<1, 4, 3, 2, 0>(2)));
}
