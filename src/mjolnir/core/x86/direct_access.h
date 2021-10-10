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
//! @tparam T_RegisterType:
//! Register type
//!
//! @param[in] reg:
//! The source register
//! @param[in] index:
//! Index of the element
//!
//! @return
//! The value of the indexed element
template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto get(T_RegisterType reg, UST index) noexcept -> ElementType<T_RegisterType>;


//! @brief
//! Get the value of a specific element from a vector register.
//!
//! @tparam t_index:
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


//! @brief
//! Set the value of a specific vector register element.
//!
//! @tparam T_RegisterType:
//! Register type
//!
//! @param[in, out] reg:
//! The target register
//! @param[in] index:
//! The element index
//! @param[in] value:
//! The new value
template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
inline void set(T_RegisterType& reg, UST index, ElementType<T_RegisterType> value) noexcept;


//! @brief
//! Set the value of a specific vector register element.
//!
//! @tparam[in] t_index:
//! The element index
//! @tparam T_RegisterType:
//! Register type
//!
//! @param[in, out] reg:
//! The target register
//! @param[in] value:
//! The new value
template <UST t_index, typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
inline void set(T_RegisterType& reg, ElementType<T_RegisterType> value) noexcept;


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/x86/intrinsics.h"

#include <array>
#include <cassert>

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto get(T_RegisterType reg, UST index) noexcept -> ElementType<T_RegisterType>
{
    assert(index < num_elements<T_RegisterType>); // NOLINT

    VectorDataArray<T_RegisterType> array = {{0}};

    mm_store(array.data(), reg);
    return array[index];
}


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


// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
inline void set(T_RegisterType& reg, UST index, ElementType<T_RegisterType> value) noexcept
{
    assert(index < num_elements<T_RegisterType>); // NOLINT

    VectorDataArray<T_RegisterType> array = {{0}};

    mm_store(array.data(), reg);
    array[index] = value;
    reg          = mm_load<T_RegisterType>(array.data());
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
inline void set(T_RegisterType& reg, ElementType<T_RegisterType> value) noexcept
{
    static_assert(t_index < num_elements<T_RegisterType>, "Index out of bounds.");

    VectorDataArray<T_RegisterType> array = {{0}};

    mm_store(array.data(), reg);
    array[t_index] = value;
    reg            = mm_load<T_RegisterType>(array.data());
}

} // namespace mjolnir::x86
