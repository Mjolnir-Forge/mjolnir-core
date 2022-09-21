#include "benchmark/benchmark.h"
#include "mjolnir/core/math/linear_algebra/determinant.h"
#include "mjolnir/core/x86/direct_access.h"

#include <algorithm>
#include <array>

using namespace mjolnir;
using namespace mjolnir::x86;


template <Number T_Type, U32 t_size>
void benchmark_determinant(benchmark::State& state)
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

        std::fill(std::begin(data), std::end(data), res);
    }
}


template <FloatVectorRegister T_RegisterType, U32 t_size>
void benchmark_determinant(benchmark::State& state)
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


// --- determinant 2x2 ------------------------------------------------------------------------------------------------

static void determinant_2x2_f32(benchmark::State& state)
{
    benchmark_determinant<F32, 2>(state);
}
BENCHMARK(determinant_2x2_f32); // NOLINT


static void determinant_2x2_f64(benchmark::State& state)
{
    benchmark_determinant<F64, 2>(state);
}
BENCHMARK(determinant_2x2_f64); // NOLINT


static void determinant_2x2_m128(benchmark::State& state)
{
    benchmark_determinant<__m128, 2>(state);
}
BENCHMARK(determinant_2x2_m128); // NOLINT


static void determinant_2x2_m128d(benchmark::State& state)
{
    benchmark_determinant<__m128d, 2>(state);
}
BENCHMARK(determinant_2x2_m128d); // NOLINT


static void determinant_2x2_m256(benchmark::State& state)
{
    benchmark_determinant<__m256, 2>(state);
}
BENCHMARK(determinant_2x2_m256); // NOLINT


static void determinant_2x2_m256d(benchmark::State& state)
{
    benchmark_determinant<__m256d, 2>(state);
}
BENCHMARK(determinant_2x2_m256d); // NOLINT


// --- determinant 3x3 ------------------------------------------------------------------------------------------------

static void determinant_3x3_f32(benchmark::State& state)
{
    benchmark_determinant<F32, 3>(state);
}
BENCHMARK(determinant_3x3_f32); // NOLINT


static void determinant_3x3_f64(benchmark::State& state)
{
    benchmark_determinant<F64, 3>(state);
}
BENCHMARK(determinant_3x3_f64); // NOLINT


static void determinant_3x3_m128(benchmark::State& state)
{
    benchmark_determinant<__m128, 3>(state);
}
BENCHMARK(determinant_3x3_m128); // NOLINT


static void determinant_3x3_m256(benchmark::State& state)
{
    benchmark_determinant<__m256, 3>(state);
}
BENCHMARK(determinant_3x3_m256); // NOLINT


static void determinant_3x3_m256d(benchmark::State& state)
{
    benchmark_determinant<__m256d, 3>(state);
}
BENCHMARK(determinant_3x3_m256d); // NOLINT


// --- determinant 4x4 ------------------------------------------------------------------------------------------------

static void determinant_4x4_f32(benchmark::State& state)
{
    benchmark_determinant<F32, 4>(state);
}
BENCHMARK(determinant_4x4_f32); // NOLINT


static void determinant_4x4_f64(benchmark::State& state)
{
    benchmark_determinant<F64, 4>(state);
}
BENCHMARK(determinant_4x4_f64); // NOLINT


static void determinant_4x4_m128(benchmark::State& state)
{
    benchmark_determinant<__m128, 4>(state);
}
BENCHMARK(determinant_4x4_m128); // NOLINT


static void determinant_4x4_m256(benchmark::State& state)
{
    benchmark_determinant<__m256, 4>(state);
}
BENCHMARK(determinant_4x4_m256); // NOLINT


static void determinant_4x4_m256d(benchmark::State& state)
{
    benchmark_determinant<__m256d, 4>(state);
}
BENCHMARK(determinant_4x4_m256d); // NOLINT


BENCHMARK_MAIN(); // NOLINT
