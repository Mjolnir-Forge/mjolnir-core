//! @file
//! testing/math/solver_testing.h
//!
//! @brief
//! Contains classes and functions to test linear solvers.


#pragma once


// === DECLARATIONS ===================================================================================================


#include "mjolnir/core/definitions.h"
#include <gtest/gtest.h>

#include <array>
#include <vector>

namespace mjolnir
{
//! \addtogroup testing
//! @{


//! @brief
//! Class that defines a single testcase for linear solvers that use scalar types as fundamental data type.
//!
//! @details
//! A testcase consists of the matrix, the right-hand side and the expected solution of a linear system of equations.
//!
//! @tparam T_Type:
//! Basic data type used by the solver
//! @tparam t_size:
//! Size of the system
template <Number T_Type, UST t_size>
class SolverTestcase
{
    const std::array<T_Type, t_size * t_size> m_mat;
    const std::array<T_Type, t_size>          m_exp;
    const std::array<T_Type, t_size>          m_rhs;

public:
    //! @brief
    //! Constructor.
    //!
    //! @param mat:
    //! The matrix of the system. The data is expected to be in column major format.
    //! @param exp:
    //! The expected solution of the system
    //! @param rhs:
    //! The right-hand side vector
    //!
    //! @note
    //! The order of those parameters is chosen to match the order as they appear in the linear equation
    //! `mat * exp = rhs`.
    SolverTestcase(const std::array<T_Type, t_size * t_size>& mat,
                   const std::array<T_Type, t_size>&          exp,
                   const std::array<T_Type, t_size>&          rhs);


    //! @brief
    //! Get the expected solution vector.
    //!
    //! @return
    //! Expected solution vector
    [[nodiscard]] auto exp() const noexcept -> const std::array<T_Type, t_size>&;


    //! @brief
    //! Get the systems matrix.
    //!
    //! @return
    //! Matrix of the system
    [[nodiscard]] auto mat() const noexcept -> const std::array<T_Type, t_size * t_size>&;

    //! @brief
    //! Get the right-hand side vector.
    //!
    //! @return
    //! Right-hand side vector
    [[nodiscard]] auto rhs() const noexcept -> const std::array<T_Type, t_size>&;


    //! @brief
    //! Check the result vector.
    //!
    //! @details
    //! The function uses the `EXPECT_DOUBLE_EQ` macro of `gtest.h`. If a wrong value is detected, the index of the
    //! value is added to the failure message.
    //!
    //! @param result:
    //! The calculated result vector.
    //! @param message_prefix:
    //! An optional string that is added in front of the message that prints the index of the failed comparison.
    void check_result(const std::array<T_Type, t_size>& result, const std::string& message_prefix = "") const noexcept;


    //! @brief
    //! Check the result vector.
    //!
    //! @details
    //! The function uses the `EXPECT_DOUBLE_EQ` macro of `gtest.h`. If a wrong value is detected, the index of the
    //! value is added to the failure message together with the index of the testcase.
    //!
    //! @param result:
    //! The calculated result vector.
    //! @param testcase_index:
    //! The number of the testcase.
    void check_result_testcase(const std::array<T_Type, t_size>& result, UST testcase_index) const noexcept;
};


// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! Get a vector of testcases for 2x2 solvers.
//!
//! @return
//! Vector of testcases for 2x2 solvers
template <Number T_Type>
[[nodiscard]] auto get_solver_testcases_2x2() -> std::vector<SolverTestcase<T_Type, 2>>;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
template <Number T_Type, UST t_size>
SolverTestcase<T_Type, t_size>::SolverTestcase(const std::array<T_Type, t_size * t_size>& mat,
                                               const std::array<T_Type, t_size>&          exp,
                                               const std::array<T_Type, t_size>&          rhs)
    : m_mat(mat)
    , m_exp(exp)
    , m_rhs(rhs)
{
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type, UST t_size>
[[nodiscard]] inline auto SolverTestcase<T_Type, t_size>::mat() const noexcept
        -> const std::array<T_Type, t_size * t_size>&
{
    return m_mat;
}

// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type, UST t_size>
[[nodiscard]] inline auto SolverTestcase<T_Type, t_size>::rhs() const noexcept -> const std::array<T_Type, t_size>&
{
    return m_rhs;
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type, UST t_size>
[[nodiscard]] inline auto SolverTestcase<T_Type, t_size>::exp() const noexcept -> const std::array<T_Type, t_size>&
{
    return m_exp;
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type, UST t_size>
inline void SolverTestcase<T_Type, t_size>::check_result(const std::array<T_Type, t_size>& result,
                                                         const std::string& message_prefix) const noexcept
{
    for (UST i = 0; i < 2; ++i)
        EXPECT_DOUBLE_EQ(result.at(i), m_exp.at(i)) << message_prefix << "value index: " << i << "\n";
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type, UST t_size>
inline void SolverTestcase<T_Type, t_size>::check_result_testcase(const std::array<T_Type, t_size>& result,
                                                                  UST testcase_index) const noexcept
{
    const std::string prefix = "testcase index: " + std::to_string(testcase_index) + " --- ";
    check_result(result, prefix);
}


// --- Free functions -------------------------------------------------------------------------------------------------

template <Number T_Type>
[[nodiscard]] inline auto get_solver_testcases_2x2() -> std::vector<SolverTestcase<T_Type, 2>>
{
    using STC = SolverTestcase<T_Type, 2>;

    std::vector<STC> testcases = {};

    // https://elsenaju.eu/Calculator/system-of-equations-2x2.htm
    testcases.emplace_back(STC({{1., 0., 0., 1.}}, {{2., 3.}}, {{2., 3.}}));   // NOLINT(readability-magic-numbers)
    testcases.emplace_back(STC({{3., 5., 2., 4.}}, {{1., -0.5}}, {{2., 3.}})); // NOLINT(readability-magic-numbers)

    return testcases;
}

} // namespace mjolnir
