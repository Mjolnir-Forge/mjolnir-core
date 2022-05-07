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
//! Insert details...
//!
//! @param test_func:
//! The name of the function that defines a single test case. The function needs to be a template function with 2
//! template parameters. The first one must be a variable type. The second one an unsigned integer. The integer values
//! represent the number of the current test case. This number can be used to deduce other static test values like
//! template parameters that should vary between test cases.
//! @param num_test_cases:
//! The number of test cases.
// NOLINTNEXTLINE
#    define TYPED_TEST_SERIES(test_func, num_test_cases)                                                               \
        auto start_typed_test_series = []()                                                                            \
        {                                                                                                              \
            auto test_series = []<UST... t_index>([[maybe_unused]] std::index_sequence<t_index...> seq)                \
            {                                                                                                          \
                CREATE_INPUT_VALUES;                                                                                   \
                (void) std::initializer_list<I32>{(CALL_TEST_CASE_FUNC(test_func), 0)...};                             \
            };                                                                                                         \
            test_series(std::make_index_sequence<num_test_cases>());                                                   \
        };                                                                                                             \
        start_typed_test_series()
#else
// NOLINTNEXTLINE
#    define TYPED_TEST_SERIES(test_func, num_test_cases)                                                               \
        constexpr UST t_index = 0;                                                                                     \
        CREATE_INPUT_VALUES;                                                                                           \
        CALL_TEST_CASE_FUNC(test_func)
#endif


//! @}
