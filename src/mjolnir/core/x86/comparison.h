//! @file
//! comparison.h
//!
//! @brief
//! Contains functions to compare verctor registers.


#pragma once

#include "mjolnir/core/fundamental_types.h"


// === DECLARATION ====================================================================================================

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================


#include "mjolnir/core/math/math.h"
#include "mjolnir/core/utility/bit_operations.h"
#include "mjolnir/core/utility/parameter_pack.h"
#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/x86.h"

#include <bitset>   // todo: remove ##############################################
#include <iostream> // todo: remove $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$
#include <limits>

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

/*
template <FloatVectorRegister T_RegisterType, bool... t_cmp>
constexpr auto SSECalculateComparisonValueAllTrue()
{
    using ReturnType = decltype(mm_movemask_epi8(mm_cast_fi(mm_setzero<T_RegisterType>())));


    constexpr UST n_bits = sizeof(ElementType<T_RegisterType>);
    constexpr UST val    = power_of_2(n_bits) - 1;
    return bit_construct_from_ints<n_bits, ReturnType, (t_cmp * val)...>(true);
}
*/

// --------------------------------------------------------------------------------------------------------------------

template <typename T_CompFunction, FloatVectorRegister T_RegisterType, bool... t_cmp>
inline auto compare_all_true(T_RegisterType lhs, T_RegisterType rhs, T_CompFunction comp_func) noexcept -> bool
{
    // constexpr UST n_e    = num_elements<T_RegisterType>;
    constexpr UST n_bits = sizeof(ElementType<T_RegisterType>);
    constexpr UST val    = power_of_2(n_bits) - 1;
    // todo: assert size of pack = n_e
    // todo: assert not all false

    auto           result = mm_movemask_epi8(mm_cast_fi(comp_func(lhs, rhs)));
    constexpr auto ref    = bit_construct_from_ints<n_bits, decltype(result), (t_cmp * val)...>(true);


    // std::cout << std::bitset<16>(result) << "-" << std::bitset<16>(ref) << std::endl;
    if constexpr (not pp_all_true<t_cmp...>()) // todo: pack all true implementieren
        result &= ref;                         // Set bits of elemts that should not be compared to zero

    // std::cout << std::bitset<32>(result) << "-" << std::bitset<32>(ref) << std::endl;
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
    return compare_all_true<decltype(mm_cmp_eq<T_RegisterType>), T_RegisterType, t_cmp...>(
            lhs, rhs, &mm_cmp_eq<T_RegisterType>);
}


} // namespace mjolnir::x86
