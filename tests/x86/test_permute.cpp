#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permute.h"
#include <gtest/gtest.h>


using namespace mjolnir;
using namespace mjolnir::x86;

// ====================================================================================================================
// Setup
// ====================================================================================================================

// create test suites -------------------------------------------------------------------------------------------------

template <typename T_Type>
class TypedTest : public ::testing::Test
{
};


using VectorRegisterTestTypes = ::testing::Types<__m128, __m128d, __m256, __m256d>; // NOLINT


// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(TypedTest, VectorRegisterTestTypes, );


// ====================================================================================================================
// Tests
// ====================================================================================================================

// --------------------------------------------------------------------------------------------------------------------

template <U32 t_i, U32 t_j>
void test_blend()
{
    std::cout << t_i << "-" << t_j << std::endl;
    const __m128d a = _mm_setr_pd(1., 2.);
    const __m128d b = _mm_setr_pd(3., 4.);

    const __m128d c = blend<t_i, t_j>(a, b);

    EXPECT_DOUBLE_EQ(get<0>(c), ((t_i == 0) ? 1. : 3.));
    EXPECT_DOUBLE_EQ(get<1>(c), ((t_j == 0) ? 2. : 4.));
}

#include <algorithm>
#include <cstdint>
#include <utility>

template <UST... I>
auto test(std::index_sequence<I...>)
{
    std::initializer_list<I32>{(test_blend<I / 2, I % 2>(), 0)...};
}

TEST(Blend, __m128d) // NOLINT
{
    test(std::make_index_sequence<4>());
}
