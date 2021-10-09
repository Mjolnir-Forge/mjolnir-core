//! @file
//! x86/intrinsics.h
//!
//! @brief
//! Contains generalized/template versions of the x86 intrinsics


#pragma once

#include "mjolnir/core/x86/concepts.h"
#include "mjolnir/core/x86/constants.h"
#include "mjolnir/core/x86/x86.h"

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Compute the bitwise NOT of all elements in `a` and then AND with `b`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] a:
//! Register from which the bitwise NOT is computed
//! @param [in] b:
//! Second register
//!
//! @return
//! Result of the bitwise operation `NOT a AND b` per register element
template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto mm_andnot(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


//! @brief
//! Broadcast a single value a to all elements of the register
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] value:
//! The value that should be broadcasted
//!
//! @return
//! Register with broadcasted value
template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto mm_set1(ElementType<T_RegisterType> value) noexcept -> T_RegisterType;


//! @brief
//! Store the content of a register to a memory address
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in, out] ptr:
//! Pointer to the memory where the content should be stored
//! @param [in] reg:
//! The register that should be stored
template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
inline void mm_store(ElementType<T_RegisterType>* ptr, T_RegisterType reg) noexcept;

//! @}
} // namespace mjolnir::x86


// ====================================================================================================================

namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto mm_andnot(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_andnot_ps(a, b);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_andnot_pd(a, b);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_andnot_ps(a, b);
    else
        return _mm256_andnot_pd(a, b);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
[[nodiscard]] inline auto mm_set1(ElementType<T_RegisterType> value) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_set1_ps(value);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_set1_pd(value);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_set1_ps(value);
    else
        return _mm256_set1_pd(value);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_RegisterType>
requires FloatVectorRegister<T_RegisterType>
inline void mm_store(ElementType<T_RegisterType>* ptr, T_RegisterType reg) noexcept
{
    if constexpr (is_m128<T_RegisterType>)
        _mm_store_ps(ptr, reg);
    else if constexpr (is_m128d<T_RegisterType>)
        _mm_store_pd(ptr, reg);
    else if constexpr (is_m256<T_RegisterType>)
        _mm256_store_ps(ptr, reg);
    else
        _mm256_store_pd(ptr, reg);
}


} // namespace mjolnir::x86
