//! @file
//! math/linear_algebra/determinant.h
//!
//! @brief
//! Functions to calculate the determinant of a matrix.


#pragma once

#include "mjolnir/core/fundamental_types.h"

#include <array>

// === DECLARATIONS ===================================================================================================

namespace mjolnir
{
//! \addtogroup core_math
//! @{

template <typename T_Type>
[[nodiscard]] constexpr auto determinant_2x2(std::array<T_Type, 4> mat) noexcept -> T_Type;

//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] constexpr auto determinant_2x2(std::array<T_Type, 4> mat) noexcept -> T_Type
{
    return mat[3] * mat[0] - mat[1] * mat[2];
}


// --------------------------------------------------------------------------------------------------------------------

} // namespace mjolnir
