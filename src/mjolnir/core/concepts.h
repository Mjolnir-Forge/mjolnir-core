//! @file
//! concepts.h
//!
//! @brief
//! Contains some basic concepts that are frequently needed


#pragma once

#include <type_traits>

namespace mjolnir
{
//! \addtogroup core
//! @{

//! @brief
//! Concept for a number type
//!
//! @tparam T_Type
//! Type
template <typename T_Type>
concept Number = std::is_integral_v<T_Type> || std::is_floating_point_v<T_Type>;


//! @}
} // namespace mjolnir
