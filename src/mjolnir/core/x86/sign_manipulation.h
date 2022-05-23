//! @file
//! sign_manipulation.h
//!
//! @brief
//! Contains math functions for vector registers


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/x86/definitions.h"


namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Return a new register with the absolute values of the input register.
//!
//! @tparam T_RegisterType:
//! Register type
//!
//! @param[in] src:
//! The initial values
//!
//! @return
//! Register containing the absolute values
//!
//! @remark
//! source: https://stackoverflow.com/q/23847377/6700329
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto abs(T_RegisterType src) noexcept -> T_RegisterType;


//! @brief
//! Return a new register consisting of the magnitudes from `src_magnitude` and the signs of `src_sign`.
//!
//! @tparam t_all_mag_positive:
//! If `true`, the function expects all values of `src_magnitude` to be positive. This allows for some minor performance
//! gain. The default value is `false`
//!
//! @param[in] src_magnitude:
//! The register that provides the magnitudes for the returned register
//! @param[in] src_sign:
//! The register that provides the signs for the returned register
//!
//! @return
//! New register with the magnitudes from `src_magnitude` and the signs of `src_sign`.
template <bool t_all_mag_positive = false, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto copy_sign(T_RegisterType src_magnitude, T_RegisterType src_sign) noexcept -> T_RegisterType;


//! @brief
//! Return a new register with the negated elements of the source register.
//!
//! @tparam T_RegisterType
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Register with negated values
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto negate_all(T_RegisterType src) noexcept -> T_RegisterType;


//! @brief
//! Return a copy of the source register with the selected elements being negated.
//!
//! @tparam t_neg:
//! A parameter pack of the same size as the number of register elements. If a value is `true`, the corresponding
//! element will be negated. Otherwise the element is just copied.
//! @tparam T_RegisterType
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Register with negated values
template <bool... t_neg, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto negate_selected(T_RegisterType src) noexcept -> T_RegisterType;


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/x86/comparison.h"
#include "mjolnir/core/x86/intrinsics.h"

#include <cassert>


namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto abs(T_RegisterType src) noexcept -> T_RegisterType
{
    using EType     = ElementType<T_RegisterType>;
    const auto mask = mm_set1<T_RegisterType>(static_cast<EType>(-0.0));

    return mm_andnot(mask, src);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_all_mag_positive, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto copy_sign(T_RegisterType src_magnitude, T_RegisterType src_sign) noexcept -> T_RegisterType
{
    using EType     = ElementType<T_RegisterType>;
    const auto mask = mm_set1<T_RegisterType>(static_cast<EType>(-0.0));

    T_RegisterType sign = mm_and(src_sign, mask);

    if constexpr (! t_all_mag_positive)
        src_magnitude = mm_andnot(mask, src_magnitude);

    assert(is_memory_zero(mm_and(src_magnitude, mask)) && "Signed bit of one or more values set."); // NOLINT
    return mm_or(sign, src_magnitude);
}


// --------------------------------------------------------------------------------------------------------------------


template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto negate_all(T_RegisterType src) noexcept -> T_RegisterType
{
    constexpr UST n_e = num_elements<T_RegisterType>;

    if constexpr (n_e == 2)
        return negate_selected<1, 1>(src);
    else if constexpr (n_e == 4)
        return negate_selected<1, 1, 1, 1>(src);
    else
        return negate_selected<1, 1, 1, 1, 1, 1, 1, 1>(src);
}


// --------------------------------------------------------------------------------------------------------------------


template <bool... t_neg, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto negate_selected(T_RegisterType src) noexcept -> T_RegisterType
{
    using EType       = ElementType<T_RegisterType>;
    constexpr UST n_e = num_elements<T_RegisterType>;

    static_assert(sizeof...(t_neg) == n_e,
                  "Number of boolean template parameters must be 0 or equal to the number of register elements");

    constexpr auto get_mask = [](bool a) constexpr->EType
    {
        if (a)
            return static_cast<EType>(-0.);
        return static_cast<EType>(0.);
    };
    auto mask = mm_setr<T_RegisterType>(get_mask(t_neg)...);

    return mm_xor(src, mask);
}

} // namespace mjolnir::x86
