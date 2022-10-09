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
    //! Solve a linear system of equations of size 2.
    //!
    //! @tparam T_Type:
    //! The basic type of a matrix/vector element
    //!
    //! @param mat:
    //! The matrix of the system of equations. The data has to be provided in column major format
    //! @param rhs:
    //! The right-hand side vector of the system
    //!
    //! @return
    //! Solution vector
    template <Number T_Type>
    [[nodiscard]] static constexpr auto solve(const std::array<T_Type, 4>& mat,
                                              const std::array<T_Type, 2>& rhs) noexcept -> std::array<T_Type, 2>;


    //! @brief
    //! Solve a linear system of equations of size 2.
    //!
    //! @tparam T_RegisterType:
    //! The register type that stores the columns of a matrix/vector
    //!
    //! @param mat:
    //! The matrix of the system of equations. The data has to be provided in column major format
    //! @param rhs:
    //! The right-hand side vector of the system
    //!
    //! @return
    //! Solution vector
    template <x86::FloatVectorRegister T_RegisterType>
    [[nodiscard]] static auto solve(const std::array<T_RegisterType, 2>& mat, T_RegisterType rhs) noexcept
            -> T_RegisterType;


    //! @brief
    //! Solve a linear system of equations of size 3.
    //!
    //! @tparam T_Type:
    //! The basic type of a matrix/vector element
    //!
    //! @param mat:
    //! The matrix of the system of equations. The data has to be provided in column major format
    //! @param rhs:
    //! The right-hand side vector of the system
    //!
    //! @return
    //! Solution vector
    template <Number T_Type>
    [[nodiscard]] static constexpr auto solve(const std::array<T_Type, 9>& mat,
                                              const std::array<T_Type, 3>& rhs) noexcept -> std::array<T_Type, 3>;


    //! @brief
    //! Solve a linear system of equations of size 3.
    //!
    //! @tparam T_RegisterType:
    //! The register type that stores the columns of a matrix/vector
    //!
    //! @param mat:
    //! The matrix of the system of equations. The data has to be provided in column major format
    //! @param rhs:
    //! The right-hand side vector of the system
    //!
    //! @return
    //! Solution vector
    template <x86::FloatVectorRegister T_RegisterType>
    [[nodiscard]] static auto solve(const std::array<T_RegisterType, 3>& mat, T_RegisterType rhs) noexcept
            -> T_RegisterType;
};

//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================

namespace mjolnir
{
template <Number T_Type>
[[nodiscard]] constexpr auto Cramer::solve(const std::array<T_Type, 4>& mat, const std::array<T_Type, 2>& rhs) noexcept
        -> std::array<T_Type, 2>
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
[[nodiscard]] auto Cramer::solve(const std::array<T_RegisterType, 2>& mat, T_RegisterType rhs) noexcept
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
[[nodiscard]] constexpr auto Cramer::solve(const std::array<T_Type, 9>& mat, const std::array<T_Type, 3>& rhs) noexcept
        -> std::array<T_Type, 3>
{
    T_Type det_mat = determinant_3x3(mat);

    auto r_0 = std::array<T_Type, 9>{rhs[0], rhs[1], rhs[2], mat[3], mat[4], mat[5], mat[6], mat[7], mat[8]};
    auto r_1 = std::array<T_Type, 9>{mat[0], mat[1], mat[2], rhs[0], rhs[1], rhs[2], mat[6], mat[7], mat[8]};
    auto r_2 = std::array<T_Type, 9>{mat[0], mat[1], mat[2], mat[3], mat[4], mat[5], rhs[0], rhs[1], rhs[2]};

    T_Type x_0 = determinant_3x3(r_0) / det_mat;
    T_Type x_1 = determinant_3x3(r_1) / det_mat;
    T_Type x_2 = determinant_3x3(r_2) / det_mat;

    return std::array<T_Type, 3>{x_0, x_1, x_2};
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] auto Cramer::solve(const std::array<T_RegisterType, 3>& mat, T_RegisterType rhs) noexcept
        -> T_RegisterType
{
#ifdef test
    using namespace x86;
    static_assert(num_elements<T_RegisterType> > 2, "Registers with 2 elements are not supported.");

    // calculate all cross products
    auto r_yzx = permute<1, 2, 0, 3>(rhs);
    auto c_yzx = permute<1, 2, 0, 3>(mat[2]);
    auto b_yzx = permute<1, 2, 0, 3>(mat[1]);
    auto a_yzx = permute<1, 2, 0, 3>(mat[0]);

    auto r_zxy = permute<2, 0, 1, 3>(rhs);
    auto a_zxy = permute<2, 0, 1, 3>(mat[0]);

    auto a_ryz = blend_at<0>(mat[0], rhs);
    auto a_rzx = blend_at<0>(a_yzx, r_yzx);
    auto a_rxy = blend_at<0>(a_zxy, r_zxy);

    auto prod_bc = mm_mul(b_yzx, mat[2]);
    auto prod_rc = mm_mul(r_yzx, mat[2]);
    auto prod_br = mm_mul(b_yzx, rhs);

    auto tmp_bc = mm_fmsub(mat[1], c_yzx, prod_bc);
    auto tmp_rc = mm_fmsub(rhs, c_yzx, prod_rc);
    auto tmp_br = mm_fmsub(mat[1], r_yzx, prod_br);


    auto t_0 = shuffle<1, 2, 2, 0>(tmp_bc, tmp_rc);
    auto t_1 = shuffle<0, 0, 1, 0>(tmp_bc, tmp_rc);

    auto g0 = shuffle<0, 2, 0, 0>(t_0, tmp_br);
    auto g1 = shuffle<1, 3, 1, 0>(t_0, tmp_br);
    auto g2 = shuffle<0, 2, 2, 0>(t_1, tmp_br);

    auto tt0    = mm_mul(mat[0], g0);
    auto uu0    = mm_mul(a_ryz, g0);
    auto tt1    = mm_fmadd(a_yzx, g1, tt0);
    auto uu1    = mm_fmadd(a_rzx, g1, uu0);
    auto dets_u = mm_fmadd(a_zxy, g2, tt1);
    auto dets   = mm_fmadd(a_rxy, g2, uu1);


    auto det_mat = broadcast<0>(dets_u);
    return mm_div(dets, det_mat);
#else
    using namespace x86;
    static_assert(num_elements<T_RegisterType> > 2, "Registers with 2 elements are not supported.");

    // calculate all cross products

    auto r_yzx = permute<1, 2, 0, 3>(rhs);
    auto a_yzx = permute<1, 2, 0, 3>(mat[0]);
    auto b_yzx = permute<1, 2, 0, 3>(mat[1]);
    auto c_yzx = permute<1, 2, 0, 3>(mat[2]);

    auto a0 = broadcast<0>(mat[0]);
    auto a1 = broadcast<1>(mat[0]);
    auto a2 = broadcast<2>(mat[0]);
    auto r2 = broadcast<2>(rhs);


    auto prod_bc = mm_mul(b_yzx, mat[2]);
    auto prod_rc = mm_mul(r_yzx, mat[2]);
    auto prod_br = mm_mul(b_yzx, rhs);

    auto tmp_bc = mm_fmsub(mat[1], c_yzx, prod_bc);
    auto tmp_rc = mm_fmsub(rhs, c_yzx, prod_rc);
    auto tmp_br = mm_fmsub(mat[1], r_yzx, prod_br);

    auto l0 = blend_at<0>(a0, rhs);
    auto l1 = blend_at<0>(a1, r_yzx);
    auto l2 = blend_at<0>(a2, r2);

    auto tmp_0 = shuffle<1, 2, 1, 2>(tmp_bc, tmp_rc);
    auto tmp_1 = shuffle<0, 0, 0, 0>(tmp_bc, tmp_rc);

    auto h0 = shuffle<0, 2, 1, 0>(tmp_0, tmp_br);
    auto h1 = shuffle<1, 3, 2, 0>(tmp_0, tmp_br);
    auto h2 = shuffle<0, 2, 0, 0>(tmp_1, tmp_br);

    auto t0     = mm_mul(l0, h0);
    auto u0     = mm_mul(a0, h0);
    auto t1     = mm_fmadd(l1, h1, t0);
    auto u1     = mm_fmadd(a1, h1, u0);
    auto dets   = mm_fmadd(l2, h2, t1);
    auto dets_u = mm_fmadd(a2, h2, u1);


    auto det_mat = broadcast<0>(dets_u);
    return mm_div(dets, det_mat);
#endif
}


// --------------------------------------------------------------------------------------------------------------------

} // namespace mjolnir
