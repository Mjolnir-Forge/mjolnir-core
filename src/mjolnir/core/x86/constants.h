//! @file
//! intrinsics.h
//!
//! @brief
//! Contains x86 specific constants


#pragma once
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/type.h"
#include "mjolnir/core/x86/x86.h"
#include <type_traits>

namespace mjolnir
{
//! \addtogroup core_x86
//! @{

// --------------------------------------------------------------------------------------------------------------------
// internal
// --------------------------------------------------------------------------------------------------------------------

//! \cond DO_NOT_DOCUMENT
namespace internal
{
inline constexpr const UST alignment_bytes_default = 4;
inline constexpr const UST alignment_bytes_sse     = 16;
inline constexpr const UST alignment_bytes_avx     = 32;

//! @brief
//! Return the alignment in bytes for a register type.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @return
//! Alignment in bytes
template <typename T_RegisterType>
[[nodiscard]] inline consteval auto get_alignment_bytes() noexcept -> UST;


} // namespace internal
//! \endcond

// --------------------------------------------------------------------------------------------------------------------
// public
// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! Register type dependent alignment in bytes.
//!
//! @tparam _registerType:
//! RegisterType
template <typename T_RegisterType>
inline constexpr const U32 alignment_bytes = internal::get_alignment_bytes<T_RegisterType>();

//! @}
} // namespace mjolnir


// ====================================================================================================================

// --------------------------------------------------------------------------------------------------------------------
// internal
// --------------------------------------------------------------------------------------------------------------------


namespace mjolnir
{
//! \cond DO_NOT_DOCUMENT
namespace internal
{
template <typename T_RegisterType>
[[nodiscard]] inline consteval auto get_alignment_bytes() noexcept -> UST
{
    if constexpr (is_any_of<T_RegisterType, __m128, __m128d, __m128i>())
        return alignment_bytes_sse;
    if constexpr (is_any_of<T_RegisterType, __m256, __m256d, __m256i>())
        return alignment_bytes_avx;
    else
        return alignment_bytes_default;
}
} // namespace internal
//! \endcond


// --------------------------------------------------------------------------------------------------------------------
// public
// --------------------------------------------------------------------------------------------------------------------


// --------------------------------------------------------------------------------------------------------------------


} // namespace mjolnir
