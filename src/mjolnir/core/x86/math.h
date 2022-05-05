//! @file
//! abs.h
//!
//! @brief
//! Contains x86 functions to calculate the absolute values.


#pragma once
#include "mjolnir/core/x86/definitions.h"
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
//! @param[in] src:
//! The initial values
//!
//! @return
//! Register containing the absolute values
//!
//! @remark
//! source: https://stackoverflow.com/q/23847377/6700329
template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto abs(T_RegisterType src) noexcept -> T_RegisterType;

//! @}
} // namespace mjolnir::x86


// ====================================================================================================================

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto abs(T_RegisterType src) noexcept -> T_RegisterType
{
    using EType     = ElementType<T_RegisterType>;
    const auto mask = mm_set1<T_RegisterType>(static_cast<EType>(-0.0));

    return mm_andnot(mask, src);
}


} // namespace mjolnir::x86
