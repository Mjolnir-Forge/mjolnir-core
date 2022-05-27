#include "mjolnir/core/memory/stack_memory.h"
#include <gtest/gtest.h>

// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================


// --- test construction ----------------------------------------------------------------------------------------------

TEST(test_stack_allocator, test_construction) // NOLINT
{
    constexpr UST num_bytes = 1024;

    // todo:
    // test num allocations
    // test free memory is 0
    auto mem = StackMemory(num_bytes);
    EXPECT_EQ(mem.get_memory_size(), 0);
}


// --- test initialization --------------------------------------------------------------------------------------------

TEST(test_stack_allocator, test_initialization) // NOLINT
{
}
