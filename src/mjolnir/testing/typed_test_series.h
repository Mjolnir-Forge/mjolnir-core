//! @file
//! typed_test_series.h
//!
//! @brief
//! Contains a macro that generates a test series for typed tests


#pragma once


//! \addtogroup testing
//! @{

#ifndef STATIC_ANALYSIS

//! @brief
//! Macro that generates a test series for typed tests.
//!
//! @details
//! This macro can be used inside the body of `TYPED_TEST` from the google test framework. It calls the function with
//! the name passed as `test_func_name` as often as specified by `num_test_cases`. The called function needs to be a
//! template function with a variable type as first and and an unsigned integer as second parameter. During each call,
//! the integer parameter is increased by one, starting at 0. It serves as test case index. The current type of the
//! typed test is passed as type parameter.
//!
//! Since the test case index is a template parameter and therefore known at compile time, it can be used to derive
//! other compile time constants like template parameters that should be used.
//!
//! Before you can use this MACRO, you need to define two other macros:
//!
//! - `CREATE_TEST_CASE_INPUT_VALUES` is inserted befor the call to the test function. It can be used to create and
//!    initialize variables that should be passed to the test case function
//! - `TEST_CASE_ARGUMENTS` specifies the arguments to the called test case function.
//!
//! In case the test case function has no arguments, one would simply use:
//!
//! ~~~ cpp
//! #define CREATE_TEST_CASE_INPUT_VALUES
//! #define TEST_CASE_ARGUMENTS
//! ~~~
//!
//! The corresponding test case function could look like this:
//!
//! ~~~ cpp
//! template <typename T_Type, UST t_test_case_index>
//! void test_case_func()
//! {
//!     ...
//! }
//! ~~~
//!
//! @param test_func_name:
//! The name of the function that defines a single test case. The function needs to be a template function with 2
//! template parameters. The first one must be a variable type. The second one an unsigned integer. The integer values
//! represent the number of the current test case. This number can be used to deduce other static test values like
//! template parameters that should vary between test cases.
//! @param num_test_cases:
//! The number of test cases.
//!
//! @todo
//! Add example for function with parameters
// NOLINTNEXTLINE
#    define TYPED_TEST_SERIES(test_func_name, num_test_cases)                                                          \
        auto start_typed_test_series = []()                                                                            \
        {                                                                                                              \
            auto test_series = []<UST... t_index>([[maybe_unused]] std::index_sequence<t_index...> seq)                \
            {                                                                                                          \
                CREATE_TEST_CASE_INPUT_VALUES;                                                                         \
                (void) std::initializer_list<I32>{(CALL_TEST_CASE_FUNC(test_func_name), 0)...};                        \
            };                                                                                                         \
            test_series(std::make_index_sequence<num_test_cases>());                                                   \
        };                                                                                                             \
        start_typed_test_series()
#else
// NOLINTNEXTLINE
#    define TYPED_TEST_SERIES(test_func_name, num_test_cases)                                                          \
        constexpr UST t_index = 0;                                                                                     \
        CREATE_TEST_CASE_INPUT_VALUES;                                                                                 \
        CALL_TEST_CASE_FUNC(test_func_name)
#endif

//! \cond DO_NOT_DOCUMENT

//! The purpose of this macro is to silence a clang-tidy warning since the currently used clang-tidy version in the CI
//! pipelines does not support multi-line silencing of issues. Placing a `NOLINT` inside of a multi-line macro is not
//! possible
//!
#define CALL_TEST_CASE_FUNC(func_name) func_name<TypeParam, t_index>(TEST_CASE_ARGUMENTS) // NOLINT

//! \endcond

//! @}
