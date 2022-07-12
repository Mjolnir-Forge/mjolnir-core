#include "mjolnir/core/math/linear_algebra/vector_products.h"
#include "mjolnir/core/x86/direct_access.h"
#include "mjolnir/testing/x86/floating_point_vector_register_test_suite.h"
#include <gtest/gtest.h>

// ====================================================================================================================
// Setup
// ====================================================================================================================

using namespace mjolnir;
using namespace mjolnir::x86;


// --- test suite -----------------------------------------------------------------------------------------------------

template <mjolnir::Number T_RegisterType>
class VectorProductTestSuite : public ::testing::Test
{
};


using VectorProductTestTypes = ::testing::Types<F32, F64>; // NOLINT

TYPED_TEST_SUITE(VectorProductTestSuite, VectorProductTestTypes, );


// ====================================================================================================================
// Tests
// ====================================================================================================================


// --- test dot_product (serial version) --------------------------------------------------------------------------

TYPED_TEST(VectorProductTestSuite, dot_product) // NOLINT
{
    EXPECT_DOUBLE_EQ((dot_product<2, TypeParam>({{0., 0.}}, {{2., 5.}})), 0.);
    EXPECT_DOUBLE_EQ((dot_product<2, TypeParam>({{2., 3.}}, {{4., 4.}})), 20.);
    EXPECT_DOUBLE_EQ((dot_product<2, TypeParam>({{-4., 3.}}, {{6., 2.}})), -18.);

    EXPECT_DOUBLE_EQ((dot_product<3, TypeParam>({{0., 0., 0.}}, {{2., 5., -2.}})), 0.);
    EXPECT_DOUBLE_EQ((dot_product<3, TypeParam>({{2., 3., 1.}}, {{4., 4., 2.}})), 22.);
    EXPECT_DOUBLE_EQ((dot_product<3, TypeParam>({{-4., 3., 4.}}, {{6., 2., -3.}})), -30.);

    EXPECT_DOUBLE_EQ((dot_product<4, TypeParam>({{0., 0., 0., 0.}}, {{2., 5., -2., 7.}})), 0.);
    EXPECT_DOUBLE_EQ((dot_product<4, TypeParam>({{2., 3., 1., 5.}}, {{4., 4., 2., 2.}})), 32.);
    EXPECT_DOUBLE_EQ((dot_product<4, TypeParam>({{-4., 3., 4., -5.}}, {{6., 2., -3., -2.}})), -20.);
}


// --- test dot_product with vector size 2(vectorized) ----------------------------------------------------------------

TYPED_TEST(FloatingPointVectorRegisterTestSuite, dot_product_size_2) // NOLINT
{
    auto a = mm_setzero<TypeParam>();
    auto b = mm_setzero<TypeParam>();

    set<0>(a, 0.);
    set<1>(a, 0.);
    set<0>(b, 2.);
    set<1>(b, 5.);

    EXPECT_DOUBLE_EQ(dot_product<2>(a, b), 0.);


    set<0>(a, 2.);
    set<1>(a, 3.);
    set<0>(b, 4.);
    set<1>(b, 4.);

    EXPECT_DOUBLE_EQ(dot_product<2>(a, b), 22.);


    set<0>(a, -4.);
    set<1>(a, 3.);
    set<0>(b, 6.);
    set<1>(b, 2.);

    EXPECT_DOUBLE_EQ(dot_product<2>(a, b), -18.);
}
