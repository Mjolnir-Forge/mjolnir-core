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


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/utility/bit_operations.h"


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


} // namespace mjolnir::x86
