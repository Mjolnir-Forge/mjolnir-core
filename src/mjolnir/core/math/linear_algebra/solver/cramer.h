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
    using namespace x86;

    if constexpr (is_double_precision<T_RegisterType>)
    {
        auto p_mat_c1 = permute<1, 0>(mat[1]);
        auto p_rhs    = permute<1, 0>(rhs);

        auto products_mat = mm_mul(mat[0], p_mat_c1);
        auto products_x0  = mm_mul(rhs, p_mat_c1);
        auto products_x1  = mm_mul(mat[0], p_rhs);

        auto p_products_mat = permute<1, 0>(products_mat);
        auto p_products_x0  = permute<1, 0>(products_x0);
        auto p_products_x1  = permute<1, 0>(products_x1);

        auto det_mat = mm_sub(products_mat, p_products_mat);
        auto det_x0  = mm_sub(products_x0, p_products_x0);
        auto det_x1  = mm_sub(products_x1, p_products_x1);

        T_RegisterType det_x = blend_at<1>(det_x0, det_x1);

        return mm_div(det_x, det_mat);
    }
    else
    {
        auto shf_mat = shuffle<1, 0, 0, 1>(mat[1], mat[0]);
        auto p_rhs_2 = permute<0, 1, 1, 0>(rhs);

        auto products_mat = mm_mul(mat[0], shf_mat);
        auto products_x   = mm_mul(p_rhs_2, shf_mat);

        auto shf_prod_0 = shuffle<0, 2, 0, 0>(products_x, products_mat);
        auto shf_prod_1 = shuffle<1, 3, 1, 1>(products_x, products_mat);

        auto dets = mm_sub(shf_prod_0, shf_prod_1);

        auto det_mat_2 = permute<2, 3, 2, 3>(dets);

        return mm_div(dets, det_mat_2);
    }
}


} // namespace mjolnir
