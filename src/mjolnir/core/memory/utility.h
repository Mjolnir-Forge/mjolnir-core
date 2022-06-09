//! @file
//! memory/utility.h
//!
//! @brief
//! Utility functions and classes for the memory submodule


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/fundamental_types.h"



namespace mjolnir
{
//! \addtogroup core_memory
//! @{


[[nodiscard]] inline constexpr auto get_aligned_address(UPT address, UST alignment) noexcept -> UPT;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================



namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline constexpr auto get_aligned_address(UPT address, UST alignment) noexcept -> UPT
{
    // todo -> implement assert
    // assert(IsPowerOf2(alignment), "Alignment must be a power of 2."); // NOLINT
    auto mask           = -static_cast<IPT>(alignment);
    auto offset_addr    = static_cast<IPT>(address + (alignment - 1));

    return static_cast<UPT>(offset_addr & mask);
}



} // namespace mjolnir
