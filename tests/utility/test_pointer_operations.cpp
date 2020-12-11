#include "mjolnir/core/utility/pointer_operations.h"
#include <gtest/gtest.h>


using namespace mjolnir;

TEST(alignment, is_aligned_and_misalignment) // NOLINT cert-err58-cpp
{
    constexpr UST alignment = alignof(U32);

    U32 val = 0;

    EXPECT_EQ(misalignment<alignment>(&val), 0);
    EXPECT_EQ(misalignment(&val, alignment), 0);
    EXPECT_TRUE(is_aligned<alignment>(&val));
    EXPECT_TRUE(is_aligned(&val, alignment));


    U8* misaligned_pointer = reinterpret_cast<U8*>(&val); // NOLINT: intentional use of reinterpret_cast
    misaligned_pointer += 2;                              // NOLINT: intentional use of pointer arithmetic


    EXPECT_EQ(misalignment<alignment>(misaligned_pointer), 2);
    EXPECT_EQ(misalignment(misaligned_pointer, alignment), 2);
    EXPECT_FALSE(is_aligned<alignment>(misaligned_pointer));
    EXPECT_FALSE(is_aligned(misaligned_pointer, alignment));
}


class IsAlignedTests : public ::testing::TestWithParam<std::tuple<UST, bool>>
{
};


TEST_P(IsAlignedTests, test_is_aligned) // NOLINT
{
    UST  misalignment = std::get<0>(GetParam());
    bool expected     = std::get<1>(GetParam());

    constexpr UST alignment = 8;

    alignas(alignment) U64 instance;

    U8* misaligned_pointer =
            reinterpret_cast<U8*>(&instance); // NOLINT: intentional type punning to apply byte sized misalignments
    misaligned_pointer += misalignment;       // NOLINT: intentional use of pointer arithmetic

    EXPECT_EQ(expected, is_aligned(misaligned_pointer, alignment));
}


INSTANTIATE_TEST_SUITE_P(test_is_alignedT,
                         IsAlignedTests,
                         ::testing::Values(std::make_tuple(0, true),
                                           std::make_tuple(1, false),
                                           std::make_tuple(2, false),
                                           std::make_tuple(3, false),
                                           std::make_tuple(4, false),
                                           std::make_tuple(5, false),
                                           std::make_tuple(6, false),
                                           std::make_tuple(7, false)));
