//! @file
//! alignment.h
//!
//!
//! @brief
//! A second test header
//!


#pragma once

#include "mjolnir/core/fundamental_types.h"

#include <cstdint>


namespace mjolnir
{
//! \addtogroup core_utility
//! @{


//! @brief Calculate the misalignment of a pointer.
//!
//! @param [in] pointer: Pointer that should be checked
//! @param [in] alignment: Required alignment
//!
//! @return Misalignment of the pointer in bytes
inline auto misalignment(const volatile void* pointer, UST alignment) -> UST;


//! @brief Check if a passed pointer is aligned.
//!
//! @param [in] pointer: Pointer that should be checked
//! @param [in] alignment: Required alignment
//!
//! @return `true` or `false`
inline auto is_aligned(const volatile void* pointer, UST alignment) -> bool;


//! @brief Turn a pointer into an integer.
//!
//! @param [in] pointer: Pointer that should be converted
//!
//! @return Integer representation of the pointer
inline auto pointer_to_integer(const volatile void* pointer) -> std::uintptr_t;


//! @}
} // namespace mjolnir

// ====================================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

inline auto is_aligned(const volatile void* pointer, UST alignment) -> bool
{
    // https://stackoverflow.com/a/28678613
    return misalignment(pointer, alignment) % alignment == 0;
}


// --------------------------------------------------------------------------------------------------------------------

inline auto misalignment(const volatile void* pointer, UST alignment) -> UST
{
    return static_cast<UST>(pointer_to_integer(pointer) % alignment); // NOLINT: intentional use of reinterpret_cast
}


// --------------------------------------------------------------------------------------------------------------------

inline auto pointer_to_integer(const volatile void* pointer) -> std::uintptr_t
{
    // https://stackoverflow.com/a/26586211
    return reinterpret_cast<std::uintptr_t>(pointer);
}


} // namespace mjolnir
