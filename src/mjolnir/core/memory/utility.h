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

//! @brief
//! Get the next pointer address after the passed one that fulfills the given alignment requirements.
//!
//! @details
//! If the passed address is already aligned correctly, it will be returned instead.
//!
//! @param[in] address:
//! The reference address
//! @param[in] alignment:
//! The required alignment
//!
//! @return
//! Aligned pointer address
[[nodiscard]] inline constexpr auto align_address(UPT address, UST alignment) noexcept -> UPT;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline constexpr auto align_address(UPT address, UST alignment) noexcept -> UPT
{
    // todo -> implement assert
    // assert(IsPowerOf2(alignment), "Alignment must be a power of 2."); // NOLINT

    // source: https://stackoverflow.com/a/4840428/6700329
    // The original code was slightly modified since MSVC complained about the unary minus on an unsigned type.
    UST decr_align = alignment - 1;

    return address + decr_align & ~decr_align;
}


} // namespace mjolnir
