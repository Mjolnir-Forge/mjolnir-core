#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/math/math.h"
#include "mjolnir/core/utility/bit_operations.h"
#include "mjolnir/core/utility/is_close.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/core/x86/math.h"
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


// ====================================================================================================================
// Tests
// ====================================================================================================================


// test abs -----------------------------------------------------------------------------------------------------------

TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_abs) // NOLINT
{
    constexpr UST n_e = num_elements<TypeParam>;
    using EType       = ElementType<TypeParam>;

    constexpr std::array<std::array<EType, 8>, 4> test_values = {{{{-1, 2, -3, -4, 5, 6, 7, 8}},
                                                                  {{3, -0, 6, 2, -1, 9, -3, -5}},
                                                                  {{-3, -1, -3, -5, -6, -2, -0, -1}},
                                                                  {{6, 2, 5, 5, 1, 0, 6, 3}}}};

    constexpr UST n_tests = test_values.size();

    for (UST i = 0; i < n_tests; ++i)
    {
        auto test_case_values = test_values.at(i);
        auto a                = mm_setzero<TypeParam>();

        for (UST j = 0; j < n_e; j++)
            set(a, j, test_case_values.at(j));

        auto res = abs(a);

        for (UST j = 0; j < n_e; j++)
            EXPECT_EQ(get(res, j), std::abs(test_case_values.at(j)));
    }
}


// test copy_sign -----------------------------------------------------------------------------------------------------

TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_copy_sign) // NOLINT
{
    constexpr UST n_e = num_elements<TypeParam>;
    using EType       = ElementType<TypeParam>;

    constexpr std::array<std::array<EType, 8>, 4> test_values = {{{{-1, 2, -3, -4, 5, 6, 7, 8}},
                                                                  {{3, -0, 6, 2, -1, 9, -3, -5}},
                                                                  {{-3, -1, -3, -5, -6, -2, -0, -1}},
                                                                  {{6, 2, 5, 5, 1, 0, 6, 3}}}};

    constexpr UST n_tests = test_values.size();

    for (UST i = 0; i < n_tests; ++i)
        for (UST j = 0; j < n_tests; ++j)
        {
            auto values_mag  = test_values.at(i);
            auto values_sign = test_values.at(j);

            auto mag     = mm_setzero<TypeParam>();
            auto mag_abs = mm_setzero<TypeParam>();
            auto sign    = mm_setzero<TypeParam>();

            for (UST k = 0; k < n_e; k++)
            {
                set(mag, k, values_mag.at(k));
                set(mag_abs, k, std::abs(values_mag.at(k)));
                set(sign, k, values_sign.at(k));
            }

            auto res     = copy_sign(mag, sign);
            auto res_abs = copy_sign<true>(mag_abs, sign);

            for (UST k = 0; k < n_e; k++)
            {
                auto exp = std::copysign(values_mag.at(k), values_sign.at(k));
                EXPECT_EQ(get(res, k), exp);
                EXPECT_EQ(get(res_abs, k), exp);
            }
        }
}


// test negate --------------------------------------------------------------------------------------------------------

TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_negate) // NOLINT
{
    constexpr UST n_e = num_elements<TypeParam>;
    using EType       = ElementType<TypeParam>;

    constexpr std::array<std::array<EType, 8>, 4> test_values = {{{{-1, 2, -3, -4, 5, 6, 7, 8}},
                                                                  {{3, -0, 6, 2, -1, 9, -3, -5}},
                                                                  {{-3, -1, -3, -5, -6, -2, -0, -1}},
                                                                  {{6, 2, 5, 5, 1, 0, 6, 3}}}};

    constexpr UST n_tests = test_values.size();

    for (UST i = 0; i < n_tests; ++i)
    {
        auto test_case_values = test_values.at(i);
        auto a                = mm_setzero<TypeParam>();

        for (UST j = 0; j < n_e; j++)
            set(a, j, test_case_values.at(j));

        auto res = negate(a);

        for (UST j = 0; j < n_e; j++)
            EXPECT_EQ(get(res, j), -1 * test_case_values.at(j));
    }
}


// test negate_selected -----------------------------------------------------------------------------------------------

template <typename T_RegisterType>
[[nodiscard]] constexpr inline auto get_negate_index_array(U32 test_case_index) noexcept
        -> std::array<bool, num_elements<T_RegisterType>>
{
    std::array<bool, num_elements<T_RegisterType>> a = {{0}};
    for (UST i = 0; i < a.size(); ++i)
        a.at(i) = not is_bit_set(test_case_index, i);
    return a;
}


template <FloatVectorRegister T_RegisterType, UST t_test_case_index>
inline void test_negate_selected_test_case()
{
    constexpr UST n_e = num_elements<T_RegisterType>;
    using EType       = ElementType<T_RegisterType>;

    constexpr std::array<std::array<EType, 8>, 4> test_values = {{{{-1, 2, -3, -4, 5, 6, 7, 8}},
                                                                  {{3, -0, 6, 2, -1, 9, -3, -5}},
                                                                  {{-3, -1, -3, -5, -6, -2, -0, -1}},
                                                                  {{6, 2, 5, 5, 1, 0, 6, 3}}}};

    constexpr auto b = get_negate_index_array<T_RegisterType>(t_test_case_index);

    constexpr UST n_tests = test_values.size();

    for (UST i = 0; i < n_tests; ++i)
    {
        auto test_case_values = test_values.at(i);
        auto a                = mm_setzero<T_RegisterType>();
        auto res              = mm_setzero<T_RegisterType>();

        for (UST j = 0; j < n_e; j++)
            set(a, j, test_case_values.at(j));

        if constexpr (n_e == 2)
            res = negate_selected<b[0], b[1]>(a);
        else if constexpr (n_e == 4)
            res = negate_selected<b[0], b[1], b[2], b[3]>(a);
        else
            res = negate_selected<b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]>(a); // NOLINT - magic number

        for (UST j = 0; j < n_e; j++)
        {
            auto mul = static_cast<EType>((b.at(j)) ? -1. : 1.);
            auto exp = test_case_values.at(j) * mul;
            EXPECT_EQ(get(res, j), exp);
        }
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_negate_selected) // NOLINT
{
    constexpr UST n_test_cases = power_of_2(num_elements<TypeParam>);
    TYPED_TEST_SERIES(test_negate_selected_test_case, n_test_cases)
}
