//! @file
//! x86/definitions.h
//!
//! @brief
//! Contains x86 vectorization specific constants, concepts and definitions


#pragma once
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/type.h"
#include "mjolnir/core/x86/x86.h"
#include <type_traits>

#include <array>

// === DECLARATION ====================================================================================================

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Concept for a x86 vector register
//!
//! @tparam T_Type
//! Type
template <typename T_Type>
concept VectorRegister = is_any_of<T_Type, __m128, __m128d, __m128i, __m256, __m256d, __m256i>();


//! @brief
//! Concept for a x86 vector register that has floating-point elements.
//!
//! @tparam T_Type
//! Type
template <typename T_Type>
concept FloatVectorRegister = is_any_of<T_Type, __m128, __m128d, __m256, __m256d>();


//! @brief
//! Concept for a x86 SSE vector register that has floating-point elements.
//!
//! @tparam T_Type
//! Type
template <typename T_Type>
concept FloatSSERegister = is_any_of<T_Type, __m128, __m128d>();


//! @brief
//! Concept for a x86 vector register that has floating-point elements.
//!
//! @tparam T_Type
//! Type
template <typename T_Type>
concept FloatAVXRegister = is_any_of<T_Type, __m256, __m256d>();


//! @brief
//! Concept for a x86 vector register that has integer elements.
//!
//! @tparam T_Type
//! Type
template <typename T_Type>
concept IntegerVectorRegister = is_any_of<T_Type, __m128i, __m256i>();


//! @brief
//! Type dependent constant that is only `true` for `__m128` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m128 = std::is_same_v<T_Type, __m128>;


//! @brief
//! Type dependent constant that is only `true` for `__m128d` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m128d = std::is_same_v<T_Type, __m128d>;


//! @brief
//! Type dependent constant that is only `true` for `__m128i` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m128i = std::is_same_v<T_Type, __m128i>;


//! @brief
//! Type dependent constant that is only `true` for `__m256` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m256 = std::is_same_v<T_Type, __m256>;


//! @brief
//! Type dependent constant that is only `true` for `__m256d` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m256d = std::is_same_v<T_Type, __m256d>;


//! @brief
//! Type dependent constant that is only `true` for `__m256i` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m256i = std::is_same_v<T_Type, __m256i>;


//! @brief
//! Type dependent constant that is only `true` for supported x86 vector registers.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_vector_register = is_any_of<T_Type, __m128, __m128d, __m128i, __m256, __m256d, __m256i>();


//! @brief
//! Type dependent constant that is only `true` for SSE vector registers.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_sse_register = is_any_of<T_Type, __m128, __m128d, __m128i>();


//! @brief
//! Type dependent constant that is only `true` for AVX vector registers.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_avx_register = is_any_of<T_Type, __m256, __m256d, __m256i>();


//! @brief
//! Type dependent constant that is only `true` for x86 vector registers that have floating-point types as elements.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_float_register = is_any_of<T_Type, __m128, __m128d, __m256, __m256d>();


//! @brief
//! Type dependent constant that is only `true` for x86 vector registers that have integer types as elements.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_integer_register = is_any_of<T_Type, __m128i, __m256i>();


// ---internal declarations -------------------------------------------------------------------------------------------

//! \cond DO_NOT_DOCUMENT
namespace internal
{
//! Return the alignment in bytes for a register type.
template <VectorRegister T_Type>
[[nodiscard]] consteval auto get_alignment_bytes() noexcept -> UST;

//! Return the number of lanes of a register type.
template <VectorRegister T_Type>
[[nodiscard]] consteval auto get_num_lanes() noexcept -> UST;


} // namespace internal
//! \endcond


// --- continued public declarations ----------------------------------------------------------------------------------

//! @brief
//! The element type of an x86 vector register that is based on floating-point types.
//!
//! @tparam T_RegisterType:
//! Register type
template <FloatVectorRegister T_RegisterType>
using ElementType = typename std::conditional_t<is_any_of<T_RegisterType, __m128d, __m256d>(), F64, F32>;


//! @brief
//! `true` if the element type has single precision and `false` otherwise.
//!
//! @tparam T_RegisterType:
//! Register type
template <FloatVectorRegister T_RegisterType>
inline constexpr bool is_single_precision = std::is_same_v<ElementType<T_RegisterType>, F32>;


//! @brief
//! `true` if the element type has double precision and `false` otherwise.
//!
//! @tparam T_RegisterType:
//! Register type
template <FloatVectorRegister T_RegisterType>
inline constexpr bool is_double_precision = std::is_same_v<ElementType<T_RegisterType>, F64>;


//! @brief
//! Alignment requirement of an x86 vector register in bytes.
//!
//! @tparam T_RegisterType:
//! Register type
template <VectorRegister T_RegisterType>
inline constexpr UST alignment_bytes = internal::get_alignment_bytes<T_RegisterType>();


//! @brief
//! Number of register lanes.
//!
//! @tparam T_RegisterType:
//! Register type
template <VectorRegister T_RegisterType>
inline constexpr UST num_lanes = internal::get_num_lanes<T_RegisterType>();


//! @brief
//! Number of register elemnts.
//!
//! @tparam T_RegisterType:
//! Register type
template <FloatVectorRegister T_RegisterType>
inline constexpr UST num_elements = sizeof(T_RegisterType) / sizeof(ElementType<T_RegisterType>);


//! @brief
//! Number of elements per register lane.
//!
//! @tparam T_RegisterType:
//! Register type
template <FloatVectorRegister T_RegisterType>
inline constexpr UST num_lane_elements = num_elements<T_RegisterType> / num_lanes<T_RegisterType>;


//! @brief
//! `true` if the register has multiple lanes and `false` otherwise.
//!
//! @tparam T_RegisterType:
//! Register type
template <FloatVectorRegister T_RegisterType>
inline constexpr bool is_multi_lane = num_lanes<T_RegisterType> > 1;


//! @brief
//! A `std::array` of correct alignment, type and size to store all elements of a vector register type.
//!
//! @tparam T_RegisterType:
//! Register type
template <FloatVectorRegister T_RegisterType>
struct alignas(alignment_bytes<T_RegisterType>) VectorDataArray
    : public std::array<ElementType<T_RegisterType>, num_elements<T_RegisterType>>
{
};


//! @}
} // namespace mjolnir::x86


// === DEFINITIONS ====================================================================================================

//! \cond DO_NOT_DOCUMENT
namespace mjolnir::x86::internal
{
// --------------------------------------------------------------------------------------------------------------------

template <VectorRegister T_Type>
[[nodiscard]] consteval auto get_alignment_bytes() noexcept -> UST
{
    constexpr UST alignment_bytes_sse = 16;
    constexpr UST alignment_bytes_avx = 32;

    if constexpr (is_sse_register<T_Type>)
        return alignment_bytes_sse;
    else
        return alignment_bytes_avx;
}


// --------------------------------------------------------------------------------------------------------------------

template <VectorRegister T_Type>
[[nodiscard]] consteval auto get_num_lanes() noexcept -> UST
{
    constexpr UST num_lanes_sse = 1;
    constexpr UST num_lanes_avx = 2;

    if constexpr (is_sse_register<T_Type>)
        return num_lanes_sse;
    else
        return num_lanes_avx;
}


} // namespace internal
//! \endcond


} // namespace mjolnir::x86
