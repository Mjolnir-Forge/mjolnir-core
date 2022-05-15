//! @file
//! parameter_pack.h
//!
//! @brief
//! Contains utility functions for parameter packs.


#pragma once

#include "mjolnir/core/fundamental_types.h"

#include <concepts>


// === DECLARATION ====================================================================================================

namespace mjolnir
{
//! \addtogroup core_utility
//! @{


//! @brief
//! Return `true` if the passed function object returns `true` for all parameter pack values as input.
//!
//! @details
//! All values of the parameter pack need to be of the same type.
//!
//! @tparam T_CommonType:
//! The common type of all parameter pack elements.
//! @tparam t_pack:
//! The parameter pack that should be checked
//! @tparam T_Func:
//! Type of the passed callable object.
//!
//! @param[in] func:
//! The function or functor that should be evaluated with all parameter pack elements
//!
//! @return
//! `true` or `false`
template <typename T_CommonType, T_CommonType... t_pack, std::invocable<T_CommonType> T_Func>
[[nodiscard]] consteval auto pp_all(T_Func func) noexcept -> bool;


//! @brief
//! Return `true` if all values of an unsigned integer parameter pack are less then a given value and `false` otherwise.
//!
//! @tparam t_pack:
//! The parameter pack that should be checked
//!
//! @param[in] value:
//! The reference value
//!
//! @return
//! `true` or `false`
template <UST... t_pack>
[[nodiscard]] consteval auto pp_all_less(UST value) noexcept -> bool;


//! @brief
//! Return `true` if all values of an boolean parameter pack are `false` and `false` otherwise.
//!
//! @tparam t_pack:
//! The parameter pack that should be checked
//!
//! @return
//! `true` or `false`
template <bool... t_pack>
[[nodiscard]] consteval auto pp_all_false() noexcept -> bool;


//! @brief
//! Return `true` if all values of an boolean parameter pack are `true` and `false` otherwise.
//!
//! @tparam t_pack:
//! The parameter pack that should be checked
//!
//! @return
//! `true` or `false`
template <bool... t_pack>
[[nodiscard]] consteval auto pp_all_true() noexcept -> bool;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


#include <algorithm>
#include <array>

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_CommonType, T_CommonType... t_pack, std::invocable<T_CommonType> T_Func>
[[nodiscard]] consteval auto pp_all(T_Func func) noexcept -> bool
{
    constexpr UST                  size = sizeof...(t_pack);
    std::array<T_CommonType, size> a    = {{t_pack...}};

    return std::all_of(a.begin(), a.end(), func);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST... t_pack>
[[nodiscard]] consteval auto pp_all_less(UST value) noexcept -> bool
{
    auto f = [value](UST e) -> bool
    {
        return e < value;
    };

    return pp_all<UST, t_pack...>(f);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_pack>
[[nodiscard]] consteval auto pp_all_false() noexcept -> bool
{
    auto f = [](bool e) -> bool
    {
        return not e;
    };

    return pp_all<bool, t_pack...>(f);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_pack>
[[nodiscard]] consteval auto pp_all_true() noexcept -> bool
{
    auto f = [](bool e) -> bool
    {
        return e;
    };

    return pp_all<bool, t_pack...>(f);
}


} // namespace mjolnir
