#include "mjolnir/core/utility/parameter_pack.h"
#include <gtest/gtest.h>


using namespace mjolnir;

// ====================================================================================================================
// Setup
// ====================================================================================================================

// ====================================================================================================================
// Tests
// ====================================================================================================================


// test pp_all_less ---------------------------------------------------------------------------------------------------

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


// test pp_all_false --------------------------------------------------------------------------------------------------

TEST(ParameterPackTester, test_pp_all_false) // NOLINT
{
    EXPECT_TRUE((pp_all_false<false>()));
    EXPECT_TRUE((pp_all_false<false, false>()));
    EXPECT_TRUE((pp_all_false<false, false, false>()));
    EXPECT_TRUE((pp_all_false<false, false, false, false>()));

    EXPECT_FALSE((pp_all_false<true>()));

    EXPECT_FALSE((pp_all_false<true, true>()));
    EXPECT_FALSE((pp_all_false<true, false>()));
    EXPECT_FALSE((pp_all_false<false, true>()));

    EXPECT_FALSE((pp_all_false<true, true, true>()));
    EXPECT_FALSE((pp_all_false<true, true, false>()));
    EXPECT_FALSE((pp_all_false<true, false, true>()));
    EXPECT_FALSE((pp_all_false<false, true, true>()));
    EXPECT_FALSE((pp_all_false<true, false, false>()));
    EXPECT_FALSE((pp_all_false<false, true, false>()));
    EXPECT_FALSE((pp_all_false<false, false, true>()));


    EXPECT_FALSE((pp_all_false<true, false, false, true>()));
}


// test pp_all_true ---------------------------------------------------------------------------------------------------

TEST(ParameterPackTester, test_pp_all_true) // NOLINT
{
    EXPECT_TRUE((pp_all_true<true>()));
    EXPECT_TRUE((pp_all_true<true, true>()));
    EXPECT_TRUE((pp_all_true<true, true, true>()));
    EXPECT_TRUE((pp_all_true<true, true, true, true>()));

    EXPECT_FALSE((pp_all_true<false>()));

    EXPECT_FALSE((pp_all_true<true, false>()));
    EXPECT_FALSE((pp_all_true<false, false>()));
    EXPECT_FALSE((pp_all_true<false, true>()));

    EXPECT_FALSE((pp_all_true<true, true, false>()));
    EXPECT_FALSE((pp_all_true<true, false, true>()));
    EXPECT_FALSE((pp_all_true<false, true, true>()));
    EXPECT_FALSE((pp_all_true<true, false, false>()));
    EXPECT_FALSE((pp_all_true<false, true, false>()));
    EXPECT_FALSE((pp_all_true<false, false, true>()));
    EXPECT_FALSE((pp_all_true<false, false, false>()));


    EXPECT_FALSE((pp_all_true<true, false, false, true>()));
}
