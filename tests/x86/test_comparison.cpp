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
        \ CALL_TEST_CASE_FUNC(test_func);
#endif


// NOLINTNEXTLINE
#define CALL_CHECK_FUNC_2(check_func_name) check_func_name<b[0], b[1]>(a.at(0), v)
// NOLINTNEXTLINE
#define CALL_CHECK_FUNC_4(check_func_name) check_func_name<b[0], b[1], b[2], b[3]>(a.at(0), v)
// NOLINTNEXTLINE
#define CALL_CHECK_FUNC_8(check_func_name) check_func_name<b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]>(a.at(0), v)

// NOLINTNEXTLINE
#define COMPARISON_TESTCASE_BODY(check_func_name)                                                                      \
    auto           a = get_test_register_array<T_RegisterType>();                                                      \
    constexpr auto b = get_test_case_boolean_array<T_RegisterType, t_test_case_index>();                               \
                                                                                                                       \
    for (auto& v : a)                                                                                                  \
    {                                                                                                                  \
        if constexpr (num_elements<T_RegisterType> == 2)                                                               \
            CALL_CHECK_FUNC_2(check_func_name);                                                                        \
        else if constexpr (num_elements<T_RegisterType> == 4)                                                          \
            CALL_CHECK_FUNC_4(check_func_name);                                                                        \
        else                                                                                                           \
            CALL_CHECK_FUNC_8(check_func_name);                                                                        \
    }

// ====================================================================================================================
// Tests
// ====================================================================================================================

#include <iostream>

template <bool... t_bool, FloatVectorRegister T_RegisterType>
void check_compare_equal(T_RegisterType a, T_RegisterType b)
{
    constexpr UST n_e        = num_elements<T_RegisterType>;
    bool          res        = compare_all_equal<t_bool...>(a, b);
    bool          exp_result = true;

    std::array<bool, n_e> check = {{t_bool...}};
    for (UST i = 0; i < n_e; ++i)
    {
        if (check.at(i))
        {
            // std::cout << get(a, i) << "-" << get(b, i) << "==" << (get(a, i) == get(b, i)) << std::endl;
            exp_result &= (get(a, i) == get(b, i));
        }
        std::cout << check.at(i);
    }
    std::cout << std::endl;

    // std::cout << res << '-' << exp_result << std::endl;
    EXPECT_EQ(res, exp_result);
    // if (not res == exp_result)
    //    auto u = compare_all_equal<t_bool...>(a, b);
}


template <typename T_RegisterType>
[[nodiscard]] auto get_test_register_array()
{
    return std::array<T_RegisterType, 5>{{mm_setr<T_RegisterType>(1, 2, 3, 4),
                                          mm_setr<T_RegisterType>(4, 2, 3, 1),
                                          mm_setr<T_RegisterType>(1, 2, 2, 4),
                                          mm_setr<T_RegisterType>(1, 0, 3, 2),
                                          mm_setr<T_RegisterType>(2, 1, 1, 0)}};
}


template <>
[[nodiscard]] auto get_test_register_array<__m128d>()
{
    return std::array<__m128d, 5>{{mm_setr<__m128d>(1, 2),
                                   mm_setr<__m128d>(4, 2),
                                   mm_setr<__m128d>(1, 4),
                                   mm_setr<__m128d>(0, 1),
                                   mm_setr<__m128d>(3, 4)}};
}


template <>
[[nodiscard]] auto get_test_register_array<__m256>()
{
    return std::array<__m256, 4>{{mm_setr<__m256>(1, 2, 3, 4, 5, 6, 7, 8),
                                  mm_setr<__m256>(0, 0, 1, 3, 2, 5, 2, 1),
                                  mm_setr<__m256>(4, 3, 5, 6, 6, 7, 9, 9),
                                  mm_setr<__m256>(4, 2, 3, 1, 4, 6, 2, 6)}};
}


template <typename T_RegisterType, UST t_test_case_index>
[[nodiscard]] constexpr auto get_test_case_boolean_array() -> std::array<bool, num_elements<T_RegisterType>>
{
    constexpr UST n_e = num_elements<T_RegisterType>;

    std::array<bool, n_e> a = {{0}};
    for (UST i = 0; i < n_e; ++i)
        a[i] = not is_bit_set(t_test_case_index, i);
    return a;
}


template <typename T_RegisterType, UST t_test_case_index>
void test_compare_all_equal(){COMPARISON_TESTCASE_BODY(check_compare_equal)}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_align_right) // NOLINT
{
    TYPED_TEST_SERIES(test_compare_all_equal, power_of_2(num_elements<TypeParam>) - 1)
}
