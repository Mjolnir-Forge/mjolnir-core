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

//! \cond DO_NOT_DOCUMENT
namespace internal
{
//! Helper function declaration to determine the type of a parameter pack. There is no definition since it is never
//! called.
template <typename T_Type>
auto pack_type(T_Type...) -> T_Type;


} // namespace internal
//! \endcond


//! @brief
//! The type of a parameter pack that consist of values and not of types.
//!
//! @tparam t_pack
//! The parameter pack
template <auto... t_pack>
using PackType = decltype(internal::pack_type(t_pack...));


//! @brief
//! Return `true` if the passed function object returns `true` for all parameter pack values as input.
//!
//! @details
//! All values of the parameter pack need to be of the same type.
//!
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
template <auto... t_pack, std::invocable<PackType<t_pack...>> T_Func>
[[nodiscard]] consteval auto pack_all(T_Func func) noexcept -> bool;


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
[[nodiscard]] consteval auto pack_all_less(UST value) noexcept -> bool;


//! @brief
//! Return `true` if all values of an boolean parameter pack are `false` and `false` otherwise.
//!
//! @tparam t_pack:
//! The parameter pack that should be checked
//!
//! @return
//! `true` or `false`
template <bool... t_pack>
[[nodiscard]] consteval auto pack_all_false() noexcept -> bool;


//! @brief
//! Return `true` if all values of an boolean parameter pack are `true` and `false` otherwise.
//!
//! @tparam t_pack:
//! The parameter pack that should be checked
//!
//! @return
//! `true` or `false`
template <bool... t_pack>
[[nodiscard]] consteval auto pack_all_true() noexcept -> bool;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


#include <algorithm>
#include <array>

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <auto... t_pack, std::invocable<PackType<t_pack...>> T_Func>
[[nodiscard]] consteval auto pack_all(T_Func func) noexcept -> bool
{
    using Type         = PackType<t_pack...>;
    constexpr UST size = sizeof...(t_pack);

    std::array<Type, size> a = {{t_pack...}};

    return std::ranges::all_of(a, func);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST... t_pack>
[[nodiscard]] consteval auto pack_all_less(UST value) noexcept -> bool
{
    auto f = [value](UST e)
    {
        return e < value;
    };

    return pack_all<t_pack...>(f);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_pack>
[[nodiscard]] consteval auto pack_all_false() noexcept -> bool
{
    auto f = [](bool e)
    {
        return ! e;
    };

    return pack_all<t_pack...>(f);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_pack>
[[nodiscard]] consteval auto pack_all_true() noexcept -> bool
{
    auto f = [](bool e)
    {
        return e;
    };

    return pack_all<t_pack...>(f);
}


} // namespace mjolnir
