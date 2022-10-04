//! @file
//! math/linear_algebra/solver/cramer.h
//!
//! @brief
//! Solvers based on cramers rule for dense matrices of different size.


#pragma once

#include "mjolnir/core/definitions.h"
#include "mjolnir/core/exception.h"
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/math/linear_algebra/determinant.h"

#include <array>

// === DECLARATIONS ===================================================================================================

namespace mjolnir
{
//! \addtogroup core_math
//! @{

template <Number T_Type>
[[nodiscard]] constexpr auto cramer(const std::array<T_Type, 4>& mat, const std::array<T_Type, 2>& rhs) noexcept
        -> std::array<T_Type, 2>;

//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
template <Number T_Type>
[[nodiscard]] constexpr auto cramer(const std::array<T_Type, 4>& mat, const std::array<T_Type, 2>& rhs) noexcept
        -> std::array<T_Type, 2>
{
    T_Type det_mat = determinant_2x2(mat);

    T_Type x_0 = (mat[3] * rhs[0] - mat[2] * rhs[1]) / det_mat;
    T_Type x_1 = (mat[0] * rhs[1] - mat[1] * rhs[0]) / det_mat;

    return std::array<T_Type, 2>{x_0, x_1};
}


} // namespace mjolnir
