#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/math/math.h"
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
    auto a = mm_setzero<TypeParam>();                                                                                  \
    auto b = mm_setzero<TypeParam>();                                                                                  \
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
    //! @todo create power of 2 function in math header

    TYPED_TEST_SERIES(test_blend_test_case, power_of_2(num_elements<TypeParam>))
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


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_blend_from_to) // NOLINT
{
    TYPED_TEST_SERIES(test_blend_from_to_test_case, gauss_summation(num_elements<TypeParam>))
}


// --- test broadcast -------------------------------------------------------------------------------------------------

template <typename T_RegisterType, UST t_test_case_index>
void test_broadcast(T_RegisterType a, [[maybe_unused]] T_RegisterType b) // NOLINT - complexity
{
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    T_RegisterType c = broadcast<t_test_case_index>(a);


    for (UST i = 0; i < n_le; ++i)
    {
        EXPECT_DOUBLE_EQ(get(c, i), get(a, t_test_case_index));
        if constexpr (is_avx_register<T_RegisterType>)
        {
            EXPECT_DOUBLE_EQ(get(c, i + n_le), get(a, t_test_case_index + n_le));
        }
    }

    if constexpr (is_avx_register<T_RegisterType>)
    {
        constexpr UST idx_1 = (t_test_case_index + 1) % n_le;

        c = broadcast<t_test_case_index, idx_1>(a);


        for (UST i = 0; i < n_le; ++i)
        {
            EXPECT_DOUBLE_EQ(get(c, i), get(a, t_test_case_index));
            EXPECT_DOUBLE_EQ(get(c, i + n_le), get(a, idx_1 + n_le));
        }
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_broadcast) // NOLINT
{
    TYPED_TEST_SERIES(test_broadcast, num_lane_elements<TypeParam>)
}


// --- test broadcast_across_lanes ------------------------------------------------------------------------------------

template <typename T_RegisterType, UST t_test_case_index>
void test_broadcast_across_lanes_test_case(T_RegisterType a, [[maybe_unused]] T_RegisterType b)
{
    T_RegisterType c = broadcast_across_lanes<t_test_case_index>(a);

    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
    {
        EXPECT_DOUBLE_EQ(get(c, i), get(a, t_test_case_index));
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_broadcast_across_lanes) // NOLINT
{
    TYPED_TEST_SERIES(test_broadcast_across_lanes_test_case, num_elements<TypeParam>)
}


// --- test exchange --------------------------------------------------------------------------------------------------

template <typename T_RegisterType, UST t_test_case_index>
void test_exchange_test_case(T_RegisterType a, [[maybe_unused]] T_RegisterType b)
{
    constexpr UST idx_0 = t_test_case_index % num_elements<T_RegisterType>;
    constexpr UST idx_1 = t_test_case_index / num_elements<T_RegisterType>;

    T_RegisterType c = a;
    T_RegisterType d = b;

    exchange<idx_0, idx_1>(c, d);

    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
    {
        auto exp_c = (i == idx_0) ? get(b, idx_1) : get(a, i);
        auto exp_d = (i == idx_1) ? get(a, idx_0) : get(b, i);

        EXPECT_DOUBLE_EQ(get(c, i), exp_c);
        EXPECT_DOUBLE_EQ(get(d, i), exp_d);
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_exchange) // NOLINT
{TYPED_TEST_SERIES(test_exchange_test_case, power(num_elements<TypeParam>, 2))}


// --- test insert ----------------------------------------------------------------------------------------------------


TEST(TestM128, test_insert) // NOLINT
{
    auto check_all_equal = [](__m128 val, F32 v_0, F32 v_1, F32 v_2, F32 v_3)
    {
        EXPECT_DOUBLE_EQ(get(val, 0), v_0);
        EXPECT_DOUBLE_EQ(get(val, 1), v_1);
        EXPECT_DOUBLE_EQ(get(val, 2), v_2);
        EXPECT_DOUBLE_EQ(get(val, 3), v_3);
    };

    auto a = mm_setr<__m128>(1.F, 2.F, 3.F, 4.F); // NOLINT - magic number
    auto b = mm_setr<__m128>(5.F, 6.F, 7.F, 8.F); // NOLINT - magic number

    // test source index
    check_all_equal(insert<0, 0>(a, b), 1, 6, 7, 8); // NOLINT - magic number
    check_all_equal(insert<1, 0>(a, b), 2, 6, 7, 8); // NOLINT - magic number
    check_all_equal(insert<2, 0>(a, b), 3, 6, 7, 8); // NOLINT - magic number
    check_all_equal(insert<3, 0>(a, b), 4, 6, 7, 8); // NOLINT - magic number

    // test destination index
    check_all_equal(insert<2, 1>(a, b), 5, 3, 7, 8); // NOLINT - magic number
    check_all_equal(insert<2, 2>(a, b), 5, 6, 3, 8); // NOLINT - magic number
    check_all_equal(insert<2, 3>(a, b), 5, 6, 7, 3); // NOLINT - magic number

    // test set zero flags
    check_all_equal(insert<3, 1, false, false, true, false>(a, b), 5, 4, 0, 8); // NOLINT - magic number
    check_all_equal(insert<1, 0, false, true, false, true>(a, b), 2, 0, 7, 0);  // NOLINT - magic number
    check_all_equal(insert<2, 3, true, true, true, false>(a, b), 0, 0, 0, 3);
    check_all_equal(insert<0, 2, true, true, true, true>(a, b), 0, 0, 0, 0);
}


// --- test permute ---------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
[[nodiscard]] constexpr inline auto get_permute_index_array(UST test_case_index) noexcept
{
    std::array<U32, num_lane_elements<T_RegisterType>> a = {{0}};

    for (UST i = 0; i < a.size(); ++i)
        if constexpr (is_single_precision<T_RegisterType>)
            a.at(i) = static_cast<UST>(0b11) & (test_case_index >> (i * 2));
        else
            a.at(i) = static_cast<UST>(0b1) & (test_case_index >> (i));
    return a;
}


template <typename T_RegisterType, UST t_index>
void test_permute_test_case(T_RegisterType a, [[maybe_unused]] T_RegisterType b) // NOLINT - complexity
{
    constexpr UST  n_le = num_lane_elements<T_RegisterType>;
    constexpr auto v    = get_permute_index_array<T_RegisterType>(t_index);
    auto           c    = mm_setzero<T_RegisterType>();


    if constexpr (is_double_precision<T_RegisterType>)
        c = permute<v[0], v[1]>(a);
    else
        c = permute<v[0], v[1], v[2], v[3]>(a);


    for (UST i = 0; i < n_le; ++i)
    {
        EXPECT_DOUBLE_EQ(get(c, i), get(a, v.at(i)));
        if constexpr (is_multi_lane<T_RegisterType>)
        {
            EXPECT_DOUBLE_EQ(get(c, i + n_le), get(a, v.at(i) + n_le));
        }
    }


    if constexpr (is_avx_register<T_RegisterType>)
    {
        if constexpr (is_m256d<T_RegisterType>)
            c = permute<v[0], v[1], v[1], v[0]>(a);
        else
            c = permute<v[0], v[1], v[2], v[3], v[1], v[2], v[3], v[0]>(a);

        for (UST i = 0; i < n_le; ++i)
        {
            EXPECT_DOUBLE_EQ(get(c, i), get(a, v.at(i)));
            EXPECT_DOUBLE_EQ(get(c, i + n_le), get(a, v.at((i + 1) % n_le) + n_le));
        }
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_permute) // NOLINT
{
    TYPED_TEST_SERIES(test_permute_test_case, power(num_lane_elements<TypeParam>, num_lane_elements<TypeParam>))
}


// --- test permute_lanes ---------------------------------------------------------------------------------------------

template <typename T_RegisterType, UST t_test_index>
void test_permute_lanes_test_case(T_RegisterType a, [[maybe_unused]] T_RegisterType b) // NOLINT - complexity
{
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    constexpr UST lane_0 = is_bit_set(t_test_index, 1) ? 1 : 0;
    constexpr UST lane_1 = is_bit_set(t_test_index, 1) ? 1 : 0;

    auto c = permute_lanes<lane_0, lane_1>(a);

    for (UST i = 0; i < n_le; ++i)
    {
        UST lane_index_0 = lane_0 * n_le + i;
        UST lane_index_1 = lane_1 * n_le + i;

        EXPECT_DOUBLE_EQ(get(c, i), get(a, lane_index_0));
        EXPECT_DOUBLE_EQ(get(c, i + n_le), get(a, lane_index_1));
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_permute_lanes) // NOLINT
{
    if constexpr (is_avx_register<TypeParam>)
    {
        TYPED_TEST_SERIES(test_permute_lanes_test_case, 4)
    }
}


// --- test shuffle ---------------------------------------------------------------------------------------------------

template <typename T_RegisterType, UST t_test_index>
void test_shuffle_test_case(T_RegisterType a, T_RegisterType b) // NOLINT - complexity
{
    std::cout << std::endl;
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_shuffle) // NOLINT
{
    TYPED_TEST_SERIES(test_shuffle_test_case, 4)
}


// --- test shuffle_lanes ---------------------------------------------------------------------------------------------

template <typename T_RegisterType, UST t_test_index>
void test_shuffle_lanes_test_case(T_RegisterType a, T_RegisterType b) // NOLINT - complexity
{
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    constexpr UST src_0  = is_bit_set(t_test_index, 0) ? 1 : 0;
    constexpr UST src_1  = is_bit_set(t_test_index, 1) ? 1 : 0;
    constexpr UST lane_0 = is_bit_set(t_test_index, 2) ? 1 : 0;
    constexpr UST lane_1 = is_bit_set(t_test_index, 3) ? 1 : 0;

    auto c = shuffle_lanes<src_0, lane_0, src_1, lane_1>(a, b);

    for (UST i = 0; i < n_le; ++i)
    {
        UST            lane_index_0 = lane_0 * n_le + i;
        UST            lane_index_1 = lane_1 * n_le + i;
        T_RegisterType src_lane_0   = (src_0 == 0) ? a : b;
        T_RegisterType src_lane_1   = (src_1 == 0) ? a : b;

        EXPECT_DOUBLE_EQ(get(c, i), get(src_lane_0, lane_index_0));
        EXPECT_DOUBLE_EQ(get(c, i + n_le), get(src_lane_1, lane_index_1));
    }
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_shuffle_lanes) // NOLINT
{
    if constexpr (is_avx_register<TypeParam>)
    {
        TYPED_TEST_SERIES(test_shuffle_lanes_test_case, power(2, 4))
    }
}


// --- test swap_lanes ------------------------------------------------------------------------------------------------

TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_swap_lanes) // NOLINT
{
    if constexpr (is_avx_register<TypeParam>)
    {
        constexpr UST n_le = num_lane_elements<TypeParam>;

        auto a = mm_setzero<TypeParam>();
        for (UST i = 0; i < num_elements<TypeParam>; ++i)
            set(a, i, static_cast<ElementType<TypeParam>>(i + 1));


        auto c = swap_lanes(a);

        for (UST i = 0; i < n_le; ++i)
        {
            EXPECT_DOUBLE_EQ(get(c, i), get(a, i + n_le));
            EXPECT_DOUBLE_EQ(get(c, i + n_le), get(a, i));
        }
    }
}
