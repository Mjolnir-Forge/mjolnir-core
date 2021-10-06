//! @file
//! intrinsics.h
//!
//! @brief
//! Contains generalized/template versions of the x86 intrinsics


#include "mjolnir/core/x86/constants.h"
#include "mjolnir/core/x86/x86.h"

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{

//! @brief
//! Broadcast a single value a to all elements of the register
//!
//! @tparam T_RegisterType:
//! The register type
//! @tparam T_Type:
//! Type of the broadcasted value
//!
//! @param [in] value:
//! The value that should be broadcasted
//!
//! @return
//! Register with broadcasted value
template <typename T_RegisterType>
[[nodiscard]] inline auto mm_set1(ValueType<T_RegisterType> value) noexcept -> T_RegisterType;


//! @}
} // namespace mjolnir::x86


// ====================================================================================================================

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
[[nodiscard]] inline auto mm_set1(ValueType<T_RegisterType> value) noexcept -> T_RegisterType
{
    static_assert(is_float_register<T_RegisterType>, "Type is not a supported float-based x86 vector register type.");

    if constexpr (is_m128<T_RegisterType>)
        return _mm_set1_ps(value);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_set1_pd(value);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_set1_ps(value);
    else
        return _mm256_set1_pd(value);
}
} // namespace mjolnir::x86
