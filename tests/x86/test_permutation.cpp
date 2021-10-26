#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/bit_operations.h"
#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"
#include <gtest/gtest.h>
#include <initializer_list>

#include <utility>

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
#define CREATE_SOURCE_VALUES                                                                                           \
    TypeParam a = mm_setzero<TypeParam>();                                                                             \
    TypeParam b = mm_setzero<TypeParam>();                                                                             \
                                                                                                                       \
    for (UST i = 0; i < num_elements<TypeParam>; ++i)                                                                  \
    {                                                                                                                  \
        set(a, i, static_cast<ElementType<TypeParam>>(i + 1));                                                         \
        set(b, i, static_cast<ElementType<TypeParam>>(i + 1 + num_elements<TypeParam>));                               \
    }


// NOLINTNEXTLINE
#define CALL_TEST_CASE_FUNC(func_name) func_name<TypeParam, t_index>(a, b)


#ifndef STATIC_ANALYSIS
// NOLINTNEXTLINE
#    define TYPED_TEST_SERIES(test_func, num_test_cases)                                                               \
        auto start_typed_test_series = []()                                                                            \
        {                                                                                                              \
            auto test_series = []<UST... t_index>([[maybe_unused]] std::index_sequence<t_index...> seq)                \
            {                                                                                                          \
                CREATE_SOURCE_VALUES;                                                                                  \
                (void) std::initializer_list<I32>{(CALL_TEST_CASE_FUNC(test_func), 0)...};                             \
            };                                                                                                         \
            test_series(std::make_index_sequence<num_test_cases>());                                                   \
        };                                                                                                             \
        start_typed_test_series();
#else
// NOLINTNEXTLINE
#    define TYPED_TEST_SERIES(test_func, num_test_cases)                                                               \
        constexpr UST t_index = 0;                                                                                     \
        CREATE_SOURCE_VALUES;                                                                                          \
        CALL_TEST_CASE_FUNC(test_func);
#endif

// ====================================================================================================================
// Tests
// ====================================================================================================================


// --- test_align_right -----------------------------------------------------------------------------------------------

template <typename T_RegisterType, U32 t_shift>
void test_align_right_test_case(T_RegisterType a, T_RegisterType b)
{
    constexpr UST n_l  = num_lanes<T_RegisterType>;
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    // create result register
    T_RegisterType c = align_right<t_shift>(a, b);


    // check results
    for (UST i = 0; i < n_le; ++i)
        for (UST j = 0; j < n_l; ++j)
        {
            UST idx     = i + j * n_le;
            UST idx_exp = idx + t_shift;

            ElementType<T_RegisterType> exp = (i < n_le - t_shift) ? get(b, idx_exp) : get(a, idx_exp - n_le);

            EXPECT_DOUBLE_EQ(get(c, idx), exp);
        }
}

TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_align_right) // NOLINT
{
    TYPED_TEST_SERIES(test_align_right_test_case, num_lane_elements<TypeParam>)
}


// --- test_blend -----------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
[[nodiscard]] constexpr inline auto get_blend_index_array(U32 test_case_index) noexcept
{
    std::array<U32, num_elements<T_RegisterType>> a = {{0}};
    for (UST i = 0; i < a.size(); ++i)
        a.at(i) = is_bit_set(test_case_index, i) ? 1 : 0;
    return a;
}


template <typename T_RegisterType, U32 t_test_case_index>
void test_blend_test_case(T_RegisterType a, T_RegisterType b)
{
    // create result register
    auto           c = mm_setzero<T_RegisterType>();
    constexpr auto v = get_blend_index_array<T_RegisterType>(t_test_case_index);

    if constexpr (num_elements<T_RegisterType> == 2)
        c = blend<v[0], v[1]>(a, b);
    else if constexpr (num_elements<T_RegisterType> == 4)
        c = blend<v[0], v[1], v[2], v[3]>(a, b);
    else
        c = blend<v[0], v[1], v[2], v[3], v[4], v[5], v[6], v[7]>(a, b); // NOLINT


    // check results
    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
        EXPECT_DOUBLE_EQ(get(c, i), (is_bit_set(t_test_case_index, i) ? get(b, i) : get(a, i)));
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_blend) // NOLINT
{
    TYPED_TEST_SERIES(test_blend_test_case, ((1U) << num_elements<TypeParam>) )
}


// --- test_blend_above -----------------------------------------------------------------------------------------------

template <typename T_RegisterType, U32 t_index>
void test_blend_above_test_case(T_RegisterType a, T_RegisterType b)
{
    T_RegisterType c = blend_above<t_index>(a, b);

    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
    {
        auto exp = (i > t_index) ? get(b, i) : get(a, i);
        EXPECT_DOUBLE_EQ(get(c, i), exp);
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_blend_above) // NOLINT
{
    TYPED_TEST_SERIES(test_blend_above_test_case, num_elements<TypeParam>)
}


// --- test_blend_at --------------------------------------------------------------------------------------------------

template <typename T_RegisterType, UST t_index>
void test_blend_at_test_case(T_RegisterType a, T_RegisterType b)
{
    T_RegisterType c = blend_at<t_index>(a, b);

    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
    {
        auto exp = (i == t_index) ? get(b, i) : get(a, i);
        EXPECT_DOUBLE_EQ(get(c, i), exp);
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_blend_at) // NOLINT
{
    TYPED_TEST_SERIES(test_blend_at_test_case, num_elements<TypeParam>)
}


// --- test_blend_below -----------------------------------------------------------------------------------------------

template <typename T_RegisterType, UST t_index>
void test_blend_below_test_case(T_RegisterType a, T_RegisterType b)
{
    T_RegisterType c = blend_below<t_index>(a, b);


    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
    {
        if constexpr (t_index == 0)
            EXPECT_DOUBLE_EQ(get(c, i), get(a, i));
        else
        {
            auto exp = (i < t_index) ? get(b, i) : get(a, i);
            EXPECT_DOUBLE_EQ(get(c, i), exp);
        }
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_blend_below) // NOLINT
{
    TYPED_TEST_SERIES(test_blend_below_test_case, num_elements<TypeParam>)
}


// --- test_blend_from_to ---------------------------------------------------------------------------------------------

template <typename T_RegisterType>
[[nodiscard]] constexpr inline auto get_blend_from_to_index_array(U32 test_case_index) noexcept
{
    std::array<UST, 2> indices = {{0}};
    for (UST i = 0; i < test_case_index; ++i)
    {
        if (indices[1] == num_elements<T_RegisterType> - 1)
        {
            indices[0]++;
            indices[1] = indices[0];
        }
        else
            indices[1]++;
    }
    return indices;
}


template <typename T_RegisterType, UST t_test_case_index>
void test_blend_from_to_test_case(T_RegisterType a, T_RegisterType b)
{
    constexpr auto indices = get_blend_from_to_index_array<T_RegisterType>(t_test_case_index);

    T_RegisterType c = blend_from_to<indices[0], indices[1]>(a, b);

    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
    {
        auto exp = (i >= indices[0] && i <= indices[1]) ? get(b, i) : get(a, i);
        EXPECT_DOUBLE_EQ(get(c, i), exp);
    }
}


template <typename T_RegisterType>
[[nodiscard]] constexpr inline auto num_blend_from_to_test_cases() noexcept -> UST
{
    constexpr UST n = num_elements<T_RegisterType>;
    return (n * n + n) / 2;
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_blend_from_to) // NOLINT
{
    TYPED_TEST_SERIES(test_blend_from_to_test_case, num_blend_from_to_test_cases<TypeParam>())
}


// --- test_blend_below -----------------------------------------------------------------------------------------------

template <typename T_RegisterType>
[[nodiscard]] constexpr inline auto get_permute_index_array(UST test_case_index) noexcept
{
    std::array<U32, num_lane_elements<T_RegisterType>> a = {{0}};
    for (UST i = 0; i < a.size(); ++i)
        if constexpr (is_m128<T_RegisterType> || is_m256<T_RegisterType>)
            a.at(i) = static_cast<UST>(0b11) & (test_case_index >> (i * 2));
        else
            a.at(i) = static_cast<UST>(0b1) & (test_case_index >> (i));
    return a;
}


template <typename T_RegisterType, UST t_index>
void test_permute_test_case(T_RegisterType a, [[maybe_unused]] T_RegisterType b) // NOLINT
{
    constexpr UST  n_le = num_lane_elements<T_RegisterType>;
    constexpr auto v    = get_permute_index_array<T_RegisterType>(t_index);
    auto           c    = mm_setzero<T_RegisterType>();

    if constexpr (is_m128d<T_RegisterType> || is_m256d<T_RegisterType>)
        c = permute<v[0], v[1]>(a);
    else
        c = permute<v[0], v[1], v[2], v[3]>(a);


    for (UST i = 0; i < n_le; ++i)
    {
        EXPECT_DOUBLE_EQ(get(c, i), get(a, v.at(i)));
        if constexpr (num_lanes<T_RegisterType> == 2)
        {
            EXPECT_DOUBLE_EQ(get(c, i + n_le), get(a, v.at(i) + n_le));
        }
    }


    if constexpr (is_m256d<T_RegisterType>)
    {
        c = permute<v[0], v[1], v[1], v[0]>(a);

        for (UST i = 0; i < n_le; ++i)
        {
            EXPECT_DOUBLE_EQ(get(c, i), get(a, v.at(i)));
            EXPECT_DOUBLE_EQ(get(c, i + n_le), get(a, v.at((i + 1) % n_le) + n_le));
        }
    }
}

template <typename T_RegisterType>
[[nodiscard]] constexpr inline auto num_permute_test_cases() noexcept -> UST
{
    constexpr UST n = num_lane_elements<T_RegisterType>;

    UST num_test_cases = 1;
    for (UST i = 0; i < n; ++i)
        num_test_cases *= n;

    return num_test_cases;
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_permute) // NOLINT
{
    TYPED_TEST_SERIES(test_permute_test_case, num_permute_test_cases<TypeParam>())
}
