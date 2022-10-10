#include "mjolnir/core/math/linear_algebra/solver/cramer.h"
#include "mjolnir/core/x86/direct_access.h"
#include <benchmark/benchmark.h>

#include <algorithm>


// ====================================================================================================================
// Setup
// ====================================================================================================================

using namespace mjolnir;
using namespace mjolnir::x86;

// --- helper functions -----------------------------------------------------------------------------------------------

template <Number T_Type, UST t_size>
[[nodiscard]] constexpr auto get_matrix() noexcept -> std::array<T_Type, t_size * t_size>
{
    if constexpr (t_size == 2)
        return std::array<T_Type, 4>{{3., 2., 1., 6.}}; // NOLINT(readability-magic-numbers)
    else
        return std::array<T_Type, 9>{{3., 2., 1., 6., 2., 1., 4., 1., 1.}}; // NOLINT(readability-magic-numbers)
}


template <FloatVectorRegister T_RegisterType, UST t_size>
[[nodiscard]] inline auto get_matrix() noexcept -> std::array<T_RegisterType, t_size>
{
    auto mat_vals = get_matrix<ElementType<T_RegisterType>, t_size>();

    std::array<T_RegisterType, t_size> mat = {{{0}}};

    for (UST i = 0; i < t_size; ++i)
        for (UST j = 0; j < t_size; ++j)
            set(mat.at(i), j, mat_vals.at(i * t_size + j));

    return mat;
}


template <Number T_Type, UST t_size>
[[nodiscard]] constexpr auto get_rhs() noexcept -> std::array<T_Type, t_size>
{
    if constexpr (t_size == 2)
        return std::array<T_Type, 2>{{4., 3.}}; // NOLINT(readability-magic-numbers)
    else
        return std::array<T_Type, 3>{{1., 2., 3.}}; // NOLINT(readability-magic-numbers)
}


template <FloatVectorRegister T_RegisterType, UST t_size>
[[nodiscard]] inline auto get_rhs() noexcept -> T_RegisterType
{
    auto rhs_vals = get_rhs<ElementType<T_RegisterType>, t_size>();
    auto rhs      = mm_setzero<T_RegisterType>();

    for (UST i = 0; i < t_size; ++i)
        set(rhs, i, rhs_vals.at(i));

    return rhs;
}


// ====================================================================================================================
// Benchmarks
// ====================================================================================================================

template <typename T_Type, UST t_size>
static void bm_solver(benchmark::State& state)
{
    auto mat = get_matrix<T_Type, t_size>();
    auto rhs = get_rhs<T_Type, t_size>();

    benchmark::DoNotOptimize(mat);

    for ([[maybe_unused]] auto s : state)
    {
        rhs = Cramer::solve(mat, rhs);
        benchmark::ClobberMemory();
    }

    benchmark::DoNotOptimize(rhs);
}


BENCHMARK(bm_solver<F32, 2>)->Name("2x2 - F32");       // NOLINT
BENCHMARK(bm_solver<__m128, 2>)->Name("2x2 - m128");   // NOLINT
BENCHMARK(bm_solver<__m256, 2>)->Name("2x2 - m256");   // NOLINT
BENCHMARK(bm_solver<F64, 2>)->Name("2x2 - F64");       // NOLINT
BENCHMARK(bm_solver<__m128d, 2>)->Name("2x2 - m128d"); // NOLINT
BENCHMARK(bm_solver<__m256d, 2>)->Name("2x2 - m256d"); // NOLINT
BENCHMARK(bm_solver<F32, 3>)->Name("3x3 - F32");       // NOLINT
BENCHMARK(bm_solver<__m128, 3>)->Name("3x3 - m128");   // NOLINT
BENCHMARK(bm_solver<__m256, 3>)->Name("3x3 - m256");   // NOLINT
BENCHMARK(bm_solver<F64, 3>)->Name("3x3 - F64");       // NOLINT
BENCHMARK(bm_solver<__m256d, 3>)->Name("3x3 - m256d"); // NOLINT
BENCHMARK_MAIN();                                      // NOLINT
