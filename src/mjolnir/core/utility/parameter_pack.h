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

template <UST... t_pack>
[[nodiscard]] inline consteval auto pp_all_less(UST value) noexcept -> bool;

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


} // namespace mjolnir
