//! @file
//! type.h
//!
//! @brief
//! Contains multiple utility functions for type related operations


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/concepts.h"
#include "mjolnir/core/fundamental_types.h"
#include <type_traits>

#include <concepts>

namespace mjolnir
{
//! \addtogroup core_utility
//! @{


//! @brief
//! Return `true` if the tested type is identical to any of a list of types.
//!
//! @details
//! The first template parameter is the type that should be tested. All additional types passed as template
//! parameters are compared to the first one. If any of them is identical, the function returns `true`. If no match is
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
[[nodiscard]] consteval auto is_any_of() noexcept -> bool;


//! @brief
//! Cast an integer to an equally sized unsigned type.
//!
//! @details
//! If the original type is already unsigned, the input is returned unmodified.
//!
//! @tparam T_Type
//! Type of the input value
//!
//! @param[in] value:
//! Integer value that should be cast
//!
//! @return
//! Unsigned value
template <std::integral T_Type>
[[nodiscard]] constexpr auto signed_to_unsigned(T_Type value) -> EquallySizedUnsignedType<T_Type>;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------


template <typename T_Type, typename T_OtherType, typename... T_TypeList>
[[nodiscard]] consteval auto is_any_of() noexcept -> bool
{
    if constexpr (! std::is_same_v<T_Type, T_OtherType>)
    {
        if constexpr (sizeof...(T_TypeList) != 0)
            return is_any_of<T_Type, T_TypeList...>();
        else
            return false;
    }
    else
        return true;
}


// --------------------------------------------------------------------------------------------------------------------

template <std::integral T_Type>
[[nodiscard]] constexpr auto signed_to_unsigned(T_Type value) -> EquallySizedUnsignedType<T_Type>
{
    if constexpr (std::is_signed_v<T_Type>)
        return static_cast<EquallySizedUnsignedType<T_Type>>(value);
    else
        return value;
}

} // namespace mjolnir
