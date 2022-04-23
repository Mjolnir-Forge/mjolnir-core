//! @file
//! parameter_pack.h
//!
//! @brief
//! Contains utility functions for parameter packs.


#pragma once

#include "mjolnir/core/fundamental_types.h"

#include <array>
#include <concepts>


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

    for (auto& e : a)
        if (e >= value)
            return false;
    return true;
}


} // namespace mjolnir
