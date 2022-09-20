#include "benchmark/benchmark.h"


auto some_function(int in) -> int
{
    int a = 2;
    int b = a * 3 + in;
    return b;
}

static void BM_SomeFunction(benchmark::State& state)
{
    // Perform setup here
    for (auto _ : state)
    {
        // This code gets timed
        some_function(3);
    }
}
// Register the function as a benchmark
BENCHMARK(BM_SomeFunction);
// Run the benchmark
BENCHMARK_MAIN();
