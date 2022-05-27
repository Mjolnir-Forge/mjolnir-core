#include "mjolnir/core/memory/stack_memory.h"
#include <gtest/gtest.h>

// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================


// --- test gauss_summation -------------------------------------------------------------------------------------------

TEST(test_stack_allocator, gauss_summation) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = StackMemory(num_bytes);
}
