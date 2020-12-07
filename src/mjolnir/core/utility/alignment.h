//! @file
//! alignment.h
//!
//! @brief
//! A set of functions that deal with the alignment of data types.
//!
//! \addtogroup core
//! @{

#pragma once

#include "mjolnir/core/fundamental_types.h"


namespace mjolnir
{
//! @brief Calculate the misalignment of a pointer
//!
//! @param pointer: Pointer that should be checked
//! @param alignment: Required alignment
//!
//! @return Misalignment of the pointer in bytes
inline auto misalignment(const volatile void* pointer, UST alignment) -> UST;


//! @brief Check if a passed pointer is aligned
//!
//! @param pointer: Pointer that should be checked
//! @param alignment: Required alignment
//!
//! @return `true` or `false`
inline auto is_aligned(const volatile void* pointer, UST alignment) -> bool;


// ====================================================================================================================

#include <stdint.h>


// --------------------------------------------------------------------------------------------------------------------

inline auto is_aligned(const volatile void* pointer, UST alignment) -> bool
{
    return misalignment(pointer, alignment) % alignment == 0;
}


// --------------------------------------------------------------------------------------------------------------------

inline auto misalignment(const volatile void* pointer, UST alignment) -> UST
{
    return reinterpret_cast<std::uintptr_t>(pointer) % alignment;
}


} // namespace mjolnir


//! @}
