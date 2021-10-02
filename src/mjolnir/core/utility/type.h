//! @file
//! type.h
//!
//! @brief
//! Contains multiple utility functions for type related operations


#pragma once

namespace mjolnir
{
//! \addtogroup core_utility
//! @{


//! @brief
//! Return `true` if the tested type is identical to any of a list of types.
//!
//! @details
//! The first template parameter is the type that should be tested. All additional types passed as template
//! parameters are compared to the first one. If any of them is identical, the funciton returns `true`. If no match is
//! found, `false` is returned.
//!
//! @tparam T_Type:
//! The type that should be compared against a list of other types
//! @tparam T_OtherType:
//! The first item inside the list of types thar should be compared to `T_Type`
//! @tparam T_TypeList:
//! An arbitrary number of additional types that should be compared to `T_Type`
//!
//! @return
//! `true` if `T_Type` is identical to `T_OtherType` or any of the types in `T_TypeList` and `false` otherwise.
template <typename T_Type, typename T_OtherType, typename... T_TypeList>
inline consteval auto is_any_of() -> bool;


//! @}
} // namespace mjolnir


// ====================================================================================================================

#include <type_traits>

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------


template <typename T_Type, typename T_OtherType, typename... T_TypeList>
inline consteval auto is_any_of() -> bool
{
    if constexpr (not std::is_same<T_Type, T_OtherType>::value)
    {
        if constexpr (sizeof...(T_TypeList) != 0)
            return is_any_of<T_Type, T_TypeList...>();
        else
            return false;
    }
    else
        return true;
}

} // namespace mjolnir
