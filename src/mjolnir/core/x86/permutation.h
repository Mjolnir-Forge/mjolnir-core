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


//! @brief
//! Blend elements from `src_0` and `src_1` into a new register using the template parameters `t_e_0` and `t_e_1`.
//!
//! @details
//! The template parameters can only have the values 0 or 1. Other values will cause a compile-time error. If a
//! parameter is 0, the result value of the corresponding element is taken from `src_0`. Otherwise, the value of
//! `src_1` is used.
//!
//! @tparam t_e_0:
//! Selects the source of the 1. element
//! @tparam t_e_1:
//! Selects the source of the 2. element
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with blended values
template <U32 t_e_0, U32 t_e_1>
[[nodiscard]] inline auto blend(__m128d src_0, __m128d src_1) noexcept -> __m128d;


//! @brief
//! Blend elements from `src_0` and `src_1` into a new register using the template parameters `t_e_0` to `t_e_3`.
//!
//! @details
//! The template parameters can only have the values 0 or 1. Other values will cause a compile-time error. If a
//! parameter is 0, the result value of the corresponding element is taken from `src_0`. Otherwise, the value of
//! `src_1` is used.
//!
//! @tparam t_e_0:
//! Selects the source of the 1. element
//! @tparam t_e_1:
//! Selects the source of the 2. element
//! @tparam t_e_2:
//! Selects the source of the 3. element
//! @tparam t_e_3:
//! Selects the source of the 4. element
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with blended values
template <U32 t_e_0, U32 t_e_1, U32 t_e_2, U32 t_e_3, typename T_RegisterType>
[[nodiscard]] inline auto blend(T_RegisterType src_0, T_RegisterType src_1) -> T_RegisterType;


//! @brief
//! Blend elements from `src_0` and `src_1` into a new register using the template parameters `t_e_0` to `t_e_7`.
//!
//! @details
//! The template parameters can only have the values 0 or 1. Other values will cause a compile-time error. If a
//! parameter is 0, the result value of the corresponding element is taken from `src_0`. Otherwise, the value of
//! `src_1` is used.
//!
//! @tparam t_e_0:
//! Selects the source of the 1. element
//! @tparam t_e_1:
//! Selects the source of the 2. element
//! @tparam t_e_2:
//! Selects the source of the 3. element
//! @tparam t_e_3:
//! Selects the source of the 4. element
//! @tparam t_e_4:
//! Selects the source of the 5. element
//! @tparam t_e_5:
//! Selects the source of the 6. element
//! @tparam t_e_6:
//! Selects the source of the 7. element
//! @tparam t_e_7:
//! Selects the source of the 8. element
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with blended values
template <U32 t_e_0, U32 t_e_1, U32 t_e_2, U32 t_e_3, U32 t_e_4, U32 t_e_5, U32 t_e_6, U32 t_e_7>
[[nodiscard]] inline auto blend(__m256 src_0, __m256 src_1) noexcept -> __m256;


//! @brief
//! Create a control integer for blend intrinsics and vector registers of size 2.
//!
//! @details
//! The template parameters can only have the values 0 or 1. Other values will cause a compile-time error. If a
//! parameter is 0, the result value of the corresponding element is taken from the first source. Otherwise, the value
//! of the second source is used.
//!
//! @tparam t_e_0:
//! Selects the source of the 1. element
//! @tparam t_e_1:
//! Selects the source of the 2. element
//!
//! @return
//! Mask for blending
template <U32 t_e_0, U32 t_e_1>
[[nodiscard]] consteval inline auto blend_mask() noexcept -> U32;


//! @brief
//! Create a control integer for blend intrinsics and vector registers of size 4.
//!
//! @details
//! The template parameters can only have the values 0 or 1. Other values will cause a compile-time error. If a
//! parameter is 0, the result value of the corresponding element is taken from the first source. Otherwise, the value
//! of the second source is used.
//!
//! @tparam t_e_0:
//! Selects the source of the 1. element
//! @tparam t_e_1:
//! Selects the source of the 2. element
//! @tparam t_e_2:
//! Selects the source of the 3. element
//! @tparam t_e_3:
//! Selects the source of the 4. element
//!
//! @return
//! Mask for blending
template <U32 t_e_0, U32 t_e_1, U32 t_e_2, U32 t_e_3>
[[nodiscard]] consteval inline auto blend_mask() noexcept -> U32;


//! @brief
//! Create a control integer for blend intrinsics and vector registers of size 8.
//!
//! @details
//! The template parameters can only have the values 0 or 1. Other values will cause a compile-time error. If a
//! parameter is 0, the result value of the corresponding element is taken from the first source. Otherwise, the value
//! of the second source is used.
//!
//! @tparam t_e_0:
//! Selects the source of the 1. element
//! @tparam t_e_1:
//! Selects the source of the 2. element
//! @tparam t_e_2:
//! Selects the source of the 3. element
//! @tparam t_e_3:
//! Selects the source of the 4. element
//! @tparam t_e_4:
//! Selects the source of the 5. element
//! @tparam t_e_5:
//! Selects the source of the 6. element
//! @tparam t_e_6:
//! Selects the source of the 7. element
//! @tparam t_e_7:
//! Selects the source of the 8. element
//!
//! @return
//! Mask for blending
template <U32 t_e_0, U32 t_e_1, U32 t_e_2, U32 t_e_3, U32 t_e_4, U32 t_e_5, U32 t_e_6, U32 t_e_7>
[[nodiscard]] consteval inline auto blend_mask() noexcept -> U32;


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

template <U32 t_e_0, U32 t_e_1, U32 t_e_2, U32 t_e_3, typename T_RegisterType>
[[nodiscard]] inline auto blend(T_RegisterType src_0, T_RegisterType src_1) -> T_RegisterType
{
    static_assert(num_elements<T_RegisterType> == 4, "Only registers with 4 elements supported.");

    return mm_blend<blend_mask<t_e_0, t_e_1, t_e_2, t_e_3>()>(src_0, src_1);
}


// --------------------------------------------------------------------------------------------------------------------

template <U32 t_e_0, U32 t_e_1, U32 t_e_2, U32 t_e_3, U32 t_e_4, U32 t_e_5, U32 t_e_6, U32 t_e_7>
[[nodiscard]] inline auto blend(__m256 src_0, __m256 src_1) noexcept -> __m256
{
    return mm_blend<blend_mask<t_e_0, t_e_1, t_e_2, t_e_3, t_e_4, t_e_5, t_e_6, t_e_7>()>(src_0, src_1);
}


// --------------------------------------------------------------------------------------------------------------------

template <U32 t_e_0, U32 t_e_1>
[[nodiscard]] consteval inline auto blend_mask() noexcept -> U32
{
    static_assert(t_e_0 < 2 && t_e_1 < 2, "Values t_e_0 and t_e_1 must be in the interval [0, 1].");

    return ((t_e_1 << 1U) | (t_e_0));
}


// --------------------------------------------------------------------------------------------------------------------

template <U32 t_e_0, U32 t_e_1, U32 t_e_2, U32 t_e_3>
[[nodiscard]] consteval inline auto blend_mask() noexcept -> U32
{
    static_assert(t_e_0 < 2 && t_e_1 < 2 && t_e_2 < 2 && t_e_3 < 2,
                  "Values t_e_0 to t_e_3 must be in the interval [0, 1]");

    return (((t_e_3) << 3U) | ((t_e_2) << 2U) | ((t_e_1) << 1U) | (t_e_0));
}


// --------------------------------------------------------------------------------------------------------------------

template <U32 t_e_0, U32 t_e_1, U32 t_e_2, U32 t_e_3, U32 t_e_4, U32 t_e_5, U32 t_e_6, U32 t_e_7>
[[nodiscard]] consteval inline auto blend_mask() noexcept -> U32
{
    static_assert(t_e_0 < 2 && t_e_1 < 2 && t_e_2 < 2 && t_e_3 < 2 && t_e_4 < 2 && t_e_5 < 2 && t_e_6 < 2 && t_e_7 < 2,
                  "Values t_e_0 to t_e_7 must be in the interval [0, 1]");

    // NOLINTNEXTLINE - magic numbers
    return (((t_e_7) << 7U) | ((t_e_6) << 6U) | ((t_e_5) << 5U) | ((t_e_4) << 4U) | ((t_e_3) << 3U) | ((t_e_2) << 2U)
            | ((t_e_1) << 1U) | (t_e_0));
}


} // namespace mjolnir::x86
