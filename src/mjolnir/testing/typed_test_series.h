//! @file
//! typed_test_series.h
//!
//! @brief
//! Contains a macro that generates a test series for typed tests


#pragma once


#ifndef STATIC_ANALYSIS
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
        CREATE_SOURCE_VALUES;                                                                                          \
        CALL_TEST_CASE_FUNC(test_func)
#endif



//! \addtogroup testing
//! @{



//! \cond DO_NOT_DOCUMENT
//! \endcond


//! @}

