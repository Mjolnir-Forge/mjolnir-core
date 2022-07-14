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


// ---internal declarations -------------------------------------------------------------------------------------------

//! \cond DO_NOT_DOCUMENT
namespace internal
{
template <UST t_num_elements, DoublePrecisionVectorRegister T_RegisterType>
[[nodiscard]] inline auto element_sum_first_n(T_RegisterType src) noexcept -> ElementType<T_RegisterType>;


template <UST t_num_elements, SinglePrecisionVectorRegister T_RegisterType>
[[nodiscard]] inline auto element_sum_first_n(T_RegisterType src) noexcept -> ElementType<T_RegisterType>;
} // namespace internal
//! \endcond


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================


#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"
#include "mjolnir/core/x86/x86.h"

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------


template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto broadcast_element_sum(T_RegisterType src) noexcept -> T_RegisterType
{
    if constexpr (is_single_precision<T_RegisterType>)
    {
        T_RegisterType sum = mm_add(src, permute<1, 0, 3, 2>(src));
        sum                = mm_add(sum, permute<2, 3, 0, 1>(sum));

        if constexpr (is_avx_register<T_RegisterType>)
            sum = mm_add(sum, swap_lanes(sum));

        return sum;
    }
    else
    {
        T_RegisterType sum = mm_add(src, permute<1, 0>(src));

        if constexpr (is_avx_register<T_RegisterType>)
            sum = mm_add(sum, swap_lanes(sum));

        return sum;
    }
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

    return internal::element_sum_first_n<t_num_elements, T_RegisterType>(src);
}


// --- internal definitions -------------------------------------------------------------------------------------------

//! \cond DO_NOT_DOCUMENT
namespace internal
{
// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_elements, DoublePrecisionVectorRegister T_RegisterType>
[[nodiscard]] inline auto element_sum_first_n(T_RegisterType src) noexcept -> ElementType<T_RegisterType>
{
    if constexpr (t_num_elements == 1)
        return mm_cvt_float(src);
    else if constexpr (t_num_elements == num_elements<T_RegisterType>)
        return element_sum(src);
    else
    {
        T_RegisterType sum = mm_add(src, permute<1, 0>(src));

        if constexpr (t_num_elements == 3)
            sum = mm_add(sum, swap_lanes(src));

        return mm_cvt_float(sum);
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_elements, SinglePrecisionVectorRegister T_RegisterType>
[[nodiscard]] inline auto element_sum_first_n(T_RegisterType src) noexcept -> ElementType<T_RegisterType>
{
    if constexpr (t_num_elements == 1)
        return mm_cvt_float(src);
    else if constexpr (t_num_elements == num_elements<T_RegisterType>)
        return element_sum(src);
    else if constexpr (t_num_elements == 7) // NOLINT(readability-magic-numbers)
    {
        auto zero = mm_setzero<__m256>();
        return element_sum(blend_at<t_num_elements>(src, zero));
    }
    else
    {
        T_RegisterType sum = mm_add(src, permute<1, 0, 3, 2>(src));

        if constexpr (t_num_elements == 3)
            sum = mm_add(sum, broadcast<2>(src));

        if constexpr (t_num_elements == 4 || t_num_elements == 5) // NOLINT(readability-magic-numbers)
        {
            sum = mm_add(sum, permute<2, 3, 0, 1>(sum));

            if constexpr (t_num_elements == 5) // NOLINT(readability-magic-numbers)
                sum = mm_add(sum, swap_lanes(src));
        }

        if constexpr (t_num_elements == 6) // NOLINT(readability-magic-numbers)
        {
            __m256 tmp = sum;
            sum        = mm_add(sum, permute<2, 3, 0, 1>(sum));
            sum        = mm_add(sum, swap_lanes(tmp));
        }

        return mm_cvt_float(sum); // NOLINT(readability-misleading-indentation)
    }
}
//! \endcond
} // namespace internal


} // namespace mjolnir::x86
