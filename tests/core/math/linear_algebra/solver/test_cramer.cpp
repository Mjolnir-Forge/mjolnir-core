#include "mjolnir/core/math/linear_algebra/solver/cramer.h"
#include <gtest/gtest.h>

#include <vector>

using namespace mjolnir;
// using namespace mjolnir::x86;


template <Number T_Type, UST t_size>
class SolverTestcase
{
    const std::array<T_Type, t_size * t_size> m_mat;
    const std::array<T_Type, t_size>          m_exp;
    const std::array<T_Type, t_size>          m_rhs;

public:
    SolverTestcase(const std::array<T_Type, t_size * t_size>& mat,
                   const std::array<T_Type, t_size>&          exp,
                   const std::array<T_Type, t_size>&          rhs)
        : m_mat(mat)
        , m_exp(exp)
        , m_rhs(rhs)
    {
    }

    [[nodiscard]] auto mat() const noexcept -> const std::array<T_Type, t_size * t_size>&
    {
        return m_mat;
    }


    [[nodiscard]] auto rhs() const noexcept -> const std::array<T_Type, t_size>&
    {
        return m_rhs;
    }

    [[nodiscard]] auto exp() const noexcept -> const std::array<T_Type, t_size>&
    {
        return m_exp;
    }


    void check_result(const std::array<T_Type, t_size>& result, const std::string& message_prefix) const noexcept
    {
        for (UST i = 0; i < 2; ++i)
            EXPECT_DOUBLE_EQ(result.at(i), m_exp.at(i)) << message_prefix << "value index: " << i << "\n";
    }
};

template <Number T_Type>
[[nodiscard]] auto get_solver_2x2_testcases() -> std::vector<SolverTestcase<T_Type, 2>>
{
    using STC = SolverTestcase<T_Type, 2>;

    std::vector<STC> testcases = {};

    testcases.emplace_back(STC({{1., 0., 0., 1.}}, {{2., 3.}}, {{2., 3.}}));   // NOLINT(readability-magic-numbers)
    testcases.emplace_back(STC({{3., 5., 2., 4.}}, {{1., -0.5}}, {{2., 3.}})); // NOLINT(readability-magic-numbers)

    return testcases;
}


// ====================================================================================================================
// Setup
// ====================================================================================================================

// --- test suite -----------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
class SolverTestSuite : public ::testing::Test
{
};

using SolverTestTypes = ::testing::Types<F32, F64>; // NOLINT

// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(SolverTestSuite, SolverTestTypes, );


// https://elsenaju.eu/Calculator/system-of-equations-2x2.htm
TYPED_TEST(SolverTestSuite, cramer_2x2) // NOLINT
{
    UST tc_idx = 0;
    for (const auto& tc : get_solver_2x2_testcases<TypeParam>())
    {
        auto x = cramer(tc.mat(), tc.rhs());
        tc.check_result(x, "testcase index: " + std::to_string(tc_idx) + " --- ");

        ++tc_idx;
    }
}
