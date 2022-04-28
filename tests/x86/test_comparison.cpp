#include "mjolnir/core/math/math.h"
#include "mjolnir/core/utility/parameter_pack.h"
#include "mjolnir/core/x86/comparison.h"
#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/direct_access.h"
#include <gtest/gtest.h>

#include <array>

using namespace mjolnir;
using namespace mjolnir::x86;

// ====================================================================================================================
// Setup
// ====================================================================================================================

// create test suites -------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
class TestFloatingPointVectorRegisterTypes : public ::testing::Test
{
};


using VectorRegisterTestTypes = ::testing::Types<__m128, __m128d, __m256, __m256d>; // NOLINT


// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(TestFloatingPointVectorRegisterTypes, VectorRegisterTestTypes, );


// NOLINTNEXTLINE
#define CALL_TEST_CASE_FUNC(func_name) func_name<TypeParam, t_index>()


#ifndef STATIC_ANALYSIS
// NOLINTNEXTLINE
#    define TYPED_TEST_SERIES(test_func, num_test_cases)                                                               \
        auto start_typed_test_series = []()                                                                            \
        {                                                                                                              \
            auto test_series = []<UST... t_index>([[maybe_unused]] std::index_sequence<t_index...> seq)                \
            {                                                                                                          \
                (void) std::initializer_list<I32>{(CALL_TEST_CASE_FUNC(test_func), 0)...};                             \
            };                                                                                                         \
            test_series(std::make_index_sequence<num_test_cases>());                                                   \
        };                                                                                                             \
        start_typed_test_series();
#else
// NOLINTNEXTLINE
#    define TYPED_TEST_SERIES(test_func, num_test_cases)                                                               \
        constexpr UST t_index = 0;                                                                                     \
        CALL_TEST_CASE_FUNC(test_func);
#endif


// create test registers ----------------------------------------------------------------------------------------------

template <typename T_RegisterType>
[[nodiscard]] auto get_test_register_array()
{
    return std::array<T_RegisterType, 5>{{mm_setr<T_RegisterType>(1.F, 2.F, 3.F, 4.F),
                                          mm_setr<T_RegisterType>(4.F, 2.F, 3.F, 1.F),
                                          mm_setr<T_RegisterType>(1.F, 2.F, 2.F, 4.F),
                                          mm_setr<T_RegisterType>(1.F, 0.F, 3.F, 2.F),
                                          mm_setr<T_RegisterType>(2.F, 1.F, 1.F, 0.F)}};
}


template <>
[[nodiscard]] auto get_test_register_array<__m128d>()
{
    return std::array<__m128d, 5>{{mm_setr<__m128d>(1.F, 2.F),
                                   mm_setr<__m128d>(4.F, 2.F),
                                   mm_setr<__m128d>(1.F, 4.F),
                                   mm_setr<__m128d>(0.F, 1.F),
                                   mm_setr<__m128d>(3.F, 4.F)}};
}


template <>
[[nodiscard]] auto get_test_register_array<__m256>()
{
    return std::array<__m256, 4>{{mm_setr<__m256>(1.F, 2.F, 3.F, 4.F, 5.F, 6.F, 7.F, 8.F),
                                  mm_setr<__m256>(0.F, 0.F, 1.F, 3.F, 2.F, 5.F, 2.F, 1.F),
                                  mm_setr<__m256>(4.F, 3.F, 5.F, 6.F, 6.F, 7.F, 9.F, 9.F),
                                  mm_setr<__m256>(4.F, 2.F, 3.F, 1.F, 4.F, 6.F, 2.F, 6.F)}};
}


// test case macro for selective comparison ---------------------------------------------------------------------------

template <typename T_RegisterType, UST t_test_case_index>
[[nodiscard]] constexpr auto get_selective_test_case_boolean_array() -> std::array<bool, num_elements<T_RegisterType>>
{
    constexpr UST n_e = num_elements<T_RegisterType>;

    std::array<bool, n_e> a = {{0}};
    for (UST i = 0; i < n_e; ++i)
        a.at(i) = not is_bit_set(t_test_case_index, i);
    return a;
}


// NOLINTNEXTLINE
#define CALL_CMP_FUNC_2(cmp_func_name) cmp_func_name<c[0], c[1]>(a, b)
// NOLINTNEXTLINE
#define CALL_CMP_FUNC_4(cmp_func_name) cmp_func_name<c[0], c[1], c[2], c[3]>(a, b)
// NOLINTNEXTLINE
#define CALL_CMP_FUNC_8(cmp_func_name) cmp_func_name<c[0], c[1], c[2], c[3], c[4], c[5], c[6], c[7]>(a, b)

// NOLINTNEXTLINE
#define SELECTIVE_COMPARISON_TESTCASE(test_case_func_name, cmp_func_name, cmp_operator)                                \
    template <typename T_RegisterType, UST t_test_case_index>                                                          \
    void test_case_func_name()                                                                                         \
    {                                                                                                                  \
        auto test_func = [](T_RegisterType a, T_RegisterType b)                                                        \
        {                                                                                                              \
            constexpr auto c = get_selective_test_case_boolean_array<T_RegisterType, t_test_case_index>();             \
                                                                                                                       \
            constexpr UST n_e        = num_elements<T_RegisterType>;                                                   \
            bool          res        = false;                                                                          \
            bool          exp_result = true;                                                                           \
                                                                                                                       \
            if constexpr (num_elements<T_RegisterType> == 2)                                                           \
                res = CALL_CMP_FUNC_2(cmp_func_name);                                                                  \
            else if constexpr (num_elements<T_RegisterType> == 4)                                                      \
                res = CALL_CMP_FUNC_4(cmp_func_name);                                                                  \
            else                                                                                                       \
                res = CALL_CMP_FUNC_8(cmp_func_name);                                                                  \
                                                                                                                       \
            for (UST i = 0; i < n_e; ++i)                                                                              \
                if (c.at(i))                                                                                           \
                    exp_result &= (get(a, i) cmp_operator get(b, i));                                                  \
                                                                                                                       \
            EXPECT_EQ(res, exp_result);                                                                                \
        };                                                                                                             \
                                                                                                                       \
                                                                                                                       \
        auto t = get_test_register_array<T_RegisterType>();                                                            \
                                                                                                                       \
        for (auto& v : t)                                                                                              \
            test_func(t.at(0), v);                                                                                     \
    }


// test case macro for sequential comparison --------------------------------------------------------------------------


template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline constexpr auto get_idx_start(UST test_case_idx) noexcept -> UST
{
    constexpr UST n_e       = num_elements<T_RegisterType>;
    UST           idx_start = 0;

    while (test_case_idx >= gauss_summation(n_e) - gauss_summation(n_e - 1 - idx_start))
        ++idx_start;
    return idx_start;
}


// NOLINTNEXTLINE
#define CALL_SEQ_CMP_FUNC(cmp_func_name) cmp_func_name<idx_start, length>(a, b)

// NOLINTNEXTLINE
#define SEQUENTIAL_COMPARISON_TESTCASE(test_case_func_name, cmp_func_name, cmp_operator)                               \
    template <typename T_RegisterType, UST t_test_case_index>                                                          \
    void test_case_func_name()                                                                                         \
    {                                                                                                                  \
        auto test_func = [](T_RegisterType a, T_RegisterType b)                                                        \
        {                                                                                                              \
            constexpr UST n_e                = num_elements<T_RegisterType>;                                           \
            constexpr UST idx_start          = get_idx_start<T_RegisterType>(t_test_case_index);                       \
            constexpr UST prev_start_inc_idx = gauss_summation(n_e) - gauss_summation(n_e - idx_start);                \
            constexpr UST length             = t_test_case_index - prev_start_inc_idx + 1;                             \
                                                                                                                       \
            bool res = CALL_SEQ_CMP_FUNC(cmp_func_name);                                                               \
                                                                                                                       \
            bool exp_result = true;                                                                                    \
            for (UST i = 0; i < length; ++i)                                                                           \
                exp_result &= (get(a, i + idx_start) cmp_operator get(b, i + idx_start));                              \
                                                                                                                       \
            EXPECT_EQ(res, exp_result);                                                                                \
        };                                                                                                             \
                                                                                                                       \
                                                                                                                       \
        auto t = get_test_register_array<T_RegisterType>();                                                            \
                                                                                                                       \
        for (auto& v : t)                                                                                              \
            test_func(t.at(0), v);                                                                                     \
    }


// ====================================================================================================================
// Tests
// ====================================================================================================================

#include <iostream>

// tast compare_all_in_sequence_equal ---------------------------------------------------------------------------------

SEQUENTIAL_COMPARISON_TESTCASE(test_compare_all_in_sequence_equal, compare_all_in_sequence_equal, ==)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_all_in_sequence_equal) // NOLINT
{
    constexpr UST n_e = num_elements<TypeParam>;
    TYPED_TEST_SERIES(test_compare_all_in_sequence_equal, gauss_summation(n_e))
}


// test compare_equal -------------------------------------------------------------------------------------------------

SELECTIVE_COMPARISON_TESTCASE(test_compare_all_selected_equal, compare_all_selected_equal, ==)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_align_right) // NOLINT
{
    TYPED_TEST_SERIES(test_compare_all_selected_equal, power_of_2(num_elements<TypeParam>) - 1)
}
