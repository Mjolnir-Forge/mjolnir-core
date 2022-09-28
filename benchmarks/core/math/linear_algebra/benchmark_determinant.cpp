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

BENCHMARK(bm_determinant<F32, 2>)->Name("F32 - 2x2"); // NOLINT
BENCHMARK(bm_determinant<F32, 3>)->Name("F32 - 3x3"); // NOLINT
BENCHMARK(bm_determinant<F32, 4>)->Name("F32 - 4x4"); // NOLINT

BENCHMARK(bm_determinant<F64, 2>)->Name("F64 - 2x2"); // NOLINT
BENCHMARK(bm_determinant<F64, 3>)->Name("F64 - 3x3"); // NOLINT
BENCHMARK(bm_determinant<F64, 4>)->Name("F64 - 4x4"); // NOLINT

BENCHMARK(bm_determinant<__m128d, 2>)->Name("m128d - 2x2"); // NOLINT

BENCHMARK(bm_determinant<__m128, 2>)->Name("m128 - 2x2"); // NOLINT
BENCHMARK(bm_determinant<__m128, 3>)->Name("m128 - 3x3"); // NOLINT
BENCHMARK(bm_determinant<__m128, 4>)->Name("m128 - 4x4"); // NOLINT

BENCHMARK(bm_determinant<__m256d, 2>)->Name("m256d - 2x2"); // NOLINT
BENCHMARK(bm_determinant<__m256d, 3>)->Name("m256d - 3x3"); // NOLINT
BENCHMARK(bm_determinant<__m256d, 4>)->Name("m256d - 4x4"); // NOLINT

BENCHMARK(bm_determinant<__m256, 2>)->Name("m256 - 2x2"); // NOLINT
BENCHMARK(bm_determinant<__m256, 3>)->Name("m256 - 3x3"); // NOLINT
BENCHMARK(bm_determinant<__m256, 4>)->Name("m256 - 4x4"); // NOLINT


BENCHMARK_MAIN(); // NOLINT
