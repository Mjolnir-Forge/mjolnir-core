#include "mjolnir/core/definitions.h"
#include "mjolnir/core/memory/linear_memory.h"
#include <benchmark/benchmark.h>

#include <array>
#include <chrono>


using namespace mjolnir;

constexpr UST memory_size     = 10000000;
constexpr UST num_allocations = 10;

auto get_allocation_sizes() -> std::array<UST, num_allocations>
{
    return {{8, 32, 2048, 128, 64, 4096, 16, 256, 1024, 4}}; // NOLINT(readability-magic-numbers)
}


// --- baseline -------------------------------------------------------------------------------------------------------

static void bm_timing_baseline(benchmark::State& state)
{
    for ([[maybe_unused]] auto _ : state)
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto end   = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);

        state.SetIterationTime(elapsed_seconds.count());
    }
}


// --- LinearMemory ---------------------------------------------------------------------------------------------------

void bm_allocate_10(benchmark::State& state)
{
    auto mem = LinearMemory();
    mem.initialize(memory_size);

    std::array<void*, num_allocations> mem_ptr    = {{nullptr}};
    auto                               alloc_size = get_allocation_sizes();

    for ([[maybe_unused]] auto _ : state)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (UST i = 0; i < num_allocations; ++i)
            mem_ptr[i] = mem.allocate(alloc_size[i]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

        benchmark::ClobberMemory();

        auto end = std::chrono::high_resolution_clock::now();

        for (UST i = 0; i < num_allocations; ++i)
            mem.deallocate(mem_ptr[i], alloc_size[i]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)
        mem.reset();


        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        state.SetIterationTime(elapsed_seconds.count());
    }
    benchmark::DoNotOptimize(mem_ptr);
}


void bm_deallocate_10_fifo(benchmark::State& state)
{
    auto mem = LinearMemory();
    mem.initialize(memory_size);

    std::array<void*, num_allocations> mem_ptr    = {{nullptr}};
    auto                               alloc_size = get_allocation_sizes();

    for ([[maybe_unused]] auto _ : state)
    {
        for (UST i = 0; i < num_allocations; ++i)
            mem_ptr[i] = mem.allocate(alloc_size[i]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

        benchmark::ClobberMemory();

        auto start = std::chrono::high_resolution_clock::now();

        for (UST i = 0; i < num_allocations; ++i)
            mem.deallocate(mem_ptr[i], alloc_size[i]); // NOLINT(cppcoreguidelines-pro-bounds-constant-array-index)

        auto end = std::chrono::high_resolution_clock::now();

        mem.reset();


        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        state.SetIterationTime(elapsed_seconds.count());
    }
    benchmark::DoNotOptimize(mem_ptr);
}


// --- malloc/free ----------------------------------------------------------------------------------------------------

void bm_allocate_10_malloc(benchmark::State& state)
{
    std::array<void*, num_allocations> mem_ptr    = {{nullptr}};
    auto                               alloc_size = get_allocation_sizes();

    for ([[maybe_unused]] auto _ : state)
    {
        auto start = std::chrono::high_resolution_clock::now();

        for (UST i = 0; i < num_allocations; ++i)
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index,cppcoreguidelines-owning-memory)
            mem_ptr[i] = malloc(alloc_size[i]); // NOLINT(cppcoreguidelines-no-malloc, hicpp-no-malloc)

        benchmark::ClobberMemory();

        auto end = std::chrono::high_resolution_clock::now();

        for (UST i = 0; i < num_allocations; ++i)
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index,cppcoreguidelines-owning-memory)
            std::free(mem_ptr[i]); // NOLINT(cppcoreguidelines-no-malloc, hicpp-no-malloc)


        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        state.SetIterationTime(elapsed_seconds.count());
    }
    benchmark::DoNotOptimize(mem_ptr);
}


void bm_deallocate_10_free_fifo(benchmark::State& state)
{
    std::array<void*, num_allocations> mem_ptr    = {{nullptr}};
    auto                               alloc_size = get_allocation_sizes();

    for ([[maybe_unused]] auto _ : state)
    {
        for (UST i = 0; i < num_allocations; ++i)
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index,cppcoreguidelines-owning-memory)
            mem_ptr[i] = malloc(alloc_size[i]); // NOLINT(cppcoreguidelines-no-malloc, hicpp-no-malloc)

        benchmark::ClobberMemory();

        auto start = std::chrono::high_resolution_clock::now();

        for (UST i = 0; i < num_allocations; ++i)
            // NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-constant-array-index,cppcoreguidelines-owning-memory)
            std::free(mem_ptr[i]); // NOLINT(cppcoreguidelines-no-malloc, hicpp-no-malloc)

        auto end = std::chrono::high_resolution_clock::now();

        auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
        state.SetIterationTime(elapsed_seconds.count());
    }
    benchmark::DoNotOptimize(mem_ptr);
}

// --- register benchmarks --------------------------------------------------------------------------------------------

BENCHMARK(bm_timing_baseline)->UseManualTime()->Name("baseline");                                  // NOLINT
BENCHMARK(bm_allocate_10)->UseManualTime()->Name("10 allocations - LinearMemory");                 // NOLINT
BENCHMARK(bm_allocate_10_malloc)->UseManualTime()->Name("10 allocations - malloc");                // NOLINT
BENCHMARK(bm_deallocate_10_fifo)->UseManualTime()->Name("10 deallocations (fifo) - LinearMemory"); // NOLINT
BENCHMARK(bm_deallocate_10_free_fifo)->UseManualTime()->Name("10 deallocations (fifo) - free");    // NOLINT
BENCHMARK_MAIN();                                                                                  // NOLINT
