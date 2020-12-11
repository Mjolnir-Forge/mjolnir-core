#include "mjolnir/core/utility/pointer_operations.h"
#include <gtest/gtest.h>

#include <array>


using namespace mjolnir;


TEST(alignment, is_aligned_and_misalignment) // NOLINT
{
    constexpr UST alignment = alignof(U32);

    U32 val = 0;

    EXPECT_EQ(misalignment<alignment>(&val), 0);
    EXPECT_TRUE(is_aligned<alignment>(&val));


    U8* misaligned_pointer = reinterpret_cast<U8*>(&val); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    misaligned_pointer += 2;                              // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)


    EXPECT_EQ(misalignment<alignment>(misaligned_pointer), 2);
    EXPECT_FALSE(is_aligned<alignment>(misaligned_pointer));
}

// test_is_aligned ----------------------------------------------------------------------------------------------------

class TestIsAligned : public ::testing::TestWithParam<std::tuple<UST, bool>>
{
};


TEST_P(TestIsAligned, test_is_aligned) // NOLINT
{
    UST  ptr_misalignment = std::get<0>(GetParam());
    bool expected         = std::get<1>(GetParam());

    constexpr UST alignment = 4;

    alignas(alignment) std::array<U32, 2> instance;


    U8* misaligned_pointer = reinterpret_cast<U8*>(&instance); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    misaligned_pointer += ptr_misalignment; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

    EXPECT_EQ(expected, is_aligned(misaligned_pointer, alignment));
}


// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(alignment,
                         TestIsAligned,
                         ::testing::Values(std::make_tuple(0, true),
                                           std::make_tuple(1, false),
                                           std::make_tuple(2, false),
                                           std::make_tuple(3, false),
                                           std::make_tuple(4, true),
                                           std::make_tuple(5, false),
                                           std::make_tuple(6, false),
                                           std::make_tuple(7, false)));


// test_misalignment --------------------------------------------------------------------------------------------------

class TestMisalignment : public ::testing::TestWithParam<std::tuple<UST, UST>>
{
};


TEST_P(TestMisalignment, test_misalignment) // NOLINT
{
    UST ptr_misalignment = std::get<0>(GetParam());
    UST expected         = std::get<1>(GetParam());

    constexpr UST alignment = 4;

    alignas(alignment) std::array<U32, 2> instance;


    U8* misaligned_pointer = reinterpret_cast<U8*>(&instance); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    misaligned_pointer += ptr_misalignment; // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

    EXPECT_EQ(expected, misalignment(misaligned_pointer, alignment));
}


// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(alignment,
                         TestMisalignment,
                         ::testing::Values(std::make_tuple(0, 0),
                                           std::make_tuple(1, 1),
                                           std::make_tuple(2, 2),
                                           std::make_tuple(3, 3),
                                           std::make_tuple(4, 0),
                                           std::make_tuple(5, 1),
                                           std::make_tuple(6, 2),
                                           std::make_tuple(7, 3)));


// TODO: parametrize template tests:
// https://stackoverflow.com/questions/48669279/create-gtest-value-parametrized-test-for-a-template-class
