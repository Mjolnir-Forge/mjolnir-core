//! @file
//! pointer_operations.h
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


//! @brief
//! Check if a passed pointer is aligned.
//!
//! @tparam t_alignment:
//! Required alignment
//!
//! @param [in] pointer:
//! Pointer that should be checked
//!
//! @return
//! `true` or `false`
template <UST t_alignment>
[[nodiscard]] inline auto is_aligned(const volatile void* pointer) noexcept -> bool;


//! @brief
//! Check if a passed pointer is aligned.
//!
//! @param [in] pointer:
//! Pointer that should be checked
//! @param [in] alignment:
//! Required alignment
//!
//! @return
//! `true` or `false`
//!
//! @remark
//! Use the template version if the required alignment is known at compile-time. It is usually faster.
//! Clang 10 with `-O3` generates the following assembly for this function:
//! ~~~ asm
//! is_aligned(void const volatile*, unsigned long):
//!     mov     rax, rdi
//!     xor     edx, edx
//!     div     rsi
//!     test    rdx, rdx
//!     sete    al
//!     ret
//! ~~~
//! @remark
//! The output for the template version with 4 byte alignment looks like this:
//! ~~~ asm
//! bool is_aligned<4ul>(void const volatile*):
//!     test    dil, 3
//!     sete    al
//!     ret
//! ~~~
//! @remark
//! Note that it consists of less instructions and that the expensive `div` instruction is removed.
[[nodiscard]] inline auto is_aligned(const volatile void* pointer, UST alignment) noexcept -> bool;


//! @brief
//! Calculate the misalignment of a pointer.
//!
//! @tparam t_alignment:
//! Required alignment
//!
//! @param [in] pointer:
//! Pointer that should be checked
//!
//! @return
//! Misalignment of the pointer in bytes
template <UST t_alignment>
[[nodiscard]] inline auto misalignment(const volatile void* pointer) noexcept -> UST;


//! @brief
//! Calculate the misalignment of a pointer.
//!
//! @param [in] pointer:
//! Pointer that should be checked
//! @param [in] alignment:
//! Required alignment
//!
//! @return
//! Misalignment of the pointer in bytes
//!
//! @remark
//! Use the template version if the required alignment is known at compile-time. It is usually faster.
//! Clang 10 with `-O3` generates the following assembly for this function:
//! ~~~ asm
//! misalignment(void const volatile*, unsigned long):
//!     mov     rax, rdi
//!     xor     edx, edx
//!     div     rsi
//!     mov     rax, rdx
//!     ret
//! ~~~
//! @remark
//! The output for the template version with 4 byte alignment looks like this:
//! ~~~ asm
//! unsigned long misalignment<4ul>(void const volatile*):
//!     mov     rax, rdi
//!     and     eax, 3
//!     ret
//! ~~~
//! @remark
//! Note that it consists of less instructions and that the expensive `div` instruction is removed.
[[nodiscard]] inline auto misalignment(const volatile void* pointer, UST alignment) noexcept -> UST;


//! @brief
//! Turn a pointer into an integer.
//!
//! @param [in] pointer:
//! Pointer that should be converted
//!
//! @return
//! Integer representation of the pointer
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
    return reinterpret_cast<std::uintptr_t>(pointer); // OLINT: intentional use of reinterpret_cast
}


} // namespace mjolnir
