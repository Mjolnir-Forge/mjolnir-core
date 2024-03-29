//! @file
//! pointer_operations.h
//!
//! @brief
//! Contains multiple utility functions for pointers


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/fundamental_types.h"

#include <cstdint>


namespace mjolnir
{
//! \addtogroup core_utility
//! @{

//! @brief
//! Turn an integer into a pointer of the chosen type.
//!
//! @tparam T_Type:
//! The pointer type
//!
//! @param[in] integer:
//! The integer that should be converted
//!
//! @return
//! Converted pointer.
//!
//! @todo
//! Replace reinterpret_cast with bit_cast (requires increase of compiler version)
template <typename T_Type = void>
[[nodiscard]] inline auto integer_to_pointer(UPT integer) noexcept -> T_Type*;


//! @brief
//! Check if a passed pointer is aligned.
//!
//! @tparam t_alignment:
//! Required alignment
//! @tparam T_Type
//! The pointer type
//!
//! @param [in] pointer:
//! Pointer that should be checked
//!
//! @return
//! `true` or `false`
template <UST t_alignment, typename T_Type>
[[nodiscard]] inline auto is_aligned(const volatile T_Type* pointer) noexcept -> bool;


//! @brief
//! Check if a passed pointer is aligned.
//!
//! @tparam T_Type
//! The pointer type
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
template <typename T_Type>
[[nodiscard]] inline auto is_aligned(const volatile T_Type* pointer, UST alignment) noexcept -> bool;


//! @brief
//! Calculate the misalignment of a pointer.
//!
//! @tparam t_alignment:
//! Required alignment
//! @tparam T_Type
//! The pointer type
//!
//! @param [in] pointer:
//! Pointer that should be checked
//!
//! @return
//! Misalignment of the pointer in bytes
template <UST t_alignment, typename T_Type>
[[nodiscard]] inline auto misalignment(const volatile T_Type* pointer) noexcept -> UST;


//! @brief
//! Calculate the misalignment of a pointer.
//!
//! @tparam T_Type
//! The pointer type
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
template <typename T_Type>
[[nodiscard]] inline auto misalignment(const volatile T_Type* pointer, UST alignment) noexcept -> UST;


//! @brief
//! Turn a pointer into an integer.
//!
//! @tparam T_Type
//! The pointer type
//!
//! @param [in] pointer:
//! Pointer that should be converted
//!
//! @return
//! Integer representation of the pointer
//!
//! @todo
//! Replace reinterpret_cast with bit_cast (requires increase of compiler version)
template <typename T_Type>
[[nodiscard]] inline auto pointer_to_integer(const volatile T_Type* pointer) noexcept -> UPT;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================

namespace mjolnir
{
template <typename T_Type>
[[nodiscard]] inline auto integer_to_pointer(UPT integer) noexcept -> T_Type*
{
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast,performance-no-int-to-ptr)
    return reinterpret_cast<T_Type*>(integer);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_alignment, typename T_Type>
[[nodiscard]] inline auto is_aligned(const volatile T_Type* pointer) noexcept -> bool
{
    // https://stackoverflow.com/a/28678613
    return misalignment<t_alignment>(pointer) == 0;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] inline auto is_aligned(const volatile T_Type* pointer, UST alignment) noexcept -> bool
{
    // https://stackoverflow.com/a/28678613
    return misalignment(pointer, alignment) == 0;
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_alignment, typename T_Type>
[[nodiscard]] inline auto misalignment(const volatile T_Type* pointer) noexcept -> UST
{
    return pointer_to_integer(pointer) % t_alignment;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] inline auto misalignment(const volatile T_Type* pointer, UST alignment) noexcept -> UST
{
    return pointer_to_integer(pointer) % alignment;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] inline auto pointer_to_integer(const volatile T_Type* pointer) noexcept -> UPT
{
    // https://stackoverflow.com/a/26586211
    // NOLINTNEXTLINE(cppcoreguidelines-pro-type-reinterpret-cast) intentional, see link above
    return reinterpret_cast<UPT>(pointer);
}


} // namespace mjolnir
