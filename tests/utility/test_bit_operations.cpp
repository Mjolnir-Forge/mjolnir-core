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


// --- test bit_construct_set_first_n_bits ----------------------------------------------------------------------------

TEST(test_bit_operations, bit_construct_set_first_n_bits) // NOLINT
{
    EXPECT_EQ((bit_construct_set_first_n_bits<U8, 0>()), 0b00000000);
    EXPECT_EQ((bit_construct_set_first_n_bits<U8, 2>()), 0b00000011);
    EXPECT_EQ((bit_construct_set_first_n_bits<U8, 5>()), 0b00011111);
    EXPECT_EQ((bit_construct_set_first_n_bits<U8, 7>()), 0b01111111);
    EXPECT_EQ((bit_construct_set_first_n_bits<U8, 8>()), 0b11111111);

    // specified number of bits can exceed number of tybe bits
    EXPECT_EQ((bit_construct_set_first_n_bits<U8, 9>()), 0b11111111);
    EXPECT_EQ((bit_construct_set_first_n_bits<U8, 11>()), 0b11111111);

    // Test other types
    EXPECT_EQ((bit_construct_set_first_n_bits<U32, 6>()), 0b00111111);
    EXPECT_NE((bit_construct_set_first_n_bits<U32, 11>()), 0b11111111);
    EXPECT_EQ((bit_construct_set_first_n_bits<UST, 1>()), 0b00000001);
    EXPECT_NE((bit_construct_set_first_n_bits<UST, 11>()), 0b11111111);
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


// --- test clear_bits ------------------------------------------------------------------------------------------------

TEST(test_bit_operations, clear_bits) // NOLINT
{
    U8 a = 0b11111111; // NOLINT - magic number

    clear_bits<2>(a, 2);
    EXPECT_EQ(a, 0b11110011);

    clear_bits<3>(a, 5); // NOLINT - magic number
    EXPECT_EQ(a, 0b00010011);

    // test setting already set bit
    clear_bits<5>(a, 1); // NOLINT - magic number
    EXPECT_EQ(a, 0b00000001);

    // test other type
    UST b = 0b11111111; // NOLINT - magic number

    clear_bits<5>(b, 2); // NOLINT - magic number
    EXPECT_EQ(b, 0b10000011);
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


// --- test set_bits ------------------------------------------------------------------------------------------------

TEST(test_bit_operations, set_bits) // NOLINT
{
    U8 a = 0b00000000;

    set_bits<2>(a, 2);
    EXPECT_EQ(a, 0b00001100);

    set_bits<3>(a, 5); // NOLINT - magic number
    EXPECT_EQ(a, 0b11101100);

    // test setting already set bit
    set_bits<5>(a, 1); // NOLINT - magic number
    EXPECT_EQ(a, 0b11111110);

    // test other type
    UST b = 0b00000000;

    set_bits<5>(b, 2); // NOLINT - magic number
    EXPECT_EQ(b, 0b01111100);
}


// --- test set_bits_to_int -------------------------------------------------------------------------------------------

TEST(test_bit_operations, set_bits_to_int) // NOLINT
{
    U8 a = 0b00000000;

    set_bits_to_int<3>(a, 2, 0b101); // NOLINT - magic number
    EXPECT_EQ(a, 0b00010100);

    set_bits_to_int<2>(a, 5, 0b01); // NOLINT - magic number
    EXPECT_EQ(a, 0b00110100);

    // test overwriting already set bit
    set_bits_to_int<4>(a, 3, 0b1001); // NOLINT - magic number
    EXPECT_EQ(a, 0b01001100);

    // test writing whithout clearing
    set_bits_to_int<3, false>(a, 1, 0b101); // NOLINT - magic number
    EXPECT_EQ(a, 0b01001110);

    set_bits_to_int<2, false>(a, 4, 0b11); // NOLINT - magic number
    EXPECT_EQ(a, 0b01111110);

    // test other type
    UST b = 0b00000000;

    set_bits_to_int<5>(b, 2, 0b11001); // NOLINT - magic number
    EXPECT_EQ(b, 0b01100100);
}
