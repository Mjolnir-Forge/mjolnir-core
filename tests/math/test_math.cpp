#include "mjolnir/core/math/math.h"
#include <gtest/gtest.h>

// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================


// --- test is_power_of_2 ---------------------------------------------------------------------------------------------

TEST(test_math, is_power_of_2_true) // NOLINT
{
    EXPECT_TRUE(is_power_of_2<U32>(1U));
    EXPECT_TRUE(is_power_of_2<U32>(2U));
    EXPECT_TRUE(is_power_of_2<U32>(4U));
    EXPECT_TRUE(is_power_of_2<U32>(8U));
    EXPECT_TRUE(is_power_of_2<U32>(16U));
    EXPECT_TRUE(is_power_of_2<U32>(32U));
    EXPECT_TRUE(is_power_of_2<U32>(64U));
    EXPECT_TRUE(is_power_of_2<U32>(128U));

    EXPECT_TRUE(is_power_of_2<I32>(1));
    EXPECT_TRUE(is_power_of_2<I32>(2));
    EXPECT_TRUE(is_power_of_2<I32>(4));
    EXPECT_TRUE(is_power_of_2<I32>(8));
    EXPECT_TRUE(is_power_of_2<I32>(16));
    EXPECT_TRUE(is_power_of_2<I32>(32));
    EXPECT_TRUE(is_power_of_2<I32>(64));
    EXPECT_TRUE(is_power_of_2<I32>(128));
}


TEST(test_math, is_power_of_2_false) // NOLINT
{
    EXPECT_FALSE(is_power_of_2<U32>(0U));
    EXPECT_FALSE(is_power_of_2<U32>(3U));
    EXPECT_FALSE(is_power_of_2<U32>(6U));
    EXPECT_FALSE(is_power_of_2<U32>(7U));
    EXPECT_FALSE(is_power_of_2<U32>(19U));
    EXPECT_FALSE(is_power_of_2<U32>(27U));
    EXPECT_FALSE(is_power_of_2<U32>(93U));
    EXPECT_FALSE(is_power_of_2<U32>(138U));

    EXPECT_FALSE(is_power_of_2<I32>(0));
    EXPECT_FALSE(is_power_of_2<I32>(3));
    EXPECT_FALSE(is_power_of_2<I32>(6));
    EXPECT_FALSE(is_power_of_2<I32>(7));
    EXPECT_FALSE(is_power_of_2<I32>(19));
    EXPECT_FALSE(is_power_of_2<I32>(27));
    EXPECT_FALSE(is_power_of_2<I32>(93));
    EXPECT_FALSE(is_power_of_2<I32>(138));
}


TEST(test_math, is_power_of_2_false_negative) // NOLINT
{
    EXPECT_FALSE(is_power_of_2<I32>(-1));
    EXPECT_FALSE(is_power_of_2<I32>(-2));
    EXPECT_FALSE(is_power_of_2<I32>(-4));
    EXPECT_FALSE(is_power_of_2<I32>(-8));
    EXPECT_FALSE(is_power_of_2<I32>(-16));
    EXPECT_FALSE(is_power_of_2<I32>(-32));
    EXPECT_FALSE(is_power_of_2<I32>(-64));
    EXPECT_FALSE(is_power_of_2<I32>(-128));

    EXPECT_FALSE(is_power_of_2<I32>(-0));
    EXPECT_FALSE(is_power_of_2<I32>(-3));
    EXPECT_FALSE(is_power_of_2<I32>(-6));
    EXPECT_FALSE(is_power_of_2<I32>(-7));
    EXPECT_FALSE(is_power_of_2<I32>(-19));
    EXPECT_FALSE(is_power_of_2<I32>(-27));
    EXPECT_FALSE(is_power_of_2<I32>(-93));
    EXPECT_FALSE(is_power_of_2<I32>(-138));
}


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


// --- test power_of_2 ------------------------------------------------------------------------------------------------

TEST(test_math, power_of_2) // NOLINT
{
    // test with integers
    EXPECT_EQ(power_of_2(0), 1);
    EXPECT_EQ(power_of_2(1), 2);
    EXPECT_EQ(power_of_2(2), 4);
    EXPECT_EQ(power_of_2(4), 16);
    EXPECT_EQ(power_of_2(8), 256);

    EXPECT_DOUBLE_EQ(power_of_2<F64>(0), 1.);
    EXPECT_DOUBLE_EQ(power_of_2<F64>(1), 2.);
    EXPECT_DOUBLE_EQ(power_of_2<F64>(2), 4.);
    EXPECT_DOUBLE_EQ(power_of_2<F64>(4), 16.);
    EXPECT_DOUBLE_EQ(power_of_2<F64>(8), 256.);
}
