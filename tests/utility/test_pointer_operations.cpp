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
