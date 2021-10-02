#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/type.h"
#include <gtest/gtest.h>

//! Inspired by: https://stackoverflow.com/a/22272614/6700329

using namespace mjolnir;


// ====================================================================================================================
// Tests
// ====================================================================================================================


// test_is_any_of ------------------------------------------------------------------------------------------------------

// define support class for expected results
template <class T_Type>
struct test_is_any_of : public ::testing::Test
{
    static bool expected_result;
};


// define test
TYPED_TEST_SUITE_P(test_is_any_of);
TYPED_TEST_P(test_is_any_of, IsAnyOf)
{
    bool result = mjolnir::is_any_of<TypeParam, I32, UST, F64>();
    EXPECT_EQ(result, test_is_any_of<TypeParam>::expected_result);
}
REGISTER_TYPED_TEST_SUITE_P(test_is_any_of, IsAnyOf);


// define expected results
template <typename T_Type>
bool test_is_any_of<T_Type>::expected_result =
        std::is_same<T_Type, I32>::value || std::is_same<T_Type, UST>::value || std::is_same<T_Type, F64>::value;


// define test cases
typedef ::testing::Types<I8, I32, I64, U32, U64, UST, F32, F64> tested_types;
INSTANTIATE_TYPED_TEST_SUITE_P(IsAnyOfTestSuite, test_is_any_of, tested_types);
