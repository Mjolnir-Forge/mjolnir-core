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
    return std::array<T_RegisterType, 10>{{mm_setr<T_RegisterType>(1, 2, 3, 4),   // NOLINT - magic number
                                           mm_setr<T_RegisterType>(1, 2, 4, 4),   // NOLINT - magic number
                                           mm_setr<T_RegisterType>(3, 5, 4, 5),   // NOLINT - magic number
                                           mm_setr<T_RegisterType>(1, 2, 2, 4),   // NOLINT - magic number
                                           mm_setr<T_RegisterType>(0, 1, 1, 2),   // NOLINT - magic number
                                           mm_setr<T_RegisterType>(0, 0, 5, 5),   // NOLINT - magic number
                                           mm_setr<T_RegisterType>(4, 2, 3, 1),   // NOLINT - magic number
                                           mm_setr<T_RegisterType>(1, 2, 4, 4),   // NOLINT - magic number
                                           mm_setr<T_RegisterType>(1, 0, 3, 2),   // NOLINT - magic number
                                           mm_setr<T_RegisterType>(2, 1, 1, 0)}}; // NOLINT - magic number
}


template <>
[[nodiscard]] auto get_test_register_array<__m128d>()
{
    return std::array<__m128d, 7>{{mm_setr<__m128d>(1, 2),   // NOLINT - magic number
                                   mm_setr<__m128d>(4, 2),   // NOLINT - magic number
                                   mm_setr<__m128d>(4, 3),   // NOLINT - magic number
                                   mm_setr<__m128d>(0, 2),   // NOLINT - magic number
                                   mm_setr<__m128d>(0, 1),   // NOLINT - magic number
                                   mm_setr<__m128d>(0, 4),   // NOLINT - magic number
                                   mm_setr<__m128d>(4, 0)}}; // NOLINT - magic number
}


template <>
[[nodiscard]] auto get_test_register_array<__m256>()
{
    return std::array<__m256, 10>{{mm_setr<__m256>(1, 2, 3, 4, 5, 6, 7, 8),   // NOLINT - magic number
                                   mm_setr<__m256>(1, 2, 3, 4, 5, 7, 7, 8),   // NOLINT - magic number
                                   mm_setr<__m256>(2, 3, 4, 5, 6, 7, 8, 9),   // NOLINT - magic number
                                   mm_setr<__m256>(1, 2, 3, 4, 5, 6, 6, 8),   // NOLINT - magic number
                                   mm_setr<__m256>(0, 0, 1, 3, 2, 5, 2, 1),   // NOLINT - magic number
                                   mm_setr<__m256>(4, 3, 5, 6, 6, 7, 9, 9),   // NOLINT - magic number
                                   mm_setr<__m256>(0, 0, 1, 1, 7, 9, 9, 9),   // NOLINT - magic number
                                   mm_setr<__m256>(9, 3, 3, 1, 2, 7, 3, 9),   // NOLINT - magic number
                                   mm_setr<__m256>(6, 3, 8, 4, 1, 5, 7, 5),   // NOLINT - magic number
                                   mm_setr<__m256>(4, 2, 3, 1, 4, 6, 2, 6)}}; // NOLINT - magic number
}


// test case macro for comparison of all elements ---------------------------------------------------------------------

// NOLINTNEXTLINE
#define CALL_FULL_CMP_FUNC(cmp_func_name) cmp_func_name(a, b)

// NOLINTNEXTLINE
#define FULL_COMPARISON_TESTCASE(test_case_func_name, cmp_func_name, cmp_operator)                                     \
    template <typename T_RegisterType, UST t_test_case_index>                                                          \
    void test_case_func_name()                                                                                         \
    {                                                                                                                  \
        auto test_func = [](T_RegisterType a, T_RegisterType b)                                                        \
        {                                                                                                              \
            constexpr UST n_e = num_elements<T_RegisterType>;                                                          \
                                                                                                                       \
            bool res = CALL_FULL_CMP_FUNC(cmp_func_name);                                                              \
                                                                                                                       \
            bool exp_result = true;                                                                                    \
            for (UST i = 0; i < n_e; ++i)                                                                              \
                exp_result &= (get(a, i) cmp_operator get(b, i));                                                      \
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


// test compare_all_equal ---------------------------------------------------------------------------------------------

FULL_COMPARISON_TESTCASE(test_compare_all_equal, compare_all_equal, ==)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_all_equal) // NOLINT
{
    constexpr UST n_testcases = 1;
    TYPED_TEST_SERIES(test_compare_all_equal, n_testcases)
}


// test compare_all_greater -------------------------------------------------------------------------------------------

FULL_COMPARISON_TESTCASE(test_compare_all_greater, compare_all_greater, >)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_all_greater) // NOLINT
{
    constexpr UST n_testcases = 1;
    TYPED_TEST_SERIES(test_compare_all_greater, n_testcases)
}


// test compare_all_greater_equal -------------------------------------------------------------------------------------

FULL_COMPARISON_TESTCASE(test_compare_all_greater_equal, compare_all_greater_equal, >=)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_all_greater_equal) // NOLINT
{
    constexpr UST n_testcases = 1;
    TYPED_TEST_SERIES(test_compare_all_greater_equal, n_testcases)
}


// test compare_all_less ----------------------------------------------------------------------------------------------

FULL_COMPARISON_TESTCASE(test_compare_all_less, compare_all_less, <)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_all_less) // NOLINT
{
    constexpr UST n_testcases = 1;
    TYPED_TEST_SERIES(test_compare_all_less, n_testcases)
}


// test compare_all_less_equal ----------------------------------------------------------------------------------------

FULL_COMPARISON_TESTCASE(test_compare_all_less_equal, compare_all_less_equal, <=)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_all_less_equal) // NOLINT
{
    constexpr UST n_testcases = 1;
    TYPED_TEST_SERIES(test_compare_all_less_equal, n_testcases)
}


// test compare_in_sequence_equal -------------------------------------------------------------------------------------

SEQUENTIAL_COMPARISON_TESTCASE(test_compare_in_sequence_equal, compare_in_sequence_equal, ==)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_in_sequence_equal) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = gauss_summation(n_e);
    TYPED_TEST_SERIES(test_compare_in_sequence_equal, n_testcases)
}


// test compare_in_sequence_greater -----------------------------------------------------------------------------------

SEQUENTIAL_COMPARISON_TESTCASE(test_compare_in_sequence_greater, compare_in_sequence_greater, >)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_in_sequence_greater) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = gauss_summation(n_e);
    TYPED_TEST_SERIES(test_compare_in_sequence_greater, n_testcases)
}


// test compare_in_sequence_greater_equal -----------------------------------------------------------------------------

SEQUENTIAL_COMPARISON_TESTCASE(test_compare_in_sequence_greater_equal, compare_in_sequence_greater_equal, >=)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_in_sequence_greater_equal) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = gauss_summation(n_e);
    TYPED_TEST_SERIES(test_compare_in_sequence_greater_equal, n_testcases)
}


// test compare_in_sequence_less --------------------------------------------------------------------------------------

SEQUENTIAL_COMPARISON_TESTCASE(test_compare_in_sequence_less, compare_in_sequence_less, <)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_in_sequence_less) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = gauss_summation(n_e);
    TYPED_TEST_SERIES(test_compare_in_sequence_less, n_testcases)
}


// test compare_in_sequence_less_equal --------------------------------------------------------------------------------

SEQUENTIAL_COMPARISON_TESTCASE(test_compare_in_sequence_less_equal, compare_in_sequence_less_equal, <=)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_in_sequence_less_equal) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = gauss_summation(n_e);
    TYPED_TEST_SERIES(test_compare_in_sequence_less_equal, n_testcases)
}


// test compare_selected_equal ----------------------------------------------------------------------------------------

SELECTIVE_COMPARISON_TESTCASE(test_compare_selected_equal, compare_selected_equal, ==)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_selected_equal) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = power_of_2(n_e) - 1;
    TYPED_TEST_SERIES(test_compare_selected_equal, n_testcases)
}


// test compare_selected_greater --------------------------------------------------------------------------------------

SELECTIVE_COMPARISON_TESTCASE(test_compare_selected_greater, compare_selected_greater, >)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_selected_greater) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = power_of_2(n_e) - 1;
    TYPED_TEST_SERIES(test_compare_selected_greater, n_testcases)
}


// test compare_selected_greater_equal --------------------------------------------------------------------------------

SELECTIVE_COMPARISON_TESTCASE(test_compare_selected_greater_equal, compare_selected_greater_equal, >=)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_selected_greater_equal) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = power_of_2(n_e) - 1;
    TYPED_TEST_SERIES(test_compare_selected_greater_equal, n_testcases)
}


// test compare_selected_less -----------------------------------------------------------------------------------------

SELECTIVE_COMPARISON_TESTCASE(test_compare_selected_less, compare_selected_less, <)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_selected_less) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = power_of_2(n_e) - 1;
    TYPED_TEST_SERIES(test_compare_selected_less, n_testcases)
}


// test compare_selected_less_equal -----------------------------------------------------------------------------------

SELECTIVE_COMPARISON_TESTCASE(test_compare_selected_less_equal, compare_selected_less_equal, <=)


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_compare_selected_less_equal) // NOLINT
{
    constexpr UST n_e         = num_elements<TypeParam>;
    constexpr UST n_testcases = power_of_2(n_e) - 1;
    TYPED_TEST_SERIES(test_compare_selected_less_equal, n_testcases)
}


// test is_memory_zero ------------------------------------------------------------------------------------------------


template <typename T_RegisterType, UST t_test_case_index>
void test_is_memory_zero_testcase()
{
    using EType       = ElementType<T_RegisterType>;
    constexpr UST n_e = num_elements<T_RegisterType>;

    auto a = mm_setzero<T_RegisterType>();

    EXPECT_TRUE(is_memory_zero(a));

    for (UST i = 0; i < n_e; ++i)
    {
        set(a, i, static_cast<EType>(i + 1));
        EXPECT_FALSE(is_memory_zero(a));
        set(a, i, 0.);
        EXPECT_TRUE(is_memory_zero(a));
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_is_memory_zero) // NOLINT
{
    constexpr UST n_testcases = 1;
    TYPED_TEST_SERIES(test_is_memory_zero_testcase, n_testcases)
}
