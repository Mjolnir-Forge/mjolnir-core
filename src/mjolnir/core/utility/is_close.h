//! @file
//! is_close.h
//!
//! @brief
//! Contains comparison functions with tolerances.


#pragma once

#include "mjolnir/core/definitions.h"

namespace mjolnir
{
//! \addtogroup core_utility
//! @{


//! @brief
//! The default absolute tolerance
//!
//! @tparam T_Type
//! Tolerance type
template <typename T_Type>
inline constexpr T_Type default_tolerance_abs = static_cast<T_Type>(1E-6);

//! @brief
//! Return ´true´ if the difference between `lhs` and `rhs` is inside an absolute tolerance and `false` otherwise.
//!
//! @tparam T_Type
//! Type of the function parameters
//!
//! @param [in] lhs
//! Left-hand side value
//! @param [in] rhs
//! Right-hand side value
//! @param [in] tolerance
//! The tolerance of the comparison
//!
//! @return
//! ´true´ if the difference between `lhs` and `rhs` is inside an absolute tolerance and `false` otherwise.
//!
//! @todo
//! Add debug exception for negative tolerances
template <typename T_Type>
requires Number<T_Type>
[[nodiscard]] inline auto
is_close_abs(T_Type lhs, T_Type rhs, T_Type tolerance = default_tolerance_abs<T_Type>) noexcept -> bool;


//! @}
} // namespace mjolnir


// ====================================================================================================================


#include <cmath>

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------


template <typename T_Type>
requires Number<T_Type>
[[nodiscard]] inline auto is_close_abs(T_Type lhs, T_Type rhs, T_Type tolerance) noexcept -> bool
{
    return std::abs(lhs - rhs) <= tolerance;
}


} // namespace mjolnir
