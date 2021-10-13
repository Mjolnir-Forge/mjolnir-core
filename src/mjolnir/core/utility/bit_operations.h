//! @file
//! utility/bir_operations.h
//!
//! @brief
//! Contains utility functions for bit related operations like setting and reading specific bits.


#pragma once

#include "mjolnir/core/fundamental_types.h"




namespace mjolnir
{
//! \addtogroup core_utility
//! @{

//! @brief
//! Return `true` if a specific bit is set and `false` otherwise
//!
//! @tparam T_Type
//! Type of the cheched variable
//!
//! @param[in] variable:
//! The variable that should be checked
//! @param[in] position
//! The position of the bit
//!
//! @return
//! `true` or `false`
//!
//! @test
//! Write a test
template <typename T_Type>
[[nodiscard]] constexpr inline auto is_bit_set(T_Type variable, UST position) noexcept -> bool;


//! @}
} // namespace mjolnir


// ====================================================================================================================

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] constexpr inline auto is_bit_set(T_Type variable, UST position) noexcept -> bool
{
    return (variable & (1U << (position)));
}


} // namespace mjolnir
