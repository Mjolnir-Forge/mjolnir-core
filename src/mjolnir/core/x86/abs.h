//! @file
//! abs.h
//!
//! @brief
//! Contains x86 functions to calculate the absolute values.


#pragma once
#include "mjolnir/core/x86/concepts.h"
#include "mjolnir/core/x86/intrinsics.h"

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Return a new register with the absolute values of the input register.
//!
//! @tparam T_RegisterType:
//! Register type
//!
//! @param[in] value:
//! The initial values
//!
//! @return
//! Register containing the absolute values
//!
//! @remark
//! source: https://stackoverflow.com/q/23847377/6700329
template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto abs(T_RegisterType value) noexcept -> T_RegisterType;

//! @}
} // namespace mjolnir::x86


// ====================================================================================================================

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto abs(T_RegisterType value) noexcept -> T_RegisterType
{
    const T_RegisterType mask = mm_set1<T_RegisterType>(static_cast<ElementType<T_RegisterType>>(-0.0));
    return mm_andnot(mask, value);
}


} // namespace mjolnir::x86
