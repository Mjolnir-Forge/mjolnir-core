#include "mjolnir/testing/new_delete_counter.h"
#include <gtest/gtest.h>

#include "vector"

// === SETUP ==========================================================================================================

using namespace mjolnir;

constexpr UST alignment = 32;
struct alignas(alignment) AlignedStruct
{
    I64 m_member_a = 0;
    I64 m_member_b = 0;
    I64 m_member_c = 0;
    I64 m_member_d = 0;
};


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

    auto* a = new I32(1); // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete(a);
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test new delete with array -------------------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, new_delete_array) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    // If the array is not initialized with a value, GCC behaves weird and calls new and delete very often
    auto* a = new F32[3]{0}; // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete[](a);
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test new delete with object (nothrow) --------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, new_delete_object_nothrow) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new (std::nothrow) I32(1); // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete(a, std::nothrow);
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test new delete with array (nothrow) ---------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, new_delete_array_nothrow) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new (std::nothrow) F32[3]{0}; // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete[](a, std::nothrow);
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test new delete with object (aligned) --------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, new_delete_aligned_object) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new AlignedStruct; // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete(a, static_cast<std::align_val_t>(alignof(AlignedStruct)));
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test new delete with array (aligned) ---------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, new_delete_aligned_array) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new AlignedStruct[3]; // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete[](a, static_cast<std::align_val_t>(alignof(AlignedStruct)));
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test new delete with object (aligned nothrow) ------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, new_delete_aligned_object_nothrow) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new (std::nothrow) AlignedStruct; // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete(a, static_cast<std::align_val_t>(alignof(AlignedStruct)), std::nothrow);
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test new delete with array (aligned nothrow) -------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, new_delete_aligned_array_nothrow) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new (std::nothrow) AlignedStruct[3]; // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

    ::operator delete[](a, static_cast<std::align_val_t>(alignof(AlignedStruct)), std::nothrow);
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


#ifdef __cpp_sized_deallocation

// --- test sized delete with object ----------------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, sized_delete_object) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new I32(1); // NOLINT(cppcoreguidelines-owning-memory)

    ::operator delete(a, sizeof(I32));
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test sized delete with array -----------------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, sized_delete_array) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    // If the array is not initialized with a value, GCC behaves weird and calls new and delete very often
    auto* a = new F32[3]{0}; // NOLINT(cppcoreguidelines-owning-memory)

    ::operator delete[](a, sizeof(F32[3]));
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test sized delete with object (aligned) ------------------------------------------------------------------------

// todo:
// Those two tests throw an exception when MSVC is used, but it wasn't clear from the CI pipeline which one. See:
// https://github.com/Mjolnir-Forge/mjolnir-core/runs/6746288813?check_suite_focus=true
// Find out what the problem is and try to fix it.
#    if not defined(_MSC_VER)

TEST(test_heap_allocation_counter_macros, sized_delete_aligned_object) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new AlignedStruct; // NOLINT(cppcoreguidelines-owning-memory)

    ::operator delete(a, sizeof(AlignedStruct), static_cast<std::align_val_t>(alignof(AlignedStruct)));
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test sized delete with array (aligned) -------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, sized_delete_aligned_array) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new AlignedStruct[3]; // NOLINT(cppcoreguidelines-owning-memory)

    ::operator delete[](a, sizeof(AlignedStruct[3]), static_cast<std::align_val_t>(alignof(AlignedStruct)));
    EXPECT_NUM_NEW_AND_DELETE_EQ(1, 1);
}

#    endif // _MSC_VER
#endif     // __cpp_sized_deallocation


// --- test multiple allocations --------------------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, multiple_allocations) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto* a = new I32(1); // NOLINT(cppcoreguidelines-owning-memory)
    auto* b = new I32(1); // NOLINT(cppcoreguidelines-owning-memory)
    auto* c = new I32(1); // NOLINT(cppcoreguidelines-owning-memory)
    ::    operator delete(a);
    EXPECT_NUM_NEW_AND_DELETE_EQ(3, 1);

    auto* d = new I32(1); // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_NUM_NEW_AND_DELETE_EQ(4, 1);

    ::operator delete(b);
    ::operator delete(c);
    ::operator delete(d);
    EXPECT_NUM_NEW_AND_DELETE_EQ(4, 4);
}


// --- test with std::vector ------------------------------------------------------------------------------------------

TEST(test_heap_allocation_counter_macros, std_vector) // NOLINT
{
    COUNT_NEW_AND_DELETE;
    {
        std::vector<F32> v{0};
        EXPECT_NUM_NEW_AND_DELETE_EQ(2, 1);

        v.push_back(2.F); // NOLINT --- magic number
        EXPECT_NUM_NEW_AND_DELETE_EQ(2, 1);

        v.reserve(3);
        EXPECT_NUM_NEW_AND_DELETE_EQ(3, 2);
    }
    EXPECT_NUM_NEW_AND_DELETE_EQ(3, 3);
}
