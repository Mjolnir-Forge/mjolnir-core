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
//! Concatenate two floating-point registers, shift the result right by `t_shift` elements, and return the result.
//!
//! @tparam t_shift:
//! Number of elements to shift
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! Left-hand side register
//! @param[in] rhs:
//! Right-hand side register
//!
//! @return
//! Result register
template <UST t_shift, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto align_right([[maybe_unused]] T_RegisterType lhs, [[maybe_unused]] T_RegisterType rhs)
        -> T_RegisterType;


//! @brief
//! Blend elements from `src_0` and `src_1` into a new register.
//!
//! @tparam t_args:
//! Parameters pack to select the source register for the individual elements. The number of parameters must be equal to
//! the number of register elements. Each value must either be 0 or 1. Other values will cause a compile-time error. If
//! a parameter is 0, the result value of the corresponding element is taken from `src_0`. Otherwise, the value of
//! `src_1` is used.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with blended values
template <UST... t_args, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType;


//! @brief
//! Get a register where elements with a higher index than `t_index` are copied from `src_1`and the rest from `src_0`.
//!
//! @tparam t_index:
//! Up to the speciefied value, all elemets of the result register are identical to `src_0`. All other values are taken
//! from `src_1`
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with blended values
template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend_above(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType;


//! @brief
//! Get a new register where the element with index `t_index` is taken from `src_1` and the rest from `src_0`
//!
//! @tparam t_index:
//! Index that specifies the index of the only elemet that is taken from `src_1`
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with blended values
template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend_at(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType;


//! @brief
//! Get a register where elements with a lower index than `t_index` are copied from `src_1`and the rest from `src_0`.
//!
//! @tparam t_index:
//! All elemets of the result register with a lower index than this value are copied from `src_1`. All other values are
//! taken from `src_0`
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with blended values
template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend_below(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType;


//! @brief
//! Get a register where elements inside the specified index range are taken from `src_1` and the rest from `src_0`.
//!
//! @tparam t_index_first:
//! Index of the first element that is taken from `src_1`
//! @tparam t_index_last:
//! Index of the last element that is taken from `src_1`
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with blended values
template <UST t_index_first, UST t_index_last, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend_from_to(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType;


//! @brief
//! Broadcast a register element per lane selected by `t_index`.
//!
//! @details
//! For mulit-lane registers, the same index is used for every lane. The broadcasting does not cross lane boundaries.
//!
//! @tparam t_index:
//! Index of the lane element that should be broadcasted
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! New register with broadcasted values
template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto broadcast(T_RegisterType src) noexcept -> T_RegisterType;


//! @brief
//! Broadcast a register element per lane selected by `t_index_0` and `t_index_1`.
//!
//! @details
//! The broadcasting does not cross lane boundaries.
//!
//! @tparam t_index_0:
//! Index of the first lane's element that should be broadcasted
//! @tparam t_index_0:
//! Index of the second lane's element that should be broadcasted
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! New register with broadcasted values
template <UST t_index_0, UST t_index_1, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto broadcast(T_RegisterType src) noexcept -> T_RegisterType;


//! @brief
//! Broadcast a register element selected by `t_index` across lane boundaries.
//!
//! @details
//! For single-lane registers, the function behaves identical to `broadcast`.
//!
//! @tparam t_index:
//! Index of the element that should be broadcasted
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! New register with broadcasted values
template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto broadcast_across_lanes(T_RegisterType src) noexcept -> T_RegisterType;


//! @brief
//! Exchange two elements selected by indices between two registers.
//!
//! @details
//! The passed registers are modified during the operation. Note that the exchange is significantly slower if the
//! exchanged elements are not inside the same lane.
//!
//! @tparam t_index_0:
//! Selects the element of `reg_0` that should be exchanged
//! @tparam t_index_1:
//! Selects the element of `reg_1` that should be exchanged
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in, out] reg_0:
//! First register
//! @param[in, out] reg_1:
//! Second register
template <UST t_index_0, UST t_index_1, FloatVectorRegister T_RegisterType>
inline void exchange(T_RegisterType& reg_0, T_RegisterType& reg_1) noexcept;


//! @brief
//! Shuffle the elements of a vector register within lanes using indices and return the result in a new register.
//!
//! @tparam t_indices
//! A set of indices equal to the number of register elements or the number of lane elements. In the latter case the
//! permutation pattern is identical for all lanes. The N-th index selects the value for the N-th element/lane element.
//! Index values may not exceed the lane size of a register. Otherwise a compile-time error is triggered.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! New register with shuffled values
//!
//! @todo
//! assert that the 8 indices for the __m256 case do not exceed number of lane elemets. Maybe write a generalized
//! function for integer based parameter packs
template <UST... t_indices, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto permute(T_RegisterType src) noexcept -> T_RegisterType;


//! @brief
//! Create a new AVX register by an arbitrary combination of the source registers lanes.
//!
//! @tparam t_lane_0:
//! Set to `0` to copy the lower lane of the source register to the lower lane of the result register. Use `1`to copy
//! the upper lane
//! @tparam t_lane_1:
//! Set to `0` to copy the lower lane of the source register to the upper lane of the result register. Use `1` to copy
//! the upper lane
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! Source register
//!
//! @return
//! New register with an arbitrary combination of the source registers lanes
template <UST t_lane_0, UST t_lane_1, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto permute_lanes(T_RegisterType src) noexcept -> T_RegisterType;


//! @brief
//! Create a new AVX register by combining arbitrary lanes from two source registers.
//!
//! @tparam t_src_0:
//! Set to `0` to select `src_0` as source for the lower lane values of the result register and to `1` for `src_1`
//! @tparam t_lane_0:
//! Set to `0` to copy the lower lane of the selected source register to the lower lane of the result register. Use `1`
//! to copy the upper lane
//! @tparam t_src_1:
//! Set to `0` to select `src_0` as source for the upper lane values of the result register and to `1` for `src_1`
//! @tparam t_lane_1:
//! Set to `0` to copy the lower lane of the selected source register to the upper lane of the result register. Use `1`
//! to copy the upper lane
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with an arbitrary combination of the two source registers lanes
template <UST t_src_0, UST t_lane_0, UST t_src_1, UST t_lane_1, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto shuffle_lanes(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType;


//! @brief
//! Swap the lanes of an AVX register and return the result.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! Source register
//!
//! @return
//! Register with swapped lanes
template <FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto swap_lanes(T_RegisterType src) noexcept -> T_RegisterType;


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/utility/bit_operations.h"
#include "mjolnir/core/x86/intrinsics.h"

#include <iostream>

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <UST t_shift, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto align_right([[maybe_unused]] T_RegisterType lhs, [[maybe_unused]] T_RegisterType rhs)
        -> T_RegisterType
{
    static_assert(t_shift <= num_lane_elements<T_RegisterType>, "t_shift must be in the range [0, num_lane_elements].");


    if constexpr (t_shift == 0)
        return rhs;
    else if constexpr (t_shift == num_lane_elements<T_RegisterType>)
        return lhs;
    else
    {
        constexpr UST element_shift = t_shift * sizeof(ElementType<T_RegisterType>);

        if constexpr (num_lanes<T_RegisterType> == 1)
            return mm_cast_if<T_RegisterType>(_mm_alignr_epi8(mm_cast_fi(lhs), mm_cast_fi(rhs), element_shift));
        else
            return mm_cast_if<T_RegisterType>(_mm256_alignr_epi8(mm_cast_fi(lhs), mm_cast_fi(rhs), element_shift));
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST... t_args, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    static_assert(sizeof...(t_args) == num_elements<T_RegisterType>,
                  "Number of template parameters must be equal to the number of register elements.");

    return mm_blend<bit_construct<UST, t_args...>(true)>(src_0, src_1);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend_above(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    static_assert(t_index < num_elements<T_RegisterType>, "`t_index` exceeds register size.");

    if constexpr (t_index == num_elements<T_RegisterType> - 1)
        return src_0;
    else
    {
        constexpr auto get_mask = [](UST index) -> UST
        {
            UST mask = 0;
            for (UST i = index + 1; i < num_elements<T_RegisterType>; ++i)
                set_bit(mask, i);
            return mask;
        };

        return mm_blend<get_mask(t_index)>(src_0, src_1);
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend_at(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    static_assert(t_index < num_elements<T_RegisterType>, "`t_index` exceeds register size.");

    return mm_blend<(UST(1) << t_index)>(src_0, src_1);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend_below(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    static_assert(t_index < num_elements<T_RegisterType>, "`t_index` exceeds register size.");

    if constexpr (t_index == 0)
        return src_0;
    else
    {
        constexpr auto get_mask = [](UST index) -> UST
        {
            UST mask = 0;
            for (UST i = 0; i < index; ++i)
                set_bit(mask, i);
            return mask;
        };

        return mm_blend<get_mask(t_index)>(src_0, src_1);
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index_first, UST t_index_last, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend_from_to(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    static_assert(t_index_first <= t_index_last, "`t_index_first` is larger than `t_index_last`.");
    static_assert(t_index_last < num_elements<T_RegisterType>, "`t_index_last` exceeds register size.");

    if constexpr (t_index_first == 0 and t_index_last == num_elements<T_RegisterType> - 1)
        return src_1;
    else
    {
        constexpr auto get_mask = [](UST idx_first, UST idx_last) -> UST
        {
            UST mask = 0;
            for (UST i = idx_first; i <= idx_last; ++i)
                set_bit(mask, i);
            return mask;
        };

        return mm_blend<get_mask(t_index_first, t_index_last)>(src_0, src_1);
    }
}

// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto broadcast(T_RegisterType src) noexcept -> T_RegisterType
{
    static_assert(t_index < num_lane_elements<T_RegisterType>, "t_index exceeds lane size.");

    if constexpr (t_index == 0 && is_sse_register<T_RegisterType>)
        return mm_broadcast(src);
    else if constexpr (is_double_precision<T_RegisterType>)
        return permute<t_index, t_index>(src);
    else
        return permute<t_index, t_index, t_index, t_index>(src);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index_0, UST t_index_1, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto broadcast(T_RegisterType src) noexcept -> T_RegisterType
{
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    static_assert(t_index_0 < n_le && t_index_1 < n_le, "Indices may not exceed lane size.");

    if constexpr (is_m256d<T_RegisterType>)
        return permute<t_index_0, t_index_0, t_index_1, t_index_1>(src);
    else
        return permute<t_index_0, t_index_0, t_index_0, t_index_0, t_index_1, t_index_1, t_index_1, t_index_1>(src);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto broadcast_across_lanes(T_RegisterType src) noexcept -> T_RegisterType
{
    if constexpr (t_index == 0)
        return mm_broadcast(src);
    else if constexpr (is_sse_register<T_RegisterType>)
        return broadcast<t_index>(src);
    else
    {
        constexpr UST idx_value = t_index % num_lane_elements<T_RegisterType>;
        constexpr UST idx_lane  = t_index / num_lane_elements<T_RegisterType>;

        return permute_lanes<idx_lane, idx_lane>(broadcast<idx_value>(src));
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index_0, UST t_index_1, FloatVectorRegister T_RegisterType>
inline void exchange(T_RegisterType& reg_0, T_RegisterType& reg_1) noexcept
{
    constexpr UST n_e        = num_elements<T_RegisterType>;
    constexpr UST n_le       = num_lane_elements<T_RegisterType>;
    constexpr UST lane_idx_0 = t_index_0 / n_le;
    constexpr UST lane_idx_1 = t_index_1 / n_le;

    static_assert(t_index_0 < n_e && t_index_1 < n_e, "Indices exceed the register size.");


    if constexpr (lane_idx_0 == lane_idx_1)
    {
        T_RegisterType tmp_0 = reg_0;
        T_RegisterType tmp_1 = reg_1;

        if constexpr (t_index_0 != t_index_1)
        {
            tmp_0 = broadcast<t_index_0 % n_le>(tmp_0);
            tmp_1 = broadcast<t_index_1 % n_le>(tmp_1);
        }

        reg_0 = blend_at<t_index_0>(reg_0, tmp_1); // NOLINT(readability-misleading-indentation) - clang-tidy bug
        reg_1 = blend_at<t_index_1>(reg_1, tmp_0);
    }
    else
    {
        constexpr UST select_reg_0  = (lane_idx_0 == 0) ? 1 : 0;
        constexpr UST select_reg_1  = (lane_idx_1 == 0) ? 1 : 0;
        constexpr U32 element_idx_0 = (lane_idx_0 == 0) ? t_index_1 % n_le : t_index_0 % n_le;
        constexpr U32 element_idx_1 = (lane_idx_0 == 0) ? t_index_0 % n_le : t_index_1 % n_le;


        T_RegisterType tmp_0 = shuffle_lanes<select_reg_0, 1, select_reg_1, 0>(reg_0, reg_1);

        if constexpr (element_idx_0 != element_idx_1)
        {
            tmp_0 = broadcast<element_idx_0, element_idx_1>(tmp_0);
        }

        reg_0 = blend_at<t_index_0>(reg_0, tmp_0);
        reg_1 = blend_at<t_index_1>(reg_1, tmp_0);
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST... t_indices, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto permute(T_RegisterType src) noexcept -> T_RegisterType
{
    constexpr UST n_e  = num_elements<T_RegisterType>;
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    static_assert(sizeof...(t_indices) == n_le || (is_avx_register<T_RegisterType> && sizeof...(t_indices) == n_e),
                  "Number of indices must be identical to the number of elements or the number of lane elements.");

    if constexpr (is_m256d<T_RegisterType> && sizeof...(t_indices) == n_le)
        return permute<t_indices..., t_indices...>(src);
    else if constexpr (is_m256<T_RegisterType> && sizeof...(t_indices) == n_e)
        return _mm256_permutevar_ps(src, _mm256_setr_epi32(t_indices...));
    else
    {
        constexpr UST num_index_bits = num_lane_elements<T_RegisterType> / 2;
        return mm_permute<bit_construct_from_ints<num_index_bits, U8, t_indices...>(true)>(src);
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_lane_0, UST t_lane_1, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto permute_lanes(T_RegisterType src) noexcept -> T_RegisterType
{
    return shuffle_lanes<0, t_lane_0, 0, t_lane_1>(src, src);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_src_0, UST t_lane_0, UST t_src_1, UST t_lane_1, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto shuffle_lanes(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    constexpr UST sel_0 = bit_construct<UST, t_src_0, t_lane_0>();
    constexpr UST sel_1 = bit_construct<UST, t_src_1, t_lane_1>();
    constexpr UST mask  = (sel_1 << 4U) | sel_0;

    return mm_permute2f128<mask>(src_0, src_1);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto swap_lanes(T_RegisterType src) noexcept -> T_RegisterType
{
    return permute_lanes<1, 0>(src);
}


// --------------------------------------------------------------------------------------------------------------------


} // namespace mjolnir::x86
