#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/bit_operations.h"
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

template <typename T_Type>
class TestFloatingPointVectorRegisterTypes : public ::testing::Test
{
};


using VectorRegisterTestTypes = ::testing::Types<__m128, __m128d, __m256, __m256d>; // NOLINT


// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(TestFloatingPointVectorRegisterTypes, VectorRegisterTestTypes, );


// NOLINTNEXTLINE
#define CALL_TEST_CASE_FUNC(func_name) func_name<TypeParam, t_index>()

// NOLINTNEXTLINE
#define TYPED_TEST_SERIES(test_func, num_test_cases)                                                                   \
    auto start_typed_test_series = []()                                                                                \
    {                                                                                                                  \
        auto test_series = []<UST... t_index>([[maybe_unused]] std::index_sequence<t_index...> seq)                    \
        {                                                                                                              \
            (void) std::initializer_list<I32>{(CALL_TEST_CASE_FUNC(test_func), 0)...};                                 \
        };                                                                                                             \
        test_series(std::make_index_sequence<num_test_cases>());                                                       \
    };                                                                                                                 \
    start_typed_test_series();


// ====================================================================================================================
// Tests
// ====================================================================================================================


// --- test_align_right -----------------------------------------------------------------------------------------------

//! A single test case for a specific `t_shift`.
template <typename T_RegisterType, U32 t_shift>
void test_align_right_test_case()
{
    constexpr UST n_e  = num_elements<T_RegisterType>;
    constexpr UST n_l  = num_lanes<T_RegisterType>;
    constexpr UST n_le = num_lane_elements<T_RegisterType>;


    // create source registers
    auto a = mm_setzero<T_RegisterType>();
    auto b = mm_setzero<T_RegisterType>();


    // set source register values
    for (UST i = 0; i < n_e; ++i)
    {
        set(a, i, static_cast<ElementType<T_RegisterType>>(i + 1));
        set(b, i, static_cast<ElementType<T_RegisterType>>(i + 1 + n_e));
    }


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

//! Calculate the template indices passed to `blend` depending on the test_case_index.
template <typename T_RegisterType>
[[nodiscard]] constexpr inline auto get_blend_index_array(U32 test_case_index) noexcept
{
    std::array<U32, num_elements<T_RegisterType>> a = {{0}};
    for (UST i = 0; i < a.size(); ++i)
        a.at(i) = is_bit_set(test_case_index, i) ? 1 : 0;
    return a;
}


//! A single test case for a specific template parameter combination derived from `t_test_case_index`.
template <typename T_RegisterType, U32 t_test_case_index>
void test_blend_test_case()
{
    // create source registers
    auto a = mm_setzero<T_RegisterType>();
    auto b = mm_setzero<T_RegisterType>();


    // set source register values
    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
    {
        set(a, i, static_cast<ElementType<T_RegisterType>>(i + 1));
        set(b, i, static_cast<ElementType<T_RegisterType>>(i + 1 + num_elements<T_RegisterType>));
    }


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


//! A single test case for a specific template parameter combination derived from `t_test_case_index`.
template <typename T_RegisterType, U32 t_index>
void test_blend_above_test_case()
{
    // create source registers
    auto a = mm_setzero<T_RegisterType>();
    auto b = mm_setzero<T_RegisterType>();


    // set source register values
    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
    {
        set(a, i, static_cast<ElementType<T_RegisterType>>(i + 1));
        set(b, i, static_cast<ElementType<T_RegisterType>>(i + 1 + num_elements<T_RegisterType>));
    }

    T_RegisterType c = blend_above<t_index>(a, b);
    for (UST i = 0; i < num_elements<T_RegisterType>; ++i)
    {
        auto exp = (i > t_index) ? get(b, i) : get(a, i);
        EXPECT_DOUBLE_EQ(get(c, i), exp);
    }
    // std::cout << get(c, i);
    //    std::cout << std::endl;
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_blend_above) // NOLINT
{
    TYPED_TEST_SERIES(test_blend_above_test_case, num_elements<TypeParam>)
}


// --- test_blend_at --------------------------------------------------------------------------------------------------

template <typename T_RegisterType, U32 t_test_case_index>
void test_blend_at_test_case()
{
    std::cout << t_test_case_index << std::endl;
}


TYPED_TEST(TestFloatingPointVectorRegisterTypes, test_blend_at) // NOLINT
{
    TYPED_TEST_SERIES(test_blend_at_test_case, num_elements<TypeParam>)
}
