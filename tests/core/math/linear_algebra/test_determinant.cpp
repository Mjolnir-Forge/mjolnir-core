#include "mjolnir/core/math/linear_algebra/determinant.h"
#include <gtest/gtest.h>

// ====================================================================================================================
// Setup
// ====================================================================================================================

using namespace mjolnir;


// ====================================================================================================================
// Tests
// ====================================================================================================================


// --- test determinant_2x2 (serial version) --------------------------------------------------------------------------

TEST(determinant_2x2, serial_version) // NOLINT
{
    std::array<F64, 4> a = {1., 0., 0., 1.};
    EXPECT_DOUBLE_EQ(determinant_2x2(a), 1.);

    std::array<F64, 4> b = {4., 2., -3., 5.}; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(b), 26.);

    std::array<F64, 4> c = {-1., 2., 4., 3.}; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(c), -11.);

    std::array<F64, 4> d = {4., 2., 6., 3.}; // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    EXPECT_DOUBLE_EQ(determinant_2x2(d), 0.);
}
