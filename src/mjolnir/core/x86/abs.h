//! @file
//! abs.h
//!
//! @brief
//! Contains x86 functions to calculate the absolute values.


#pragma once
#include "mjolnir/core/x86/intrinsics.h"

namespace mjolnir
{
//! \addtogroup core_x86
//! @{

//! @brief Return a register with the absolute values of the input register.
//! @tparam T_RegisterType: The register type
//! @param[in] val: The initial values
//! @return Register containing the absolute values
//! @remark source:
//! https://stackoverflow.com/q/23847377/6700329
template <typename T_RegisterType>
[[nodiscard]] inline auto abs(T_RegisterType val) noexcept -> T_RegisterType;

//! @}
} // namespace mjolnir


// ====================================================================================================================

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
[[nodiscard]] inline auto abs(T_RegisterType val) noexcept -> T_RegisterType
{
    return val;
}


} // namespace mjolnir
