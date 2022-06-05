#include "mjolnir/testing/new_delete_counter.h"
#include <gtest/gtest.h>

// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================


// --- test initialization --------------------------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, initialization) // NOLINT
{
    COUNT_NEW_AND_DELETE;
    EXPECT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test new delete with object ------------------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, new_delete_object) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    I32* a = new I32(1); // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete(a);
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test new delete with array -------------------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, new_delete_array) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    F32* a = new F32[3]; // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete[](a);
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}
