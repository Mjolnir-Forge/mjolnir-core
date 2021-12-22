#include "mjolnir/core/utility/bit_operations.h"
#include <gtest/gtest.h>


// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================


// --- test bit_construct ---------------------------------------------------------------------------------------------

TEST(test_bit_operations, bit_construct) // NOLINT
{
    EXPECT_EQ((bit_construct<U8>(false)), 0b00000000);

    EXPECT_EQ((bit_construct<U8, 0, 1, 1>(false)), 0b00000011);
    EXPECT_EQ((bit_construct<U8, 0, 1, 1>(true)), 0b00000110);
    EXPECT_EQ((bit_construct<U8, 1, 0, 1, 1, 0, 1, 1, 0>(false)), 0b10110110);
    EXPECT_EQ((bit_construct<U8, 1, 0, 1, 1, 0, 1, 1, 0>(true)), 0b01101101);

    // Test other types
    EXPECT_EQ((bit_construct<UST, 1, 0, 1, 1, 0, 1, 1, 0>(false)), 0b10110110);
    EXPECT_EQ((bit_construct<UST, 1, 0, 1, 1, 0, 1, 1, 0>(true)), 0b01101101);
}


// --- test bit_construct_from_ints -----------------------------------------------------------------------------------

TEST(test_bit_operations, bit_construct_from_ints) // NOLINT
{
    EXPECT_EQ((bit_construct_from_ints<2, U8, 3, 2, 3, 1>(false)), 0b11101101);
    EXPECT_EQ((bit_construct_from_ints<2, U8, 3, 2, 3, 1>(true)), 0b01111011);

    EXPECT_EQ((bit_construct_from_ints<3, U8, 6, 4>(false)), 0b00110100);
    EXPECT_EQ((bit_construct_from_ints<3, U8, 6, 4>(true)), 0b00100110);

    EXPECT_EQ((bit_construct_from_ints<4, U8, 9, 13>(false)), 0b10011101);
    EXPECT_EQ((bit_construct_from_ints<4, U8, 9, 13>(true)), 0b11011001);

    // Test other types
    EXPECT_EQ((bit_construct_from_ints<4, UST, 9, 13>(false)), 0b10011101);
    EXPECT_EQ((bit_construct_from_ints<4, UST, 9, 13>(true)), 0b11011001);
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
    EXPECT_EQ((bit_construct_set_first_n_bits<U32, 6>()), U32(0b00111111));
    EXPECT_NE((bit_construct_set_first_n_bits<U32, 11>()), U32(0b11111111));
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


// --- test get_bit (static) -----------------------------------------------------------------------------------------

TEST(test_bit_operations, get_bit_static) // NOLINT
{
    U8 a = 0b10100111; // NOLINT - magic number

    EXPECT_EQ(get_bit<0>(a), 0b00000001);
    EXPECT_EQ(get_bit<1>(a), 0b00000010);
    EXPECT_EQ(get_bit<2>(a), 0b00000100);
    EXPECT_EQ(get_bit<3>(a), 0b00000000);
    EXPECT_EQ(get_bit<4>(a), 0b00000000);
    EXPECT_EQ(get_bit<5>(a), 0b00100000);
    EXPECT_EQ(get_bit<6>(a), 0b00000000);
    EXPECT_EQ(get_bit<7>(a), 0b10000000);

    // test positive shifts
    EXPECT_EQ((get_bit<1, 0>(a)), 0b00000010);
    EXPECT_EQ((get_bit<2, 3>(a)), 0b00100000);
    EXPECT_EQ((get_bit<2, 5>(a)), 0b10000000);
    EXPECT_EQ((get_bit<3, 3>(a)), 0b00000000);

    // test negative shifts
    EXPECT_EQ((get_bit<1, -1>(a)), 0b00000001);
    EXPECT_EQ((get_bit<2, -2>(a)), 0b00000001);
    EXPECT_EQ((get_bit<3, -1>(a)), 0b00000000);
    EXPECT_EQ((get_bit<3, -3>(a)), 0b00000000);
    EXPECT_EQ((get_bit<7, -3>(a)), 0b00010000);
    EXPECT_EQ((get_bit<5, -2>(a)), 0b00001000);


    // test other type
    U16 b = 0b11111111; // NOLINT - magic number
    EXPECT_EQ((get_bit<1, 12, U16>(b)), 0b0010000000000000);
    EXPECT_EQ((get_bit<1, 12, U8, U16>(a)), 0b0010000000000000);
}


// --- test get_bit (dynamic) -----------------------------------------------------------------------------------------

TEST(test_bit_operations, get_bit_dynamic) // NOLINT
{
    U8 a = 0b10100111; // NOLINT - magic number

    EXPECT_EQ(get_bit(a, 0), 0b00000001);
    EXPECT_EQ(get_bit(a, 1), 0b00000010);
    EXPECT_EQ(get_bit(a, 2), 0b00000100);
    EXPECT_EQ(get_bit(a, 3), 0b00000000);
    EXPECT_EQ(get_bit(a, 4), 0b00000000);
    EXPECT_EQ(get_bit(a, 5), 0b00100000);
    EXPECT_EQ(get_bit(a, 6), 0b00000000);
    EXPECT_EQ(get_bit(a, 7), 0b10000000);

    // test positive shifts
    EXPECT_EQ(get_bit(a, 0, 1), 0b00000010);
    EXPECT_EQ(get_bit(a, 2, 3), 0b00100000);
    EXPECT_EQ(get_bit(a, 2, 5), 0b10000000);
    EXPECT_EQ(get_bit(a, 3, 3), 0b00000000);

    // test negative shifts
    EXPECT_EQ(get_bit(a, 1, -1), 0b00000001);
    EXPECT_EQ(get_bit(a, 2, -2), 0b00000001);
    EXPECT_EQ(get_bit(a, 3, -1), 0b00000000);
    EXPECT_EQ(get_bit(a, 3, -3), 0b00000000);
    EXPECT_EQ(get_bit(a, 7, -3), 0b00010000);
    EXPECT_EQ(get_bit(a, 5, -2), 0b00001000);


    // test other type
    U16 b = 0b11111111; // NOLINT - magic number
    EXPECT_EQ((get_bit<U16>(b, 1, 12)), 0b0010000000000000);
    EXPECT_EQ((get_bit<U8, U16>(a, 1, 12)), 0b0010000000000000);
}


// --- test get_bit_max_shift (static) --------------------------------------------------------------------------------

TEST(test_bit_operations, get_bit_shift_max_static) // NOLINT
{
    U8 a = 0b10100111; // NOLINT - magic number

    // right shift
    EXPECT_EQ(get_bit_shift_max<0>(a), 0b00000001);
    EXPECT_EQ(get_bit_shift_max<1>(a), 0b00000001);
    EXPECT_EQ(get_bit_shift_max<2>(a), 0b00000001);
    EXPECT_EQ(get_bit_shift_max<3>(a), 0b00000000);
    EXPECT_EQ(get_bit_shift_max<4>(a), 0b00000000);
    EXPECT_EQ(get_bit_shift_max<5>(a), 0b00000001);
    EXPECT_EQ(get_bit_shift_max<6>(a), 0b00000000);
    EXPECT_EQ(get_bit_shift_max<7>(a), 0b00000001);

    // left shift
    EXPECT_EQ((get_bit_shift_max<0, false>(a)), 0b10000000);
    EXPECT_EQ((get_bit_shift_max<1, false>(a)), 0b10000000);
    EXPECT_EQ((get_bit_shift_max<2, false>(a)), 0b10000000);
    EXPECT_EQ((get_bit_shift_max<3, false>(a)), 0b00000000);
    EXPECT_EQ((get_bit_shift_max<4, false>(a)), 0b00000000);
    EXPECT_EQ((get_bit_shift_max<5, false>(a)), 0b10000000);
    EXPECT_EQ((get_bit_shift_max<6, false>(a)), 0b00000000);
    EXPECT_EQ((get_bit_shift_max<7, false>(a)), 0b10000000);

    // test other type
    U16 b = 0b11111111; // NOLINT - magic number
    EXPECT_EQ((get_bit_shift_max<5, true>(b)), 0b0000000000000001);
    EXPECT_EQ((get_bit_shift_max<5, false>(b)), 0b1000000000000000);

    EXPECT_EQ((get_bit_shift_max<5, true, U16, U8>(b)), 0b00000001);
    EXPECT_EQ((get_bit_shift_max<5, false, U16, U8>(b)), 0b10000000);

    EXPECT_EQ((get_bit_shift_max<5, true, U8, U16>(a)), 0b0000000000000001);
    EXPECT_EQ((get_bit_shift_max<5, false, U8, U16>(a)), 0b1000000000000000);
}


// --- test get_bit_max_shift (dynamic) -------------------------------------------------------------------------------

TEST(test_bit_operations, get_bit_shift_max_dynamic) // NOLINT
{
    U8 a = 0b10100111; // NOLINT - magic number

    // right shift
    EXPECT_EQ(get_bit_shift_max(a, 0), 0b00000001);
    EXPECT_EQ(get_bit_shift_max(a, 1), 0b00000001);
    EXPECT_EQ(get_bit_shift_max(a, 2), 0b00000001);
    EXPECT_EQ(get_bit_shift_max(a, 3), 0b00000000);
    EXPECT_EQ(get_bit_shift_max(a, 4), 0b00000000);
    EXPECT_EQ(get_bit_shift_max(a, 5), 0b00000001);
    EXPECT_EQ(get_bit_shift_max(a, 6), 0b00000000);
    EXPECT_EQ(get_bit_shift_max(a, 7), 0b00000001);

    // left shift
    EXPECT_EQ(get_bit_shift_max<false>(a, 0), 0b10000000);
    EXPECT_EQ(get_bit_shift_max<false>(a, 1), 0b10000000);
    EXPECT_EQ(get_bit_shift_max<false>(a, 2), 0b10000000);
    EXPECT_EQ(get_bit_shift_max<false>(a, 3), 0b00000000);
    EXPECT_EQ(get_bit_shift_max<false>(a, 4), 0b00000000);
    EXPECT_EQ(get_bit_shift_max<false>(a, 5), 0b10000000);
    EXPECT_EQ(get_bit_shift_max<false>(a, 6), 0b00000000);
    EXPECT_EQ(get_bit_shift_max<false>(a, 7), 0b10000000);

    // test other type
    U16 b = 0b11111111; // NOLINT - magic number
    EXPECT_EQ(get_bit_shift_max<true>(b, 5), 0b0000000000000001);
    EXPECT_EQ(get_bit_shift_max<false>(b, 5), 0b1000000000000000);

    EXPECT_EQ((get_bit_shift_max<true, U16, U8>(b, 5)), 0b00000001);
    EXPECT_EQ((get_bit_shift_max<false, U16, U8>(b, 5)), 0b10000000);

    EXPECT_EQ((get_bit_shift_max<true, U8, U16>(a, 5)), 0b0000000000000001);
    EXPECT_EQ((get_bit_shift_max<false, U8, U16>(a, 5)), 0b1000000000000000);
}


// --- test get_bits (static) -----------------------------------------------------------------------------------------

TEST(test_bit_operations, get_bits_static) // NOLINT
{
    U8 a = 0b10100111; // NOLINT - magic number

    EXPECT_EQ((get_bits<0, 3>(a)), 0b00000111);
    EXPECT_EQ((get_bits<2, 4>(a)), 0b00100100);
    EXPECT_EQ((get_bits<3, 5>(a)), 0b10100000);

    // test positive shifts
    EXPECT_EQ((get_bits<0, 3, 2>(a)), 0b00011100);
    EXPECT_EQ((get_bits<2, 4, 1>(a)), 0b01001000);
    EXPECT_EQ((get_bits<2, 4, 2>(a)), 0b10010000);

    // test negative shifts
    EXPECT_EQ((get_bits<1, 3, -1>(a)), 0b00000011);
    EXPECT_EQ((get_bits<2, 4, -2>(a)), 0b00001001);
    EXPECT_EQ((get_bits<5, 3, -3>(a)), 0b00010100);

    // test other type
    U16 b = 0b11111111; // NOLINT - magic number
    EXPECT_EQ((get_bits<1, 3, 10, U16>(b)), 0b0011100000000000);
    EXPECT_EQ((get_bits<1, 3, 10, U8, U16>(a)), 0b0001100000000000);
}


// --- test get_bits (dynamic) ----------------------------------------------------------------------------------------

TEST(test_bit_operations, get_bits_dynamic) // NOLINT
{
    U8 a = 0b10100111; // NOLINT - magic number

    EXPECT_EQ(get_bits<3>(a, 0), 0b00000111);
    EXPECT_EQ(get_bits<4>(a, 2), 0b00100100);
    EXPECT_EQ(get_bits<5>(a, 3), 0b10100000);

    // test positive shifts
    EXPECT_EQ((get_bits<3>(a, 0, 2)), 0b00011100);
    EXPECT_EQ((get_bits<4>(a, 2, 1)), 0b01001000);
    EXPECT_EQ((get_bits<4>(a, 2, 2)), 0b10010000);

    // test negative shifts
    EXPECT_EQ((get_bits<3>(a, 1, -1)), 0b00000011);
    EXPECT_EQ((get_bits<4>(a, 2, -2)), 0b00001001);
    EXPECT_EQ((get_bits<3>(a, 5, -3)), 0b00010100);

    // test other type
    U16 b = 0b11111111; // NOLINT - magic number
    EXPECT_EQ((get_bits<3, U16>(b, 1, 10)), 0b0011100000000000);
    EXPECT_EQ((get_bits<3, U8, U16>(a, 1, 10)), 0b0001100000000000);
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
    U8 a = 0b00000000;

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


// --- test set_bits_with_int -----------------------------------------------------------------------------------------

TEST(test_bit_operations, set_bits_with_int) // NOLINT
{
    U8 a = 0b00000000;

    set_bits_with_int<3>(a, 2, 0b101); // NOLINT - magic number
    EXPECT_EQ(a, 0b00010100);

    set_bits_with_int<2>(a, 5, 0b01); // NOLINT - magic number
    EXPECT_EQ(a, 0b00110100);

    // test overwriting already set bit
    set_bits_with_int<4>(a, 3, 0b1001); // NOLINT - magic number
    EXPECT_EQ(a, 0b01001100);

    // test writing whithout clearing
    set_bits_with_int<3, false>(a, 1, 0b101); // NOLINT - magic number
    EXPECT_EQ(a, 0b01001110);

    set_bits_with_int<2, false>(a, 4, 0b11); // NOLINT - magic number
    EXPECT_EQ(a, 0b01111110);

    // test other type
    UST b = 0b00000000;

    set_bits_with_int<5>(b, 2, 0b11001); // NOLINT - magic number
    EXPECT_EQ(b, 0b01100100);
}
