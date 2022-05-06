//! @file
//! math.h
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

    if constexpr (not t_all_mag_positive)
        src_magnitude = mm_andnot(mask, src_magnitude);

    assert(is_memory_zero(mm_and(src_magnitude, mask)) && "Signed bit of one or more values set.");
    return mm_or(sign, src_magnitude);
}


} // namespace mjolnir::x86
