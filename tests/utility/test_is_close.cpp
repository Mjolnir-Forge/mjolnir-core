#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/is_close.h"
#include <gtest/gtest.h>

#include <array>


using namespace mjolnir;

// ====================================================================================================================
// Setup
// ====================================================================================================================

// create test suites -------------------------------------------------------------------------------------------------

template <typename T_Type>
class IsCloseTemplateTester : public ::testing::Test
{
};


using is_close_template_testcases = ::testing::Types<I32, F32, F64>; // NOLINT


// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(IsCloseTemplateTester, is_close_template_testcases, );


// ====================================================================================================================
// Tests
// ====================================================================================================================

TYPED_TEST(IsCloseTemplateTester, is_close_abs) // NOLINT
{
    TypeParam tolerance = 1;
    TypeParam a         = 1;
    TypeParam b         = 2;
    is_close_abs<TypeParam>(a, b, tolerance); // NOLINT
}
