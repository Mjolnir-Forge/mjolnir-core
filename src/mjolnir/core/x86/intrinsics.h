//! @file
//! intrinsics.h
//!
//! @brief
//! Contains generalized/template versions of the x86 intrinsics


#include "mjolnir/core/x86/constants.h"
#include "mjolnir/core/x86/x86.h"

namespace mjolnir
{
//! \addtogroup core_x86
//! @{


//! @}
} // namespace mjolnir


// ====================================================================================================================

namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType, typename T_Type>
inline auto mm_set1(T_Type value) -> T_RegisterType
{
    /*
        using namespace GDL::simd;
        static_assert(IsRegisterType<_registerType>, "Function can only be used with compatible register types.");

        if constexpr (Is__m128<_registerType>)
            return _mm_set1_ps(static_cast<F32>(value));
        else if constexpr (Is__m128d<_registerType>)
            return _mm_set1_pd(static_cast<F64>(value));
    #ifdef __AVX2__
        else if constexpr (Is__m256<_registerType>)
            return _mm256_set1_ps(static_cast<F32>(value));
        else
            return _mm256_set1_pd(static_cast<F64>(value));
    #endif // __AVX2__
    */
}
} // namespace mjolnir
