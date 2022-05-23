//! @file
//! element_summation.h
//!
//! @brief
//! Contains functions to quickly sum up all elements of a vector register or an array of vector registers.


#pragma once

#include "mjolnir/core/x86/definitions.h"

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Calculate the sum of all elements of `src`, broadcast it into a new register and return the result.
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Register with all elements set to the element sum of `src`
[[nodiscard]] inline auto broadcast_element_sum(__m128 src) noexcept -> __m128;


//! @brief
//! Calculate the sum of all elements of `src`, broadcast it into a new register and return the result.
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Register with all elements set to the element sum of `src`
[[nodiscard]] inline auto broadcast_element_sum(__m128d src) noexcept -> __m128d;


//! @brief
//! Calculate the sum of all elements of `src`, broadcast it into a new register and return the result.
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Register with all elements set to the element sum of `src`
[[nodiscard]] inline auto broadcast_element_sum(__m256 src) noexcept -> __m256;


//! @brief
//! Calculate the sum of all elements of `src`, broadcast it into a new register and return the result.
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Register with all elements set to the element sum of `src`
[[nodiscard]] inline auto broadcast_element_sum(__m256d src) noexcept -> __m256d;


//! @brief
//! Return the sum of all elements from `src`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Sum of all elements from `src`
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto element_sum(T_RegisterType src) noexcept -> ElementType<T_RegisterType>;


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================


#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"
#include "mjolnir/core/x86/x86.h"

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto broadcast_element_sum(__m128 src) noexcept -> __m128
{
    __m128 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    return mm_add(sum, permute<2, 3, 0, 1>(sum));
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto broadcast_element_sum(__m128d src) noexcept -> __m128d
{
    return mm_add(src, permute<1, 0>(src));
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto broadcast_element_sum(__m256 src) noexcept -> __m256
{
    __m256 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    sum        = mm_add(sum, permute<2, 3, 0, 1>(sum));
    return mm_add(sum, swap_lanes(sum));
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto broadcast_element_sum(__m256d src) noexcept -> __m256d
{
    __m256d sum = mm_add(src, permute<1, 0>(src));
    return mm_add(sum, swap_lanes(sum));
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto element_sum(T_RegisterType src) noexcept -> ElementType<T_RegisterType>
{
    T_RegisterType sum = broadcast_element_sum(src);
    return mm_cvt_float(sum);
}

} // namespace mjolnir::x86
