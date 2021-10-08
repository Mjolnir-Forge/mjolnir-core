//! @file
//! is_close.h
//!
//! @brief
//! Contains comparison functions with tolerances.


#pragma once

namespace mjolnir
{
//! \addtogroup core_utility
//! @{


//! @brief
//! Return ´true´ if the difference between `lhs` and `rhs` is inside an absolute tolerance and `false` otherwise.
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
[[nodiscard]] inline auto is_close_abs(T_Type lhs, T_Type rhs, T_Type tolerance) noexcept -> bool;


//! @}
} // namespace mjolnir


// ====================================================================================================================


#include <cmath>

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------


template <typename T_Type>
[[nodiscard]] inline auto is_close_abs(T_Type lhs, T_Type rhs, T_Type tolerance) noexcept -> bool
{
    return std::abs(lhs - rhs) <= tolerance;
}


} // namespace mjolnir
