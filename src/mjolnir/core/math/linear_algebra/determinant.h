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
#include "mjolnir/core/x86/sign_manipulation.h"

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
// NOLINTNEXTLINE(readability-magic-numbers)
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


//! @brief
//! Calculate the determinant of a 4x4 matrix.
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
// NOLINTNEXTLINE(readability-magic-numbers)
[[nodiscard]] constexpr auto determinant_4x4(const std::array<T_Type, 16>& mat) noexcept -> T_Type;


//! @brief
//! Calculate the determinant of a 4x4 matrix.
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
[[nodiscard]] inline auto determinant_4x4(const std::array<T_RegisterType, 4>& mat) noexcept
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
// NOLINTNEXTLINE(readability-magic-numbers)
[[nodiscard]] constexpr auto determinant_3x3(const std::array<T_Type, 9>& mat) noexcept -> T_Type
{
    return scalar_triple_product<T_Type>({{mat[0], mat[1], mat[2]}},
                                         {{mat[3], mat[4], mat[5]}},  // NOLINT(readability-magic-numbers)
                                         {{mat[6], mat[7], mat[8]}}); // NOLINT(readability-magic-numbers)
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto determinant_3x3(const std::array<T_RegisterType, 3>& mat) noexcept
        -> x86::ElementType<T_RegisterType>
{
    return scalar_triple_product(mat[0], mat[1], mat[2]);
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
// NOLINTNEXTLINE(readability-magic-numbers)
[[nodiscard]] constexpr auto determinant_4x4(const std::array<T_Type, 16>& mat) noexcept -> T_Type
{
    // todo copy documentation from old repo

    T_Type ab_01 = (mat[0] * mat[5] - mat[1] * mat[4]); // NOLINT(readability-magic-numbers)
    T_Type ab_12 = (mat[1] * mat[6] - mat[2] * mat[5]); // NOLINT(readability-magic-numbers)
    T_Type ab_23 = (mat[2] * mat[7] - mat[3] * mat[6]); // NOLINT(readability-magic-numbers)
    T_Type ab_30 = (mat[3] * mat[4] - mat[0] * mat[7]); // NOLINT(readability-magic-numbers)
    T_Type ab_02 = (mat[0] * mat[6] - mat[2] * mat[4]); // NOLINT(readability-magic-numbers)
    T_Type ab_13 = (mat[1] * mat[7] - mat[3] * mat[5]); // NOLINT(readability-magic-numbers)

    T_Type cd_01 = (mat[8] * mat[13] - mat[9] * mat[12]);   // NOLINT(readability-magic-numbers)
    T_Type cd_12 = (mat[9] * mat[14] - mat[10] * mat[13]);  // NOLINT(readability-magic-numbers)
    T_Type cd_23 = (mat[10] * mat[15] - mat[11] * mat[14]); // NOLINT(readability-magic-numbers)
    T_Type cd_30 = (mat[11] * mat[12] - mat[8] * mat[15]);  // NOLINT(readability-magic-numbers)
    T_Type cd_02 = (mat[8] * mat[14] - mat[10] * mat[12]);  // NOLINT(readability-magic-numbers)
    T_Type cd_13 = (mat[9] * mat[15] - mat[11] * mat[13]);  // NOLINT(readability-magic-numbers)

    return ab_01 * cd_23 - ab_12 * cd_30 + ab_23 * cd_01 - ab_30 * cd_12 - ab_02 * cd_13 - ab_13 * cd_02;
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto determinant_4x4(const std::array<T_RegisterType, 4>& mat) noexcept
        -> x86::ElementType<T_RegisterType>
{
    using namespace x86;
    if constexpr (is_m128<T_RegisterType>)
    {
        // The implementation is based on the formula that expresses the 4x4 determinant by a combination of twelve 2x2
        // determinants

        // reorder values as needed
        auto ac_b0011 = blend<0, 0, 1, 1>(mat[0], mat[2]); // for last 2 terms
        auto ac_b1100 = blend<1, 1, 0, 0>(mat[0], mat[2]);
        auto bd_b0011 = blend<0, 0, 1, 1>(mat[1], mat[3]);
        auto bd_b1100 = blend<1, 1, 0, 0>(mat[1], mat[3]);

        auto ac_b1100_2301 = permute<2, 3, 0, 1>(ac_b1100); // for last 2 terms
        auto bd_b1100_2301 = permute<2, 3, 0, 1>(bd_b1100);
        auto a_1230        = permute<1, 2, 3, 0>(mat[0]); // for first 4 terms
        auto b_1230        = permute<1, 2, 3, 0>(mat[1]);
        auto c_1230        = permute<1, 2, 3, 0>(mat[2]);
        auto d_1230        = permute<1, 2, 3, 0>(mat[3]);

        // calculate all twelve 2x2 determinants
        auto prod_abcd_45 = mm_mul(ac_b1100_2301, bd_b0011); // for last 2 terms
        auto prod_ab_03   = mm_mul(a_1230, mat[1]);          // for first 4 terms
        auto prod_cd_03   = mm_mul(c_1230, mat[3]);

        auto abcd_45 = mm_fmsub(ac_b0011, bd_b1100_2301, prod_abcd_45); // for last 2 terms
        auto ab_03   = mm_fmsub(mat[0], b_1230, prod_ab_03);            // for first 4 terms
        auto cd_03   = mm_fmsub(mat[2], d_1230, prod_cd_03);

        // reorder to multiply correct terms
        auto abcd_45_3210 = permute<3, 2, 1, 0>(abcd_45); // for last 2 terms
        auto cd_03_2301   = permute<2, 3, 0, 1>(cd_03);   // for first 4 terms

        auto cd_03_2301_neg = negate_selected<0, 1, 0, 1>(cd_03_2301);

        // multiply the 2x2 determinants
        auto products_45 = mm_mul(abcd_45, abcd_45_3210);
        auto products_03 = mm_mul(ab_03, cd_03_2301_neg);

        // set redundant elements to zero and and add both products
        products_45 = blend<0, 0, 1, 1>(products_45, mm_setzero<T_RegisterType>());

        auto tmp_sum = mm_add(products_03, products_45);

        // sum up all elements to get the determinant
        return element_sum(tmp_sum);
    }
    if constexpr (is_m256<T_RegisterType>)
    {
        // necessary permutations
        auto a_0 = permute_across_lanes<0, 1, 2, 3, 0, 1, 0, 0>(mat[0]);
        auto a_1 = permute_across_lanes<1, 2, 3, 0, 2, 3, 0, 0>(mat[0]);
        auto b_0 = permute_across_lanes<1, 2, 3, 0, 2, 3, 0, 0>(mat[1]);
        auto b_1 = permute_across_lanes<0, 1, 2, 3, 0, 1, 0, 0>(mat[1]);
        auto c_0 = permute_across_lanes<2, 0, 0, 2, 3, 2, 0, 0>(mat[2]);
        auto c_1 = permute_across_lanes<3, 3, 1, 1, 1, 0, 0, 0>(mat[2]);
        auto d_0 = permute_across_lanes<3, 3, 1, 1, 1, 0, 0, 0>(mat[3]);
        auto d_1 = permute_across_lanes<2, 0, 0, 2, 3, 2, 0, 0>(mat[3]);

        // calculate all twelve 2x2 determinants
        auto prod_ab_0 = mm_mul(a_0, b_0);
        auto prod_cd_0 = mm_mul(c_0, d_0);

        auto prod_ab = mm_fmsub(a_1, b_1, prod_ab_0);
        auto prod_cd = mm_fmsub(c_1, d_1, prod_cd_0);

        // multiply the 2x2 determinants
        auto products = mm_mul(prod_ab, prod_cd);

        // sum up all elements to get the determinant
        return element_sum_first_n<6>(products);
    }
}

} // namespace mjolnir
