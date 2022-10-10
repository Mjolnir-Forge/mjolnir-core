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
    // NOLINTNEXTLINE(readability-magic-numbers,-warnings-as-errors)
    [[nodiscard]] static constexpr auto solve_3x3(const std::array<T_Type, 9>& mat,
                                                  const std::array<T_Type, 3>& rhs) noexcept -> std::array<T_Type, 3>;


    //! Solver implementation for 3x3 systems (vectorized).
    template <x86::FloatVectorRegister T_RegisterType>
    [[nodiscard]] static auto solve_3x3(const std::array<T_RegisterType, 3>& mat, T_RegisterType rhs) noexcept
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
    if constexpr (t_size == 2)
        return solve_2x2(mat, rhs);
    else if constexpr (t_size == 3)
        return solve_3x3(mat, rhs);
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType, UST t_size>
[[nodiscard]] auto Cramer::solve(const std::array<T_RegisterType, t_size>& mat, T_RegisterType rhs) noexcept
        -> T_RegisterType
{
    if constexpr (t_size == 2)
        return solve_2x2(mat, rhs);
    else if constexpr (t_size == 3)
        return solve_3x3(mat, rhs);
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
// NOLINTNEXTLINE(readability-magic-numbers,-warnings-as-errors)
[[nodiscard]] constexpr auto Cramer::solve_3x3(const std::array<T_Type, 9>& mat,
                                               const std::array<T_Type, 3>& rhs) noexcept -> std::array<T_Type, 3>
{
    T_Type det_mat = determinant_3x3(mat);

    // NOLINTNEXTLINE(readability-magic-numbers,-warnings-as-errors)
    auto r_0 = std::array<T_Type, 9>{rhs[0], rhs[1], rhs[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8]};
    // NOLINTNEXTLINE(readability-magic-numbers,-warnings-as-errors)
    auto r_1 = std::array<T_Type, 9>{mat[0], mat[1], mat[2], rhs[0], rhs[1], rhs[2], mat[6], mat[7], mat[8]};
    // NOLINTNEXTLINE(readability-magic-numbers,-warnings-as-errors)
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
    static_assert(num_elements<T_RegisterType> > 2, "Registers with 2 elements are not supported.");


    // create all necessary permutations
    auto r_yzx = permute<1, 2, 0, 3>(rhs);
    auto c_yzx = permute<1, 2, 0, 3>(mat[2]);
    auto b_yzx = permute<1, 2, 0, 3>(mat[1]);
    auto a_yzx = permute<1, 2, 0, 3>(mat[0]);

    auto r_zxy = permute<2, 0, 1, 3>(rhs);
    auto a_zxy = permute<2, 0, 1, 3>(mat[0]);

    auto a_ryz = blend_at<0>(mat[0], rhs);
    auto a_rzx = blend_at<0>(a_yzx, r_yzx);
    auto a_rxy = blend_at<0>(a_zxy, r_zxy);


    // calculate all necessary cross product components
    auto prod_bc = mm_mul(b_yzx, mat[2]);
    auto prod_rc = mm_mul(r_yzx, mat[2]);
    auto prod_br = mm_mul(b_yzx, rhs);

    auto cross_bc = mm_fmsub(mat[1], c_yzx, prod_bc);
    auto cross_rc = mm_fmsub(rhs, c_yzx, prod_rc);
    auto cross_br = mm_fmsub(mat[1], r_yzx, prod_br);


    // shuffle all cross product terms as needed
    auto tmp_shf_cross_0 = shuffle<1, 2, 2, 0>(cross_bc, cross_rc);
    auto tmp_shf_cross_1 = shuffle<0, 0, 1, 0>(cross_bc, cross_rc);

    auto shf_cross_0 = shuffle<0, 2, 0, 0>(tmp_shf_cross_0, cross_br);
    auto shf_cross_1 = shuffle<1, 3, 1, 0>(tmp_shf_cross_0, cross_br);
    auto shf_cross_2 = shuffle<0, 2, 2, 0>(tmp_shf_cross_1, cross_br);


    // calculate all necessary determinants
    auto sum_0_m = mm_mul(mat[0], shf_cross_0);
    auto sum_0_r = mm_mul(a_ryz, shf_cross_0);
    auto sum_1_m = mm_fmadd(a_yzx, shf_cross_1, sum_0_m);
    auto sum_1_r = mm_fmadd(a_rzx, shf_cross_1, sum_0_r);
    auto dets_m  = mm_fmadd(a_zxy, shf_cross_2, sum_1_m);
    auto dets_r  = mm_fmadd(a_rxy, shf_cross_2, sum_1_r);


    // calculate solution of the system
    auto det_mat = broadcast<0>(dets_m);
    return mm_div(dets_r, det_mat);
}


// --- specialization -------------------------------------

template <>
[[nodiscard]] auto Cramer::solve_3x3(const std::array<__m256d, 3>& mat, __m256d rhs) noexcept -> __m256d
{
    using namespace x86;

    // create all necessary permutations
    auto r_yzx = permute_across_lanes<1, 2, 0, 3>(rhs);
    auto c_yzx = permute_across_lanes<1, 2, 0, 3>(mat[2]);
    auto b_yzx = permute_across_lanes<1, 2, 0, 3>(mat[1]);
    auto a_yzx = permute_across_lanes<1, 2, 0, 3>(mat[0]);

    auto r_zxy = permute_across_lanes<2, 0, 1, 3>(rhs);
    auto a_zxy = permute_across_lanes<2, 0, 1, 3>(mat[0]);

    auto a_ryz = blend_at<0>(mat[0], rhs);
    auto a_rzx = blend_at<0>(a_yzx, r_yzx);
    auto a_rxy = blend_at<0>(a_zxy, r_zxy);


    // calculate all necessary cross product components
    auto prod_bc = mm_mul(b_yzx, mat[2]);
    auto prod_rc = mm_mul(r_yzx, mat[2]);
    auto prod_br = mm_mul(b_yzx, rhs);

    auto cross_bc = mm_fmsub(mat[1], c_yzx, prod_bc);
    auto cross_rc = mm_fmsub(rhs, c_yzx, prod_rc);
    auto cross_br = mm_fmsub(mat[1], r_yzx, prod_br);


    // shuffle all cross product terms as needed
    auto tmp_blend_cross_0 = blend_at<1>(cross_bc, cross_rc);
    auto tmp_blend_cross_1 = blend_at<1>(cross_rc, cross_br);
    auto tmp_blend_cross_2 = blend_at<1>(cross_br, cross_bc);

    auto perm_cross_0     = blend_at<2>(tmp_blend_cross_0, cross_br);
    auto tmp_perm_cross_1 = blend_at<2>(tmp_blend_cross_1, cross_bc);
    auto tmp_perm_cross_2 = blend_at<2>(tmp_blend_cross_2, cross_rc);

    auto perm_cross_1 = permute_across_lanes<2, 0, 1, 0>(tmp_perm_cross_1);
    auto perm_cross_2 = permute_across_lanes<1, 2, 0, 0>(tmp_perm_cross_2);


    // calculate all necessary determinants
    auto sum_0_m = mm_mul(a_zxy, perm_cross_0);
    auto sum_0_r = mm_mul(a_rxy, perm_cross_0);
    auto sum_1_m = mm_fmadd(a_yzx, perm_cross_1, sum_0_m);
    auto sum_1_r = mm_fmadd(a_rzx, perm_cross_1, sum_0_r);
    auto dets_m  = mm_fmadd(mat[0], perm_cross_2, sum_1_m);
    auto dets_r  = mm_fmadd(a_ryz, perm_cross_2, sum_1_r);


    // calculate solution of the system
    auto det_mat = broadcast_across_lanes<0>(dets_m);
    return mm_div(dets_r, det_mat);
}

// --------------------------------------------------------------------------------------------------------------------

} // namespace mjolnir
