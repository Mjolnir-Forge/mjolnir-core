//! @file
//! pointer_operations.h
//!
//!
//! @brief
//! Contains multiple utility functions for pointers


#pragma once

#include "mjolnir/core/fundamental_types.h"

#include <cstdint>


namespace mjolnir
{
//! \addtogroup core_utility
//! @{


//! @brief Check if a passed pointer is aligned.
//!
//! @tparam t_alignment: Required alignment
//!
//! @param [in] pointer: Pointer that should be checked
//!
//! @return `true` or `false`
template <UST t_alignment>
[[nodiscard]] inline auto is_aligned(const volatile void* pointer) noexcept -> bool;


//! @brief Check if a passed pointer is aligned.
//!
//! @param [in] pointer: Pointer that should be checked
//! @param [in] alignment: Required alignment
//!
//! @return `true` or `false`
[[nodiscard]] inline auto is_aligned(const volatile void* pointer, UST alignment) noexcept -> bool;


//! @brief Calculate the misalignment of a pointer.
//!
//! @tparam t_alignment: Required alignment
//!
//! @param [in] pointer: Pointer that should be checked
//!
//! @return Misalignment of the pointer in bytes
template <UST t_alignment>
[[nodiscard]] inline auto misalignment(const volatile void* pointer) noexcept -> UST;


//! @brief Calculate the misalignment of a pointer.
//!
//! @param [in] pointer: Pointer that should be checked
//! @param [in] alignment: Required alignment
//!
//! @return Misalignment of the pointer in bytes
[[nodiscard]] inline auto misalignment(const volatile void* pointer, UST alignment) noexcept -> UST;


//! @brief Turn a pointer into an integer.
//!
//! @param [in] pointer: Pointer that should be converted
//!
//! @return Integer representation of the pointer
[[nodiscard]] inline auto pointer_to_integer(const volatile void* pointer) noexcept -> std::uintptr_t;


//! @}
} // namespace mjolnir


// ====================================================================================================================

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <UST t_alignment>
[[nodiscard]] inline auto is_aligned(const volatile void* pointer) noexcept -> bool
{
    // https://stackoverflow.com/a/28678613
    return misalignment<t_alignment>(pointer) == 0;
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto is_aligned(const volatile void* pointer, UST alignment) noexcept -> bool
{
    // https://stackoverflow.com/a/28678613
    return misalignment(pointer, alignment) == 0;
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_alignment>
[[nodiscard]] inline auto misalignment(const volatile void* pointer) noexcept -> UST
{
    return static_cast<UST>(pointer_to_integer(pointer) % t_alignment);
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto misalignment(const volatile void* pointer, UST alignment) noexcept -> UST
{
    return static_cast<UST>(pointer_to_integer(pointer) % alignment);
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto pointer_to_integer(const volatile void* pointer) noexcept -> std::uintptr_t
{
    // https://stackoverflow.com/a/26586211
    return reinterpret_cast<std::uintptr_t>(pointer); // NOLINT: intentional use of reinterpret_cast
}


} // namespace mjolnir
