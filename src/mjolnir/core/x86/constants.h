//! @file
//! intrinsics.h
//!
//! @brief
//! Contains x86 vectorization specific constants


#pragma once
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/type.h"
#include "mjolnir/core/x86/x86.h"
#include <type_traits>

namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{


//! @brief
//! Type dependent constant that is only `true` for `__m128` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m128 = std::is_same<T_Type, __m128>::value;


//! @brief
//! Type dependent constant that is only `true` for `__m128d` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m128d = std::is_same<T_Type, __m128d>::value;


//! @brief
//! Type dependent constant that is only `true` for `__m256` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m256 = std::is_same<T_Type, __m256>::value;


//! @brief
//! Type dependent constant that is only `true` for `__m256d` and `false` for all other types.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_m256d = std::is_same<T_Type, __m256d>::value;


//! @brief
//! Type dependent constant that is only `true` for supported x86 vector registers.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_register = is_any_of<T_Type, __m128, __m128d, __m128i, __m256, __m256d, __m256i>();


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
//! Type dependent constant that is only `true` for x86 vector registers that have floating-point types as values.
//!
//! @tparam T_Type:
//! Type
template <typename T_Type>
inline constexpr bool is_float_register = is_any_of<T_Type, __m128, __m128d, __m256, __m256d>();


// internal declarations
// --------------------------------------------------------------------------------------------------------------------

//! \cond DO_NOT_DOCUMENT
namespace internal
{
//! Support structure to determine the value type of an x86 vector register.
template <typename T_Type>
struct ValueTypeStruct
{
    static_assert(is_float_register<T_Type>, "Type is not a supported float-based x86 vector register type.");
    using Type = typename std::conditional<is_any_of<T_Type, __m128d, __m256d>(), F64, F32>::type;
};


//! Return the alignment in bytes for a register type.
template <typename T_Type>
[[nodiscard]] inline consteval auto get_alignment_bytes() noexcept -> UST;

//! Return the number of lanes of a register type.
template <typename T_Type>
[[nodiscard]] inline consteval auto get_num_lanes() noexcept -> UST;


} // namespace internal
//! \endcond


// continued public declarations
// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! The value type of an x86 vector register that has a floating-point type as value.
//!
//! @tparam T_RegisterType:
//! Register type
//!
//! @remark
//! Any type that is not a vector register with a floating-point type as value will trigger a `static_assert`. Integer
//! based registers are not supported because they do not have a fixed value type.
template <typename T_RegisterType>
using ValueType = typename internal::ValueTypeStruct<T_RegisterType>::Type;


//! @brief
//! Alignment requirement of an x86 vector register in bytes.
//!
//! @tparam T_RegisterType:
//! Register type
//!
//! @remark
//! Any type that is not as x86 vector register will trigger a `static_assert`
template <typename T_RegisterType>
inline constexpr UST alignment_bytes = internal::get_alignment_bytes<T_RegisterType>();


//! @brief
//! Number of register lanes.
//!
//! @tparam T_RegisterType:
//! Register type
//!
//! @remark
//! Any type that is not as x86 vector register will trigger a `static_assert`
template <typename T_RegisterType>
inline constexpr UST num_lanes = internal::get_num_lanes<T_RegisterType>();


//! @brief
//! Number of values stored inside of the register.
//!
//! @tparam T_RegisterType:
//! Register type
//!
//! @remark
//! Any type that is not a vector register with a floating-point type as value will trigger a `static_assert`. Integer
//! based registers are not supported because they do not have a fixed value type.
template <typename T_RegisterType>
inline constexpr UST num_values = sizeof(T_RegisterType) / sizeof(ValueType<T_RegisterType>);


//! @brief
//! Number of values stored inside of the register.
//!
//! @tparam T_RegisterType:
//! Register type
//!
//! @remark
//! Any type that is not a vector register with a floating-point type as value will trigger a `static_assert`. Integer
//! based registers are not supported because they do not have a fixed value type.
template <typename T_RegisterType>
inline constexpr UST num_lane_values = num_values<T_RegisterType> / num_lanes<T_RegisterType>;

//! @}
} // namespace mjolnir::x86


// ====================================================================================================================

namespace mjolnir::x86
{
//! \cond DO_NOT_DOCUMENT
namespace internal
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] inline consteval auto get_alignment_bytes() noexcept -> UST
{
    static_assert(is_register<T_Type>, "Type is not a supported x86 vector register type.");
    constexpr const UST alignment_bytes_sse = 16;
    constexpr const UST alignment_bytes_avx = 32;

    if constexpr (is_sse_register<T_Type>)
        return alignment_bytes_sse;
    else
        return alignment_bytes_avx;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type>
[[nodiscard]] inline consteval auto get_num_lanes() noexcept -> UST
{
    static_assert(is_register<T_Type>, "Type is not a supported x86 vector register type.");
    constexpr const UST num_lanes_sse = 1;
    constexpr const UST num_lanes_avx = 2;

    if constexpr (is_sse_register<T_Type>)
        return num_lanes_sse;
    else
        return num_lanes_avx;
}


} // namespace internal
//! \endcond


} // namespace mjolnir::x86
