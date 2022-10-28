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
    template <x86::FloatVectorRegister T_RegisterType, UST t_size>
    [[nodiscard]] static auto solve(const std::array<T_RegisterType, t_size>& mat, T_RegisterType rhs) noexcept
            -> T_RegisterType;


    //! @brief
    //! Solve a linear system of equations with multiple right-hand sides.
    //!
    //! @tparam T_Type:
    //! The basic type of a matrix/vector element
    //! @tparam t_size:
    //! Size of the system
    //! @tparam t_num_rhs:
    //! Number of right-hand sides
    //!
    //! @param[in] mat:
    //! The matrix of the system of equations. The data has to be provided in column major format
    //! @param[in] rhs:
    //! The right-hand side vectors of the system
    //!
    //! @return
    //! Solution vectors
    template <Number T_Type, UST t_size, UST t_num_rhs>
    [[nodiscard]] static constexpr auto
    solve_multiple_rhs(const std::array<T_Type, t_size * t_size>&               mat,
                       const std::array<std::array<T_Type, t_size>, t_num_rhs>& rhs) noexcept
            -> std::array<std::array<T_Type, t_size>, t_num_rhs>;


    //! @brief
    //! Solve a linear system of equations with multiple right-hand sides.
    //!
    //! @tparam T_RegisterType:
    //! The register type that stores the columns of a matrix/vector
    //! @tparam t_size:
    //! Size of the system
    //! @tparam t_num_rhs:
    //! Number of right-hand sides
    //!
    //! @param[in] mat:
    //! The matrix of the system of equations. The data has to be provided in column major format
    //! @param[in] rhs:
    //! The right-hand side vectors of the system
    //!
    //! @return
    //! Solution vectors
    template <x86::FloatVectorRegister T_RegisterType, UST t_size, UST t_num_rhs>
    [[nodiscard]] static auto solve_multiple_rhs(const std::array<T_RegisterType, t_size>&    mat,
                                                 const std::array<T_RegisterType, t_num_rhs>& rhs) noexcept
            -> std::array<T_RegisterType, t_num_rhs>;


private:
    //! Solver implementation for 2x2 systems with multiple right-hand sides (not vectorized).
    template <Number T_Type, UST t_num_rhs>
    [[nodiscard]] static constexpr auto
    solve_multiple_rhs_2x2(const std::array<T_Type, 4>&                        mat,
                           const std::array<std::array<T_Type, 2>, t_num_rhs>& rhs) noexcept
            -> std::array<std::array<T_Type, 2>, t_num_rhs>;


    //! Solver implementation for 2x2 systems with multiple right-hand sides (__m128).
    template <UST t_num_rhs>
    [[nodiscard]] static auto solve_multiple_rhs_2x2(const std::array<__m128, 2>&         mat,
                                                     const std::array<__m128, t_num_rhs>& rhs) noexcept
            -> std::array<__m128, t_num_rhs>;


    //! Solver implementation for 2x2 systems with multiple right-hand sides (__m128d).
    template <UST t_num_rhs>
    [[nodiscard]] static auto solve_multiple_rhs_2x2(const std::array<__m128d, 2>&         mat,
                                                     const std::array<__m128d, t_num_rhs>& rhs) noexcept
            -> std::array<__m128d, t_num_rhs>;


    //! Solver implementation for 2x2 systems with multiple right-hand sides (__m256).
    template <UST t_num_rhs>
    [[nodiscard]] static auto solve_multiple_rhs_2x2(const std::array<__m256, 2>&         mat,
                                                     const std::array<__m256, t_num_rhs>& rhs) noexcept
            -> std::array<__m256, t_num_rhs>;


    //! Solver implementation for 2x2 systems with multiple right-hand sides (__m256).
    template <UST t_num_rhs>
    [[nodiscard]] static auto solve_multiple_rhs_2x2(const std::array<__m256d, 2>&         mat,
                                                     const std::array<__m256d, t_num_rhs>& rhs) noexcept
            -> std::array<__m256d, t_num_rhs>;


    //! Solver implementation for 3x3 systems with multiple right-hand sides (not vectorized).
    template <Number T_Type, UST t_num_rhs>
    [[nodiscard]] static constexpr auto
    // NOLINTNEXTLINE(readability-magic-numbers)
    solve_multiple_rhs_3x3(const std::array<T_Type, 9>&                        mat,
                           const std::array<std::array<T_Type, 3>, t_num_rhs>& rhs) noexcept
            -> std::array<std::array<T_Type, 3>, t_num_rhs>;


    //! Calculates the result for 1 or 2 (__m256) right-hand sides of a 3x3 system. The meaning of the input values can
    //! be found in the code of `solve_multiple_rhs_3x3`.
    template <x86::FloatVectorRegister T_RegisterType>
    [[nodiscard]] static auto calculate_result_3x3(const std::array<T_RegisterType, 3>& mat,
                                                   const std::array<T_RegisterType, 3>& mat_120,
                                                   T_RegisterType                       rhs,
                                                   T_RegisterType                       a_201,
                                                   T_RegisterType                       cross_bc_201,
                                                   T_RegisterType                       det_mat) noexcept;


    //! Solver implementation for 3x3 systems with multiple right-hand sides (vectorized).
    template <x86::FloatVectorRegister T_RegisterType, UST t_num_rhs>
    [[nodiscard]] static auto solve_multiple_rhs_3x3(const std::array<T_RegisterType, 3>&         mat,
                                                     const std::array<T_RegisterType, t_num_rhs>& rhs) noexcept
            -> std::array<T_RegisterType, t_num_rhs>;


    //! Solver implementation for 3x3 systems with multiple right-hand sides (__m256d).
    template <UST t_num_rhs>
    [[nodiscard]] static auto solve_multiple_rhs_3x3(const std::array<__m256d, 3>&         mat,
                                                     const std::array<__m256d, t_num_rhs>& rhs) noexcept
            -> std::array<__m256d, t_num_rhs>;


    //! Solver implementation for 4x4 systems with multiple right-hand sides (not vectorized).
    template <Number T_Type, UST t_num_rhs>
    [[nodiscard]] static constexpr auto
    // NOLINTNEXTLINE(readability-magic-numbers)
    solve_multiple_rhs_4x4(const std::array<T_Type, 16>&                       mat,
                           const std::array<std::array<T_Type, 4>, t_num_rhs>& rhs) noexcept
            -> std::array<std::array<T_Type, 4>, t_num_rhs>;


    //! Solver implementation for 4x4 systems with multiple right-hand sides (vectorized).
    template <x86::FloatVectorRegister T_RegisterType, UST t_num_rhs>
    [[nodiscard]] static auto solve_multiple_rhs_4x4(const std::array<T_RegisterType, 4>&         mat,
                                                     const std::array<T_RegisterType, t_num_rhs>& rhs) noexcept
            -> std::array<T_RegisterType, t_num_rhs>;


    //! Solver implementation for 4x4 systems with multiple right-hand sides (__m256).
    template <UST t_num_rhs>
    [[nodiscard]] static auto solve_multiple_rhs_4x4(const std::array<__m256, 4>&         mat,
                                                     const std::array<__m256, t_num_rhs>& rhs) noexcept
            -> std::array<__m256, t_num_rhs>;
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
    std::array<std::array<T_Type, t_size>, 1> rhs_array = {rhs};
    return solve_multiple_rhs<T_Type, t_size, 1>(mat, rhs_array)[0];
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType, UST t_size>
[[nodiscard]] auto Cramer::solve(const std::array<T_RegisterType, t_size>& mat, T_RegisterType rhs) noexcept
        -> T_RegisterType
{
    // This was benchmarked against specialized versions for a single rhs. The results were more or less identical.

    using namespace x86;
    static_assert(4 >= t_size && 1 < t_size, "Only sizes 2-4 are supported.");

    std::array<T_RegisterType, 1> rhs_array = {rhs};
    return solve_multiple_rhs<T_RegisterType, t_size, 1>(mat, rhs_array)[0];
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type, UST t_size, UST t_num_rhs>
[[nodiscard]] constexpr auto
Cramer::solve_multiple_rhs(const std::array<T_Type, t_size * t_size>&               mat,
                           const std::array<std::array<T_Type, t_size>, t_num_rhs>& rhs) noexcept
        -> std::array<std::array<T_Type, t_size>, t_num_rhs>
{
    static_assert(4 >= t_size && 1 < t_size, "Only sizes 2-4 are supported.");

    if constexpr (t_size == 2)
        return solve_multiple_rhs_2x2(mat, rhs);
    else if constexpr (t_size == 3)
        return solve_multiple_rhs_3x3(mat, rhs);
    else if constexpr (t_size == 4)
        return solve_multiple_rhs_4x4(mat, rhs);
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType, UST t_size, UST t_num_rhs>
[[nodiscard]] auto Cramer::solve_multiple_rhs(const std::array<T_RegisterType, t_size>&    mat,
                                              const std::array<T_RegisterType, t_num_rhs>& rhs) noexcept
        -> std::array<T_RegisterType, t_num_rhs>
{
    using namespace x86;
    static_assert(4 >= t_size && 1 < t_size, "Only sizes 2-4 are supported.");
    static_assert(num_elements<T_RegisterType> >= t_size, "Registers size must be equal or larger than system size.");

    if constexpr (t_size == 2)
        return solve_multiple_rhs_2x2(mat, rhs);
    else if constexpr (t_size == 3)
        return solve_multiple_rhs_3x3(mat, rhs);
    else if constexpr (t_size == 4)
        return solve_multiple_rhs_4x4(mat, rhs);
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type, UST t_num_rhs>
[[nodiscard]] constexpr auto
Cramer::solve_multiple_rhs_2x2(const std::array<T_Type, 4>&                        mat,
                               const std::array<std::array<T_Type, 2>, t_num_rhs>& rhs) noexcept
        -> std::array<std::array<T_Type, 2>, t_num_rhs>
{
    T_Type det_mat = determinant_2x2(mat);

    std::array<std::array<T_Type, 2>, t_num_rhs> result = {{{0}}};

    for (UST i = 0; i < t_num_rhs; ++i)
    {
        auto r_0 = std::array<T_Type, 4>{rhs[i][0], rhs[i][1], mat[2], mat[3]}; // NOLINT
        auto r_1 = std::array<T_Type, 4>{mat[0], mat[1], rhs[i][0], rhs[i][1]}; // NOLINT

        result[i][0] = determinant_2x2(r_0) / det_mat; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[i][1] = determinant_2x2(r_1) / det_mat; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    return result;
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_rhs>
[[nodiscard]] inline auto Cramer::solve_multiple_rhs_2x2(const std::array<__m128, 2>&         mat,
                                                         const std::array<__m128, t_num_rhs>& rhs) noexcept
        -> std::array<__m128, t_num_rhs>
{
    using namespace x86;


    auto mat_data = shuffle<0, 1, 0, 1>(mat[0], mat[1]);

    auto b0a1 = permute<2, 1, 2, 1>(mat_data);
    auto a0b1 = permute<0, 3, 0, 3>(mat_data);
    auto b1a0 = permute<3, 0, 3, 0>(mat_data);
    auto a1b0 = permute<1, 2, 1, 2>(mat_data);

    auto prod_mat = mm_mul(a1b0, b0a1);
    auto det_mat  = mm_fmsub(a0b1, b1a0, prod_mat);

    constexpr UST num_loops = t_num_rhs / 2;
    constexpr UST rest      = t_num_rhs % 2;

    std::array<__m128, t_num_rhs> result = {{{0}}};
    for (UST i = 0; i < num_loops; ++i)
    {
        auto idx = 2 * i;

        auto r01 = shuffle<0, 1, 0, 1>(rhs[idx], rhs[idx + 1]); // NOLINT
        auto r10 = shuffle<1, 0, 1, 0>(rhs[idx], rhs[idx + 1]); // NOLINT

        result[idx]     = mm_mul(r10, b0a1);                // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[idx]     = mm_fmsub(r01, b1a0, result[idx]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[idx]     = mm_div(result[idx], det_mat);     // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[idx + 1] = permute<2, 3, 0, 1>(result[idx]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }
    if constexpr (rest > 0)
    {
        constexpr UST idx = t_num_rhs - 1;
        auto          r10 = permute<1, 0, 1, 0>(rhs[idx]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

        result[idx] = mm_mul(r10, b0a1);                     // NOLINT
        result[idx] = mm_fmsub(rhs[idx], b1a0, result[idx]); // NOLINT
        result[idx] = mm_div(result[idx], det_mat);          // NOLINT
    }
    return result; // NOLINT(readability-misleading-indentation)
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_rhs>
[[nodiscard]] inline auto Cramer::solve_multiple_rhs_2x2(const std::array<__m128d, 2>&         mat,
                                                         const std::array<__m128d, t_num_rhs>& rhs) noexcept
        -> std::array<__m128d, t_num_rhs>
{
    using namespace x86;


    auto a0b1 = blend_at<1>(mat[0], mat[1]);
    auto b0a1 = blend_at<0>(mat[0], mat[1]);
    auto a1b0 = shuffle<1, 0>(mat[0], mat[1]);
    auto b1a0 = shuffle<1, 0>(mat[1], mat[0]);


    auto prod_mat = mm_mul(a1b0, b0a1);
    auto det_mat  = mm_fmsub(a0b1, b1a0, prod_mat);


    std::array<__m128d, t_num_rhs> result = {{{0}}};

    for (UST i = 0; i < t_num_rhs; ++i)
    {
        auto r10 = permute<1, 0>(rhs[i]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

        result[i] = mm_mul(rhs[i], b1a0);            // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[i] = mm_fnmadd(r10, b0a1, result[i]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[i] = mm_div(result[i], det_mat);      // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    return result;
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_rhs>
[[nodiscard]] inline auto Cramer::solve_multiple_rhs_2x2(const std::array<__m256, 2>&         mat,
                                                         const std::array<__m256, t_num_rhs>& rhs) noexcept
        -> std::array<__m256, t_num_rhs>
{
    using namespace x86;


    auto mat_data = shuffle<0, 1, 0, 1>(mat[0], mat[1]);
    mat_data      = permute_lanes<0, 0>(mat_data);

    auto b0a1 = permute<2, 1, 2, 1>(mat_data);
    auto a0b1 = permute<0, 3, 0, 3>(mat_data);
    auto b1a0 = permute<3, 0, 3, 0>(mat_data);
    auto a1b0 = permute<1, 2, 1, 2>(mat_data);

    auto prod_mat = mm_mul(a1b0, b0a1);
    auto det_mat  = mm_fmsub(a0b1, b1a0, prod_mat);

    constexpr UST num_loops = t_num_rhs / 4;
    constexpr UST rest      = t_num_rhs % 4;

    std::array<__m256, t_num_rhs> result = {{{0}}};

    for (UST i = 0; i < num_loops; ++i)
    {
        auto idx = 4 * i;

        auto r01_tmp_0 = shuffle<0, 1, 0, 1>(rhs[idx], rhs[idx + 1]);     // NOLINT
        auto r10_tmp_0 = shuffle<1, 0, 1, 0>(rhs[idx], rhs[idx + 1]);     // NOLINT
        auto r01_tmp_1 = shuffle<0, 1, 0, 1>(rhs[idx + 2], rhs[idx + 3]); // NOLINT
        auto r10_tmp_1 = shuffle<1, 0, 1, 0>(rhs[idx + 2], rhs[idx + 3]); // NOLINT
        auto r01       = shuffle_lanes<0, 0, 1, 0>(r01_tmp_0, r01_tmp_1);
        auto r10       = shuffle_lanes<0, 0, 1, 0>(r10_tmp_0, r10_tmp_1);

        result[idx]     = mm_mul(r10, b0a1);                    // NOLINT
        result[idx]     = mm_fmsub(r01, b1a0, result[idx]);     // NOLINT
        result[idx]     = mm_div(result[idx], det_mat);         // NOLINT
        result[idx + 1] = permute<2, 3, 0, 1>(result[idx]);     // NOLINT
        result[idx + 2] = swap_lanes(result[idx]);              // NOLINT
        result[idx + 3] = permute<2, 3, 0, 1>(result[idx + 2]); // NOLINT
    }


    if constexpr (rest == 1)
    {
        constexpr UST idx = t_num_rhs - 1;
        auto          r10 = permute<1, 0, 1, 0>(rhs[idx]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

        result[idx] = mm_mul(r10, b0a1);                     // NOLINT
        result[idx] = mm_fmsub(rhs[idx], b1a0, result[idx]); // NOLINT
        result[idx] = mm_div(result[idx], det_mat);          // NOLINT
    }
    else if constexpr (rest == 2)
    {
        auto idx = t_num_rhs - 2;
        auto r01 = shuffle<0, 1, 0, 1>(rhs[idx], rhs[idx + 1]); // NOLINT
        auto r10 = shuffle<1, 0, 1, 0>(rhs[idx], rhs[idx + 1]); // NOLINT

        result[idx]     = mm_mul(r10, b0a1);                // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[idx]     = mm_fmsub(r01, b1a0, result[idx]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[idx]     = mm_div(result[idx], det_mat);     // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[idx + 1] = permute<2, 3, 0, 1>(result[idx]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }
    else if constexpr (rest == 3)
    {
        auto idx   = t_num_rhs - 3;
        auto r01   = shuffle<0, 1, 0, 1>(rhs[idx], rhs[idx + 1]); // NOLINT
        auto r10   = shuffle<1, 0, 1, 0>(rhs[idx], rhs[idx + 1]); // NOLINT
        auto r10_2 = permute<1, 0, 1, 0>(rhs[idx + 2]);           // NOLINT

        result[idx]     = mm_mul(r10, b0a1);                             // NOLINT
        result[idx + 2] = mm_mul(r10_2, b0a1);                           // NOLINT
        result[idx]     = mm_fmsub(r01, b1a0, result[idx]);              // NOLINT
        result[idx + 2] = mm_fmsub(rhs[idx + 2], b1a0, result[idx + 2]); // NOLINT
        result[idx]     = mm_div(result[idx], det_mat);                  // NOLINT
        result[idx + 2] = mm_div(result[idx + 2], det_mat);              // NOLINT
        result[idx + 1] = permute<2, 3, 0, 1>(result[idx]);              // NOLINT
    }


    return result;
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_rhs>
[[nodiscard]] inline auto Cramer::solve_multiple_rhs_2x2(const std::array<__m256d, 2>&         mat,
                                                         const std::array<__m256d, t_num_rhs>& rhs) noexcept
        -> std::array<__m256d, t_num_rhs>
{
    using namespace x86;


    auto a0a1 = permute_lanes<0, 0>(mat[0]);
    auto b0b1 = permute_lanes<0, 0>(mat[1]);

    auto a0b1 = shuffle<0, 1, 0, 1>(a0a1, b0b1);
    auto a1b0 = shuffle<1, 0, 1, 0>(a0a1, b0b1);
    auto b0a1 = shuffle<0, 1, 0, 1>(b0b1, a0a1);
    auto b1a0 = shuffle<1, 0, 1, 0>(b0b1, a0a1);


    auto prod_mat = mm_mul(a1b0, b0a1);
    auto det_mat  = mm_fmsub(a0b1, b1a0, prod_mat);

    constexpr UST num_loops = t_num_rhs / 2;
    constexpr UST rest      = t_num_rhs % 2;

    std::array<__m256d, t_num_rhs> result = {{{0}}};
    for (UST i = 0; i < num_loops; ++i)
    {
        auto idx = 2 * i;
        auto r01 = shuffle_lanes<0, 0, 1, 0>(rhs[idx], rhs[idx + 1]); // NOLINT
        auto r10 = permute<1, 0, 1, 0>(r01);

        result[idx] = mm_mul(r01, b1a0);                 // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[idx] = mm_fnmadd(r10, b0a1, result[idx]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[idx] = mm_div(result[idx], det_mat);      // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[idx + 1] = swap_lanes(result[idx]);       // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }
    if constexpr (rest > 0)
    {
        constexpr UST idx = t_num_rhs - 1;
        auto          r10 = permute<1, 0, 1, 0>(rhs[idx]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

        result[idx] = mm_mul(r10, b0a1);                     // NOLINT
        result[idx] = mm_fmsub(rhs[idx], b1a0, result[idx]); // NOLINT
        result[idx] = mm_div(result[idx], det_mat);          // NOLINT
    }
    return result; // NOLINT(readability-misleading-indentation)
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type, UST t_num_rhs>
[[nodiscard]] constexpr auto
// NOLINTNEXTLINE(readability-magic-numbers)
Cramer::solve_multiple_rhs_3x3(const std::array<T_Type, 9>&                        mat,
                               const std::array<std::array<T_Type, 3>, t_num_rhs>& rhs) noexcept
        -> std::array<std::array<T_Type, 3>, t_num_rhs>
{
    using MatrixType = std::array<T_Type, 9>; // NOLINT(readability-magic-numbers)

    T_Type det_mat = determinant_3x3(mat);

    std::array<std::array<T_Type, 3>, t_num_rhs> result = {{{0}}};

    for (UST i = 0; i < t_num_rhs; ++i)
    {
        // clang-format off
        auto r_0 = MatrixType{rhs[i][0], rhs[i][1], rhs[i][2],  // NOLINT
                                 mat[3],    mat[4],    mat[5],  // NOLINT(readability-magic-numbers)
                                 mat[6],    mat[7],    mat[8]}; // NOLINT(readability-magic-numbers)
        auto r_1 = MatrixType{   mat[0],    mat[1],    mat[2],
                              rhs[i][0], rhs[i][1], rhs[i][2],  // NOLINT
                                 mat[6],    mat[7],    mat[8]}; // NOLINT(readability-magic-numbers)
        auto r_2 = MatrixType{   mat[0],    mat[1],    mat[2],
                                 mat[3],    mat[4],    mat[5],  // NOLINT(readability-magic-numbers)
                              rhs[i][0], rhs[i][1], rhs[i][2]}; // NOLINT
        // clang-format on

        result[i][0] = determinant_3x3(r_0) / det_mat; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[i][1] = determinant_3x3(r_1) / det_mat; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[i][2] = determinant_3x3(r_2) / det_mat; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    return result;
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType, UST t_num_rhs>
[[nodiscard]] inline auto Cramer::solve_multiple_rhs_3x3(const std::array<T_RegisterType, 3>&         mat,
                                                         const std::array<T_RegisterType, t_num_rhs>& rhs) noexcept
        -> std::array<T_RegisterType, t_num_rhs>
{
    using namespace x86;

    constexpr UST n_l = num_lanes<T_RegisterType>;

    // broadcast first lane of matrix data for multi-lane registers
    std::array<T_RegisterType, 3> mat_pl = {{{0}}};

    if constexpr (n_l > 1)
    {
        mat_pl[0] = permute_lanes<0, 0>(mat[0]);
        mat_pl[1] = permute_lanes<0, 0>(mat[1]);
        mat_pl[2] = permute_lanes<0, 0>(mat[2]);
    }
    else
        mat_pl = mat;


    // create all necessary permutations
    // clang-format off
    std::array<T_RegisterType, 3> mat_120 = {{permute<1, 2, 0, 3>(mat_pl[0]),
                                              permute<1, 2, 0, 3>(mat_pl[1]),
                                              permute<1, 2, 0, 3>(mat_pl[2])}};
    // clang-format on
    auto a_201 = permute<2, 0, 1, 3>(mat_pl[0]);


    // calculate all necessary cross product components of the matrix determinant
    auto prod_bc      = mm_mul(mat_120[1], mat_pl[2]);
    auto cross_bc_201 = mm_fmsub(mat_pl[1], mat_120[2], prod_bc);


    // calculate the matrix determinant
    auto det_mat_terms = mm_mul(a_201, cross_bc_201);
    det_mat_terms      = blend_above<2>(det_mat_terms, mm_setzero<T_RegisterType>());

    auto det_mat = broadcast_element_sum(det_mat_terms);


    // calculate results
    std::array<T_RegisterType, t_num_rhs> result = {{{0.}}};

    if constexpr (n_l == 1)
    {
        for (UST i = 0; i < t_num_rhs; ++i)
            result[i] = calculate_result_3x3(mat_pl, mat_120, rhs[i], a_201, cross_bc_201, det_mat); // NOLINT
    }
    else
    {
        constexpr UST num_loops = t_num_rhs / n_l;
        constexpr UST rest      = t_num_rhs % n_l;

        for (UST i = 0; i < num_loops; ++i)
        {
            UST  idx   = n_l * i;
            auto rhs_p = shuffle_lanes<0, 0, 1, 0>(rhs[idx], rhs[idx + 1]); // NOLINT

            result[idx]     = calculate_result_3x3(mat_pl, mat_120, rhs_p, a_201, cross_bc_201, det_mat); // NOLINT
            result[idx + 1] = swap_lanes(result[idx]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        }

        if constexpr (rest == 1)
            result[t_num_rhs - 1] =
                    calculate_result_3x3(mat_pl, mat_120, rhs[t_num_rhs - 1], a_201, cross_bc_201, det_mat); // NOLINT
    }


    return result;
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] inline auto Cramer::calculate_result_3x3(const std::array<T_RegisterType, 3>& mat,
                                                       const std::array<T_RegisterType, 3>& mat_120,
                                                       T_RegisterType                       rhs,
                                                       T_RegisterType                       a_201,
                                                       T_RegisterType                       cross_bc_201,
                                                       T_RegisterType                       det_mat) noexcept
{
    using namespace x86;

    // create all necessary permutations
    auto r_120 = permute<1, 2, 0, 3>(rhs);
    auto r_201 = permute<2, 0, 1, 3>(rhs);

    auto a_r12 = blend_per_lane_at<0>(mat[0], rhs);
    auto a_r20 = blend_per_lane_at<0>(mat_120[0], r_120);
    auto a_r01 = blend_per_lane_at<0>(a_201, r_201);


    // calculate all necessary cross product components
    auto prod_rc = mm_mul(r_120, mat[2]);
    auto prod_br = mm_mul(mat_120[1], rhs);

    auto cross_rc_201 = mm_fmsub(rhs, mat_120[2], prod_rc);
    auto cross_br_201 = mm_fmsub(mat[1], r_120, prod_br);


    // shuffle all cross product terms as needed
    auto tmp_shf_cross_0 = shuffle<1, 2, 2, 0>(cross_bc_201, cross_rc_201);
    auto tmp_shf_cross_1 = shuffle<0, 0, 1, 0>(cross_bc_201, cross_rc_201);

    auto terms_012 = shuffle<0, 2, 0, 0>(tmp_shf_cross_0, cross_br_201);
    auto terms_120 = shuffle<1, 3, 1, 0>(tmp_shf_cross_0, cross_br_201);
    auto terms_201 = shuffle<0, 2, 2, 0>(tmp_shf_cross_1, cross_br_201);


    // calculate all necessary determinants
    auto sum_0_r = mm_mul(a_r12, terms_012);
    auto sum_1_r = mm_fmadd(a_r20, terms_120, sum_0_r);
    auto dets_r  = mm_fmadd(a_r01, terms_201, sum_1_r);

    return mm_div(dets_r, det_mat);
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_rhs>
[[nodiscard]] inline auto Cramer::solve_multiple_rhs_3x3(const std::array<__m256d, 3>&         mat,
                                                         const std::array<__m256d, t_num_rhs>& rhs) noexcept
        -> std::array<__m256d, t_num_rhs>
{
    using namespace x86;

    // create all necessary permutations
    auto c_120 = permute_across_lanes<1, 2, 0, 3>(mat[2]);
    auto b_120 = permute_across_lanes<1, 2, 0, 3>(mat[1]);
    auto a_120 = permute_across_lanes<1, 2, 0, 3>(mat[0]);

    auto a_201 = permute_across_lanes<2, 0, 1, 3>(mat[0]);


    // calculate all necessary cross product components
    auto prod_bc = mm_mul(b_120, mat[2]);

    auto cross_bc_201 = mm_fmsub(mat[1], c_120, prod_bc);


    // calculate the matrix determinant
    auto det_mat_terms = mm_mul(a_201, cross_bc_201);
    det_mat_terms      = blend_above<2>(det_mat_terms, mm_setzero<__m256d>());

    auto det_mat = broadcast_element_sum(det_mat_terms);


    // calculate results
    std::array<__m256d, t_num_rhs> result = {{{0}}};

    for (UST i = 0; i < t_num_rhs; ++i)
    {
        // create all necessary permutations
        auto r_120 = permute_across_lanes<1, 2, 0, 3>(rhs[i]); // NOLINT
        auto r_201 = permute_across_lanes<2, 0, 1, 3>(rhs[i]); // NOLINT

        auto a_r12 = blend_at<0>(mat[0], rhs[i]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        auto a_r20 = blend_at<0>(a_120, r_120);
        auto a_r01 = blend_at<0>(a_201, r_201);


        // calculate all necessary cross product components

        auto prod_rc = mm_mul(r_120, mat[2]);
        auto prod_br = mm_mul(b_120, rhs[i]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

        auto cross_rc_201 = mm_fmsub(rhs[i], c_120, prod_rc); // NOLINT
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
        auto sum_0_r = mm_mul(a_r01, terms_201);
        auto sum_1_r = mm_fmadd(a_r20, terms_120, sum_0_r);
        auto dets_r  = mm_fmadd(a_r12, terms_012, sum_1_r);

        result[i] = mm_div(dets_r, det_mat); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    return result;
}


// --------------------------------------------------------------------------------------------------------------------

template <Number T_Type, UST t_num_rhs>
[[nodiscard]] constexpr auto
// NOLINTNEXTLINE(readability-magic-numbers)
Cramer::solve_multiple_rhs_4x4(const std::array<T_Type, 16>&                       mat,
                               const std::array<std::array<T_Type, 4>, t_num_rhs>& rhs) noexcept
        -> std::array<std::array<T_Type, 4>, t_num_rhs>
{
    using MatrixType = std::array<T_Type, 16>; // NOLINT(readability-magic-numbers)

    T_Type det_mat = determinant_4x4(mat);

    std::array<std::array<T_Type, 4>, t_num_rhs> result = {{{0}}};

    for (UST i = 0; i < t_num_rhs; ++i)
    {
        // clang-format off
        auto r_0 = MatrixType{
                rhs[i][0], rhs[i][1], rhs[i][2], rhs[i][3],     // NOLINT
                   mat[4],    mat[5],    mat[6],    mat[7],     // NOLINT(readability-magic-numbers)
                   mat[8],    mat[9],   mat[10],   mat[11],     // NOLINT(readability-magic-numbers)
                  mat[12],   mat[13],   mat[14],   mat[15]};    // NOLINT(readability-magic-numbers)
        auto r_1 = MatrixType{
                   mat[0],    mat[1],    mat[2],    mat[3],
                rhs[i][0], rhs[i][1], rhs[i][2], rhs[i][3],     // NOLINT
                   mat[8],    mat[9],   mat[10],   mat[11],     // NOLINT(readability-magic-numbers)
                  mat[12],   mat[13],   mat[14],   mat[15]};    // NOLINT(readability-magic-numbers)
        auto r_2 = MatrixType{
                   mat[0],    mat[1],    mat[2],    mat[3],
                   mat[4],    mat[5],    mat[6],    mat[7],     // NOLINT(readability-magic-numbers)
                rhs[i][0], rhs[i][1], rhs[i][2], rhs[i][3],     // NOLINT
                  mat[12],   mat[13],   mat[14],   mat[15]};    // NOLINT(readability-magic-numbers)
        auto r_3 = MatrixType{
                   mat[0],    mat[1],    mat[2],    mat[3],
                   mat[4],    mat[5],    mat[6],    mat[7],     // NOLINT(readability-magic-numbers)
                   mat[8],    mat[9],   mat[10],   mat[11],     // NOLINT(readability-magic-numbers)
                rhs[i][0], rhs[i][1], rhs[i][2], rhs[i][3]};    // NOLINT
        // clang-format on

        result[i][0] = determinant_4x4(r_0) / det_mat; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[i][1] = determinant_4x4(r_1) / det_mat; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[i][2] = determinant_4x4(r_2) / det_mat; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        result[i][3] = determinant_4x4(r_3) / det_mat; // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
    }

    return result;
}


// --------------------------------------------------------------------------------------------------------------------

template <x86::FloatVectorRegister T_RegisterType, UST t_num_rhs>
[[nodiscard]] inline auto Cramer::solve_multiple_rhs_4x4(const std::array<T_RegisterType, 4>&         mat,
                                                         const std::array<T_RegisterType, t_num_rhs>& rhs) noexcept
        -> std::array<T_RegisterType, t_num_rhs>
{
    using namespace x86;
    // reorder values as needed
    auto ac_b0011 = blend<0, 0, 1, 1>(mat[0], mat[2]); // for last 2 terms
    auto ac_b1100 = blend<1, 1, 0, 0>(mat[0], mat[2]);
    auto bd_b0011 = blend<0, 0, 1, 1>(mat[1], mat[3]);
    auto bd_b1100 = blend<1, 1, 0, 0>(mat[1], mat[3]);

    auto ac_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(ac_b1100); // for last 2 terms
    auto bd_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(bd_b1100);
    auto a_1230        = permute_across_lanes<1, 2, 3, 0>(mat[0]); // for first 4 terms
    auto b_1230        = permute_across_lanes<1, 2, 3, 0>(mat[1]);
    auto c_1230        = permute_across_lanes<1, 2, 3, 0>(mat[2]);
    auto d_1230        = permute_across_lanes<1, 2, 3, 0>(mat[3]);

    // calculate all twelve 2x2 determinants
    auto prod_abcd_45 = mm_mul(ac_b1100_2301, bd_b0011); // for last 2 terms
    auto prod_ab_03   = mm_mul(a_1230, mat[1]);          // for first 4 terms
    auto prod_cd_03   = mm_mul(c_1230, mat[3]);

    auto abcd_45 = mm_fmsub(ac_b0011, bd_b1100_2301, prod_abcd_45); // for last 2 terms
    auto ab_03   = mm_fmsub(mat[0], b_1230, prod_ab_03);            // for first 4 terms
    auto cd_03   = mm_fmsub(mat[2], d_1230, prod_cd_03);

    // reorder to multiply correct terms
    auto abcd_45_3210 = permute_across_lanes<3, 2, 1, 0>(abcd_45); // for last 2 terms
    auto cd_03_2301   = permute_across_lanes<2, 3, 0, 1>(cd_03);   // for first 4 terms

    auto cd_03_2301_neg = negate_selected<0, 1, 0, 1>(cd_03_2301);

    // multiply the 2x2 determinants
    auto products_45_abcd = mm_mul(abcd_45, abcd_45_3210);
    auto products_03_abcd = mm_mul(ab_03, cd_03_2301_neg);

    // set redundant elements to zero and and add both products
    products_45_abcd = blend<0, 0, 1, 1>(products_45_abcd, mm_setzero<T_RegisterType>());

    auto tmp_sum_abcd = mm_add(products_03_abcd, products_45_abcd);


    // sum up all elements to get the determinant
    auto det_mat = broadcast_element_sum(tmp_sum_abcd);


    std::array<T_RegisterType, t_num_rhs> result = {{{0}}};

    for (UST i = 0; i < t_num_rhs; ++i)
    {
        // reorder values as needed
        auto rc_b0011 = blend<0, 0, 1, 1>(rhs[i], mat[2]); // for last 2 terms
        auto rc_b1100 = blend<1, 1, 0, 0>(rhs[i], mat[2]);
        auto ar_b0011 = blend<0, 0, 1, 1>(mat[0], rhs[i]);
        auto ar_b1100 = blend<1, 1, 0, 0>(mat[0], rhs[i]);
        auto rd_b0011 = blend<0, 0, 1, 1>(rhs[i], mat[3]);
        auto rd_b1100 = blend<1, 1, 0, 0>(rhs[i], mat[3]);
        auto br_b0011 = blend<0, 0, 1, 1>(mat[1], rhs[i]);
        auto br_b1100 = blend<1, 1, 0, 0>(mat[1], rhs[i]);


        auto rc_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(rc_b1100); // for last 2 terms
        auto ar_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(ar_b1100);
        auto rd_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(rd_b1100);
        auto br_b1100_2301 = permute_across_lanes<2, 3, 0, 1>(br_b1100);
        auto r_1230        = permute_across_lanes<1, 2, 3, 0>(rhs[i]); // for first 4 terms

        // calculate all twelve 2x2 determinants

        auto prod_rbcd_45 = mm_mul(rc_b1100_2301, bd_b0011); // for last 2 terms
        auto prod_arcd_45 = mm_mul(ac_b1100_2301, rd_b0011);
        auto prod_abrd_45 = mm_mul(ar_b1100_2301, bd_b0011);
        auto prod_abcr_45 = mm_mul(ac_b1100_2301, br_b0011);
        auto prod_rb_03   = mm_mul(r_1230, mat[1]); // for first 4 terms
        auto prod_ar_03   = mm_mul(a_1230, rhs[i]);
        auto prod_rd_03   = mm_mul(r_1230, mat[3]);
        auto prod_cr_03   = mm_mul(c_1230, rhs[i]);

        auto rbcd_45 = mm_fmsub(rc_b0011, bd_b1100_2301, prod_rbcd_45); // for last 2 terms
        auto arcd_45 = mm_fmsub(ac_b0011, rd_b1100_2301, prod_arcd_45);
        auto abrd_45 = mm_fmsub(ar_b0011, bd_b1100_2301, prod_abrd_45);
        auto abcr_45 = mm_fmsub(ac_b0011, br_b1100_2301, prod_abcr_45);
        auto rb_03   = mm_fmsub(rhs[i], b_1230, prod_rb_03); // for first 4 terms
        auto ar_03   = mm_fmsub(mat[0], r_1230, prod_ar_03);
        auto rd_03   = mm_fmsub(rhs[i], d_1230, prod_rd_03);
        auto cr_03   = mm_fmsub(mat[2], r_1230, prod_cr_03);

        // reorder to multiply correct terms
        auto rbcd_45_3210 = permute_across_lanes<3, 2, 1, 0>(rbcd_45); // for last 2 terms
        auto arcd_45_3210 = permute_across_lanes<3, 2, 1, 0>(arcd_45);
        auto abrd_45_3210 = permute_across_lanes<3, 2, 1, 0>(abrd_45);
        auto abcr_45_3210 = permute_across_lanes<3, 2, 1, 0>(abcr_45);
        auto rd_03_2301   = permute_across_lanes<2, 3, 0, 1>(rd_03); // for first 4 terms
        auto cr_03_2301   = permute_across_lanes<2, 3, 0, 1>(cr_03);

        auto rd_03_2301_neg = negate_selected<0, 1, 0, 1>(rd_03_2301);
        auto cr_03_2301_neg = negate_selected<0, 1, 0, 1>(cr_03_2301);

        // multiply the 2x2 determinants
        auto products_45_rbcd = mm_mul(rbcd_45, rbcd_45_3210);
        auto products_45_arcd = mm_mul(arcd_45, arcd_45_3210);
        auto products_45_abrd = mm_mul(abrd_45, abrd_45_3210);
        auto products_45_abcr = mm_mul(abcr_45, abcr_45_3210);
        auto products_03_rbcd = mm_mul(rb_03, cd_03_2301_neg);
        auto products_03_arcd = mm_mul(ar_03, cd_03_2301_neg);
        auto products_03_abrd = mm_mul(ab_03, rd_03_2301_neg);
        auto products_03_abcr = mm_mul(ab_03, cr_03_2301_neg);

        // set redundant elements to zero and and add both products
        products_45_rbcd = blend<0, 0, 1, 1>(products_45_rbcd, mm_setzero<T_RegisterType>());
        products_45_arcd = blend<0, 0, 1, 1>(products_45_arcd, mm_setzero<T_RegisterType>());
        products_45_abrd = blend<0, 0, 1, 1>(products_45_abrd, mm_setzero<T_RegisterType>());
        products_45_abcr = blend<0, 0, 1, 1>(products_45_abcr, mm_setzero<T_RegisterType>());


        auto tmp_sum_rbcd = mm_add(products_03_rbcd, products_45_rbcd);
        auto tmp_sum_arcd = mm_add(products_03_arcd, products_45_arcd);
        auto tmp_sum_abrd = mm_add(products_03_abrd, products_45_abrd);
        auto tmp_sum_abcr = mm_add(products_03_abcr, products_45_abcr);


        // sum up all elements to get the determinant

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

        result[i] = mm_div(dets, det_mat);
    }

    return result;
}


// --------------------------------------------------------------------------------------------------------------------

template <UST t_num_rhs>
[[nodiscard]] inline auto Cramer::solve_multiple_rhs_4x4(const std::array<__m256, 4>&         mat,
                                                         const std::array<__m256, t_num_rhs>& rhs) noexcept
        -> std::array<__m256, t_num_rhs>
{
    using namespace x86;

    // necessary permutations (last 2 elements are unused)
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
    auto products_abcd = mm_mul(prod_ab, prod_cd);


    // sum up all elements to get the determinant of the unmodified matrix
    products_abcd = blend_above<5>(products_abcd, mm_setzero<__m256>()); // NOLINT(readability-magic-numbers)
    auto det_mat  = broadcast_element_sum(products_abcd);                // NOLINT(readability-magic-numbers)


    std::array<__m256, t_num_rhs> result = {{{0}}};

    for (UST i = 0; i < t_num_rhs; ++i)
    {
        // necessary permutations (last 2 elements are unused)
        auto r_00 = permute_across_lanes<0, 1, 2, 3, 0, 1, 0, 0>(rhs[i]);
        auto r_01 = permute_across_lanes<1, 2, 3, 0, 2, 3, 0, 0>(rhs[i]);
        auto r_10 = permute_across_lanes<3, 3, 1, 1, 1, 0, 0, 0>(rhs[i]);
        auto r_11 = permute_across_lanes<2, 0, 0, 2, 3, 2, 0, 0>(rhs[i]);


        // calculate all twelve 2x2 determinants
        auto prod_rb_0 = mm_mul(r_00, b_0);
        auto prod_ar_0 = mm_mul(a_0, r_01);
        auto prod_rd_0 = mm_mul(r_11, d_0);
        auto prod_cr_0 = mm_mul(c_0, r_10);

        auto prod_rb = mm_fmsub(r_01, b_1, prod_rb_0);
        auto prod_ar = mm_fmsub(a_1, r_00, prod_ar_0);
        auto prod_rd = mm_fmsub(r_10, d_1, prod_rd_0);
        auto prod_cr = mm_fmsub(c_1, r_11, prod_cr_0);


        // multiply the 2x2 determinants
        auto products_rbcd = mm_mul(prod_rb, prod_cd);
        auto products_arcd = mm_mul(prod_ar, prod_cd);
        auto products_abrd = mm_mul(prod_ab, prod_rd);
        auto products_abcr = mm_mul(prod_ab, prod_cr);


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
        result[i] = mm_div(dets, det_mat);
    }

    return result;
}


} // namespace mjolnir
