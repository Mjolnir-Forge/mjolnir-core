#include "mjolnir/core/math/math.h"
#include <gtest/gtest.h>

// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================


// --- test gauss_summation -------------------------------------------------------------------------------------------

TEST(test_math, gauss_summation) // NOLINT
{
    // unsigned version
    EXPECT_EQ(gauss_summation<U32>(2U), 3U);
    EXPECT_EQ(gauss_summation<U32>(10U), 55U);
    EXPECT_EQ(gauss_summation<U32>(100U), 5050U);


    // signed version
    EXPECT_EQ(gauss_summation<I32>(2), 3);
    EXPECT_EQ(gauss_summation<I32>(10), 55);
    EXPECT_EQ(gauss_summation<I32>(100), 5050);
}


// --- test power -----------------------------------------------------------------------------------------------------

TEST(test_math, power) // NOLINT
{
    // test with integers
    EXPECT_EQ(power(2, 2), 4);
    EXPECT_EQ(power(4, 2), 16);
    EXPECT_EQ(power(5, 2), 25);

    EXPECT_EQ(power(2, 8), 256);
    EXPECT_EQ(power(2, 10), 1024);
    EXPECT_EQ(power(3, 3), 27);

    // test with floats
    EXPECT_DOUBLE_EQ(power(2., 2), 4.);
    EXPECT_DOUBLE_EQ(power(4., 2), 16.);
    EXPECT_DOUBLE_EQ(power(2.5, 2), 6.25);

    EXPECT_DOUBLE_EQ(power(2., 8), 256.);
    EXPECT_DOUBLE_EQ(power(2., 10), 1024.);
    EXPECT_DOUBLE_EQ(power(3., 3), 27.);
}
