//! @file
//! math/linear_algebra/vector_operations.h
//!
//! @brief
//! Functions to calculate various vector products and properties.


#pragma once

#include "mjolnir/core/definitions.h"
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/element_summation.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"

#include <array>

// === DECLARATIONS ===================================================================================================

namespace mjolnir
{
//! \addtogroup core_math
//! @{


//! @brief
//! Calculate the cross product of two 3d-vectors
//!
//! @tparam T_Type:
//! Data type of the vector elements and the returned value
//!
//! @param[in] lhs:
//! Data of the vector on the left-hand side of the operator
//! @param[in] rhs:
//! Data of the vector on the right-hand side of the operator
//!
//! @return
//! Cross product
template <Number T_Type>
[[nodiscard]] constexpr auto cross_product(const std::array<T_Type, 3>& lhs, const std::array<T_Type, 3>& rhs) noexcept
        -> std::array<T_Type, 3>;


//! @brief
//! Calculate the cross product of two 3d-vectors
//!
//! @details
//! Only the first 3 elements of a register are considered.
//!
//! @tparam T_RegisterType:
//! Register type that stores the vector data.
//!
//! @param[in] lhs:
//! Data of the vector on the left-hand side of the operator
//! @param[in] rhs:
//! Data of the vector on the right-hand side of the operator
//!
//! @return
//! Cross product
template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto cross_product(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType;


//! @brief
//! Calculate the dot product of 2 vectors
//!
//! @tparam t_size:
//! Size of the vector
//! @tparam T_Type:
//! Data type of the vector elements and the returned value
//!
//! @param[in] lhs:
//! Data of the vector on the left-hand side of the operator
//! @param[in] rhs:
//! Data of the vector on the right-hand side of the operator
//!
//! @return
//! Dot product
template <UST t_size, Number T_Type>
[[nodiscard]] constexpr auto dot_product(const std::array<T_Type, t_size>& lhs,
                                         const std::array<T_Type, t_size>& rhs) noexcept -> T_Type;


//! @brief
//! Calculate the dot product of 2 vectors
//!
//! @details
//! Only the first `t_size` elements of a register are considered.
//!
//! @tparam t_size:
//! Size of the vector. Note that this number should not exceed the number of register elements.
//! @tparam T_RegisterType:
//! Register type that stores the vector data.
//!
//! @param[in] lhs:
//! Data of the vector on the left-hand side of the operator
//! @param[in] rhs:
//! Data of the vector on the right-hand side of the operator
//!
//! @return
//! Dot product
template <UST t_size, x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto dot_product(T_RegisterType lhs, T_RegisterType rhs) noexcept
        -> x86::ElementType<T_RegisterType>;


//! @brief
//! Calculate the scalar triple product of 3 vectors
//!
//! @tparam T_Type:
//! Data type of the vector elements and the returned value
//!
//! @param[in] a:
//! Data of the first vector
//! @param[in] b:
//! Data of the second vector
//! @param[in] c:
//! Data of the third vector
//!
//! @return
//! Scalar triple product
template <Number T_Type>
[[nodiscard]] constexpr auto scalar_triple_product(const std::array<T_Type, 3>& a,
                                                   const std::array<T_Type, 3>& b,
                                                   const std::array<T_Type, 3>& c) noexcept -> T_Type;


//! @brief
//! Calculate the scalar triple product of 3 vectors
//!
//! @tparam T_RegisterType:
//! Register type that stores the vector data.
//!
//! @param[in] a:
//! Data of the first vector
//! @param[in] b:
//! Data of the second vector
//! @param[in] c:
//! Data of the third vector
//!
//! @return
//! Scalar triple product
template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto scalar_triple_product(T_RegisterType a, T_RegisterType b, T_RegisterType c) noexcept
        -> x86::ElementType<T_RegisterType>;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
[[nodiscard]] constexpr auto cross_product(const std::array<T_Type, 3>& lhs, const std::array<T_Type, 3>& rhs) noexcept
        -> std::array<T_Type, 3>
{
    return {{lhs[1] * rhs[2] - lhs[2] * rhs[1], lhs[2] * rhs[0] - lhs[0] * rhs[2], lhs[0] * rhs[1] - lhs[1] * rhs[0]}};
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto cross_product(T_RegisterType lhs, T_RegisterType rhs) noexcept -> T_RegisterType
{
    using namespace x86;

    static_assert(num_elements<T_RegisterType> >= 3, "Register can't store 3 elements.");

    if constexpr (is_single_precision<T_RegisterType>)
    {
        // source: http://threadlocalmutex.com/?p=8
        T_RegisterType lhs_yzx = permute<1, 2, 0, 3>(lhs);
        T_RegisterType rhs_yzx = permute<1, 2, 0, 3>(rhs);

        T_RegisterType tmp = mm_fmsub(lhs, rhs_yzx, mm_mul(lhs_yzx, rhs));

        return permute<1, 2, 0, 3>(tmp);
    }
    else
    {
        // Due to the latency (3) and throughput (1) of cross lane permutations it should be cheaper to do 4 subsequent
        // permutation before the fmsub than 2 before and 1 after.
        // todo: benchmark
        T_RegisterType lhs_yzx = permute_across_lanes<1, 2, 0, 3>(lhs);
        T_RegisterType lhs_zxy = permute_across_lanes<2, 0, 1, 3>(lhs);
        T_RegisterType rhs_yzx = permute_across_lanes<1, 2, 0, 3>(rhs);
        T_RegisterType rhs_zxy = permute_across_lanes<2, 0, 1, 3>(rhs);

        return mm_fmsub(lhs_yzx, rhs_zxy, mm_mul(lhs_zxy, rhs_yzx));
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_size, Number T_Type>
[[nodiscard]] constexpr auto dot_product(const std::array<T_Type, t_size>& lhs,
                                         const std::array<T_Type, t_size>& rhs) noexcept -> T_Type
{
    T_Type result = 0;
    for (UST i = 0; i < t_size; ++i)
        result += lhs[i] * rhs[i]; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

    return result;
}


// --------------------------------------------------------------------------------------------------------------------


template <UST t_size, x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto dot_product(T_RegisterType lhs, T_RegisterType rhs) noexcept
        -> x86::ElementType<T_RegisterType>
{
    using namespace x86;

    auto product = mm_mul(lhs, rhs);
    return element_sum_first_n<t_size>(product);
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
[[nodiscard]] constexpr auto scalar_triple_product(const std::array<T_Type, 3>& a,
                                                   const std::array<T_Type, 3>& b,
                                                   const std::array<T_Type, 3>& c) noexcept -> T_Type
{
    return dot_product(a, cross_product(b, c));
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto scalar_triple_product(T_RegisterType a, T_RegisterType b, T_RegisterType c) noexcept
        -> x86::ElementType<T_RegisterType>
{
    using namespace x86;
    if constexpr (is_single_precision<T_RegisterType>)
        return dot_product<3>(a, cross_product(b, c));
    else
    {
        // saves one permutation across lanes
        T_RegisterType a_zxy = permute_across_lanes<2, 0, 1, 3>(a);
        T_RegisterType b_yzx = permute_across_lanes<1, 2, 0, 3>(b);
        T_RegisterType c_yzx = permute_across_lanes<1, 2, 0, 3>(c);

        T_RegisterType tmp = mm_fmsub(b, c_yzx, mm_mul(b_yzx, c));

        return dot_product<3>(a_zxy, tmp);
    }
}

} // namespace mjolnir
