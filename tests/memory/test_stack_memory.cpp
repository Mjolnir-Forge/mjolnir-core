#include "mjolnir/core/exception.h"
#include "mjolnir/core/memory/stack_memory.h"
#include "mjolnir/testing/new_delete_counter.h"
#include <gtest/gtest.h>

// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================


// --- test construction ----------------------------------------------------------------------------------------------

TEST(test_construction, check_state) // NOLINT
{
    constexpr UST num_bytes = 1024;

    START_COUNTING_NEW_AND_DELETE;

    // todo:
    // test free memory is 0
    auto mem = StackMemory(num_bytes);

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
    EXPECT_NUM_NEW_EQ(0);
    EXPECT_NUM_DELETE_EQ(0);
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

    START_COUNTING_NEW_AND_DELETE;

    auto mem = StackMemory(num_bytes);
    mem.initialize();

    EXPECT_EQ(mem.get_memory_size(), num_bytes);
    EXPECT_TRUE(mem.is_initialized());
    EXPECT_NUM_NEW_EQ(1);
    EXPECT_NUM_DELETE_EQ(0);

    // todo:
    // test free memory is num_bytes
}


// --- test initialization exceptions ---------------------------------------------------------------------------------

TEST(test_initialization, exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = StackMemory(num_bytes);
    mem.initialize();

    EXPECT_THROW(mem.initialize(), Exception); // NOLINT

    EXPECT_EQ(mem.get_memory_size(), num_bytes);
    EXPECT_TRUE(mem.is_initialized());
}


// --- test deinitialization ------------------------------------------------------------------------------------------

TEST(test_deinitialization, check_state) // NOLINT
{
    constexpr UST num_bytes = 1024;

    START_COUNTING_NEW_AND_DELETE;

    auto mem = StackMemory(num_bytes);
    mem.initialize();
    mem.deinitialize();

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
    EXPECT_NUM_NEW_EQ(1);
    EXPECT_NUM_DELETE_EQ(1);

    // todo:
    // test free memory 0
}


// --- test deinitialization ------------------------------------------------------------------------------------------

TEST(test_deinitialization, exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = StackMemory(num_bytes);
    mem.initialize();
    mem.deinitialize();

    // todo: test exception if memory still allocated
    EXPECT_THROW(mem.deinitialize(), Exception); // NOLINT

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
}
