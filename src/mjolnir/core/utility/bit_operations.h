//! @file
//! utility/bir_operations.h
//!
//! @brief
//! Contains utility functions for bit related operations like setting and reading specific bits.


#pragma once

#include "mjolnir/core/fundamental_types.h"

#include <concepts>


namespace mjolnir
{
//! @brief
//! Construct an unsigned integer from individual bits.
//!
//! @details
//! The function sets the first N lowest bits of the returned integer where N is the number of provided parameters. Each
//! parameter defines the value of a single bit. Values larger than 1 are treated as 1. Depending on the value of
//! `t_ascending`, the bits can either be in ascending or decending order. Or in other words, `t_ascending` controls
//! if the lowest bit is represented by the first or last parameter.
//!
//! @todo
//! - assert number of parameters not larger than variable bits
//! - clamp values to 1
template <std::unsigned_integral T_Type, bool t_ascending = false, std::integral... T_Args>
[[nodiscard]] consteval inline auto bit_construct(T_Args... bit_values) noexcept -> T_Type;

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


#include <initializer_list>

#include <algorithm>
#include <cassert>

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <std::unsigned_integral T_Type, bool t_ascending, std::integral... T_Args>
[[nodiscard]] consteval inline auto bit_construct(T_Args... bit_values) noexcept -> T_Type
{
    constexpr auto set_bit_of_integer = [](auto bit_value, T_Type bit_number, T_Type& integer) -> void
    {
        if (bit_value > 1)
            throw 0;
        T_Type clamped_bit_value = std::clamp(static_cast<T_Type>(bit_value), T_Type(0), T_Type(1));
        integer                  = (clamped_bit_value << bit_number) | integer;
    };

    T_Type integer = 0;
    if constexpr (t_ascending)
    {
        T_Type bit_number = 0;
        (void) std::initializer_list<I32>{(set_bit_of_integer(bit_values, bit_number++, integer), 0)...};
    }
    else
    {
        T_Type bit_number = sizeof...(bit_values) - 1;
        (void) std::initializer_list<I32>{(set_bit_of_integer(bit_values, bit_number--, integer), 0)...};
    }
    return integer;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] constexpr inline auto is_bit_set(T_Type variable, UST position) noexcept -> bool
{
    return (variable & (1U << (position)));
}


} // namespace mjolnir
