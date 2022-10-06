#include "mjolnir/core/math/linear_algebra/solver/cramer.h"
#include "mjolnir/core/x86/direct_access.h"
#include <benchmark/benchmark.h>

#include <algorithm>

using namespace mjolnir;
using namespace mjolnir::x86;

template <Number T_Type, UST t_size>
static void bm_solver(benchmark::State& state)
{
    std::array<T_Type, t_size* t_size> mat = {{3., 2., 1., 6.}};
    std::array<T_Type, t_size>         rhs = {{4., 3.}};
    benchmark::DoNotOptimize(mat);

    for ([[maybe_unused]] auto s : state)
        rhs = cramer(mat, rhs);


    benchmark::DoNotOptimize(rhs);
}


template <FloatVectorRegister T_RegisterType, UST t_size>
static void bm_solver(benchmark::State& state)
{
    std::array<ElementType<T_RegisterType>, t_size* t_size> mat_vals = {{3., 2., 1., 6.}};
    std::array<ElementType<T_RegisterType>, t_size>         rhs_vals = {{4., 3.}};

    std::array<T_RegisterType, t_size> mat = {{{0}}};
    T_RegisterType                     rhs = mm_setzero<T_RegisterType>();

    for (UST i = 0; i < t_size; ++i)
    {
        set(rhs, i, rhs_vals.at(i));
        for (UST j = 0; j < t_size; ++j)
        {
            UST idx_arr = i * t_size + j;
            set(mat.at(i), j, mat_vals.at(idx_arr));
        }
    }
    benchmark::DoNotOptimize(mat);

    for ([[maybe_unused]] auto s : state)
        rhs = cramer(mat, rhs);

    benchmark::DoNotOptimize(rhs);
}


BENCHMARK(bm_solver<F32, 2>)->Name("2x2 - F32");       // NOLINT
BENCHMARK(bm_solver<F64, 2>)->Name("2x2 - F64");       // NOLINT
BENCHMARK(bm_solver<__m128, 2>)->Name("2x2 - m128");   // NOLINT
BENCHMARK(bm_solver<__m128d, 2>)->Name("2x2 - m128d"); // NOLINT
BENCHMARK(bm_solver<__m256, 2>)->Name("2x2 - m256");   // NOLINT
BENCHMARK(bm_solver<__m256d, 2>)->Name("2x2 - m256d"); // NOLINT

BENCHMARK_MAIN(); // NOLINT
