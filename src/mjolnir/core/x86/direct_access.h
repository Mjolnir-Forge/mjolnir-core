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
//! @param[in] src:
//! The source register
//! @param[in] index:
//! Index of the element
//!
//! @return
//! The value of the indexed element
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto get(T_RegisterType src, UST index) noexcept -> ElementType<T_RegisterType>;


//! @brief
//! Get the value of a specific element from a vector register.
//!
//! @tparam t_index:
//! Index of the element
//! @tparam T_RegisterType:
//! Register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! The value of the indexed element
template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto get(T_RegisterType src) noexcept -> ElementType<T_RegisterType>;


//! @brief
//! Set the value of a specific vector register element.
//!
//! @tparam T_RegisterType:
//! Register type
//!
//! @param[in, out] dst:
//! The target register
//! @param[in] index:
//! The element index
//! @param[in] value:
//! The new value
template <FloatVectorRegister T_RegisterType>
inline void set(T_RegisterType& dst, UST index, ElementType<T_RegisterType> value) noexcept;


//! @brief
//! Set the value of a specific vector register element.
//!
//! @tparam t_index:
//! The element index
//! @tparam T_RegisterType:
//! Register type
//!
//! @param[in, out] dst:
//! The target register
//! @param[in] value:
//! The new value
template <UST t_index, FloatVectorRegister T_RegisterType>
inline void set(T_RegisterType& dst, ElementType<T_RegisterType> value) noexcept;


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"

#include <array>
#include <cassert>

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto get(T_RegisterType src, UST index) noexcept -> ElementType<T_RegisterType>
{
    assert(index < num_elements<T_RegisterType>); // NOLINT

    VectorDataArray<T_RegisterType> array = {{0}};

    mm_store(array.data(), src);
    return array[index];
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto get(T_RegisterType src) noexcept -> ElementType<T_RegisterType>
{
    static_assert(t_index < num_elements<T_RegisterType>, "Index out of bounds.");

    if constexpr (t_index == 0)
        return mm_cvt_float(src);
    else if constexpr (t_index < num_lane_elements<T_RegisterType>)
        return mm_cvt_float(broadcast<t_index>(src));
    else
        return mm_cvt_float(broadcast_across_lanes<t_index>(src));
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
inline void set(T_RegisterType& dst, UST index, ElementType<T_RegisterType> value) noexcept
{
    assert(index < num_elements<T_RegisterType>); // NOLINT

    VectorDataArray<T_RegisterType> array = {{0}};

    mm_store(array.data(), dst);
    array[index] = value;
    dst          = mm_load<T_RegisterType>(array.data());
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_index, FloatVectorRegister T_RegisterType>
inline void set(T_RegisterType& dst, ElementType<T_RegisterType> value) noexcept
{
    static_assert(t_index < num_elements<T_RegisterType>, "Index out of bounds.");

    auto tmp = mm_set1<T_RegisterType>(value);
    dst      = blend_at<t_index>(dst, tmp);
}

} // namespace mjolnir::x86
