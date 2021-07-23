#include "mjolnir/core/utility/pointer_operations.h"
#include <gtest/gtest.h>

#include <array>


using namespace mjolnir;

// create test suites --------------------------------------------------------------------------------------------------

class PointerOperationTester : public ::testing::TestWithParam<UST>
{
};

// NOLINTNEXTLINE
INSTANTIATE_TEST_SUITE_P(pointer_operations, PointerOperationTester, ::testing::Values(0, 1, 2, 3, 4, 5, 6, 7));


template <typename T_PointerShift>
class PointerOperationTemplateTester : public ::testing::Test
{
};


using pointer_operation_template_testcases = ::testing::Types<std::integral_constant<UST, 0>,  // NOLINT
                                                              std::integral_constant<UST, 1>,  // NOLINT
                                                              std::integral_constant<UST, 2>,  // NOLINT
                                                              std::integral_constant<UST, 3>,  // NOLINT
                                                              std::integral_constant<UST, 4>,  // NOLINT
                                                              std::integral_constant<UST, 5>,  // NOLINT
                                                              std::integral_constant<UST, 6>,  // NOLINT
                                                              std::integral_constant<UST, 7>>; // NOLINT


// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(PointerOperationTemplateTester, pointer_operation_template_testcases, );


// support functions ---------------------------------------------------------------------------------------------------

constexpr UST alignment = 4;

auto get_offset_pointer(U32 offset) -> U8*
{
    alignas(alignment) static std::array<U32, 2> instance = {0};

    U8* ptr = reinterpret_cast<U8*>(&instance); // NOLINT(cppcoreguidelines-pro-type-reinterpret-cast)
    ptr += offset;                              // NOLINT(cppcoreguidelines-pro-bounds-pointer-arithmetic)

    return ptr;
}


// test_is_aligned -----------------------------------------------------------------------------------------------------

TEST_P(PointerOperationTester, test_is_aligned) // NOLINT cert-err58-cpp
{
    const UST  pointer_offset = GetParam();
    const bool expected       = (pointer_offset % alignment) == 0;

    const U8* ptr = get_offset_pointer(pointer_offset);

    EXPECT_EQ(expected, is_aligned(ptr, alignment));
}


// test_is_aligned_template --------------------------------------------------------------------------------------------

TYPED_TEST(PointerOperationTemplateTester, is_aligned) // NOLINT
{
    static constexpr UST pointer_offset = TypeParam::value;
    constexpr bool       expected       = (pointer_offset % alignment) == 0;

    const U8* ptr = get_offset_pointer(pointer_offset);

    EXPECT_EQ(expected, is_aligned<alignment>(ptr));
}


// test_misalignment ---------------------------------------------------------------------------------------------------

TEST_P(PointerOperationTester, test_misalignment) // NOLINT
{
    const UST pointer_offset = GetParam();
    const UST expected       = pointer_offset % alignment;

    const U8* ptr = get_offset_pointer(pointer_offset);

    EXPECT_EQ(expected, misalignment(ptr, alignment));
}


// test misalignment (template version) --------------------------------------------------------------------------------

TYPED_TEST(PointerOperationTemplateTester, misalignment) // NOLINT
{
    static constexpr UST pointer_offset = TypeParam::value;
    constexpr UST        expected       = pointer_offset % alignment;

    const U8* ptr = get_offset_pointer(pointer_offset);

    EXPECT_EQ(expected, misalignment<alignment>(ptr));
}
