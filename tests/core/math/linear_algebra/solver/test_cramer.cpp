#include "mjolnir/core/math/linear_algebra/solver/cramer.h"
#include "mjolnir/testing/math/solver_testing.h"
#include <gtest/gtest.h>

#include <vector>

using namespace mjolnir;


// ====================================================================================================================
// Setup
// ====================================================================================================================


// --- test suite -----------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
class SolverTestSuite : public ::testing::Test
{
};

using SolverTestTypes = ::testing::Types<F32, F64, __m128, __m128d, __m256, __m256d>; // NOLINT

// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(SolverTestSuite, SolverTestTypes, );


// ====================================================================================================================
// Tests
// ====================================================================================================================

TYPED_TEST(SolverTestSuite, cramer_2x2) // NOLINT
{
    UST tc_idx = 0;
    for (const auto& tc : get_solver_testcases_2x2<TypeParam>())
    {
        auto x = Cramer::solve(tc.mat(), tc.rhs());
        tc.check_result_testcase(x, tc_idx);
        ++tc_idx;
    }
}


// --------------------------------------------------------------------------------------------------------------------

TYPED_TEST(SolverTestSuite, cramer_3x3) // NOLINT
{
    if constexpr (! x86::is_m128d<TypeParam>)
    {
        UST tc_idx = 0;
        for (const auto& tc : get_solver_testcases_3x3<TypeParam>())
        {
            auto x = Cramer::solve(tc.mat(), tc.rhs());
            tc.check_result_testcase(x, tc_idx);
            ++tc_idx;
        }
    }
}


// --------------------------------------------------------------------------------------------------------------------

TYPED_TEST(SolverTestSuite, cramer_4x4) // NOLINT
{
    if constexpr (! x86::is_m128d<TypeParam>)
    {
        UST tc_idx = 0;
        for (const auto& tc : get_solver_testcases_4x4<TypeParam>())
        {
            auto x = Cramer::solve(tc.mat(), tc.rhs());
            tc.check_result_testcase(x, tc_idx);
            ++tc_idx;
        }
    }
}


// --------------------------------------------------------------------------------------------------------------------

TYPED_TEST(SolverTestSuite, cramer_multiple_rhs_4x4) // NOLINT
{
    if constexpr (! x86::is_vector_register<TypeParam>)
    {
        std::array<TypeParam, 4>                mat = {{1., 3., -2., 5.}};
        std::array<std::array<TypeParam, 2>, 5> exp = {{{1., 2.}, {4., -5.}, {3., 1.}, {6., 6.}, {2., -3.}}};
        std::array<std::array<TypeParam, 2>, 5> rhs = {{{-3., 13.}, {14., -13.}, {1., 14.}, {-6., 48.}, {8., -9.}}};

        auto results = Cramer::solve_multiple_rhs(mat, rhs);

        for (UST i = 0; i < 5; ++i)
            for (UST j = 0; j < 2; ++j)
                EXPECT_DOUBLE_EQ(results[i][j], exp[i][j]);
    }
}
