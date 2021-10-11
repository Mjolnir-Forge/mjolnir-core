//! @file
//! direct_access.h
//!
//! @brief
//! Contains functions to permute and blend the elements of verctor registers.


#pragma once

#include "mjolnir/core/x86/definitions.h"

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


template <U32 t_e_0, U32 t_e_1>
[[nodiscard]] consteval inline auto blend_mask() noexcept -> U32;


template <U32 t_e_0, U32 t_e_1>
[[nodiscard]] inline auto blend(__m128d src_0, __m128d src_1) noexcept -> __m128d;

//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/x86/intrinsics.h"


namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <U32 t_e_0, U32 t_e_1>
[[nodiscard]] inline auto blend(__m128d src_0, __m128d src_1) noexcept -> __m128d
{
    return mm_blend<blend_mask<t_e_0, t_e_1>()>(src_0, src_1);
}


// --------------------------------------------------------------------------------------------------------------------

template <U32 t_e_0, U32 t_e_1>
[[nodiscard]] consteval inline auto blend_mask() noexcept -> U32
{
    static_assert(t_e_0 < 2 && t_e_0 < 2, "Values t_e_0 ans t_e_1 must be in the interval [0, 1].");

    return (((t_e_1) << 1) | (t_e_0));
}

} // namespace mjolnir::x86
