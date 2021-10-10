//! @file
//! direct_access.h
//!
//! @brief
//! Contains functions to access individual elements of an x86 vector register.


#pragma once

#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/definitions.h"

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Get the value of a specific element from a vector register.
//!
//! @tparam t_index
//! Index of the element
//! @tparam T_RegisterType:
//! Register type
//!
//! @param[in] reg:
//! The source register
//!
//! @return
//! The value of the indexed element
template <UST t_index, typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto get(T_RegisterType reg) noexcept -> ElementType<T_RegisterType>;

//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/x86/intrinsics.h"

#include <array>

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto get(T_RegisterType reg) noexcept -> ElementType<T_RegisterType>
{
    static_assert(t_index < num_elements<T_RegisterType>, "Index out of bounds.");

    VectorDataArray<T_RegisterType> array = {{0}};

    mm_store(array.data(), reg);
    return array[t_index];
}

} // namespace mjolnir::x86
