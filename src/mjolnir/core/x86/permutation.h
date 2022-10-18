//! @file
//! permutation.h
//!
//! @brief
//! Contains functions to permute and blend the elements of vector registers.


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/fundamental_types.h"
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
[[nodiscard]] inline auto align_right([[maybe_unused]] T_RegisterType lhs, [[maybe_unused]] T_RegisterType rhs) noexcept
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
//! Up to the specified value, all elements of the result register are identical to `src_0`. All other values are taken
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
//! Index that specifies the index of the only element that is taken from `src_1`
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
//! All elements of the result register with a lower index than this value are copied from `src_1`. All other values are
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
//! Get a new register where the elements with lane index `t_index` are taken from `src_1` and the rest from `src_0`
//!
//! @tparam t_index:
//! Index that specifies the index of the lane element that is taken from `src_1`
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
[[nodiscard]] inline auto blend_per_lane_at(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType;


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
//! Insert a single element from `src` into `dst` and return the result in a new `__m128` register.
//!
//! @details
//! The source and the target elements are selected by template parameter indices. Optionally, elements can be set to
//! zero by providing additional boolean template values.
//!
//! @tparam t_index_src:
//! Index of the element in `src` that should be copied
//! @tparam t_index_dst:
//! Index of the element in `dst` that should receive the copied value
//! @tparam t_set_zero:
//! An optional parameter pack of boolean values. If the N-th provided value is `true`, the N-th element of the result
//! register will be set to `0`. If it is `false`, no changes are applied to this element. Note that you can set
//! multiple values to `0`. If the boolean value that corresponds to `t_index_dst` is `true`, the resulting value is
//! `0`, which makes the insertion obsolete.
//!
//! @param[in] src:
//! Source register
//! @param[in] dst:
//! Target register
//!
//! @return
//! A new `__m128` register with corresponding values
template <UST t_index_src, UST t_index_dst, bool... t_set_zero>
inline auto insert(__m128 src, __m128 dst) noexcept -> __m128;


//! @brief
//! Shuffle the elements of a vector register within lanes using indices and return the result in a new register.
//!
//! @tparam t_indices
//! A set of indices equal to the number of register elements or the number of lane elements. In the latter case the
//! permutation pattern is identical for all lanes. The N-th index selects the value for the N-th element/lane element.
//! Index values may not exceed the lane size of a register. Otherwise a compile-time error is triggered.
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
//! assert that the 8 indices for the __m256 case do not exceed number of lane elements. Maybe write a generalized
//! function for integer based parameter packs
template <UST... t_indices, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto permute(T_RegisterType src) noexcept -> T_RegisterType;


//! @brief
//! Shuffle the elements of a vector register across lanes using indices and return the result in a new register.
//!
//! @tparam t_indices
//! A set of indices equal to the number of register elements. The N-th index selects the value for the N-th
//! element/lane element. Index values may not exceed the number of register elements. Otherwise a compile-time error is
//! triggered.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! New register with shuffled values
template <UST... t_indices, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto permute_across_lanes(T_RegisterType src) noexcept -> T_RegisterType;


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
//! Return a register with the first half of the lane elements selected from `src_0` and the second half from `src_1`.
//!
//! @details
//! Each integer of the parameter pack specifies the index of the value that should be taken from the corresponding
//! source register. The first half of a lane is taken from the source `src_0` and the second from `src_1`. The number
//! of integers must be equal to the number of lane values. Therefore, the pattern is identical for each lane. The sole
//! exception is the `__m256d` register. Here the pattern might differ if you provide 4 instead of 2 indices.
//!
//! @tparam t_indices:
//! Parameter pack of indices that specify which elements are chosen from the source registers.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src_0:
//! First source register
//! @param[in] src_1:
//! Second source register
//!
//! @return
//! New register with shuffled values
template <UST... t_indices, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto shuffle(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType;


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
//! Swap two elements of a register and return the result.
//!
//! @details
//! Swapping across lane boundaries is supported but more expensive than swapping inside lanes.
//!
//! @tparam t_idx_0:
//! Index of the first element that should be swapped.
//! @tparam t_idx_1:
//! Index of the second element that should be swapped.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! New register with swapped values
template <UST t_idx_0, UST t_idx_1, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto swap(T_RegisterType src) noexcept -> T_RegisterType;


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


//! @brief
//! Return a new register with or without swapped lanes depending on the value of the boolean template parameter.
//!
//! @tparam t_swap_lanes:
//! Decides if the lanes should be swapped or not
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] src:
//! Source register
//!
//! @return
//! Register with or without swapped lanes
template <bool t_swap_lanes, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto swap_lanes_if(T_RegisterType src) noexcept -> T_RegisterType;


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/utility/bit_operations.h"
#include "mjolnir/core/utility/parameter_pack.h"
#include "mjolnir/core/x86/intrinsics.h"

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <UST t_shift, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto align_right([[maybe_unused]] T_RegisterType lhs, [[maybe_unused]] T_RegisterType rhs) noexcept
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
    static_assert(pack_all_less<t_args...>(2), "All template values must be in the range [0, 1]");

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
        constexpr auto get_mask = [](UST index)
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
        constexpr auto get_mask = [](UST index)
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

    if constexpr (t_index_first == 0 && t_index_last == num_elements<T_RegisterType> - 1)
        return src_1;
    else
    {
        constexpr auto get_mask = [](UST idx_first, UST idx_last)
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
[[nodiscard]] inline auto blend_per_lane_at(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    constexpr UST n_l  = num_lanes<T_RegisterType>;
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    static_assert(t_index < n_le, "`t_index` exceeds number of lane elements.");

    if constexpr (n_l == 1)
        return blend_at<t_index>(src_0, src_1);
    else
    {
        constexpr auto get_mask = [n_l, n_le](UST index)
        {
            UST mask = 0;
            for (UST i = 0; i < n_l; ++i)
                set_bit(mask, index + i * n_le);
            return mask;
        };
        return mm_blend<get_mask(t_index)>(src_0, src_1);
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

//! @todo: check if using permute_across lanes is faster in the last branch
template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto broadcast_across_lanes(T_RegisterType src) noexcept -> T_RegisterType
{
    static_assert(t_index < num_elements<T_RegisterType>, "Index exceeds register size.");

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


// --- internal functions of exchange ---------------------------------------------------------------------------------

//! \cond DO_NOT_DOCUMENT
namespace internal
{
//! Branch of the exchange function that covers cases where the exchanged elements are in the same register lane.
template <UST t_idx_0, UST t_idx_1, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto exchange_same_lane(T_RegisterType& reg_0, T_RegisterType& reg_1) noexcept
{
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    T_RegisterType tmp_0 = reg_0;
    T_RegisterType tmp_1 = reg_1;

    if constexpr (t_idx_0 != t_idx_1)
    {
        tmp_0 = broadcast<t_idx_0 % n_le>(tmp_0);
        tmp_1 = broadcast<t_idx_1 % n_le>(tmp_1);
    }

    reg_0 = blend_at<t_idx_0>(reg_0, tmp_1); // NOLINT(readability-misleading-indentation) - clang-tidy bug
    reg_1 = blend_at<t_idx_1>(reg_1, tmp_0);
}


// --------------------------------------------------------


//! Branch of the exchange function that covers cases where the exchanged elements are in different register lanes.
template <UST t_idx_0, UST t_idx_1, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto exchange_different_lane(T_RegisterType& reg_0, T_RegisterType& reg_1) noexcept
{
    constexpr UST n_le       = num_lane_elements<T_RegisterType>;
    constexpr UST lane_idx_0 = t_idx_0 / n_le;
    constexpr UST lane_idx_1 = t_idx_1 / n_le;

    constexpr UST select_reg_0  = (lane_idx_0 == 0) ? 1 : 0;
    constexpr UST select_reg_1  = (lane_idx_1 == 0) ? 1 : 0;
    constexpr U32 element_idx_0 = (lane_idx_0 == 0) ? t_idx_1 % n_le : t_idx_0 % n_le;
    constexpr U32 element_idx_1 = (lane_idx_0 == 0) ? t_idx_0 % n_le : t_idx_1 % n_le;


    T_RegisterType tmp_0 = shuffle_lanes<select_reg_0, 1, select_reg_1, 0>(reg_0, reg_1);

    if constexpr (element_idx_0 != element_idx_1)
        tmp_0 = broadcast<element_idx_0, element_idx_1>(tmp_0);

    reg_0 = blend_at<t_idx_0>(reg_0, tmp_0);
    reg_1 = blend_at<t_idx_1>(reg_1, tmp_0);
}

} // namespace internal
//! \endcond


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
        internal::exchange_same_lane<t_index_0, t_index_1>(reg_0, reg_1);
    else
        internal::exchange_different_lane<t_index_0, t_index_1>(reg_0, reg_1);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index_src, UST t_index_dst, bool... t_set_zero>
inline auto insert(__m128 src, __m128 dst) noexcept -> __m128
{
    constexpr UST n_e = num_elements<__m128>;
    static_assert(t_index_src < n_e && t_index_dst < n_e, "Indices exceed the register size.");

    constexpr UST set_zero_mask  = bit_construct<UST, t_set_zero...>(true);
    constexpr UST selection_mask = bit_construct_from_ints<2, UST, t_index_src, t_index_dst>();
    constexpr UST mask           = bit_construct_from_ints<4, UST, selection_mask, set_zero_mask>();

    return _mm_insert_ps(dst, src, mask);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST... t_indices, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto permute(T_RegisterType src) noexcept -> T_RegisterType
{
    constexpr UST n_e  = num_elements<T_RegisterType>;
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    static_assert(sizeof...(t_indices) == n_le || (is_avx_register<T_RegisterType> && sizeof...(t_indices) == n_e),
                  "Number of indices must be identical to the number of elements or the number of lane elements.");
    static_assert(pack_all_less<t_indices...>(n_le),
                  "All index values must be in the range [0, number of lane elements]");

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

template <UST... t_indices, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto permute_across_lanes(T_RegisterType src) noexcept -> T_RegisterType
{
    constexpr UST n_e = num_elements<T_RegisterType>;

    static_assert(sizeof...(t_indices) == n_e, "Number of indices must be equal to the number of register elements.");
    static_assert(pack_all_less<t_indices...>(n_e),
                  "All template values must be in the range [0, number of register elements]");

    if constexpr (num_lanes<T_RegisterType> == 1)
        return permute<t_indices...>(src);
    else if constexpr (is_m256d<T_RegisterType>)
    {
        constexpr UST mask = bit_construct_from_ints<2, UST, t_indices...>(true);
        return _mm256_permute4x64_pd(src, mask);
    }
    else
    {
        const __m256i mask = _mm256_setr_epi32(t_indices...);
        return _mm256_permutevar8x32_ps(src, mask);
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_lane_0, UST t_lane_1, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto permute_lanes(T_RegisterType src) noexcept -> T_RegisterType
{
    return shuffle_lanes<0, t_lane_0, 0, t_lane_1>(src, src);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST... t_indices, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto shuffle(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    constexpr UST n_e  = num_elements<T_RegisterType>;
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    static_assert(sizeof...(t_indices) == n_le || (is_m256d<T_RegisterType> && sizeof...(t_indices) == n_e),
                  "Number of indices must be identical to the number of lane elements (or elements for __m256d).");
    static_assert(pack_all_less<t_indices...>(n_le),
                  "All index values must be in the range [0, number of lane elements]");

    constexpr auto get_mask = []() -> UST
    {
        if constexpr (is_single_precision<T_RegisterType>)
            return bit_construct_from_ints<2, UST, t_indices...>(true);
        else if constexpr (sizeof...(t_indices) == num_elements<T_RegisterType>)
            return bit_construct<UST, t_indices...>(true);
        else
            return bit_construct<UST, t_indices..., t_indices...>(true);
    };

    return mm_shuffle<get_mask()>(src_0, src_1);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_src_0, UST t_lane_0, UST t_src_1, UST t_lane_1, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto shuffle_lanes(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    static_assert(pack_all_less<t_src_0, t_lane_0, t_src_1, t_lane_1>(2),
                  "All template values must be in the range [0, 1]");

    constexpr UST sel_0 = bit_construct<UST, t_src_0, t_lane_0>();
    constexpr UST sel_1 = bit_construct<UST, t_src_1, t_lane_1>();
    constexpr UST mask  = (sel_1 << 4U) | sel_0;

    return mm_permute2f128<mask>(src_0, src_1);
}


// --- internal functions of swap -------------------------------------------------------------------------------------

//! \cond DO_NOT_DOCUMENT
namespace internal
{
//! Branch of the swap function that handles cases where elements are swapped in the same lane.
template <UST t_idx_0, UST t_idx_1, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto swap_same_lane(T_RegisterType src) noexcept -> T_RegisterType
{
    constexpr UST n_e = num_elements<T_RegisterType>;

    constexpr auto get_permute_index_array = []() constexpr
    {
        constexpr UST        n_le = num_lane_elements<T_RegisterType>;
        std::array<UST, n_e> a    = {{0}};

        for (UST i = 0; i < n_e; ++i)
        {
            if (t_idx_0 == i)
                a[i] = t_idx_1 % n_le; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
            else if (t_idx_1 == i)
                a[i] = t_idx_0 % n_le; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
            else
                a[i] = i % n_le; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        }
        return a;
    };
    constexpr auto p = get_permute_index_array();


    if constexpr (n_e == 2)
        return permute<p[0], p[1]>(src);
    else if constexpr (n_e == 4)
        return permute<p[0], p[1], p[2], p[3]>(src);
    else
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        return permute<p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]>(src);
}


// --------------------------------------------------------

//! Branch of the swap function that handles cases where elements are swapped across lanes.
template <UST t_idx_0, UST t_idx_1, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto swap_different_lane(T_RegisterType src) noexcept -> T_RegisterType
{
    constexpr UST n_e  = num_elements<T_RegisterType>;
    constexpr UST n_le = num_lane_elements<T_RegisterType>;

    constexpr U32 idx_lane_0 = (t_idx_0 < t_idx_1) ? t_idx_0 : t_idx_1;
    constexpr U32 idx_lane_1 = ((t_idx_0 > t_idx_1) ? t_idx_0 : t_idx_1) % n_le;

    auto get_blend_index_array = []() constexpr->std::array<UST, n_e>
    {
        std::array<UST, n_e> a = {{0}};
        for (UST i = 0; i < n_le; ++i)
        {
            a[i]        = (idx_lane_0 == i) ? 1 : 0;
            a[i + n_le] = (idx_lane_1 == i) ? 1 : 0;
        }
        return a;
    };
    constexpr auto b = get_blend_index_array();


    T_RegisterType bc  = broadcast<idx_lane_0, idx_lane_1>(src);
    T_RegisterType tmp = swap_lanes(bc);
    if constexpr (n_e == 4)
        return blend<b[0], b[1], b[2], b[3]>(src, tmp);
    else
        // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
        return blend<b[0], b[1], b[2], b[3], b[4], b[5], b[6], b[7]>(src, tmp);
}

} // namespace internal
//! \endcond


// --------------------------------------------------------------------------------------------------------------------

template <UST t_idx_0, UST t_idx_1, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto swap(T_RegisterType src) noexcept -> T_RegisterType
{
    constexpr UST n_e = num_elements<T_RegisterType>;
    static_assert(t_idx_0 < n_e && t_idx_1 < n_e, "Indices must be smaller than the number of register elements.");

    if constexpr (t_idx_0 == t_idx_1)
        return src;
    else
    {
        constexpr UST n_le   = num_lane_elements<T_RegisterType>;
        constexpr UST lane_0 = t_idx_0 / n_le;
        constexpr UST lane_1 = t_idx_1 / n_le;

        if constexpr (lane_0 == lane_1)
            return internal::swap_same_lane<t_idx_0, t_idx_1>(src);
        else
            return internal::swap_different_lane<t_idx_0, t_idx_1>(src);
    }
}

// --------------------------------------------------------------------------------------------------------------------

template <FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto swap_lanes(T_RegisterType src) noexcept -> T_RegisterType
{
    return permute_lanes<1, 0>(src);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_swap_lanes, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto swap_lanes_if(T_RegisterType src) noexcept -> T_RegisterType
{
    if constexpr (t_swap_lanes)
        return swap_lanes(src);
    else
        return src;
}

// --------------------------------------------------------------------------------------------------------------------

} // namespace mjolnir::x86
