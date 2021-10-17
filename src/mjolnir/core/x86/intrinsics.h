//! @file
//! x86/intrinsics.h
//!
//! @brief
//! Contains generalized/template versions of the x86 intrinsics


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/x86/definitions.h"


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
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_andnot(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


//! @brief
//! Blend elements from `a` and `b` using a control mask and return the resulting vector register.
//!
//! @tparam t_mask
//! An integer value used control mask. Consult the intel intrinsics guide for further information. Note that this
//! library provides template functions in `permute.h` to determine the correct mask fo each use-case.
//! @tparam T_RegisterType
//! The register type
//!
//! @param [in] a:
//! First register
//! @param [in] b:
//! Second register
//!
//! @return
//! Register with blended values
template <I32 t_mask, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_blend(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


//! @brief
//! Bit cast a floating-point vector register to an equally sized integer vector register.
//!
//! @tparam T_RegisterTypeIn:
//! Type of the input register
//! @tparam T_RegisterTypeOut:
//! Type of the output register
//!
//! @param[in] src:
//! Input register
//!
//! @return
//! Bit cast register
template <FloatVectorRegister T_RegisterTypeIn, IntegerVectorRegister T_RegisterTypeOut>
[[nodiscard]] inline auto mm_cast_fi(T_RegisterTypeIn src) noexcept -> T_RegisterTypeOut;


//! @brief
//! Bit cast an integer vector register to an equally sized floating-point vector register.
//!
//! @tparam T_RegisterTypeOut:
//! Type of the output register
//! @tparam T_RegisterTypeIn:
//! Type of the input register
//!
//! @param[in] src:
//! Input register
//!
//! @return
//! Bit cast register
template <FloatVectorRegister T_RegisterTypeOut, IntegerVectorRegister T_RegisterTypeIn>
[[nodiscard]] inline auto mm_cast_if(T_RegisterTypeIn src) noexcept -> T_RegisterTypeOut;


//! @brief
//! Load data from an aligned memory location into a new register.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] ptr:
//! Correctly aligned pointer to the memory location
//!
//! @return
//! New register with loaded data
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_load(ElementType<T_RegisterType>* ptr) noexcept -> T_RegisterType;


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
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_set1(ElementType<T_RegisterType> value) noexcept -> T_RegisterType;


//! @brief
//! Set register elements with the supplied values in reverse order.
//!
//! @tparam T_RegisterType:
//! The register type
//! @tparam T_Args
//! Parameter pack that holds the values
//!
//! @param args
//! Values of the parameter pack
//!
//! @return
//! Register that contains the provided values
template <FloatVectorRegister T_RegisterType, typename... T_Args>
[[nodiscard]] inline auto mm_setr(T_Args... args) noexcept -> T_RegisterType;


//! @brief
//! Return a vector register with all elements set to zero.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @return
//! Vector register with all elements set to zero
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_setzero() noexcept -> T_RegisterType;


//! @brief
//! Store the content of a register to a memory address
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in, out] ptr:
//! Correctly aligned pointer to the memory where the content should be stored
//! @param [in] reg:
//! The register that should be stored
template <FloatVectorRegister T_RegisterType>
inline void mm_store(ElementType<T_RegisterType>* ptr, T_RegisterType reg) noexcept;

//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/utility/pointer_operations.h"
#include "mjolnir/core/x86/x86.h"

#include <cassert>
#include <utility>


namespace mjolnir::x86
{
// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
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

template <I32 t_mask, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_blend(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_blend_ps(a, b, t_mask);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_blend_pd(a, b, t_mask);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_blend_ps(a, b, t_mask);
    else
        return _mm256_blend_pd(a, b, t_mask);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterTypeIn, IntegerVectorRegister T_RegisterTypeOut>
[[nodiscard]] inline auto mm_cast_fi(T_RegisterTypeIn src) noexcept -> T_RegisterTypeOut
{
    if constexpr (is_m128<T_RegisterTypeIn>)
        return _mm_castps_si128(src);
    else if constexpr (is_m128d<T_RegisterTypeIn>)
        return _mm_castpd_si128(src);
    else if constexpr (is_m256<T_RegisterTypeIn>)
        return _mm256_castps_si256(src);
    else
        return _mm256_castpd_si256(src);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterTypeOut, IntegerVectorRegister T_RegisterTypeIn>
[[nodiscard]] inline auto mm_cast_if(T_RegisterTypeIn src) noexcept -> T_RegisterTypeOut
{
    if constexpr (is_m128<T_RegisterTypeOut>)
        return _mm_castsi128_ps(src);
    else if constexpr (is_m128d<T_RegisterTypeOut>)
        return _mm_castsi128_pd(src);
    else if constexpr (is_m256<T_RegisterTypeOut>)
        return _mm256_castsi256_ps(src);
    else
        return _mm256_castsi256_pd(src);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_load(ElementType<T_RegisterType>* ptr) noexcept -> T_RegisterType
{
    assert(is_aligned<alignment_bytes<T_RegisterType>>(ptr)); // NOLINT

    if constexpr (is_m128<T_RegisterType>)
        return _mm_load_ps(ptr);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_load_pd(ptr);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_load_ps(ptr);
    else
        return _mm256_load_pd(ptr);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
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

template <FloatVectorRegister T_RegisterType, typename... T_Args>
[[nodiscard]] inline auto mm_setr(T_Args... args) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_setr_ps(std::forward<T_Args>(args)...);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_setr_pd(std::forward<T_Args>(args)...);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_setr_ps(std::forward<T_Args>(args)...);
    else
        return _mm256_setr_pd(std::forward<T_Args>(args)...);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_setzero() noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_setzero_ps();
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_setzero_pd();
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_setzero_ps();
    else
        return _mm256_setzero_pd();
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
inline void mm_store(ElementType<T_RegisterType>* ptr, T_RegisterType reg) noexcept
{
    assert(is_aligned<alignment_bytes<T_RegisterType>>(ptr)); // NOLINT

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
