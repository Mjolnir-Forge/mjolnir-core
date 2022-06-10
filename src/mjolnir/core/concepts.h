//! @file
//! concepts.h
//!
//! @brief
//! Contains some basic concepts that are frequently needed


#pragma once

#include "mjolnir/core/fundamental_types.h"
#include <type_traits>

#include <concepts>

namespace mjolnir
{
//! \addtogroup core
//! @{

//! @brief
//! Concept for a number type
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
concept Number = std::is_integral_v<T_Type> || std::is_floating_point_v<T_Type>;


//! @brief
//! Unsigned integer type with the same size as the reference type.
//!
//! @tparam T_Type:
//! The reference type
// clang-format off
template <std::integral T_Type>
using EquallySizedUnsignedType = typename std::conditional_t<std::is_unsigned_v<T_Type>,  T_Type,
                                          std::conditional_t<std::is_same_v<T_Type, IPT>, UPT,
                                          std::conditional_t<sizeof(T_Type) == 1,         U8,
                                          std::conditional_t<sizeof(T_Type) == 2,         U16,
                                          std::conditional_t<sizeof(T_Type) == 4,         U32, U64>>>>>;
// clang-format on


//! @}
} // namespace mjolnir
