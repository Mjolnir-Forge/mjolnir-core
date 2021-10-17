#include "mjolnir/core/utility/bit_operations.h"
#include <gtest/gtest.h>


// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================


// --- test bit_construct ---------------------------------------------------------------------------------------------

TEST(test_bit_operations, bit_construct) // NOLINT
{
    EXPECT_EQ((bit_construct<U8, 0, 1, 1>()), 0b00000011);
    EXPECT_EQ((bit_construct<U8, 0, 1, 1>(true)), 0b00000110);
    EXPECT_EQ((bit_construct<U8, 1, 0, 1, 1, 0, 1, 1, 0>()), 0b10110110);
    EXPECT_EQ((bit_construct<U8, 1, 0, 1, 1, 0, 1, 1, 0>(true)), 0b01101101);
}


// --- test clear_bit -------------------------------------------------------------------------------------------------

TEST(test_bit_operations, clear_bit) // NOLINT
{
    U8 a = 0b11111111; // NOLINT - magic number

    clear_bit(a, 4);
    EXPECT_EQ(a, 0b11101111);

    clear_bit(a, 0);
    EXPECT_EQ(a, 0b11101110);

    clear_bit(a, 7); // NOLINT - magic number
    EXPECT_EQ(a, 0b01101110);

    // test clear already cleared bit
    clear_bit(a, 4);
    EXPECT_EQ(a, 0b01101110);
}


// --- test is_bit_set ------------------------------------------------------------------------------------------------

TEST(test_bit_operations, is_bit_set) // NOLINT
{
    U8 a = 0b10100111; // NOLINT - magic number

    EXPECT_TRUE(is_bit_set(a, 0));
    EXPECT_TRUE(is_bit_set(a, 1));
    EXPECT_TRUE(is_bit_set(a, 2));
    EXPECT_FALSE(is_bit_set(a, 3));
    EXPECT_FALSE(is_bit_set(a, 4));
    EXPECT_TRUE(is_bit_set(a, 5));
    EXPECT_FALSE(is_bit_set(a, 6));
    EXPECT_TRUE(is_bit_set(a, 7));
}


// --- test set_bit -------------------------------------------------------------------------------------------------

TEST(test_bit_operations, set_bit) // NOLINT
{
    U8 a = 0b00000000; // NOLINT - magic number

    set_bit(a, 4);
    EXPECT_EQ(a, 0b00010000);

    set_bit(a, 0);
    EXPECT_EQ(a, 0b00010001);

    set_bit(a, 7); // NOLINT - magic number
    EXPECT_EQ(a, 0b10010001);

    // test setting already set bit
    set_bit(a, 4);
    EXPECT_EQ(a, 0b10010001);
}


// --- test set_bit_to ------------------------------------------------------------------------------------------------

TEST(test_bit_operations, set_bit_to) // NOLINT
{
    U8 a = 0b10100111; // NOLINT - magic number

    set_bit_to<1>(a, 1);
    EXPECT_EQ(a, 0b10100111);

    set_bit_to<0>(a, 1);
    EXPECT_EQ(a, 0b10100101);

    set_bit_to<0>(a, 6); // NOLINT - magic number
    EXPECT_EQ(a, 0b10100101);

    set_bit_to<1>(a, 6); // NOLINT - magic number
    EXPECT_EQ(a, 0b11100101);

    set_bit_to<0>(a, 2);
    EXPECT_EQ(a, 0b11100001);

    set_bit_to<1>(a, 3);
    EXPECT_EQ(a, 0b11101001);
}
