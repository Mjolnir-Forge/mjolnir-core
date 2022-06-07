#include "mjolnir/core/exception.h"
#include "mjolnir/core/memory/stack_memory.h"
#include "mjolnir/testing/new_delete_counter.h"
#include <gtest/gtest.h>

// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================

// todo -> use templated tests where it makes sense

// --- test construction ----------------------------------------------------------------------------------------------

TEST(test_construction, check_state) // NOLINT
{
    constexpr UST num_bytes = 1024;

    COUNT_NEW_AND_DELETE;

    auto mem = StackMemory(num_bytes);

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_EQ(mem.get_free_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test construction exceptions -----------------------------------------------------------------------------------

TEST(test_construction, exceptions) // NOLINT
{
    EXPECT_THROW(StackMemory(0), Exception); // NOLINT
}


// --- test initialization --------------------------------------------------------------------------------------------

TEST(test_initialization, check_state) // NOLINT
{
    constexpr UST num_bytes = 1024;

    COUNT_NEW_AND_DELETE;

    auto mem = StackMemory(num_bytes);
    mem.initialize();

    EXPECT_EQ(mem.get_memory_size(), num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
    EXPECT_TRUE(mem.is_initialized());
    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test initialization exceptions ---------------------------------------------------------------------------------

TEST(test_initialization, exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = StackMemory(num_bytes);
    mem.initialize();

    EXPECT_THROW(mem.initialize(), Exception); // NOLINT

    EXPECT_EQ(mem.get_memory_size(), num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
    EXPECT_TRUE(mem.is_initialized());
}


// --- test allocation ------------------------------------------------------------------------------------------------

TEST(test_allocation, allocation) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = StackMemory(num_bytes);

    COUNT_NEW_AND_DELETE;

    mem.initialize();

    // cppcheck-suppress unreadVariable
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    [[maybe_unused]] void* a = mem.allocate(24);
    EXPECT_EQ(mem.get_free_memory_size(), 1000);

    // cppcheck-suppress unreadVariable
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    [[maybe_unused]] void* b = mem.allocate(16);
    EXPECT_EQ(mem.get_free_memory_size(), 984);

    // cppcheck-suppress unreadVariable
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    [[maybe_unused]] void* c = mem.allocate(64);
    EXPECT_EQ(mem.get_free_memory_size(), 920);

    // cppcheck-suppress unreadVariable
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    [[maybe_unused]] void* d = mem.allocate(920);
    EXPECT_EQ(mem.get_free_memory_size(), 0);

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test allocation ------------------------------------------------------------------------------------------------

TEST(test_allocation, aligned_allocation) // NOLINT
{
    constexpr UST num_bytes  = 1024;
    constexpr UST alloc_size = 8;

    auto mem = StackMemory(num_bytes);

    COUNT_NEW_AND_DELETE;

    mem.initialize();

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    void* a = mem.allocate(alloc_size, 64);
    EXPECT_TRUE(is_aligned<64>(a));

    UST exp_free_mem     = num_bytes - alloc_size;
    UST current_free_mem = mem.get_free_memory_size();
    EXPECT_LE(current_free_mem, exp_free_mem);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    void* b = mem.allocate(alloc_size, 8);
    EXPECT_TRUE(is_aligned<8>(b));

    exp_free_mem     = current_free_mem - alloc_size;
    current_free_mem = mem.get_free_memory_size();
    EXPECT_LE(current_free_mem, exp_free_mem);


    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    void* c = mem.allocate(alloc_size, 32);
    EXPECT_TRUE(is_aligned<32>(c));

    exp_free_mem     = current_free_mem - alloc_size;
    current_free_mem = mem.get_free_memory_size();
    EXPECT_LE(current_free_mem, exp_free_mem);

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test allocation ------------------------------------------------------------------------------------------------

TEST(test_allocation, exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = StackMemory(num_bytes);

    mem.initialize();

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-goto,hicpp-avoid-goto)
    EXPECT_THROW([[maybe_unused]] auto m = mem.allocate(num_bytes + 1), Exception);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);

    // cppcheck-suppress unreadVariable
    [[maybe_unused]] void* a = mem.allocate(num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), 0);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-goto,hicpp-avoid-goto)
    EXPECT_THROW([[maybe_unused]] auto m = mem.allocate(1), Exception);
    EXPECT_EQ(mem.get_free_memory_size(), 0);
}

// todo -> test aligned allocations

// --- test deinitialization ------------------------------------------------------------------------------------------

TEST(test_deinitialization, check_state) // NOLINT
{
    constexpr UST num_bytes = 1024;

    COUNT_NEW_AND_DELETE;

    auto mem = StackMemory(num_bytes);
    mem.initialize();
    mem.deinitialize();

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_EQ(mem.get_free_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test deinitialization ------------------------------------------------------------------------------------------

TEST(test_deinitialization, exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = StackMemory(num_bytes);
    mem.initialize();
    // todo: test exception if memory still allocated
    mem.deinitialize();

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-goto,hicpp-avoid-goto)
    EXPECT_THROW(mem.deinitialize(), Exception);

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
}
