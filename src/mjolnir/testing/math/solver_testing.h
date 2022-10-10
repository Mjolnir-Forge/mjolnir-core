//! @file
//! testing/math/solver_testing.h
//!
//! @brief
//! Contains classes and functions to test linear solvers.


#pragma once


// === DECLARATIONS ===================================================================================================


#include "mjolnir/core/definitions.h"
#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/direct_access.h"
#include <gtest/gtest.h>

#include <array>
#include <vector>

namespace mjolnir
{
//! \addtogroup testing
//! @{


//! @brief
//! Class that defines a single testcase for linear solvers.
//!
//! @details
//! A testcase consists of the matrix, the right-hand side and the expected solution of a linear system of equations.
//!
//! @tparam T_Type:
//! Basic data type used by the solver
//! @tparam t_size:
//! Size of the system
template <typename T_Type, UST t_size>
class SolverTestcase
{
    using ScalarType = typename std::conditional_t<x86::is_float_register<T_Type>, F64, T_Type>;
    using VectorType = typename std::conditional_t<x86::is_float_register<T_Type>, T_Type, std::array<T_Type, t_size>>;
    using MatrixType = typename std::conditional_t<x86::is_float_register<T_Type>,
                                                   std::array<T_Type, t_size>,
                                                   std::array<T_Type, t_size * t_size>>;


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
    SolverTestcase(const std::array<ScalarType, t_size * t_size>& mat,
                   const std::array<ScalarType, t_size>&          exp,
                   const std::array<ScalarType, t_size>&          rhs);


    //! @brief
    //! Get the expected solution vector.
    //!
    //! @return
    //! Expected solution vector
    [[nodiscard]] auto exp() const noexcept -> const VectorType&;


    //! @brief
    //! Get the systems matrix.
    //!
    //! @return
    //! Matrix of the system
    [[nodiscard]] auto mat() const noexcept -> const MatrixType&;

    //! @brief
    //! Get the right-hand side vector.
    //!
    //! @return
    //! Right-hand side vector
    [[nodiscard]] auto rhs() const noexcept -> const VectorType&;


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
    void check_result(const VectorType& result, const std::string& message_prefix = "") const noexcept;


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
    void check_result_testcase(const VectorType& result, UST testcase_index) const noexcept;

private:
    [[nodiscard]] static auto format_matrix(const std::array<ScalarType, t_size * t_size>& mat) noexcept -> MatrixType;

    [[nodiscard]] static auto format_vector(const std::array<ScalarType, t_size>& vec) noexcept -> VectorType;

    const MatrixType m_mat;
    const VectorType m_exp;
    const VectorType m_rhs;
};


// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! Get a vector of testcases for 2x2 solvers.
//!
//! @return
//! Vector of testcases for 2x2 solvers
template <typename T_Type>
[[nodiscard]] auto get_solver_testcases_2x2() -> std::vector<SolverTestcase<T_Type, 2>>;


//! @brief
//! Get a vector of testcases for 3x3 solvers.
//!
//! @return
//! Vector of testcases for 3x3 solvers
template <typename T_Type>
[[nodiscard]] auto get_solver_testcases_3x3() -> std::vector<SolverTestcase<T_Type, 3>>;


//! @brief
//! Get a vector of testcases for 4x4 solvers.
//!
//! @return
//! Vector of testcases for 4x4 solvers
template <typename T_Type>
[[nodiscard]] auto get_solver_testcases_4x4() -> std::vector<SolverTestcase<T_Type, 4>>;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
template <typename T_Type, UST t_size>
SolverTestcase<T_Type, t_size>::SolverTestcase(const std::array<ScalarType, t_size * t_size>& mat,
                                               const std::array<ScalarType, t_size>&          exp,
                                               const std::array<ScalarType, t_size>&          rhs)
    : m_mat(format_matrix(mat))
    , m_exp(format_vector(exp))
    , m_rhs(format_vector(rhs))
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, UST t_size>
[[nodiscard]] inline auto SolverTestcase<T_Type, t_size>::mat() const noexcept -> const MatrixType&
{
    return m_mat;
}

// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, UST t_size>
[[nodiscard]] inline auto SolverTestcase<T_Type, t_size>::rhs() const noexcept -> const VectorType&
{
    return m_rhs;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, UST t_size>
[[nodiscard]] inline auto SolverTestcase<T_Type, t_size>::exp() const noexcept -> const VectorType&
{
    return m_exp;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, UST t_size>
inline void SolverTestcase<T_Type, t_size>::check_result(const VectorType&  result,
                                                         const std::string& message_prefix) const noexcept
{
    using namespace x86;
    if constexpr (is_float_register<T_Type>)
    {
        for (UST i = 0; i < t_size; ++i)
            EXPECT_DOUBLE_EQ(get(result, i), get(m_exp, i)) << message_prefix << "value index: " << i << "\n";
    }
    else
    {
        for (UST i = 0; i < t_size; ++i)
            EXPECT_DOUBLE_EQ(result.at(i), m_exp.at(i)) << message_prefix << "value index: " << i << "\n";
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, UST t_size>
inline void SolverTestcase<T_Type, t_size>::check_result_testcase(const VectorType& result,
                                                                  UST               testcase_index) const noexcept
{
    const std::string prefix = "testcase index: " + std::to_string(testcase_index) + " --- ";
    check_result(result, prefix);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, UST t_size>
inline auto SolverTestcase<T_Type, t_size>::format_matrix(const std::array<ScalarType, t_size * t_size>& mat) noexcept
        -> MatrixType
{
    using namespace x86;
    if constexpr (is_float_register<T_Type>)
    {
        MatrixType mat_reg = {{0}};
        for (UST i = 0; i < t_size; ++i)
            for (UST j = 0; j < t_size; ++j)
            {
                UST arr_idx = i * t_size + j;
                set(mat_reg.at(i), j, mat.at(arr_idx));
            }
        return mat_reg;
    }
    else
        return mat;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, UST t_size>
inline auto SolverTestcase<T_Type, t_size>::format_vector(const std::array<ScalarType, t_size>& vec) noexcept
        -> VectorType
{
    using namespace x86;
    if constexpr (is_float_register<T_Type>)
    {
        VectorType vec_reg = {0};
        for (UST i = 0; i < t_size; ++i)
            set(vec_reg, i, vec.at(i));
        return vec_reg;
    }
    else
        return vec;
}


// --- Free functions -------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] inline auto get_solver_testcases_2x2() -> std::vector<SolverTestcase<T_Type, 2>>
{
    using ST = SolverTestcase<T_Type, 2>;

    std::vector<ST> testcases = {};

    testcases.emplace_back(ST({{1., 0., 0., 1.}}, {{2., 3.}}, {{2., 3.}}));         // NOLINT(readability-magic-numbers)
    testcases.emplace_back(ST({{3., 5., 2., 4.}}, {{1., -0.5}}, {{2., 3.}}));       // NOLINT(readability-magic-numbers)
    testcases.emplace_back(ST({{4., 1., 6., 3.}}, {{2., 1.}}, {{14., 5.}}));        // NOLINT(readability-magic-numbers)
    testcases.emplace_back(ST({{3., -2., 1., 4.}}, {{2., 4.}}, {{10., 12.}}));      // NOLINT(readability-magic-numbers)
    testcases.emplace_back(ST({{-5., -7., 2., -1.}}, {{5., -3.}}, {{-31., -32.}})); // NOLINT(readability-magic-numbers)

    return testcases;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] inline auto get_solver_testcases_3x3() -> std::vector<SolverTestcase<T_Type, 3>>
{
    using ST = SolverTestcase<T_Type, 3>;

    std::vector<ST> testcases = {};

    testcases.emplace_back(ST({{1., 0., 0., 0., 1., 0., 0., 0., 1.}},      // NOLINT(readability-magic-numbers)
                              {{1., 2., 3.}},                              // NOLINT(readability-magic-numbers)
                              {{1., 2., 3.}}));                            // NOLINT(readability-magic-numbers)
    testcases.emplace_back(ST({{2., 3., 1., 5., 2., 1., 1., 4., 1.}},      // NOLINT(readability-magic-numbers)
                              {{2., -3., 1.}},                             // NOLINT(readability-magic-numbers)
                              {{-10., 4., 0.}}));                          // NOLINT(readability-magic-numbers)
    testcases.emplace_back(ST({{7., 1., -4., -2., -8., -6., 2., -3., 5.}}, // NOLINT(readability-magic-numbers)
                              {{3., 5., -2.}},                             // NOLINT(readability-magic-numbers)
                              {{7., -31., -52.}}));                        // NOLINT(readability-magic-numbers)
    testcases.emplace_back(ST({{4., 8., 9., 3., 4., 7., 2., 1., 4.}},      // NOLINT(readability-magic-numbers)
                              {{4., -7., 9.}},                             // NOLINT(readability-magic-numbers)
                              {{13., 13., 23.}}));                         // NOLINT(readability-magic-numbers)
    testcases.emplace_back(ST({{-9., 2., 5., 3., 6., 8., 8., -4., 1.}},    // NOLINT(readability-magic-numbers)
                              {{6., -5., 7.}},                             // NOLINT(readability-magic-numbers)
                              {{-13., -46., -3.}}));                       // NOLINT(readability-magic-numbers)

    return testcases;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] auto get_solver_testcases_4x4() -> std::vector<SolverTestcase<T_Type, 4>>
{
    using ST = SolverTestcase<T_Type, 4>;

    std::vector<ST> testcases = {};

    // NOLINTNEXTLINE(readability-magic-numbers)
    testcases.emplace_back(ST({{1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1., 0., 0., 0., 0., 1.}},
                              {{1., 2., 3., 4.}},   // NOLINT(readability-magic-numbers)
                              {{1., 2., 3., 4.}})); // NOLINT(readability-magic-numbers)
    // NOLINTNEXTLINE(readability-magic-numbers)
    testcases.emplace_back(ST({{3., -5., 4., -6., 2., -3., 7., -5., 8., -6., 1., -1., 5., -2., 3., -9.}},
                              {{5., 2., 1., 7.}},          // NOLINT(readability-magic-numbers)
                              {{62., -51., 56., -104.}})); // NOLINT(readability-magic-numbers)
    return testcases;
}


} // namespace mjolnir
