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


namespace internal
{
inline constexpr const UST alignment_bytes_default = 4;
inline constexpr const UST alignment_bytes_sse     = 16;
inline constexpr const UST alignment_bytes_avx     = 32;

template <typename T_RegisterType>
inline consteval auto get_alignment_bytes() -> UST
{
    if constexpr (is_any_of<T_RegisterType, __m128, __m128d, __m128i>())
        return alignment_bytes_sse;
    if constexpr (is_any_of<T_RegisterType, __m256, __m256d, __m256i>())
        return alignment_bytes_avx;
    else
        return alignment_bytes_default;
}

} // namespace internal


//! @brief Register type dependent alignment in bytes.
//! @tparam _registerType: RegisterType
template <typename T_RegisterType>
inline constexpr const U32 alignment_bytes = internal::get_alignment_bytes<T_RegisterType>();

//! @}
} // namespace mjolnir


// ====================================================================================================================

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------


} // namespace mjolnir
