//! @file
//! memory/utility.h
//!
//! @brief
//! Utility functions and classes for the memory submodule


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/memory/definitions.h"
#include "mjolnir/core/utility/pointer_operations.h"

#include "memory"

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


//! @brief
//! Destroy the object that the passed pointer points to.
//!
//! @details
//! This function just calls the destructor, but adds an assertion that the pointer is not a `nullptr`.
//!
//! @tparam T_Type:
//! Type of the object
//!
//! @param[in] pointer:
//! Pointer pointing to the object that should be destroyed
template <typename T_Type>
inline void destroy(T_Type* pointer) noexcept;


//! @brief
//! Return `true` if `pointer` is part of the memory starting at `memory_start_ptr`.
//!
//! @tparam T_Type:
//! Type of the pointer
//!
//! @param[in] pointer:
//! The pointer that should be checked
//! @param[in] memory_start_ptr:
//! Pointer to the start of the memory
//! @param[in] memory_size:
//! Memory size in bytes
//!
//! @return
//! `true` or `false`
template <typename T_Type>
[[nodiscard]] constexpr auto
is_pointer_in_memory(T_Type* pointer, std::byte* memory_start_ptr, UST memory_size) noexcept -> bool;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


#include "mjolnir/core/math/math.h"

#include <cassert>

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline constexpr auto align_address(UPT address, UST alignment) noexcept -> UPT
{
    assert(is_power_of_2(alignment) && "Alignment must be a power of 2."); // NOLINT

    // source: https://stackoverflow.com/a/4840428/6700329
    // The original code was slightly modified since MSVC complained about the unary minus on an unsigned type.
    UST decr_align = alignment - 1;

    return address + decr_align & ~decr_align;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
inline void destroy(T_Type* pointer) noexcept
{
    assert(pointer != nullptr && "The passed pointer is the `nullptr`."); // NOLINT
    pointer->~T_Type();
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] constexpr auto
is_pointer_in_memory(T_Type* pointer, std::byte* memory_start_ptr, UST memory_size) noexcept -> bool
{
    UPT addr         = pointer_to_integer(pointer);
    UPT memory_start = pointer_to_integer(memory_start_ptr);
    return addr >= memory_start && addr < memory_start + memory_size;
}


} // namespace mjolnir
