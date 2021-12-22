//! @file
//! utility/bit_operations.h
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
//! \addtogroup core_utility
//! @{


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
//! parameters. Each parameter defines the value of a single bit. Depending on the value of `left_is_low`, the bits can
//! either be specified in ascending or decending order. Or in other words, `left_is_low` controls if the lowest bit is
//! represented by the first or last parameter.
//!
//! @tparam T_Type:
//! An unsigned integer type
//! @tparam t_bit_values:
//! The individuel bits (see detailed description). Values other than 0 or 1 will trigger a static assertion.
//!
//! @param[in] left_is_low:
//! If `true`, the first value represents the lowest bit. Otherwise, the last value represents the lowest bit.
//!
//! @return
//! The constructed integer value
template <std::unsigned_integral T_Type, UST... t_bit_values>
[[nodiscard]] consteval inline auto bit_construct([[maybe_unused]] bool left_is_low = false) noexcept -> T_Type;


//! @brief
//! Construct an unsigned integer from the bit patterns of multiple integer values.
//!
//! @details
//! The function takes multiple integer value of the specified size in bits. Their bit patterns are concatenated to
//! construct the bit pattern of the result value. Depending on the value of `left_is_low`, the lowest bit pattern is
//! either represented by the first or the last provided value.
//!
//! @tparam t_num_int_bits:
//! Specifies how many bits are possessed by each of the individual integers
//! @tparam T_Type:
//! An unsigned integer type
//! @tparam t_integers:
//! The individuel integer values (see detailed description). Values exceeding the bit size  will trigger an assertion.
//!
//! @param[in] left_is_low:
//! If `true`, the first value represents the lowest bit pattern. Otherwise, the last value represents the lowest bit
//! pattern.
//!
//! @return
//! The constructed integer value
template <UST t_num_int_bits, std::unsigned_integral T_Type, UST... t_integers>
[[nodiscard]] consteval inline auto bit_construct_from_ints(bool left_is_low = false) noexcept -> T_Type;


//! @brief
//! Construct an unsigned integer with its first `t_num_bits` set to 1.
//!
//! @tparam T_Type:
//! An unsigned integer type
//! @tparam t_num_bits:
//! Number of bits that should be set
//!
//! @return
//! The constructed integer value
//!
//! @note
//! Source: https://stackoverflow.com/a/45352771/6700329
template <std::unsigned_integral T_Type, UST t_num_bits>
[[nodiscard]] constexpr inline auto bit_construct_set_first_n_bits() noexcept -> T_Type;


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


//! @brief
//! Clear multiple consecutive bits of an unsigned integer.
//!
//! @tparam t_num_bits:
//! Number of bits that should be cleared
//! @tparam T_Type:
//! An unsigned integer type
//!
//! @param[in, out] integer:
//! The integer that should be modified
//! @param[in] index
//! The index of the first bit that should be modified
template <UST t_num_bits, std::unsigned_integral T_Type>
constexpr inline void clear_bits(T_Type& integer, UST index) noexcept;


//! @brief
//! Extract a bit from an integer and store it with an optional shift in a new integer.
//!
//! @tparam t_index:
//! The index of the bit that should be extracted
//! @tparam t_shift:
//! An integer that specifies the direction and magnitude that the bit should be shifted. Positive values will shift
//! it to the left (higher values) while negative values will shift the bit to the right (lower values).
//! @tparam T_Type:
//! The type of the source integer.
//! @tparam T_ReturnType
//! The type of the returned integer
//!
//! @param[in] integer:
//! The source integer
//!
//! @return
//! An integer containing the extracted and shifted bit value.
template <UST                    t_index,
          I32                    t_shift      = 0,
          std::unsigned_integral T_Type       = UST,
          std::unsigned_integral T_ReturnType = T_Type>
[[nodiscard]] constexpr inline auto get_bit(T_Type integer) noexcept -> T_ReturnType;


//! @brief
//! Extract a bit from an integer and store it with an optional shift in a new integer.
//!
//! @tparam T_Type:
//! The type of the source integer.
//! @tparam T_ReturnType
//! The type of the returned integer
//!
//! @param[in] integer:
//! The source integer
//! @param[in] index:
//! The index of the bit that should be extracted
//! @param[in] shift:
//! An integer that specifies the direction and magnitude that the bit should be shifted. Positive values will shift
//! it to the left (higher values) while negative values will shift the bit to the right (lower values).
//!
//! @return
//! An integer containing the extracted and shifted bit value.
template <std::unsigned_integral T_Type = UST, std::unsigned_integral T_ReturnType = T_Type>
[[nodiscard]] constexpr inline auto get_bit(T_Type integer, UST index, I32 shift = 0) noexcept -> T_ReturnType;


//! @brief
//! Extract a bit pattern from an integer and store it with an optional shift in a new integer.
//!
//! @tparam t_num_bits:
//! The patterns number of bits
//! @tparam t_index:
//! The index of the first bit of the bit pattern
//! @tparam t_shift:
//! An integer that specifies the direction and magnitude that the bit pattern should be shifted. Positive
//! values will shift it to the left (higher values) while negative values will shift the bit to the right
//! (lower values).
//! @tparam T_Type:
//! The type of the source integer.
//! @tparam T_ReturnType
//! The type of the returned integer
//!
//! @param[in] integer:
//! The source integer
//!
//! @return
//! An integer containing the extracted and shifted bit pattern.
template <UST                    t_index,
          UST                    t_num_bits,
          I32                    t_shift      = 0,
          std::unsigned_integral T_Type       = UST,
          std::unsigned_integral T_ReturnType = T_Type>
[[nodiscard]] constexpr inline auto get_bits(T_Type integer) noexcept -> T_ReturnType;


//! @brief
//! Extract a bit pattern from an integer and store it with an optional shift in a new integer.
//!
//! @tparam t_num_bits:
//! The patterns number of bits
//! @tparam T_Type:
//! The type of the source integer.
//! @tparam T_ReturnType
//! The type of the returned integer
//!
//! @param[in] integer:
//! The source integer
//! @tparam[in] index:
//! The index of the first bit of the bit pattern
//! @tparam[in] shift:
//! An integer that specifies the direction and magnitude that the bit pattern should be shifted. Positive
//! values will shift it to the left (higher values) while negative values will shift the bit to the right
//! (lower values).
//!
//! @return
//! An integer containing the extracted and shifted bit pattern.
template <UST t_num_bits, std::unsigned_integral T_Type = UST, std::unsigned_integral T_ReturnType = T_Type>
[[nodiscard]] constexpr inline auto get_bits(T_Type integer, UST index, I32 shift = 0) noexcept -> T_ReturnType;


//! @brief
//! Return `true` if a specific bit is set and `false` otherwise
//!
//! @tparam T_Type
//! Type of the cheched variable
//!
//! @param[in] integer:
//! The integer that should be checked
//! @param[in] index
//! The index of the bit
//!
//! @return
//! `true` or `false`
template <typename T_Type>
[[nodiscard]] constexpr inline auto is_bit_set(T_Type integer, UST index) noexcept -> bool;


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


//! @brief
//! Set multiple consecutive bits of an unsigned integer.
//!
//! @tparam t_num_bits:
//! Number of bits that should be set
//! @tparam T_Type:
//! An unsigned integer type
//!
//! @param[in, out] integer:
//! The integer that should be modified
//! @param[in] index:
//! The index of the first bit that should be modified
template <UST t_num_bits, std::unsigned_integral T_Type>
constexpr inline void set_bits(T_Type& integer, UST index) noexcept;


//! @brief
//! Set multiple consecutive bits of an unsigned integer using the bit pattern of another integer value.
//!
//! @tparam t_num_bits:
//! Number of bits that should be set. Values of the source integer `value` that can't be represented by the provided
//! number of bits will trigger an assertion
//! @tparam t_clear_bits:
//! If `false`, already set bits won't be cleared to match the source integers bit pattern.
//! @tparam T_Type:
//! An unsigned integer type
//!
//! @param[in, out] integer:
//! The integer that should be modified
//! @param[in] index:
//! The index of the first bit that should be modified
//! @param[in] value:
//! The source integer
template <UST t_num_bits, bool t_clear_bits = true, std::unsigned_integral T_Type>
constexpr inline void set_bits_with_int(T_Type& integer, UST index, UST value) noexcept;


//! @}
} // namespace mjolnir


// ====================================================================================================================


#include <initializer_list>

#include <algorithm>
#include <cassert>
#include <limits>

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <std::unsigned_integral T_Type, UST... t_bit_values>
[[nodiscard]] consteval inline auto bit_construct([[maybe_unused]] bool left_is_low) noexcept -> T_Type
{
    static_assert(sizeof...(t_bit_values) <= num_bits<T_Type>, "Number of bit values exceeds number of type bits.");

    T_Type integer = 0;
    if constexpr (sizeof...(t_bit_values) != 0)
    {
        if (left_is_low)
        {
            UST bit_index = 0;
            (void) std::initializer_list<I32>{(set_bit_to<t_bit_values>(integer, bit_index++), 0)...};
        }
        else
        {
            UST bit_index = sizeof...(t_bit_values) - 1;
            (void) std::initializer_list<I32>{(set_bit_to<t_bit_values>(integer, bit_index--), 0)...};
        }
    }
    return integer; // NOLINT(readability-misleading-indentation) --- clang-tidy bug
}


// --------------------------------------------------------------------------------------------------------------------

template <std::unsigned_integral T_Type, UST t_num_bits>
[[nodiscard]] constexpr inline auto bit_construct_set_first_n_bits() noexcept -> T_Type
{
    // INFO:
    // The static_cast for the `true` case is actually needed because the `~` operator seems to perform a promotion to
    // `int`. See the following link: https://stackoverflow.com/q/30473958/6700329
    if constexpr (t_num_bits >= num_bits<T_Type>)
        return static_cast<T_Type>(~(T_Type(0)));
    else
        return static_cast<T_Type>(((UST(1)) << t_num_bits) - 1);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_int_bits, std::unsigned_integral T_Type, UST... t_integers>
[[nodiscard]] consteval inline auto bit_construct_from_ints(bool left_is_low) noexcept -> T_Type
{
    static_assert(sizeof...(t_integers) * t_num_int_bits <= num_bits<T_Type>,
                  "Totoal number of provided bits exceeds number of type bits.");

    T_Type integer = 0;
    if (left_is_low)
    {
        UST bit_index = 0;
        (void) std::initializer_list<I32>{(set_bits_with_int<t_num_int_bits, false>(integer, bit_index, t_integers),
                                           bit_index += t_num_int_bits,
                                           0)...};
    }
    else
    {
        UST bit_index = (sizeof...(t_integers) - 1) * t_num_int_bits;
        (void) std::initializer_list<I32>{(set_bits_with_int<t_num_int_bits, false>(integer, bit_index, t_integers),
                                           bit_index -= t_num_int_bits,
                                           0)...};
    }

    return integer;
}


// --------------------------------------------------------------------------------------------------------------------

template <std::unsigned_integral T_Type>
constexpr inline void clear_bit(T_Type& integer, UST index) noexcept
{
    assert(index < num_bits<T_Type> && "Index exceeds number of bits."); // NOLINT

    integer &= static_cast<T_Type>(~(UST(1) << index));
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_bits, std::unsigned_integral T_Type>
constexpr inline void clear_bits(T_Type& integer, UST index) noexcept
{
    assert(index + t_num_bits <= num_bits<T_Type>); // NOLINT

    constexpr UST bits = bit_construct_set_first_n_bits<UST, t_num_bits>();
    integer &= static_cast<T_Type>(~(bits << index));
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, I32 t_shift, std::unsigned_integral T_Type, std::unsigned_integral T_ReturnType>
[[nodiscard]] constexpr inline auto get_bit(T_Type integer) noexcept -> T_ReturnType
{
    static_assert(t_index < num_bits<T_Type>, "Index exceeds number of bits");

    T_ReturnType bit = (integer & static_cast<T_Type>(UST(1) << (t_index)));

    if constexpr (t_shift == 0)
        return bit;
    else if constexpr (t_shift > 0)
    {
        static_assert(t_index + t_shift < num_bits<T_ReturnType>, "Shift exceeds highest bit.");
        return bit << static_cast<UST>(t_shift);
    }
    else
    {
        static_assert(t_index >= std::abs(t_shift), "Shift exceeds lowest bit");
        return bit >> static_cast<UST>(std::abs(t_shift));
    }
}


// --------------------------------------------------------------------------------------------------------------------


template <std::unsigned_integral T_Type, std::unsigned_integral T_ReturnType>
[[nodiscard]] constexpr inline auto get_bit(T_Type integer, UST index, I32 shift) noexcept -> T_ReturnType
{
    assert(index < num_bits<T_Type>); // NOLINT

    T_ReturnType bit = (integer & static_cast<T_Type>(UST(1) << (index)));


    if (shift >= 0)
    {
        assert(index + shift < num_bits<T_ReturnType>); // NOLINT
        return bit << static_cast<UST>(shift);
    }
    else
    {
        assert(index >= std::abs(shift)); // NOLINT
        return bit >> static_cast<UST>(std::abs(shift));
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST                    t_index,
          bool                   t_shift_right = true,
          std::unsigned_integral T_Type        = UST,
          std::unsigned_integral T_ReturnType  = T_Type>
[[nodiscard]] constexpr inline auto get_bit_shift_max(T_Type integer) noexcept -> T_ReturnType
{
    constexpr I32 shift = (t_shift_right) ? -static_cast<I32>(t_index) : num_bits<T_ReturnType> - t_index - 1;
    return get_bit<t_index, shift, T_Type, T_ReturnType>(integer);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_shift_right = true, std::unsigned_integral T_Type = UST, std::unsigned_integral T_ReturnType = T_Type>
[[nodiscard]] constexpr inline auto get_bit_shift_max(T_Type integer, UST index) noexcept -> T_ReturnType
{
    if constexpr (t_shift_right)
        return get_bit<T_Type, T_ReturnType>(integer, index, -static_cast<I32>(index));
    else
        return get_bit<T_Type, T_ReturnType>(integer, index, num_bits<T_ReturnType> - index - 1);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, UST t_num_bits, I32 t_shift, std::unsigned_integral T_Type, std::unsigned_integral T_ReturnType>
[[nodiscard]] constexpr inline auto get_bits(T_Type integer) noexcept -> T_ReturnType
{
    static_assert(t_num_bits > 1, "Number of bits must be larger than 0.");
    static_assert(t_index + t_num_bits <= num_bits<T_Type>, "Required bits exceed maximum number of bits.");

    constexpr UST mask = bit_construct_set_first_n_bits<UST, t_num_bits>() << (t_index);
    T_ReturnType  bits = integer & mask;

    if constexpr (t_shift == 0)
    {
        static_assert(t_index + t_num_bits <= num_bits<T_ReturnType>, "Shifted bits exceed highest bit.");
        return bits;
    }
    else if constexpr (t_shift > 0)
    {
        static_assert(t_index + t_num_bits + t_shift <= num_bits<T_ReturnType>, "Shifted bits exceed highest bit.");
        return bits << static_cast<UST>(t_shift);
    }
    else
    {
        static_assert(t_index >= std::abs(t_shift), "Shifted bits exceed lowest bit.");
        return bits >> static_cast<UST>(std::abs(t_shift));
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_bits, std::unsigned_integral T_Type, std::unsigned_integral T_ReturnType>
[[nodiscard]] constexpr inline auto get_bits(T_Type integer, UST index, I32 shift) noexcept -> T_ReturnType
{
    static_assert(t_num_bits > 1, "Number of bits must be larger than 0.");
    assert(index + t_num_bits <= num_bits<T_Type>); // NOLINT

    UST          mask = bit_construct_set_first_n_bits<UST, t_num_bits>() << (index);
    T_ReturnType bits = integer & mask;


    if (shift >= 0)
    {
        assert(index + t_num_bits + shift <= num_bits<T_ReturnType>); // NOLINT
        return bits << static_cast<UST>(shift);
    }
    else
    {
        assert(index >= std::abs(shift)); // NOLINT
        return bits >> static_cast<UST>(std::abs(shift));
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST                    t_index,
          UST                    t_num_bits,
          bool                   t_shift_right = true,
          std::unsigned_integral T_Type        = UST,
          std::unsigned_integral T_ReturnType  = T_Type>
[[nodiscard]] constexpr inline auto get_bits_shift_max(T_Type integer) noexcept -> T_ReturnType
{
    constexpr I32 shift = (t_shift_right) ? -static_cast<I32>(t_index) : num_bits<T_ReturnType> - t_index - t_num_bits;
    return get_bits<t_index, t_num_bits, shift, T_Type, T_ReturnType>(integer);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST                    t_num_bits,
          bool                   t_shift_right = true,
          std::unsigned_integral T_Type        = UST,
          std::unsigned_integral T_ReturnType  = T_Type>
[[nodiscard]] constexpr inline auto get_bits_shift_max(T_Type integer, UST index) noexcept -> T_ReturnType
{
    if constexpr (t_shift_right)
        return get_bits<t_num_bits, T_Type, T_ReturnType>(integer, index, -static_cast<I32>(index));
    else
        return get_bits<t_num_bits, T_Type, T_ReturnType>(integer, index, num_bits<T_ReturnType> - index - t_num_bits);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] constexpr inline auto is_bit_set(T_Type integer, UST index) noexcept -> bool
{
    return static_cast<bool>(get_bit(integer, index));
}


// --------------------------------------------------------------------------------------------------------------------

template <std::unsigned_integral T_Type>
constexpr inline void set_bit(T_Type& integer, UST index) noexcept
{
    assert(index < num_bits<T_Type> && "Index exceeds number of bits."); // NOLINT

    integer |= static_cast<T_Type>(UST(1) << index);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_value, std::unsigned_integral T_Type>
constexpr inline void set_bit_to(T_Type& integer, UST index) noexcept
{
    static_assert(t_value <= 1UL, "Bit value must either be 0 or 1.");

    if constexpr (t_value == 1UL)
        set_bit(integer, index);
    else
        clear_bit(integer, index);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_bits, std::unsigned_integral T_Type>
constexpr inline void set_bits(T_Type& integer, UST index) noexcept
{
    assert(index + t_num_bits <= num_bits<T_Type>); // NOLINT

    constexpr UST bits = bit_construct_set_first_n_bits<UST, t_num_bits>();
    integer |= static_cast<T_Type>(bits << index);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_bits, bool t_clear_bits, std::unsigned_integral T_Type>
constexpr inline void set_bits_with_int(T_Type& integer, UST index, UST value) noexcept
{
    [[maybe_unused]] constexpr UST max_value = bit_construct_set_first_n_bits<UST, t_num_bits>();

    assert(value <= max_value && "Value doesn't fit into specified number of bits."); // NOLINT
    assert(index + t_num_bits <= num_bits<T_Type>);                                   // NOLINT

    if constexpr (t_clear_bits)
        clear_bits<t_num_bits>(integer, index);

    integer |= static_cast<T_Type>(value << index);
}


} // namespace mjolnir
