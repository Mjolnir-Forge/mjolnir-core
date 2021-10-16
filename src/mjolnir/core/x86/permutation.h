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
//! Blend elements from `src_0` and `src_1` into a new register using the template parameters `t_e_0` to `t_e_3`.
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
template <U32... t_args, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType;


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/utility/bit_operations.h"
#include "mjolnir/core/x86/intrinsics.h"

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <U32... t_args, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto blend(T_RegisterType src_0, T_RegisterType src_1) noexcept -> T_RegisterType
{
    static_assert(sizeof...(t_args) == num_elements<T_RegisterType>,
                  "Number of template parameters must be equal to the number of register elements.");

    return mm_blend<bit_construct<U32, t_args...>(true)>(src_0, src_1);
}


// --------------------------------------------------------------------------------------------------------------------


} // namespace mjolnir::x86
