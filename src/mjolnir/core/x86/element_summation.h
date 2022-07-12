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
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Register with all elements set to the element sum of `src`
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto broadcast_element_sum(T_RegisterType src) noexcept -> T_RegisterType;


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


//! @brief
//! Return the sum of the first `t_num_elements` elements from `src`.
//!
//! @tparam t_num_elements:
//! Number of elements that should be summed up.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Sum of the first `t_num_elements` elements from `src`
template <UST t_num_elements, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto element_sum_first_n(T_RegisterType src) noexcept -> ElementType<T_RegisterType>;


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================


#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"
#include "mjolnir/core/x86/x86.h"

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto broadcast_element_sum(__m128 src) noexcept -> __m128
{
    __m128 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    return mm_add(sum, permute<2, 3, 0, 1>(sum));
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto broadcast_element_sum(__m128d src) noexcept -> __m128d
{
    return mm_add(src, permute<1, 0>(src));
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto broadcast_element_sum(__m256 src) noexcept -> __m256
{
    __m256 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    sum        = mm_add(sum, permute<2, 3, 0, 1>(sum));
    return mm_add(sum, swap_lanes(sum));
}


// --------------------------------------------------------------------------------------------------------------------

template <>
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


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_elements, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto element_sum_first_n(T_RegisterType src) noexcept -> ElementType<T_RegisterType>
{
    constexpr UST n_e = num_elements<T_RegisterType>;

    static_assert(t_num_elements > 0, "`t_num_elements` must be larger than 0.");
    static_assert(t_num_elements <= n_e, "`t_num_elements` must be less or equal to the number of register elements.");


    // All other cases are implemented in template specializations
    static_assert(t_num_elements == n_e || t_num_elements == 1, "Internal error: missing template specialization");


    if constexpr (t_num_elements == 1)
        return mm_cvt_float(src);
    else
        return element_sum(src);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<2, __m128>(__m128 src) noexcept -> F32
{
    __m128 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    return mm_cvt_float(sum);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<2, __m256>(__m256 src) noexcept -> F32
{
    __m256 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    return mm_cvt_float(sum);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<2, __m256d>(__m256d src) noexcept -> F64
{
    __m256d sum = mm_add(src, permute<1, 0>(src));
    return mm_cvt_float(sum);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<3, __m128>(__m128 src) noexcept -> F32
{
    __m128 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    sum        = mm_add(sum, broadcast<2>(src));
    return mm_cvt_float(sum);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<3, __m256>(__m256 src) noexcept -> F32
{
    __m256 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    sum        = mm_add(sum, broadcast<2>(src));
    return mm_cvt_float(sum);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<3, __m256d>(__m256d src) noexcept -> F64
{
    __m256d sum = mm_add(src, permute<1, 0>(src));
    sum         = mm_add(sum, swap_lanes(src));
    return mm_cvt_float(sum);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<4, __m256>(__m256 src) noexcept -> F32
{
    __m256 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    sum        = mm_add(sum, permute<2, 3, 0, 1>(sum));
    return mm_cvt_float(sum);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<5, __m256>(__m256 src) noexcept -> F32
{
    __m256 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    sum        = mm_add(sum, permute<2, 3, 0, 1>(sum));
    sum        = mm_add(sum, swap_lanes(src));
    return mm_cvt_float(sum);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<6, __m256>(__m256 src) noexcept -> F32
{
    __m256 sum = mm_add(src, permute<1, 0, 3, 2>(src));
    __m256 tmp = sum;
    sum        = mm_add(sum, permute<2, 3, 0, 1>(sum));
    sum        = mm_add(sum, swap_lanes(tmp));
    return mm_cvt_float(sum);
}


// --------------------------------------------------------------------------------------------------------------------

template <>
[[nodiscard]] inline auto element_sum_first_n<7, __m256>(__m256 src) noexcept -> F32
{
    __m256 zero = mm_setzero<__m256>();
    return element_sum(blend_at<7>(src, zero));
}


} // namespace mjolnir::x86
