#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/core/x86/element_summation.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/x86.h"
#include "mjolnir/testing/x86/floating_point_vector_register_test_suite.h"
#include <gtest/gtest.h>

#include <array>

using namespace mjolnir;
using namespace mjolnir::x86;

// ====================================================================================================================
// Setup
// ====================================================================================================================


// ====================================================================================================================
// Tests
// ====================================================================================================================

// --------------------------------------------------------------------------------------------------------------------


TYPED_TEST(FloatingPointVectorRegisterTestSuite, test_broadcast_element_sum) // NOLINT
{
    using EType       = ElementType<TypeParam>;
    constexpr UST n_e = num_elements<TypeParam>;

    auto src = mm_setzero<TypeParam>();

    EType exp{0.};

    for (UST i = 0; i < n_e; ++i)
    {
        auto val = static_cast<EType>((3 * (i + 1)) % 8); // NOLINT(readability-magic-numbers)
        set(src, i, val);
        exp += val;
    }

    TypeParam res = broadcast_element_sum(src);

    for (UST i = 0; i < n_e; ++i)
        EXPECT_DOUBLE_EQ(get(res, i), exp);
}


// --------------------------------------------------------------------------------------------------------------------


TYPED_TEST(FloatingPointVectorRegisterTestSuite, test_element_sum) // NOLINT
{
    using EType       = ElementType<TypeParam>;
    constexpr UST n_e = num_elements<TypeParam>;

    auto src = mm_setzero<TypeParam>();

    EType exp{0.};

    for (UST i = 0; i < n_e; ++i)
    {
        auto val = static_cast<EType>((7 * (i + 1)) % 10); // NOLINT(readability-magic-numbers)
        set(src, i, val);
        exp += val;
    }

    EType res = element_sum(src);

    EXPECT_DOUBLE_EQ(res, exp);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_elements, FloatVectorRegister T_RegisterType>
void element_sum_first_n_testcase()
{
    using EType       = ElementType<T_RegisterType>;
    constexpr UST n_e = num_elements<T_RegisterType>;

    auto src = mm_setzero<T_RegisterType>();

    EType exp{0.};

    for (UST i = 0; i < n_e; ++i)
    {
        auto val = static_cast<EType>((7 * (i + 1)) % 10); // NOLINT(readability-magic-numbers)
        set(src, i, val);
        if (i < t_num_elements)
            exp += val;
    }

    EType res = element_sum_first_n<t_num_elements>(src);

    EXPECT_DOUBLE_EQ(res, exp);
}


TYPED_TEST(FloatingPointVectorRegisterTestSuite, test_element_sum_first_n) // NOLINT
{
    constexpr UST n_e = num_elements<TypeParam>;

    element_sum_first_n_testcase<1, TypeParam>();
    element_sum_first_n_testcase<2, TypeParam>();
    if constexpr (n_e > 2)
    {
        element_sum_first_n_testcase<3, TypeParam>();
        element_sum_first_n_testcase<4, TypeParam>();
    }
    if constexpr (n_e > 4) // NOLINT(readability-misleading-indentation)
    {
        element_sum_first_n_testcase<5, TypeParam>(); // NOLINT(readability-magic-numbers)
        element_sum_first_n_testcase<6, TypeParam>(); // NOLINT(readability-magic-numbers)
        element_sum_first_n_testcase<7, TypeParam>(); // NOLINT(readability-magic-numbers)
        element_sum_first_n_testcase<8, TypeParam>(); // NOLINT(readability-magic-numbers)
    }
}
