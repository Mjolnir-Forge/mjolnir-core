//! @file
//! x86/intrinsics.h
//!
//! @brief
//! Contains generalized/template versions of the x86 intrinsics


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/x86/definitions.h"

#include <concepts>

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Perform an element-wise addition of `lhs` and `rhs` and return the result.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! Results of the element-wise addition.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_add(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Compute the bitwise AND of `a` and `b`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] a:
//! First register
//! @param [in] b:
//! Second register
//!
//! @return
//! Result of the bitwise AND operation
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_and(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


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
//! An integer value used as control mask. Consult the intel intrinsics guide for further information. Note that this
//! library provides template functions in `permute.h` to apply the correct mask for each use-case.
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
//! Broadcasts the lowest floating point element across lanes to all elements of the returned register.
//!
//! @warning
//! Since the value is broadcastet across lanes, the latency of this operation is much higher for multi-lane registers
//! (AVX) than for single lane registers (SSE).
//!
//! @tparam T_RegisterType
//! The register type
//!
//! @param[in] src:
//! The source register
//!
//! @return
//! Register with broadcastet values
//!
//! @todo:
//! Fix GCC bug once GCC 11 is available and set as new minimal compiler version. See comments in function definition.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_broadcast(T_RegisterType src) noexcept -> T_RegisterType;


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
template <FloatVectorRegister T_RegisterTypeIn>
[[nodiscard]] inline auto mm_cast_fi(T_RegisterTypeIn src) noexcept;


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
//! Compare the register elements in `lhs` and `rhs` for equality and return the result.
//!
//! @details
//! If a comparison is false, the corresponding value of the returned register is `0`. For single precision registers, a
//! true comparison yields `0xFFFFFFFF`. For double precision registers it is `0xFFFFFFFFFFFFFFFF`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! Register with the comparison results. See detailed description.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cmp_eq(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Compare element-wise if the register elements of `lhs` are greater equal than the ones in `rhs`.
//!
//! @details
//! If a comparison is false, the corresponding value of the returned register is `0`. For single precision registers, a
//! true comparison yields `0xFFFFFFFF`. For double precision registers it is `0xFFFFFFFFFFFFFFFF`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! Register with the comparison results. See detailed description.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cmp_ge(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Compare element-wise if the register elements of `lhs` are greater than the ones in `rhs`.
//!
//! @details
//! If a comparison is false, the corresponding value of the returned register is `0`. For single precision registers, a
//! true comparison yields `0xFFFFFFFF`. For double precision registers it is `0xFFFFFFFFFFFFFFFF`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! Register with the comparison results. See detailed description.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cmp_gt(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Compare element-wise if the register elements of `lhs` are less equal than the ones in `rhs`.
//!
//! @details
//! If a comparison is false, the corresponding value of the returned register is `0`. For single precision registers, a
//! true comparison yields `0xFFFFFFFF`. For double precision registers it is `0xFFFFFFFFFFFFFFFF`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! Register with the comparison results. See detailed description.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cmp_le(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Compare element-wise if the register elements of `lhs` are less than the ones in `rhs`.
//!
//! @details
//! If a comparison is false, the corresponding value of the returned register is `0`. For single precision registers, a
//! true comparison yields `0xFFFFFFFF`. For double precision registers it is `0xFFFFFFFFFFFFFFFF`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! Register with the comparison results. See detailed description.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cmp_lt(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Return the first element of `src`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] src:
//! The source register
//!
//! @return
//! First element of `src`
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cvt_float(T_RegisterType src) -> ElementType<T_RegisterType>;


//! @brief
//! Perform an element-wise division of `lhs` by `rhs` and return the result.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! Results of the element-wise division.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_div(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Perform an element-wise multiplication of `a` and `b`, add `c` and return the result.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] a:
//! The register left of the multiplication operator
//! @param[in] b:
//! The register right of the multiplication operator
//! @param[in] c:
//! The register that will be added after the multiplication
//!
//! @return
//! Results of the element-wise multiplication with subsequent addition
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_fmadd(T_RegisterType a, T_RegisterType b, T_RegisterType c) noexcept -> T_RegisterType;


//! @brief
//! Perform an element-wise multiplication of `a` and `b`, subtract `c` and return the result.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] a:
//! The register left of the multiplication operator
//! @param[in] b:
//! The register right of the multiplication operator
//! @param[in] c:
//! The register that will be subtracted after the multiplication
//!
//! @return
//! Results of the element-wise multiplication with subsequent subtraction
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_fmsub(T_RegisterType a, T_RegisterType b, T_RegisterType c) noexcept -> T_RegisterType;


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
//! Create mask from the most significant bit of each 8-bit element in `src`, and return the result as unsigned integer.
//!
//! @details
//! For SSE registers a 16 bit integer is returned. For AVX registers a 32 bit integer is returned.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] src:
//! The source register
//!
//! @return
//! The created mask
template <IntegerVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_movemask_epi8(T_RegisterType src) noexcept;


//! @brief
//! Perform an element-wise multiplication of `lhs` and `rhs` and return the result.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! Results of the element-wise multiplication.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_mul(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Compute the bitwise OR of `a` and `b`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] a:
//! First register
//! @param [in] b:
//! Second register
//!
//! @return
//! Result of the bitwise OR operation
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_or(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


//! @brief
//! Shuffle the elements in `src` using the control mask `t_mask` and return the resulting vector register.
//!
//! @tparam t_mask
//! An integer value used as control mask. Consult the intel intrinsics guide for further information. Note that
//! this library provides template functions in `permute.h` to apply the correct mask for each use-case.
//! @tparam T_RegisterType
//! The register type
//!
//! @param [in] src:
//! Source register
//!
//! @return
//! Register with shuffled values
template <I32 t_mask, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_permute(T_RegisterType src) noexcept -> T_RegisterType;


//! @brief
//! Shuffle 128-bits lanes selected by `t_mask` from `a` and `b`, and return the results in a new register.
//!
//! @tparam t_mask
//! An integer value used as control mask. Consult the intel intrinsics guide for further information. Note that this
//! library provides template functions in `permute.h` to apply the correct mask for each use-case.
//! @tparam T_RegisterType
//! The register type
//!
//!
//! @param [in] a:
//! First register
//! @param [in] b:
//! Second register
//!
//! @return
//! Register with permuted 128 bit lanes values
template <I32 t_mask, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto mm_permute2f128(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


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
//! Return a register with the first half of the lane elements selected from `a` and the second half from `b`.
//!
//! @tparam t_mask:
//! An integer value used as control mask. Consult the intel intrinsics guide for further information. Note that this
//! library provides template functions in `permute.h` to apply the correct mask for each use-case.
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] a:
//! First register
//! @param [in] b:
//! Second register
//!
//! @return
//! New register with shuffled values
template <UST t_mask, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_shuffle(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


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


//! @brief
//! Subtract `rhs` element-wise from `rhs` and return the result.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param[in] lhs:
//! The register left of the operator
//! @param[in] rhs:
//! The register right of the operator
//!
//! @return
//! Results of the element-wise subtraction.
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_sub(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Store the higher elements of `a` and `b` alternately in a new register and return it.
//!
//! @details
//! The first element is taken from `a`
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] a:
//! First register
//! @param [in] b:
//! Second register
//!
//! @return
//! Higher elements of `a` and `b` in alternating order
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_unpackhi(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


//! @brief
//! Store the lower elements of `a` and `b` alternately in a new register and return it.
//!
//! @details
//! The first element is taken from `a`
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] a:
//! First register
//! @param [in] b:
//! Second register
//!
//! @return
//! Lower elements of `a` and `b` in alternating order
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_unpacklo(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


//! @brief
//! Compute the bitwise XOR of `a` and `b`.
//!
//! @tparam T_RegisterType:
//! The register type
//!
//! @param [in] a:
//! First register
//! @param [in] b:
//! Second register
//!
//! @return
//! Result of the bitwise XOR operation
template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_xor(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType;


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
[[nodiscard]] inline auto mm_add(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_add_ps(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_add_pd(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_add_ps(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else
        return _mm256_add_pd(lhs, rhs); // NOLINT(portability-simd-intrinsics)
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_and(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_and_ps(a, b);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_and_pd(a, b);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_and_ps(a, b);
    else
        return _mm256_and_pd(a, b);
}


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

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_broadcast(T_RegisterType src) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_broadcastss_ps(src);
    else if constexpr (is_m128d<T_RegisterType>)
        // The following command is currently missing in gcc - see https://stackoverflow.com/q/58270381/6700329
        // Should be fixed in gcc 11 - see https://gcc.gnu.org/bugzilla/show_bug.cgi?id=95483
        // return _mm_broadcastsd_pd(src);
        return _mm_movedup_pd(src);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_broadcastss_ps(_mm256_castps256_ps128(src));
    else
        return _mm256_broadcastsd_pd(_mm256_castpd256_pd128(src));
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterTypeIn>
[[nodiscard]] inline auto mm_cast_fi(T_RegisterTypeIn src) noexcept
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
[[nodiscard]] inline auto mm_cmp_eq(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_cmpeq_ps(lhs, rhs);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_cmpeq_pd(lhs, rhs);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_cmp_ps(lhs, rhs, _CMP_EQ_OS);
    else
        return _mm256_cmp_pd(lhs, rhs, _CMP_EQ_OS);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cmp_ge(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_cmpge_ps(lhs, rhs);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_cmpge_pd(lhs, rhs);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_cmp_ps(lhs, rhs, _CMP_GE_OS);
    else
        return _mm256_cmp_pd(lhs, rhs, _CMP_GE_OS);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cmp_gt(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_cmpgt_ps(lhs, rhs);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_cmpgt_pd(lhs, rhs);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_cmp_ps(lhs, rhs, _CMP_GT_OS);
    else
        return _mm256_cmp_pd(lhs, rhs, _CMP_GT_OS);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cmp_le(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_cmple_ps(lhs, rhs);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_cmple_pd(lhs, rhs);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_cmp_ps(lhs, rhs, _CMP_LE_OS);
    else
        return _mm256_cmp_pd(lhs, rhs, _CMP_LE_OS);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cmp_lt(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_cmplt_ps(lhs, rhs);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_cmplt_pd(lhs, rhs);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_cmp_ps(lhs, rhs, _CMP_LT_OS);
    else
        return _mm256_cmp_pd(lhs, rhs, _CMP_LT_OS);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_cvt_float(T_RegisterType src) -> ElementType<T_RegisterType>
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_cvtss_f32(src);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_cvtsd_f64(src);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_cvtss_f32(src);
    else
        return _mm256_cvtsd_f64(src);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_div(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_div_ps(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_div_pd(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_div_ps(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else
        return _mm256_div_pd(lhs, rhs); // NOLINT(portability-simd-intrinsics)
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_fmadd(T_RegisterType a, T_RegisterType b, T_RegisterType c) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_fmadd_ps(a, b, c);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_fmadd_pd(a, b, c);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_fmadd_ps(a, b, c);
    else
        return _mm256_fmadd_pd(a, b, c);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_fmsub(T_RegisterType a, T_RegisterType b, T_RegisterType c) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_fmsub_ps(a, b, c);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_fmsub_pd(a, b, c);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_fmsub_ps(a, b, c);
    else
        return _mm256_fmsub_pd(a, b, c);
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

template <IntegerVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_movemask_epi8(T_RegisterType src) noexcept
{
    if constexpr (is_m128i<T_RegisterType>)
        return static_cast<U16>(_mm_movemask_epi8(src));
    else
        return static_cast<U32>(_mm256_movemask_epi8(src));
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_mul(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_mul_ps(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_mul_pd(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_mul_ps(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else
        return _mm256_mul_pd(lhs, rhs); // NOLINT(portability-simd-intrinsics)
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_or(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_or_ps(a, b);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_or_pd(a, b);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_or_ps(a, b);
    else
        return _mm256_or_pd(a, b);
}


// --------------------------------------------------------------------------------------------------------------------

template <I32 t_mask, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_permute(T_RegisterType src) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_permute_ps(src, t_mask);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_permute_pd(src, t_mask);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_permute_ps(src, t_mask);
    else
        return _mm256_permute_pd(src, t_mask);
}


// --------------------------------------------------------------------------------------------------------------------

template <I32 t_mask, FloatAVXRegister T_RegisterType>
[[nodiscard]] inline auto mm_permute2f128(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType
{
    if constexpr (is_m256<T_RegisterType>)
        return _mm256_permute2f128_ps(a, b, t_mask);
    else
        return _mm256_permute2f128_pd(a, b, t_mask);
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
    using EType = ElementType<T_RegisterType>;

    if constexpr (is_m128<T_RegisterType>)
        return _mm_setr_ps(static_cast<EType>(args)...);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_setr_pd(static_cast<EType>(args)...);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_setr_ps(static_cast<EType>(args)...);
    else
        return _mm256_setr_pd(static_cast<EType>(args)...);
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

template <UST t_mask, FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_shuffle(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_shuffle_ps(a, b, t_mask);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_shuffle_pd(a, b, t_mask);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_shuffle_ps(a, b, t_mask);
    else
        return _mm256_shuffle_pd(a, b, t_mask);
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


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_sub(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_sub_ps(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_sub_pd(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_sub_ps(lhs, rhs); // NOLINT(portability-simd-intrinsics)
    else
        return _mm256_sub_pd(lhs, rhs); // NOLINT(portability-simd-intrinsics)
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_unpackhi(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_unpackhi_ps(a, b);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_unpackhi_pd(a, b);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_unpackhi_ps(a, b);
    else
        return _mm256_unpackhi_pd(a, b);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_unpacklo(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_unpacklo_ps(a, b);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_unpacklo_pd(a, b);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_unpacklo_ps(a, b);
    else
        return _mm256_unpacklo_pd(a, b);
}


// --------------------------------------------------------------------------------------------------------------------

template <FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto mm_xor(T_RegisterType a, T_RegisterType b) noexcept -> T_RegisterType
{
    if constexpr (is_m128<T_RegisterType>)
        return _mm_xor_ps(a, b);
    else if constexpr (is_m128d<T_RegisterType>)
        return _mm_xor_pd(a, b);
    else if constexpr (is_m256<T_RegisterType>)
        return _mm256_xor_ps(a, b);
    else
        return _mm256_xor_pd(a, b);
}


} // namespace mjolnir::x86
