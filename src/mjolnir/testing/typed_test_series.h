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
//! This macro can be used inside the body of a `TYPED_TEST` from the google test framework. It calls the function with
//! the name passed as `test_func_name` as often as specified by `num_test_cases`. The called function needs to be a
//! template function with a variable type as first and and an unsigned integer as second parameter. During each call,
//! the integer parameter is increased by one, starting at 0. It serves as test case index. The current type of the
//! typed test is passed as type parameter.
//!
//! Since the test case index is a template parameter and therefore known at compile time, it can be used to derive
//! other compile time constants like template parameters that should be used.
//!
//! Before you can use this macro, you need to define two other macros:
//!
//! - `CREATE_TEST_CASE_INPUT_VALUES` is inserted before the call to the test function. It can be used to create and
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
//! @note
//! If the preprocessor symbol `STATIC_ANALYSIS` is defined, the number of test cases will automatically set to 1. The
//! reason is that clang-tidy evaluates each test case individually causing a massive overhead for some tests. This
//! reduction in the number of test cases shouldn't have an impact on the results of the clang-tidy run. However, it
//! results in a significantly decreased runtime.
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
                /* NOLINTNEXTLINE(bugprone-macro-parentheses) */                                                       \
                (void) std::initializer_list<I32>{(test_func_name<TypeParam, t_index>(TEST_CASE_ARGUMENTS), 0)...};    \
            };                                                                                                         \
            test_series(std::make_index_sequence<num_test_cases>());                                                   \
        };                                                                                                             \
        start_typed_test_series()
#else
// NOLINTNEXTLINE
#    define TYPED_TEST_SERIES(test_func_name, num_test_cases)                                                          \
        CREATE_TEST_CASE_INPUT_VALUES;                                                                                 \
        test_func_name<TypeParam, 0>(TEST_CASE_ARGUMENTS) // NOLINT(bugprone-macro-parentheses)
#endif


//! @}
