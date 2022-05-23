#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/type.h"
#include <gtest/gtest.h>

//! Inspired by: https://stackoverflow.com/a/22272614/6700329

using namespace mjolnir;


// ====================================================================================================================
// Tests
// ====================================================================================================================


// test_is_any_of -----------------------------------------------------------------------------------------------------

// define support class for expected results
template <class T_Type>
struct TestIsAnyOf : public ::testing::Test
{
};

// define test
TYPED_TEST_SUITE_P(TestIsAnyOf);     // NOLINT
TYPED_TEST_P(TestIsAnyOf, test_case) // NOLINT
{
    constexpr bool result     = mjolnir::is_any_of<TypeParam, I32, UST, F64>();
    constexpr bool exp_result = std::is_same<TypeParam, I32>::value || std::is_same<TypeParam, UST>::value
                                || std::is_same<TypeParam, F64>::value;
    EXPECT_EQ(result, exp_result);
}
REGISTER_TYPED_TEST_SUITE_P(TestIsAnyOf, test_case); // NOLINT


// define test cases
using IsAnyOfTestCases = ::testing::Types<I8, I32, I64, U32, U64, UST, F32, F64>;
// cppcheck-suppress syntaxError
INSTANTIATE_TYPED_TEST_SUITE_P(is_any_of_test_suite, TestIsAnyOf, IsAnyOfTestCases, ); // NOLINT
