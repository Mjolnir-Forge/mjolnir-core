//! @file
//! math/linear_algebra/solver/cramer.h
//!
//! @brief
//! Solvers based on cramers rule for dense matrices of different size.


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/definitions.h"
#include "mjolnir/core/exception.h"
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/math/linear_algebra/determinant.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"

#include <array>


namespace mjolnir
{
//! \addtogroup core_math
//! @{


//! @brief
//! Provides multiple methods to solve linear systems using Cramer's Rule.
class Cramer
{
public:
    //! @brief
    //! Solve a linear system of equations.
    //!
    //! @tparam T_Type:
    //! The basic type of a matrix/vector element
    //! @tparam t_size:
    //! Size of the system
    //!
    //! @param[in] mat:
    //! The matrix of the system of equations. The data has to be provided in column major format
    //! @param[in] rhs:
    //! The right-hand side vector of the system
    //!
    //! @return
    //! Solution vector
    template <Number T_Type, UST t_size>
    [[nodiscard]] static constexpr auto solve(const std::array<T_Type, t_size * t_size>& mat,
                                              const std::array<T_Type, t_size>&          rhs) noexcept
            -> std::array<T_Type, t_size>;


    //! @brief
    //! Solve a linear system of equations.
    //!
    //! @tparam T_RegisterType:
    //! The register type that stores the columns of a matrix/vector
    //! @tparam t_size
    //! Size of the system
    //!
    //! @param[in] mat:
    //! The matrix of the system of equations. The data has to be provided in column major format
    //! @param[in] rhs:
    //! The right-hand side vector of the system
    //!
    //! @return
    //! Solution vector
    template <x86::FloatVectorRegister T_RegisterType, UST t_size>
    [[nodiscard]] static auto solve(const std::array<T_RegisterType, t_size>& mat, T_RegisterType rhs) noexcept
            -> T_RegisterType;


private:
    //! Solver implementation for 2x2 systems (not vectorized).
    template <Number T_Type>
    [[nodiscard]] static constexpr auto solve_2x2(const std::array<T_Type, 4>& mat,
                                                  const std::array<T_Type, 2>& rhs) noexcept -> std::array<T_Type, 2>;


    //! Solver implementation for 2x2 systems (vectorized).
    template <x86::FloatVectorRegister T_RegisterType>
    [[nodiscard]] static auto solve_2x2(const std::array<T_RegisterType, 2>& mat, T_RegisterType rhs) noexcept
            -> T_RegisterType;


    //! Solver implementation for 3x3 systems (not vectorized).
    template <Number T_Type>
    // NOLINTNEXTLINE(readability-magic-numbers)
    [[nodiscard]] static constexpr auto solve_3x3(const std::array<T_Type, 9>& mat,
                                                  const std::array<T_Type, 3>& rhs) noexcept -> std::array<T_Type, 3>;


    //! Solver implementation for 3x3 systems (vectorized).
    template <x86::FloatVectorRegister T_RegisterType>
    [[nodiscard]] static auto solve_3x3(const std::array<T_RegisterType, 3>& mat, T_RegisterType rhs) noexcept
            -> T_RegisterType;


    //! Solver implementation for 4x4 systems (not vectorized).
    template <Number T_Type>
    // NOLINTNEXTLINE(readability-magic-numbers)
    [[nodiscard]] static constexpr auto solve_4x4(const std::array<T_Type, 16>& mat,
                                                  const std::array<T_Type, 4>&  rhs) noexcept -> std::array<T_Type, 4>;


    //! Solver implementation for 3x3 systems (vectorized).
    template <x86::FloatVectorRegister T_RegisterType>
    [[nodiscard]] static auto solve_4x4(const std::array<T_RegisterType, 4>& mat, T_RegisterType rhs) noexcept
            -> T_RegisterType;
};

//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================

namespace mjolnir
{
template <Number T_Type, UST t_size>
[[nodiscard]] constexpr auto Cramer::solve(const std::array<T_Type, t_size * t_size>& mat,
                                           const std::array<T_Type, t_size>& rhs) noexcept -> std::array<T_Type, t_size>
{
    static_assert(4 >= t_size && 1 < t_size, "Only sizes 2-4 are supported.");

    if constexpr (t_size == 2)
        return solve_2x2(mat, rhs);
    else if constexpr (t_size == 3)
        return solve_3x3(mat, rhs);
    else
        return solve_4x4(mat, rhs);
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType, UST t_size>
[[nodiscard]] auto Cramer::solve(const std::array<T_RegisterType, t_size>& mat, T_RegisterType rhs) noexcept
        -> T_RegisterType
{
    using namespace x86;
    static_assert(4 >= t_size && 1 < t_size, "Only sizes 2-4 are supported.");
    static_assert(num_elements<T_RegisterType> >= t_size, "Registers size must be equal or larger than system size.");

    if constexpr (t_size == 2)
        return solve_2x2(mat, rhs);
    else if constexpr (t_size == 3)
        return solve_3x3(mat, rhs);
    else
        return solve_4x4(mat, rhs);
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
[[nodiscard]] constexpr auto Cramer::solve_2x2(const std::array<T_Type, 4>& mat,
                                               const std::array<T_Type, 2>& rhs) noexcept -> std::array<T_Type, 2>
{
    T_Type det_mat = determinant_2x2(mat);

    auto r_0 = std::array<T_Type, 4>{rhs[0], rhs[1], mat[2], mat[3]};
    auto r_1 = std::array<T_Type, 4>{mat[0], mat[1], rhs[0], rhs[1]};

    T_Type x_0 = determinant_2x2(r_0) / det_mat;
    T_Type x_1 = determinant_2x2(r_1) / det_mat;

    return std::array<T_Type, 2>{x_0, x_1};
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] auto Cramer::solve_2x2(const std::array<T_RegisterType, 2>& mat, T_RegisterType rhs) noexcept
        -> T_RegisterType
{
    // Several implementations have been tested and benchmarked. This one was the fastest. Note that the throughput of
    // `mm_mul` and `mm_fmsub` is 0.5. Therefore, reducing those instructions to 1 instead of 2 doesn't yield any
    // performance boost. The required permutations for this actually cause the code to be slower.

    using namespace x86;

    auto b0a1  = blend_at<0>(mat[0], mat[1]);
    auto a0b1  = blend_at<1>(mat[0], mat[1]);
    auto b1a0  = swap<1, 0>(a0b1);
    auto a1b0  = swap<1, 0>(b0a1);
    auto p_rhs = swap<1, 0>(rhs);

    auto prod_mat = mm_mul(a1b0, b0a1);
    auto prod_rhs = mm_mul(p_rhs, b0a1);

    auto det_mat = mm_fmsub(a0b1, b1a0, prod_mat);
    auto det_rhs = mm_fmsub(rhs, b1a0, prod_rhs);

    return mm_div(det_rhs, det_mat);
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
// NOLINTNEXTLINE(readability-magic-numbers)
[[nodiscard]] constexpr auto Cramer::solve_3x3(const std::array<T_Type, 9>& mat,
                                               const std::array<T_Type, 3>& rhs) noexcept -> std::array<T_Type, 3>
{
    T_Type det_mat = determinant_3x3(mat);

    // NOLINTNEXTLINE(readability-magic-numbers)
    auto r_0 = std::array<T_Type, 9>{rhs[0], rhs[1], rhs[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8]};
    // NOLINTNEXTLINE(readability-magic-numbers)
    auto r_1 = std::array<T_Type, 9>{mat[0], mat[1], mat[2], rhs[0], rhs[1], rhs[2], mat[6], mat[7], mat[8]};
    // NOLINTNEXTLINE(readability-magic-numbers)
    auto r_2 = std::array<T_Type, 9>{mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], rhs[0], rhs[1], rhs[2]};

    T_Type x_0 = determinant_3x3(r_0) / det_mat;
    T_Type x_1 = determinant_3x3(r_1) / det_mat;
    T_Type x_2 = determinant_3x3(r_2) / det_mat;

    return std::array<T_Type, 3>{x_0, x_1, x_2};
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] auto Cramer::solve_3x3(const std::array<T_RegisterType, 3>& mat, T_RegisterType rhs) noexcept
        -> T_RegisterType
{
    using namespace x86;

    // create all necessary permutations
    auto r_120 = permute<1, 2, 0, 3>(rhs);
    auto c_120 = permute<1, 2, 0, 3>(mat[2]);
    auto b_120 = permute<1, 2, 0, 3>(mat[1]);
    auto a_120 = permute<1, 2, 0, 3>(mat[0]);

    auto r_201 = permute<2, 0, 1, 3>(rhs);
    auto a_201 = permute<2, 0, 1, 3>(mat[0]);

    auto a_r12 = blend_at<0>(mat[0], rhs);
    auto a_r20 = blend_at<0>(a_120, r_120);
    auto a_r01 = blend_at<0>(a_201, r_201);


    // calculate all necessary cross product components
    auto prod_bc = mm_mul(b_120, mat[2]);
    auto prod_rc = mm_mul(r_120, mat[2]);
    auto prod_br = mm_mul(b_120, rhs);

    auto cross_bc_201 = mm_fmsub(mat[1], c_120, prod_bc);
    auto cross_rc_201 = mm_fmsub(rhs, c_120, prod_rc);
    auto cross_br_201 = mm_fmsub(mat[1], r_120, prod_br);


    // shuffle all cross product terms as needed
    auto tmp_shf_cross_0 = shuffle<1, 2, 2, 0>(cross_bc_201, cross_rc_201);
    auto tmp_shf_cross_1 = shuffle<0, 0, 1, 0>(cross_bc_201, cross_rc_201);

    auto terms_012 = shuffle<0, 2, 0, 0>(tmp_shf_cross_0, cross_br_201);
    auto terms_120 = shuffle<1, 3, 1, 0>(tmp_shf_cross_0, cross_br_201);
    auto terms_201 = shuffle<0, 2, 2, 0>(tmp_shf_cross_1, cross_br_201);


    // calculate all necessary determinants
    auto sum_0_m = mm_mul(mat[0], terms_012);
    auto sum_0_r = mm_mul(a_r12, terms_012);
    auto sum_1_m = mm_fmadd(a_120, terms_120, sum_0_m);
    auto sum_1_r = mm_fmadd(a_r20, terms_120, sum_0_r);
    auto dets_m  = mm_fmadd(a_201, terms_201, sum_1_m);
    auto dets_r  = mm_fmadd(a_r01, terms_201, sum_1_r);


    // calculate solution of the system
    auto det_mat = broadcast<0>(dets_m);
    return mm_div(dets_r, det_mat);
}


// --- specialization -------------------------------------

//! \cond DO_NOT_DOCUMENT

template <>
[[nodiscard]] inline auto Cramer::solve_3x3(const std::array<__m256d, 3>& mat, __m256d rhs) noexcept -> __m256d
{
    using namespace x86;

    // create all necessary permutations
    auto r_120 = permute_across_lanes<1, 2, 0, 3>(rhs);
    auto c_120 = permute_across_lanes<1, 2, 0, 3>(mat[2]);
    auto b_120 = permute_across_lanes<1, 2, 0, 3>(mat[1]);
    auto a_120 = permute_across_lanes<1, 2, 0, 3>(mat[0]);

    auto r_201 = permute_across_lanes<2, 0, 1, 3>(rhs);
    auto a_201 = permute_across_lanes<2, 0, 1, 3>(mat[0]);

    auto a_r12 = blend_at<0>(mat[0], rhs);
    auto a_r20 = blend_at<0>(a_120, r_120);
    auto a_r01 = blend_at<0>(a_201, r_201);


    // calculate all necessary cross product components
    auto prod_bc = mm_mul(b_120, mat[2]);
    auto prod_rc = mm_mul(r_120, mat[2]);
    auto prod_br = mm_mul(b_120, rhs);

    auto cross_bc_201 = mm_fmsub(mat[1], c_120, prod_bc);
    auto cross_rc_201 = mm_fmsub(rhs, c_120, prod_rc);
    auto cross_br_201 = mm_fmsub(mat[1], r_120, prod_br);


    // shuffle all cross product terms as needed
    auto tmp_blend_cross_0 = blend_at<1>(cross_bc_201, cross_rc_201);
    auto tmp_blend_cross_1 = blend_at<1>(cross_rc_201, cross_br_201);
    auto tmp_blend_cross_2 = blend_at<1>(cross_br_201, cross_bc_201);

    auto terms_201        = blend_at<2>(tmp_blend_cross_0, cross_br_201);
    auto tmp_perm_cross_1 = blend_at<2>(tmp_blend_cross_1, cross_bc_201);
    auto tmp_perm_cross_2 = blend_at<2>(tmp_blend_cross_2, cross_rc_201);

    auto terms_120 = permute_across_lanes<2, 0, 1, 0>(tmp_perm_cross_1);
    auto terms_012 = permute_across_lanes<1, 2, 0, 0>(tmp_perm_cross_2);


    // calculate all necessary determinants
    auto sum_0_m = mm_mul(a_201, terms_201);
    auto sum_0_r = mm_mul(a_r01, terms_201);
    auto sum_1_m = mm_fmadd(a_120, terms_120, sum_0_m);
    auto sum_1_r = mm_fmadd(a_r20, terms_120, sum_0_r);
    auto dets_m  = mm_fmadd(mat[0], terms_012, sum_1_m);
    auto dets_r  = mm_fmadd(a_r12, terms_012, sum_1_r);


    // calculate solution of the system
    auto det_mat = broadcast_across_lanes<0>(dets_m);
    return mm_div(dets_r, det_mat);
}

//! \endcond


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type>
// NOLINTNEXTLINE(readability-magic-numbers)
[[nodiscard]] constexpr auto Cramer::solve_4x4(const std::array<T_Type, 16>& mat,
                                               const std::array<T_Type, 4>&  rhs) noexcept -> std::array<T_Type, 4>
{
    // This implementation is extremely slow if `T_Type` is `F64`. Writing out all equations instead of using the
    // determinant functions didn't improve the performance. The time consumption factor of approximately 100 (when
    // compared to the F32 version) is probably caused by cache misses.

    T_Type det_mat = determinant_4x4(mat);

    // clang-format off
    auto r_0 = std::array<T_Type, 16>{                  // NOLINT(readability-magic-numbers)
                rhs[0],  rhs[1],  rhs[2],  rhs[3],
                mat[4],  mat[5],  mat[6],  mat[7],      // NOLINT(readability-magic-numbers)
                mat[8],  mat[9],  mat[10], mat[11],     // NOLINT(readability-magic-numbers)
                mat[12], mat[13], mat[14], mat[15]};    // NOLINT(readability-magic-numbers)
    auto r_1 = std::array<T_Type, 16>{                  // NOLINT(readability-magic-numbers)
                mat[0],  mat[1],  mat[2],  mat[3],
                rhs[0],  rhs[1],  rhs[2],  rhs[3],
                mat[8],  mat[9],  mat[10], mat[11],     // NOLINT(readability-magic-numbers)
                mat[12], mat[13], mat[14], mat[15]};    // NOLINT(readability-magic-numbers)
    auto r_2 = std::array<T_Type, 16>{                  // NOLINT(readability-magic-numbers)
                mat[0],  mat[1],  mat[2],  mat[3],
                mat[4],  mat[5],  mat[6],  mat[7],      // NOLINT(readability-magic-numbers)
                rhs[0],  rhs[1],  rhs[2],  rhs[3],
                mat[12], mat[13], mat[14], mat[15]};    // NOLINT(readability-magic-numbers)
    auto r_3 = std::array<T_Type, 16>{                  // NOLINT(readability-magic-numbers)
                mat[0],  mat[1],  mat[2],  mat[3],
                mat[4],  mat[5],  mat[6],  mat[7],      // NOLINT(readability-magic-numbers)
                mat[8],  mat[9],  mat[10], mat[11],     // NOLINT(readability-magic-numbers)
                rhs[0],  rhs[1],  rhs[2],  rhs[3]};
    // clang-format on

    T_Type x_0 = determinant_4x4(r_0) / det_mat;
    T_Type x_1 = determinant_4x4(r_1) / det_mat;
    T_Type x_2 = determinant_4x4(r_2) / det_mat;
    T_Type x_3 = determinant_4x4(r_3) / det_mat;

    return std::array<T_Type, 4>{x_0, x_1, x_2, x_3};
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] auto Cramer::solve_4x4(const std::array<T_RegisterType, 4>& mat, T_RegisterType rhs) noexcept
        -> T_RegisterType
{
    using namespace x86;

    // todo: check if calculation of last 2 terms of 2 determinants can be merged

    // reorder values as needed
    auto ac_b0011 = blend<0, 0, 1, 1>(mat[0], mat[2]); // for last 2 terms
    auto ac_b1100 = blend<1, 1, 0, 0>(mat[0], mat[2]);
    auto rc_b0011 = blend<0, 0, 1, 1>(rhs, mat[2]);
    auto rc_b1100 = blend<1, 1, 0, 0>(rhs, mat[2]);
    auto ar_b0011 = blend<0, 0, 1, 1>(mat[0], rhs);
    auto ar_b1100 = blend<1, 1, 0, 0>(mat[0], rhs);
    auto bd_b0011 = blend<0, 0, 1, 1>(mat[1], mat[3]);
    auto bd_b1100 = blend<1, 1, 0, 0>(mat[1], mat[3]);
    auto rd_b0011 = blend<0, 0, 1, 1>(rhs, mat[3]);
    auto rd_b1100 = blend<1, 1, 0, 0>(rhs, mat[3]);
    auto br_b0011 = blend<0, 0, 1, 1>(mat[1], rhs);
    auto br_b1100 = blend<1, 1, 0, 0>(mat[1], rhs);

    auto ac_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(ac_b1100); // for last 2 terms
    auto rc_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(rc_b1100);
    auto ar_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(ar_b1100);
    auto bd_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(bd_b1100);
    auto rd_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(rd_b1100);
    auto br_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(br_b1100);
    auto a_1230        = permute_across_lanes<1, 2, 3, 0>(mat[0]); // for first 4 terms
    auto b_1230        = permute_across_lanes<1, 2, 3, 0>(mat[1]);
    auto c_1230        = permute_across_lanes<1, 2, 3, 0>(mat[2]);
    auto d_1230        = permute_across_lanes<1, 2, 3, 0>(mat[3]);
    auto r_1230        = permute_across_lanes<1, 2, 3, 0>(rhs);

    // calculate all twelve 2x2 determinants
    auto prod_abcd_45 = mm_mul(ac_b1100_2301, bd_b0011); // for last 2 terms
    auto prod_rbcd_45 = mm_mul(rc_b1100_2301, bd_b0011);
    auto prod_arcd_45 = mm_mul(ac_b1100_2301, rd_b0011);
    auto prod_abrd_45 = mm_mul(ar_b1100_2301, bd_b0011);
    auto prod_abcr_45 = mm_mul(ac_b1100_2301, br_b0011);
    auto prod_ab_03   = mm_mul(a_1230, mat[1]); // for first 4 terms
    auto prod_rb_03   = mm_mul(r_1230, mat[1]);
    auto prod_ar_03   = mm_mul(a_1230, rhs);
    auto prod_cd_03   = mm_mul(c_1230, mat[3]);
    auto prod_rd_03   = mm_mul(r_1230, mat[3]);
    auto prod_cr_03   = mm_mul(c_1230, rhs);

    auto abcd_45 = mm_fmsub(ac_b0011, bd_b1100_2301, prod_abcd_45); // for last 2 terms
    auto rbcd_45 = mm_fmsub(rc_b0011, bd_b1100_2301, prod_rbcd_45);
    auto arcd_45 = mm_fmsub(ac_b0011, rd_b1100_2301, prod_arcd_45);
    auto abrd_45 = mm_fmsub(ar_b0011, bd_b1100_2301, prod_abrd_45);
    auto abcr_45 = mm_fmsub(ac_b0011, br_b1100_2301, prod_abcr_45);
    auto ab_03   = mm_fmsub(mat[0], b_1230, prod_ab_03); // for first 4 terms
    auto rb_03   = mm_fmsub(rhs, b_1230, prod_rb_03);
    auto ar_03   = mm_fmsub(mat[0], r_1230, prod_ar_03);
    auto cd_03   = mm_fmsub(mat[2], d_1230, prod_cd_03);
    auto rd_03   = mm_fmsub(rhs, d_1230, prod_rd_03);
    auto cr_03   = mm_fmsub(mat[2], r_1230, prod_cr_03);

    // reorder to multiply correct terms
    auto abcd_45_3210 = permute_across_lanes<3, 2, 1, 0>(abcd_45); // for last 2 terms
    auto rbcd_45_3210 = permute_across_lanes<3, 2, 1, 0>(rbcd_45);
    auto arcd_45_3210 = permute_across_lanes<3, 2, 1, 0>(arcd_45);
    auto abrd_45_3210 = permute_across_lanes<3, 2, 1, 0>(abrd_45);
    auto abcr_45_3210 = permute_across_lanes<3, 2, 1, 0>(abcr_45);
    auto cd_03_2301   = permute_across_lanes<2, 3, 0, 1>(cd_03); // for first 4 terms
    auto rd_03_2301   = permute_across_lanes<2, 3, 0, 1>(rd_03);
    auto cr_03_2301   = permute_across_lanes<2, 3, 0, 1>(cr_03);

    auto cd_03_2301_neg = negate_selected<0, 1, 0, 1>(cd_03_2301);
    auto rd_03_2301_neg = negate_selected<0, 1, 0, 1>(rd_03_2301);
    auto cr_03_2301_neg = negate_selected<0, 1, 0, 1>(cr_03_2301);

    // multiply the 2x2 determinants
    auto products_45_abcd = mm_mul(abcd_45, abcd_45_3210);
    auto products_45_rbcd = mm_mul(rbcd_45, rbcd_45_3210);
    auto products_45_arcd = mm_mul(arcd_45, arcd_45_3210);
    auto products_45_abrd = mm_mul(abrd_45, abrd_45_3210);
    auto products_45_abcr = mm_mul(abcr_45, abcr_45_3210);
    auto products_03_abcd = mm_mul(ab_03, cd_03_2301_neg);
    auto products_03_rbcd = mm_mul(rb_03, cd_03_2301_neg);
    auto products_03_arcd = mm_mul(ar_03, cd_03_2301_neg);
    auto products_03_abrd = mm_mul(ab_03, rd_03_2301_neg);
    auto products_03_abcr = mm_mul(ab_03, cr_03_2301_neg);

    // set redundant elements to zero and and add both products
    products_45_abcd = blend<0, 0, 1, 1>(products_45_abcd, mm_setzero<T_RegisterType>());
    products_45_rbcd = blend<0, 0, 1, 1>(products_45_rbcd, mm_setzero<T_RegisterType>());
    products_45_arcd = blend<0, 0, 1, 1>(products_45_arcd, mm_setzero<T_RegisterType>());
    products_45_abrd = blend<0, 0, 1, 1>(products_45_abrd, mm_setzero<T_RegisterType>());
    products_45_abcr = blend<0, 0, 1, 1>(products_45_abcr, mm_setzero<T_RegisterType>());


    auto tmp_sum_abcd = mm_add(products_03_abcd, products_45_abcd);
    auto tmp_sum_rbcd = mm_add(products_03_rbcd, products_45_rbcd);
    auto tmp_sum_arcd = mm_add(products_03_arcd, products_45_arcd);
    auto tmp_sum_abrd = mm_add(products_03_abrd, products_45_abrd);
    auto tmp_sum_abcr = mm_add(products_03_abcr, products_45_abcr);


    // sum up all elements to get the determinant
    auto det_mat = broadcast_element_sum(tmp_sum_abcd);

    T_RegisterType out_0 = {0};
    T_RegisterType out_1 = {0};
    T_RegisterType out_2 = {0};
    T_RegisterType out_3 = {0};

    // todo: Replace this branch with 4x4 matrix transpositon once it is implemented
    if constexpr (is_m128<T_RegisterType>)
    {
        auto tmp_0 = shuffle<0, 1, 0, 1>(tmp_sum_rbcd, tmp_sum_arcd);
        auto tmp_1 = shuffle<2, 3, 2, 3>(tmp_sum_rbcd, tmp_sum_arcd);
        auto tmp_2 = shuffle<0, 1, 0, 1>(tmp_sum_abrd, tmp_sum_abcr);
        auto tmp_3 = shuffle<2, 3, 2, 3>(tmp_sum_abrd, tmp_sum_abcr);

        out_0 = shuffle<0, 2, 0, 2>(tmp_0, tmp_2);
        out_1 = shuffle<1, 3, 1, 3>(tmp_0, tmp_2);
        out_2 = shuffle<0, 2, 0, 2>(tmp_1, tmp_3);
        out_3 = shuffle<1, 3, 1, 3>(tmp_1, tmp_3);
    }
    else
    {
        auto tmp_0 = shuffle_lanes<0, 1, 1, 0>(tmp_sum_rbcd, tmp_sum_abrd);
        auto tmp_1 = shuffle_lanes<0, 1, 1, 0>(tmp_sum_arcd, tmp_sum_abcr);

        auto tmp_2 = blend_above<1>(tmp_sum_rbcd, tmp_0);
        auto tmp_3 = blend_above<1>(tmp_sum_arcd, tmp_1);
        auto tmp_4 = blend_above<1>(tmp_0, tmp_sum_abrd);
        auto tmp_5 = blend_above<1>(tmp_1, tmp_sum_abcr);

        out_0 = mm_unpacklo(tmp_2, tmp_3);
        out_1 = mm_unpackhi(tmp_2, tmp_3);
        out_2 = mm_unpacklo(tmp_4, tmp_5);
        out_3 = mm_unpackhi(tmp_4, tmp_5);
    }

    auto sum_0 = mm_add(out_0, out_1);
    auto sum_1 = mm_add(out_2, out_3);

    auto dets = mm_add(sum_0, sum_1);

    return mm_div(dets, det_mat);
}


// --- specialization -------------------------------------

//! \cond DO_NOT_DOCUMENT

template <>
[[nodiscard]] auto Cramer::solve_4x4(const std::array<__m256, 4>& mat, __m256 rhs) noexcept -> __m256
{
    using namespace x86;

    // necessary permutations (last 2 elements are unused)
    auto a_0  = permute_across_lanes<0, 1, 2, 3, 0, 1, 0, 0>(mat[0]);
    auto a_1  = permute_across_lanes<1, 2, 3, 0, 2, 3, 0, 0>(mat[0]);
    auto b_0  = permute_across_lanes<1, 2, 3, 0, 2, 3, 0, 0>(mat[1]);
    auto b_1  = permute_across_lanes<0, 1, 2, 3, 0, 1, 0, 0>(mat[1]);
    auto c_0  = permute_across_lanes<2, 0, 0, 2, 3, 2, 0, 0>(mat[2]);
    auto c_1  = permute_across_lanes<3, 3, 1, 1, 1, 0, 0, 0>(mat[2]);
    auto d_0  = permute_across_lanes<3, 3, 1, 1, 1, 0, 0, 0>(mat[3]);
    auto d_1  = permute_across_lanes<2, 0, 0, 2, 3, 2, 0, 0>(mat[3]);
    auto r_00 = permute_across_lanes<0, 1, 2, 3, 0, 1, 0, 0>(rhs);
    auto r_01 = permute_across_lanes<1, 2, 3, 0, 2, 3, 0, 0>(rhs);
    auto r_10 = permute_across_lanes<3, 3, 1, 1, 1, 0, 0, 0>(rhs);
    auto r_11 = permute_across_lanes<2, 0, 0, 2, 3, 2, 0, 0>(rhs);


    // calculate all twelve 2x2 determinants
    auto prod_ab_0 = mm_mul(a_0, b_0);
    auto prod_cd_0 = mm_mul(c_0, d_0);
    auto prod_rb_0 = mm_mul(r_00, b_0);
    auto prod_ar_0 = mm_mul(a_0, r_01);
    auto prod_rd_0 = mm_mul(r_11, d_0);
    auto prod_cr_0 = mm_mul(c_0, r_10);

    auto prod_ab = mm_fmsub(a_1, b_1, prod_ab_0);
    auto prod_cd = mm_fmsub(c_1, d_1, prod_cd_0);
    auto prod_rb = mm_fmsub(r_01, b_1, prod_rb_0);
    auto prod_ar = mm_fmsub(a_1, r_00, prod_ar_0);
    auto prod_rd = mm_fmsub(r_10, d_1, prod_rd_0);
    auto prod_cr = mm_fmsub(c_1, r_11, prod_cr_0);


    // multiply the 2x2 determinants
    auto products_abcd = mm_mul(prod_ab, prod_cd);
    auto products_rbcd = mm_mul(prod_rb, prod_cd);
    auto products_arcd = mm_mul(prod_ar, prod_cd);
    auto products_abrd = mm_mul(prod_ab, prod_rd);
    auto products_abcr = mm_mul(prod_ab, prod_cr);


    // sum up all elements to get the determinant of the unmodified matrix
    products_abcd = blend_above<5>(products_abcd, mm_setzero<__m256>());
    auto det_mat  = broadcast_element_sum(products_abcd); // NOLINT(readability-magic-numbers)


    // todo: replace code below with multi register version of `element_sum` function once it is implemented
    // transpose data to sum everything up
    auto tmp_0 = shuffle<0, 1, 0, 1>(products_rbcd, products_arcd);
    auto tmp_1 = shuffle<2, 3, 2, 3>(products_rbcd, products_arcd);
    auto tmp_2 = shuffle<0, 1, 0, 1>(products_abrd, products_abcr);
    auto tmp_3 = shuffle<2, 3, 2, 3>(products_abrd, products_abcr);

    auto term_0 = shuffle<0, 2, 0, 2>(tmp_0, tmp_2);
    auto term_1 = shuffle<1, 3, 1, 3>(tmp_0, tmp_2);
    auto term_2 = shuffle<0, 2, 0, 2>(tmp_1, tmp_3);
    auto term_3 = shuffle<1, 3, 1, 3>(tmp_1, tmp_3);
    auto term_4 = swap_lanes(term_0);
    auto term_5 = swap_lanes(term_1);


    // calculate determinants
    auto sum_01 = mm_add(term_0, term_1);
    auto sum_23 = mm_add(term_2, term_3);
    auto sum_45 = mm_add(term_4, term_5);

    auto sum_0123 = mm_add(sum_01, sum_23);

    auto dets = mm_add(sum_0123, sum_45);


    // return result
    return mm_div(dets, det_mat);
}


// --------------------------------------------------------------------------------------------------------------------

//! \endcond

} // namespace mjolnir
