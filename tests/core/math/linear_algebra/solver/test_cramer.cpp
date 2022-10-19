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

// multiple sizes of the right-hand side are tested to cover all branches

TYPED_TEST(SolverTestSuite, cramer_multiple_rhs_2x2_8) // NOLINT
{
    UST tc_idx = 0;
    for (const auto& tc : get_solver_testcases_multiple_rhs_2x2<TypeParam, 8>())
    {
        auto result = Cramer::solve_multiple_rhs(tc.mat(), tc.rhs());
        tc.check_result_testcase(result, tc_idx);
        ++tc_idx;
    }
}


TYPED_TEST(SolverTestSuite, cramer_multiple_rhs_2x2_9) // NOLINT
{
    UST tc_idx = 0;
    for (const auto& tc : get_solver_testcases_multiple_rhs_2x2<TypeParam, 9>())
    {
        auto result = Cramer::solve_multiple_rhs(tc.mat(), tc.rhs());
        tc.check_result_testcase(result, tc_idx);
        ++tc_idx;
    }
}


TYPED_TEST(SolverTestSuite, cramer_multiple_rhs_2x2_10) // NOLINT
{
    UST tc_idx = 0;
    for (const auto& tc : get_solver_testcases_multiple_rhs_2x2<TypeParam, 10>())
    {
        auto result = Cramer::solve_multiple_rhs(tc.mat(), tc.rhs());
        tc.check_result_testcase(result, tc_idx);
        ++tc_idx;
    }
}


TYPED_TEST(SolverTestSuite, cramer_multiple_rhs_2x2_11) // NOLINT
{
    UST tc_idx = 0;
    for (const auto& tc : get_solver_testcases_multiple_rhs_2x2<TypeParam, 11>())
    {
        auto result = Cramer::solve_multiple_rhs(tc.mat(), tc.rhs());
        tc.check_result_testcase(result, tc_idx);
        ++tc_idx;
    }
}


// --------------------------------------------------------------------------------------------------------------------

// multiple sizes of the right-hand side are tested to cover all branches

TYPED_TEST(SolverTestSuite, cramer_multiple_rhs_3x3_8) // NOLINT
{
    if constexpr (! x86::is_m128d<TypeParam> && ! x86::is_m256d<TypeParam>)
    {
        UST tc_idx = 0;
        for (const auto& tc : get_solver_testcases_multiple_rhs_3x3<TypeParam, 8>())
        {
            auto result = Cramer::solve_multiple_rhs(tc.mat(), tc.rhs());
            tc.check_result_testcase(result, tc_idx);
            ++tc_idx;
        }
    }
}


TYPED_TEST(SolverTestSuite, cramer_multiple_rhs_3x3_9) // NOLINT
{
    if constexpr (! x86::is_m128d<TypeParam>)
    {
        UST tc_idx = 0;
        for (const auto& tc : get_solver_testcases_multiple_rhs_3x3<TypeParam, 9>())
        {
            auto result = Cramer::solve_multiple_rhs(tc.mat(), tc.rhs());
            tc.check_result_testcase(result, tc_idx);
            ++tc_idx;
        }
    }
}


// --------------------------------------------------------------------------------------------------------------------

// multiple sizes of the right-hand side are tested to cover all branches

TYPED_TEST(SolverTestSuite, cramer_multiple_rhs_4x4_8) // NOLINT
{
    if constexpr (! x86::is_vector_register<TypeParam>)
    {
        UST tc_idx = 0;
        for (const auto& tc : get_solver_testcases_multiple_rhs_4x4<TypeParam, 8>())
        {
            auto result = Cramer::solve_multiple_rhs(tc.mat(), tc.rhs());
            tc.check_result_testcase(result, tc_idx);
            ++tc_idx;
        }
    }
}
