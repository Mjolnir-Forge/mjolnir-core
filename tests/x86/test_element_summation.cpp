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
    if constexpr (not is_m256d<TypeParam>)
    {
        using EType       = ElementType<TypeParam>;
        constexpr UST n_e = num_elements<TypeParam>;

        auto src = mm_setzero<TypeParam>();

        EType exp{0.};

        for (UST i = 0; i < n_e; ++i)
        {
            auto val = static_cast<EType>((3 * (i + 1)) % 8); // NOLINT - magic number
            set(src, i, val);
            exp += val;
        }

        TypeParam res = broadcast_element_sum(src);

        for (UST i = 0; i < n_e; ++i)
            EXPECT_DOUBLE_EQ(get(res, i), exp);
    }
}


// --------------------------------------------------------------------------------------------------------------------
