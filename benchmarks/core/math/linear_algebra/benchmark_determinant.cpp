#include "benchmark/benchmark.h"
#include "mjolnir/core/math/linear_algebra/determinant.h"
#include "mjolnir/core/x86/direct_access.h"

#include <algorithm>
#include <array>

using namespace mjolnir;
using namespace mjolnir::x86;


template <Number T_Type, UST t_size>
static void bm_determinant(benchmark::State& state)
{
    std::array<T_Type, t_size* t_size> data = {{0}};
    benchmark::DoNotOptimize(data);

    for ([[maybe_unused]] auto s : state)
    {
        T_Type res = 0.;

        if constexpr (t_size == 2)
            res = determinant_2x2(data);
        else if constexpr (t_size == 3)
            res = determinant_3x3(data);
        else
            res = determinant_4x4(data);

        std::ranges::fill(data, res);
    }
}


template <FloatVectorRegister T_RegisterType, UST t_size>
static void bm_determinant(benchmark::State& state)
{
    using Type = ElementType<T_RegisterType>;

    std::array<T_RegisterType, t_size> data = {{{0}}};
    benchmark::DoNotOptimize(data);

    for ([[maybe_unused]] auto s : state)
    {
        Type res = 0.;

        if constexpr (t_size == 2)
            res = determinant_2x2(data);
        else if constexpr (t_size == 3)
            res = determinant_3x3(data);
        else
            res = determinant_4x4(data);

        set<0>(data[0], res);
    }
}


// --- benchmarks -----------------------------------------------------------------------------------------------------

BENCHMARK(bm_determinant<F32, 2>)->Name("2x2 - F32");       // NOLINT
BENCHMARK(bm_determinant<F64, 2>)->Name("2x2 - F64");       // NOLINT
BENCHMARK(bm_determinant<__m128, 2>)->Name("2x2 - m128");   // NOLINT
BENCHMARK(bm_determinant<__m128d, 2>)->Name("2x2 - m128d"); // NOLINT
BENCHMARK(bm_determinant<__m256, 2>)->Name("2x2 - m256");   // NOLINT
BENCHMARK(bm_determinant<__m256d, 2>)->Name("2x2 - m256d"); // NOLINT

BENCHMARK(bm_determinant<F32, 3>)->Name("3x3 - F32");       // NOLINT
BENCHMARK(bm_determinant<F64, 3>)->Name("3x3 - F64");       // NOLINT
BENCHMARK(bm_determinant<__m128, 3>)->Name("3x3 - m128");   // NOLINT
BENCHMARK(bm_determinant<__m256d, 3>)->Name("3x3 - m256d"); // NOLINT
BENCHMARK(bm_determinant<__m256, 3>)->Name("3x3 - m256");   // NOLINT

BENCHMARK(bm_determinant<F32, 4>)->Name("4x4 - F32");       // NOLINT
BENCHMARK(bm_determinant<F64, 4>)->Name("4x4 - F64");       // NOLINT
BENCHMARK(bm_determinant<__m128, 4>)->Name("4x4 - m128");   // NOLINT
BENCHMARK(bm_determinant<__m256d, 4>)->Name("4x4 - m256d"); // NOLINT
BENCHMARK(bm_determinant<__m256, 4>)->Name("4x4 - m256");   // NOLINT


BENCHMARK_MAIN(); // NOLINT
