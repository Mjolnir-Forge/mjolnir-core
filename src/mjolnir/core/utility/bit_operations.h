//! @file
//! utility/bir_operations.h
//!
//! @brief
//! Contains utility functions for bit related operations like setting and reading specific bits.
//!
//! @note
//! sources:
//! - https://stackoverflow.com/a/47990/6700329


#pragma once

#include "mjolnir/core/fundamental_types.h"

#include <concepts>
#include <limits>


namespace mjolnir
{
//! @brief
//! The size of a type in bits.
//!
//! @tparam T_Type:
//! The type
template <typename T_Type>
inline constexpr UST num_bits = sizeof(T_Type) * CHAR_BIT;


//! @brief
//! Construct an unsigned integer by setting its individual bits.
//!
//! @details
//! The function sets the first N lowest bits of the returned integer where N is the number of provided template
//! parameters. Each parameter defines the value of a single bit. Depending on the value of `ascending`, the bits can
//! either be in ascending or decending order. Or in other words, `ascending` controls if the lowest bit is represented
//! by the first or last parameter.
//!
//! @tparam T_Type:
//! An unsigned integer type
//! @tparam t_bit_values:
//! The individuel bits (see detailed description). Values other than 0 or 1 will trigger a static assertion.
//!
//! @param[in] ascending:
//! If `true`, the first passed bit represents the lowest bit. If `false`, the lowest bit is represented by the last
//! value in `t_bit_values`
//!
//! @return
//! Integer constructed from bit values
template <std::unsigned_integral T_Type, UST... t_bit_values>
[[nodiscard]] consteval inline auto bit_construct(bool ascending = false) noexcept -> T_Type;


//! @brief
//! Clear a single specific bit of an unsigned integer.
//!
//! @tparam T_Type:
//! An unsigned integer type
//!
//! @param[in, out] integer:
//! The integer that should be modified
//! @param[in] index
//! The index of the bit that should be modified
template <std::unsigned_integral T_Type>
constexpr inline void clear_bit(T_Type& integer, UST index) noexcept;


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
template <typename T_Type>
[[nodiscard]] constexpr inline auto is_bit_set(T_Type variable, UST position) noexcept -> bool;


//! @brief
//! Set a single specific bit of an unsigned integer.
//!
//! @tparam T_Type:
//! An unsigned integer type
//!
//! @param[in, out] integer:
//! The integer that should be modified
//! @param[in] index
//! The index of the bit that should be modified
template <std::unsigned_integral T_Type>
constexpr inline void set_bit(T_Type& integer, UST index) noexcept;


//! @brief
//! Set a single bit of an unsigned integer to the specified value
//!
//! @tparam t_value:
//! The new value of the bit
//! @tparam T_Type:
//! An unsigned integer type
//!
//! @param[in, out] integer:
//! The integer that should be modified
//! @param[in] index
//! The index of the bit that should be modified
template <UST t_value, std::unsigned_integral T_Type>
constexpr inline void set_bit_to(T_Type& integer, UST index) noexcept;


//! @}
} // namespace mjolnir


// ====================================================================================================================


#include <initializer_list>

#include <algorithm>
#include <cassert>

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <std::unsigned_integral T_Type, UST... t_bit_values>
[[nodiscard]] consteval inline auto bit_construct(bool ascending) noexcept -> T_Type
{
    static_assert(sizeof...(t_bit_values) <= num_bits<T_Type>, "Number of bit values exceeds number of type bits.");

    T_Type integer = 0;
    if (ascending)
    {
        T_Type bit_index = 0;
        (void) std::initializer_list<I32>{(set_bit_to<t_bit_values>(integer, bit_index++), 0)...};
    }
    else
    {
        T_Type bit_index = sizeof...(t_bit_values) - 1;
        (void) std::initializer_list<I32>{(set_bit_to<t_bit_values>(integer, bit_index--), 0)...};
    }
    return integer;
}


// --------------------------------------------------------------------------------------------------------------------

template <std::unsigned_integral T_Type>
constexpr inline void clear_bit(T_Type& integer, UST index) noexcept
{
    assert(index < num_bits<T_Type> && "Index exceeds number of bits."); // NOLINT

    integer &= ~(1U << index);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] constexpr inline auto is_bit_set(T_Type variable, UST position) noexcept -> bool
{
    return (variable & (1U << (position)));
}


// --------------------------------------------------------------------------------------------------------------------

template <std::unsigned_integral T_Type>
constexpr inline void set_bit(T_Type& integer, UST index) noexcept
{
    assert(index < num_bits<T_Type> && "Index exceeds number of bits."); // NOLINT

    integer |= 1U << index;
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_value, std::unsigned_integral T_Type>
constexpr inline void set_bit_to(T_Type& integer, UST index) noexcept
{
    static_assert(t_value <= 1, "Bit value must either be 0 or 1.");

    if constexpr (t_value == 1)
        set_bit(integer, index);
    else
        clear_bit(integer, index);
}

} // namespace mjolnir
