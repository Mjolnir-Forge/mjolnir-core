//! @file
//! comparison.h
//!
//! @brief
//! Contains functions to compare verctor registers.


#pragma once

#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/definitions.h"

#include <concepts>

// === DECLARATION ====================================================================================================

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Return `true` if the comparisons of all selected register elements are `true` and `false` otherwise.
//!
//! @tparam t_cmp:
//! A parameter pack of boolean values with the size of the number of register elements. If a value is `true`, the
//! corresponding element is compared. Otherwise, it is ignored
//! @tparam T_RegisterType:
//! The register type
//! @tparam T_CompFunc:
//! The Type of a callable object that performs the register element comparison.
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//! @param[in] comp_func:
//! A comparison function or functor that provides the comparison of the register elements. Functors should be preferred
//! to assure inlining. See  https://stackoverflow.com/a/12718449/6700329
//!
//! @return
//! `true` or `false`
template <bool... t_cmp, FloatVectorRegister T_RegisterType, std::invocable<T_RegisterType, T_RegisterType> T_CompFunc>
[[nodiscard]] inline auto compare_all_true(T_RegisterType lhs, T_RegisterType rhs, T_CompFunc comp_func) noexcept
        -> bool;


//! @brief
//! Return `true` if all selected register elements are equal and `false` otherwise.
//!
//! @tparam t_cmp:
//! A parameter pack of boolean values with the size of the number of register elements. If a value is `true`, the
//! corresponding element is compared. Otherwise, it is ignored.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! `true` or `false`
template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool;

//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================


#include "mjolnir/core/math/math.h"
#include "mjolnir/core/utility/bit_operations.h"
#include "mjolnir/core/utility/parameter_pack.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/x86.h"

#include <bitset>   // todo: remove ##############################################
#include <iostream> // todo: remove $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include <limits>

namespace mjolnir::x86
{
//! \cond DO_NOT_DOCUMENT
namespace internal
{
//! The following structs are just wrappers around comparison functions from `mjolnir/core/x86/intrinsics.h`. They
//! are used in conjunction with the different generalized comparison functions of this header and define the performed
//! comparison operation. Using a callable wrapper struct as template parameter instead of the function itself makes
//! optimizing the code easier for the compiler. See the following links for more info:
//! https://stackoverflow.com/a/12718449/6700329
//! https://stackoverflow.com/a/356993/6700329

template <FloatVectorRegister T_RegisterType>
struct CompareEqual
{
    [[nodiscard]] inline auto operator()(T_RegisterType lhs, T_RegisterType rhs) const noexcept -> T_RegisterType
    {
        return mm_cmp_eq<T_RegisterType>(lhs, rhs);
    }
};

// --------------------------------------------------------

} // namespace internal
//! \endcond


// --------------------------------------------------------------------------------------------------------------------

template <bool... t_cmp, FloatVectorRegister T_RegisterType, std::invocable<T_RegisterType, T_RegisterType> T_CompFunc>
[[nodiscard]] inline auto compare_all_true(T_RegisterType lhs, T_RegisterType rhs, T_CompFunc comp_func) noexcept
        -> bool
{
    constexpr UST n_e    = num_elements<T_RegisterType>;
    constexpr UST n_bits = sizeof(ElementType<T_RegisterType>);
    constexpr UST val    = power_of_2(n_bits) - 1;

    static_assert(sizeof...(t_cmp) == n_e, "Number of template parameters must be equal to the number of elements.");
    static_assert(not pp_all_false<t_cmp...>(), "At least one template parameter must be `true`.");


    auto           result = mm_movemask_epi8(mm_cast_fi(comp_func(lhs, rhs)));
    constexpr auto ref    = bit_construct_from_ints<n_bits, decltype(result), (t_cmp * val)...>(true);

    if constexpr (not pp_all_true<t_cmp...>())
        result &= ref; // Set bits of elemtens that shouldn't be compared to zero

    return result == ref;
}


// --------------------------------------------------------------------------------------------------------------------

// template <FloatVectorRegister T_RegisterType>
//[[nodiscard]] inline auto compare_all_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
//{
//    return true;
//}


// --------------------------------------------------------------------------------------------------------------------

// template <UST t_idx_first, UST t_idx_last, FloatVectorRegister T_RegisterType>
//[[nodiscard]] inline auto compare_all_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
//{
//    return true;
//}


// --------------------------------------------------------------------------------------------------------------------


template <bool... t_cmp, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto compare_all_equal(T_RegisterType lhs, T_RegisterType rhs) noexcept -> bool
{
    return compare_all_true<t_cmp...>(lhs, rhs, internal::CompareEqual<T_RegisterType>());
}


} // namespace mjolnir::x86
