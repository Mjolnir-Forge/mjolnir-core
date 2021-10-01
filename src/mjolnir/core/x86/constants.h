//! @file
//! intrinsics.h
//!
//! @brief
//! Contains x86 specific constants


#pragma once

#include "mjolnir/core/x86/x86.h"
#include "mjolnir/core/fundamental_types.h"

namespace mjolnir
{
//! \addtogroup core_x86
//! @{

//! @brief Alignment of an SSE register in bytes
inline constexpr const UST alignment_bytes_sse = 16;

//! @brief Alignment of an AVX register in bytes
inline constexpr const UST alignment_bytes_avx = 32;

//! @brief Register type dependent alignment in bytes.
//! @tparam _registerType: RegisterType
template <typename T_RegisterType>
inline constexpr const U32 alignment_bytes = 0;
template <>
inline constexpr const U32 alignment_bytes<__m128> = alignment_bytes_sse;
template <>
inline constexpr const U32 alignment_bytes<__m128d> = alignment_bytes_sse;
template <>
inline constexpr const U32 alignment_bytes<__m128i> = alignment_bytes_sse;
#ifdef __AVX2__
template <>
inline constexpr const U32 alignment_bytes<__m256> = alignment_bytes_avx;
template <>
inline constexpr const U32 alignment_bytes<__m256d> = alignment_bytes_avx;
template <>
inline constexpr const U32 alignment_bytes<__m256i> = alignment_bytes_avx;
#endif // __AVX2__

//! @}
} // namespace mjolnir


// ====================================================================================================================

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------


} // namespace mjolnir
