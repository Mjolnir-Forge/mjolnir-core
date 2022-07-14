//! @file
//! math/linear_algebra/determinant.h
//!
//! @brief
//! Functions to calculate the determinant of a matrix.


#pragma once

#include "mjolnir/core/definitions.h"
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/math/linear_algebra/vector_products.h"
#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"

#include <array>

// === DECLARATIONS ===================================================================================================

namespace mjolnir
{
//! \addtogroup core_math
//! @{

//! @brief
//! Calculate the determinant of a 2x2 matrix.
//!
//! @tparam T_Type
//! The type of the matrix elements and the returned value
//!
//! @param[in] mat:
//! The matrix data as array in row-major or column-major format
//!
//! @return
//! Determinant of the matrix
template <Number T_Type>
[[nodiscard]] constexpr auto determinant_2x2(const std::array<T_Type, 4>& mat) noexcept -> T_Type;


//! @brief
//! Calculate the determinant of a 2x2 matrix.
//!
//! @tparam T_RegisterType
//! Vector register type that represents a row or column of the matrix
//!
//! @param[in] mat:
//! The matrix data as array in row-major or column-major format. Each register represents a single row/column
//!
//! @return
//! Determinant of the matrix
template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto determinant_2x2(const std::array<T_RegisterType, 2>& mat) noexcept
        -> x86::ElementType<T_RegisterType>;


//! @brief
//! Calculate the determinant of a 3x3 matrix.
//!
//! @tparam T_Type
//! The type of the matrix elements and the returned value
//!
//! @param[in] mat:
//! The matrix data as array in row-major or column-major format
//!
//! @return
//! Determinant of the matrix
template <Number T_Type>
[[nodiscard]] constexpr auto determinant_3x3(const std::array<T_Type, 9>& mat) noexcept -> T_Type;


//! @brief
//! Calculate the determinant of a 3x3 matrix.
//!
//! @tparam T_RegisterType
//! Vector register type that represents a row or column of the matrix
//!
//! @param[in] mat:
//! The matrix data as array in row-major or column-major format. Each register represents a single row/column
//!
//! @return
//! Determinant of the matrix
template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto determinant_3x3(const std::array<T_RegisterType, 3>& mat) noexcept
        -> x86::ElementType<T_RegisterType>;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
[[nodiscard]] constexpr auto determinant_2x2(const std::array<T_Type, 4>& mat) noexcept -> T_Type
{
    return mat[3] * mat[0] - mat[1] * mat[2];
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto determinant_2x2(const std::array<T_RegisterType, 2>& mat) noexcept
        -> x86::ElementType<T_RegisterType>
{
    using namespace x86;

    if constexpr (is_double_precision<T_RegisterType>)
    {
        auto products = mm_mul(mat[0], permute<1, 0>(mat[1]));
        auto result   = mm_sub(products, permute<1, 0>(products));

        return mm_cvt_float(result);
    }
    else
    {
        auto products = mm_mul(mat[0], permute<1, 0, 3, 2>(mat[1]));
        auto result   = mm_sub(products, permute<1, 0, 3, 2>(products));

        return mm_cvt_float(result);
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
[[nodiscard]] constexpr auto determinant_3x3(const std::array<T_Type, 9>& mat) noexcept -> T_Type
{
    return scalar_triple_product<T_Type>(
            {{mat[0], mat[1], mat[2]}}, {{mat[3], mat[4], mat[5]}}, {{mat[6], mat[7], mat[8]}});
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto determinant_3x3(const std::array<T_RegisterType, 3>& mat) noexcept
        -> x86::ElementType<T_RegisterType>
{
    return scalar_triple_product(mat[0], mat[1], mat[2]);
}


} // namespace mjolnir
