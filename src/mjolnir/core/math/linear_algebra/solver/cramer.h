//! @file
//! math/linear_algebra/solver/cramer.h
//!
//! @brief
//! Solvers based on cramers rule for dense matrices of different size.


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/definitions.h"
#include "mjolnir/core/exception.h"
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/math/linear_algebra/determinant.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"

#include <array>


namespace mjolnir
{
//! \addtogroup core_math
//! @{


//! @brief
//! Solve a linear system of equations of size 2 using Cramer's Rule.
//!
//! @tparam T_Type:
//! The basic type of a matrix/vector element
//!
//! @param mat:
//! The matrix of the system of equations. The data has to be provided in column major format
//! @param rhs:
//! The right-hand side vector of the system
//!
//! @return
//! Solution vector
template <Number T_Type>
[[nodiscard]] constexpr auto cramer(const std::array<T_Type, 4>& mat, const std::array<T_Type, 2>& rhs) noexcept
        -> std::array<T_Type, 2>;


//! @brief
//! Solve a linear system of equations of size 2 using Cramer's Rule.
//!
//! @tparam T_RegisterType:
//! The register type that stores the columns of a matrix/vector
//!
//! @param mat:
//! The matrix of the system of equations. The data has to be provided in column major format
//! @param rhs:
//! The right-hand side vector of the system
//!
//! @return
//! Solution vector
template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] auto cramer(const std::array<T_RegisterType, 2>& mat, T_RegisterType rhs) noexcept -> T_RegisterType;

//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
template <Number T_Type>
[[nodiscard]] inline constexpr auto cramer(const std::array<T_Type, 4>& mat, const std::array<T_Type, 2>& rhs) noexcept
        -> std::array<T_Type, 2>
{
    T_Type det_mat = determinant_2x2(mat);

    T_Type x_0 = (mat[3] * rhs[0] - mat[2] * rhs[1]) / det_mat;
    T_Type x_1 = (mat[0] * rhs[1] - mat[1] * rhs[0]) / det_mat;

    return std::array<T_Type, 2>{x_0, x_1};
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] auto cramer(const std::array<T_RegisterType, 2>& mat, T_RegisterType rhs) noexcept -> T_RegisterType
{
    // Several implementations have been tested and benchmarked. This one was the fastest. Note that the throughput of
    // `mm_mul` and `mm_fmsub` is 0.5. Therefore, reducing those instructions to 1 instead of 2 doesn't yield any
    // performance boost. The required permutations for this actually cause the code to be slower.

    using namespace x86;

    auto b0a1  = blend_at<0>(mat[0], mat[1]);
    auto a0b1  = blend_at<1>(mat[0], mat[1]);
    auto b1a0  = swap<1, 0>(a0b1);
    auto a1b0  = swap<1, 0>(b0a1);
    auto p_rhs = swap<1, 0>(rhs);

    auto prod_mat = mm_mul(a1b0, b0a1);
    auto prod_rhs = mm_mul(p_rhs, b0a1);

    auto det_mat = mm_fmsub(a0b1, b1a0, prod_mat);
    auto det_rhs = mm_fmsub(rhs, b1a0, prod_rhs);

    return mm_div(det_rhs, det_mat);
}


} // namespace mjolnir
