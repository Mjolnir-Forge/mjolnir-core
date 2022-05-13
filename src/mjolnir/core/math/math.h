//! @file
//! math/math.h
//!
//! @brief
//! Contains basic mathematical functions.


#pragma once

#include "mjolnir/core/concepts.h"
#include "mjolnir/core/fundamental_types.h"

#include <concepts>


namespace mjolnir
{
//! \addtogroup core_math
//! @{


//! @brief
//! Calculate the sum of the first `n` positive numbers.
//!
//! @tparam T_Type
//! An integer type
//!
//! @param[in] n:
//! Specifies how many numbers should be summed up.
//!
//! @return
//! The sum of the first `n` positive numbers
template <std::integral T_Type>
[[nodiscard]] inline constexpr auto gauss_summation(T_Type n) -> T_Type;


//! @brief
//! Calculate the power of a number using an integer based exponent.
//!
//! @tparam T_Type:
//! An integer or floating-point type
//!
//! @param[in] base:
//! The base number
//! @param[in] exponent:
//! The exponent
//!
//! @return
//! The result of the power function
template <Number T_Type>
[[nodiscard]] inline constexpr auto power(T_Type base, std::integral auto exponent) noexcept -> T_Type;


//! @brief
//! Calculate the power of 2 using an integer based exponent.
//!
//! @note
//! This function is faster than the `power` function with a base of 2.
//!
//! @tparam T_Type:
//! An integer or floating-point type used for the returned value
//!
//! @param[in] exponent:
//! The exponent
//!
//! @return
//! The result of the power function
template <Number T_Type = UST>
[[nodiscard]] inline constexpr auto power_of_2(std::integral auto exponent) noexcept -> T_Type;


//! @}
} // namespace mjolnir


// ====================================================================================================================


#include <cassert>


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <std::integral T_Type>
[[nodiscard]] inline constexpr auto gauss_summation(T_Type n) -> T_Type
{
    assert(n >= 0 && "n must be a positive number"); // NOLINT

      return (n * n + n) / 2;
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
// NOLINTNEXTLINE(misc-no-recursion)
[[nodiscard]] inline constexpr auto power(T_Type base, std::integral auto exponent) noexcept -> T_Type
{
    assert(exponent >= 0 && "exponent must be a positive number"); // NOLINT

    if (exponent == 0)
        return 1;
    if (exponent % 2 == 0)
        return power(base, exponent / 2) * power(base, exponent / 2);
    return base * power(base, (exponent - 1) / 2) * power(base, (exponent - 1) / 2);
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
[[nodiscard]] inline constexpr auto power_of_2(std::integral auto exponent) noexcept -> T_Type
{
    assert(exponent >= 0 && "exponent must be a positive number"); // NOLINT

    return static_cast<T_Type>(UST(1) << static_cast<UST>(exponent));
}


} // namespace mjolnir
