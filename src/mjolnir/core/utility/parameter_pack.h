//! @file
//! parameter_pack.h
//!
//! @brief
//! Contains utility functions for parameter packs.


#pragma once

#include "mjolnir/core/fundamental_types.h"

#include <algorithm>
#include <array>


// === DECLARATION ====================================================================================================

namespace mjolnir
{
//! \addtogroup core_utility
//! @{


//! @brief
//! Return `true` if all values of an unsigned integer parameter pack are less then a given value and `false` otherwise.
//!
//! @tparam t_pack:
//! The parameter pack that should be checked
//!
//! @param[in] value:
//! The reference value
//!
//! @return
//! `true` or `false`
template <UST... t_pack>
[[nodiscard]] inline consteval auto pp_all_less(UST value) noexcept -> bool;


//! @brief
//! Return `true` if all values of an boolean parameter pack are `true` and `false` otherwise.
//!
//! @tparam t_pack:
//! The parameter pack that should be checked
//!
//! @return
//! `true` or `false`
template <bool... t_pack>
[[nodiscard]] inline consteval auto pp_all_true() noexcept -> bool;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <UST... t_pack>
[[nodiscard]] inline consteval auto pp_all_less(UST value) noexcept -> bool
{
    constexpr UST         size = sizeof...(t_pack);
    std::array<UST, size> a    = {{t_pack...}};

    auto f = [value](UST e) -> bool
    {
        return e < value;
    };

    return std::all_of(a.begin(), a.end(), f);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_pack>
[[nodiscard]] inline consteval auto pp_all_true() noexcept -> bool
{
    constexpr UST          size = sizeof...(t_pack);
    std::array<bool, size> a    = {{t_pack...}};

    auto f = [](UST e) -> bool
    {
        return e;
    };

    return std::all_of(a.begin(), a.end(), f);
}

} // namespace mjolnir
