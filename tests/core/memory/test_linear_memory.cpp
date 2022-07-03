#if defined(_MSC_VER)
#    pragma warning(disable : 4324) // Some objects trigger this warning more or less on purpose during alignment tests
#endif

#include "mjolnir/core/exception.h"
#include "mjolnir/core/memory/linear_memory.h"
#include "mjolnir/core/utility/pointer_operations.h"
#include "mjolnir/testing/memory/memory_test_classes.h"
#include "mjolnir/testing/new_delete_counter.h"
#include <gtest/gtest.h>

#include <array>
#include <memory>
#include <numbers>


// === SETUP ==========================================================================================================

using namespace mjolnir;


// === TESTS ==========================================================================================================

// --- test construction ----------------------------------------------------------------------------------------------

TEST(test_linear_memory, construction) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto mem = LinearMemory();

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_EQ(mem.get_free_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test initialization --------------------------------------------------------------------------------------------

TEST(test_linear_memory, initialization) // NOLINT
{
    constexpr UST num_bytes = 1024;

    COUNT_NEW_AND_DELETE;

    auto mem = LinearMemory();
    mem.initialize(num_bytes);

    EXPECT_EQ(mem.get_memory_size(), num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
    EXPECT_TRUE(mem.is_initialized());
    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test initialization_with_external_memory -----------------------------------------------------------------------

TEST(test_linear_memory, initialization_with_external_memory) // NOLINT
{
    constexpr UST num_bytes = 1024;

    COUNT_NEW_AND_DELETE;
    {
        auto* mem_ptr = new std::byte[num_bytes]; // NOLINT(cppcoreguidelines-owning-memory)
        EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

        auto mem = LinearMemory();
        mem.initialize(num_bytes, mem_ptr);

        EXPECT_EQ(mem.get_memory_size(), num_bytes);
        EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
        EXPECT_TRUE(mem.is_initialized());
        EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);
    }
    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test initialization exceptions ---------------------------------------------------------------------------------

TEST(test_linear_memory, initialization_exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory();

    EXPECT_THROW(mem.initialize(0), ValueError); // NOLINT

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_EQ(mem.get_free_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());

    mem.initialize(num_bytes);

    EXPECT_THROW(mem.initialize(num_bytes), Exception); // NOLINT

    EXPECT_EQ(mem.get_memory_size(), num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
    EXPECT_TRUE(mem.is_initialized());
}


// --- test initialization_with_external_memory_exceptions ------------------------------------------------------------

TEST(test_linear_memory, initialization_with_external_memory_exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    // cppcheck-suppress unreadVariable
    [[maybe_unused]] I32 exp_num_new = 0;
    // cppcheck-suppress unreadVariable
    [[maybe_unused]] I32 exp_num_del = 0;

    COUNT_NEW_AND_DELETE;
    {
        auto* mem_ptr = new std::byte[num_bytes]; // NOLINT(cppcoreguidelines-owning-memory)
        EXPECT_NUM_NEW_AND_DELETE_EQ(1, 0);

        auto mem = LinearMemory();
        EXPECT_THROW(mem.initialize(0, mem_ptr), ValueError); // NOLINT

        EXPECT_EQ(mem.get_memory_size(), 0);
        EXPECT_EQ(mem.get_free_memory_size(), 0);
        EXPECT_FALSE(mem.is_initialized());

        mem.initialize(num_bytes, mem_ptr);


        EXPECT_THROW(mem.initialize(num_bytes, mem_ptr), Exception); // NOLINT

        EXPECT_EQ(mem.get_memory_size(), num_bytes);
        EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
        EXPECT_TRUE(mem.is_initialized());

#ifndef DISABLE_NEW_DELETE_COUNTER
        exp_num_new = new_delete_counter.get_num_new_calls();
        exp_num_del = new_delete_counter.get_num_delete_calls() + 1;
#endif
    }
    ASSERT_NUM_NEW_AND_DELETE_EQ(exp_num_new, exp_num_del);
}

// --- test allocation ------------------------------------------------------------------------------------------------

TEST(test_linear_memory, allocation) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory();

    COUNT_NEW_AND_DELETE;

    mem.initialize(num_bytes);

    // cppcheck-suppress unreadVariable
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    [[maybe_unused]] const void* a = mem.allocate(24);
    EXPECT_EQ(mem.get_free_memory_size(), 1000);

    // cppcheck-suppress unreadVariable
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    [[maybe_unused]] const void* b = mem.allocate(16);
    EXPECT_EQ(mem.get_free_memory_size(), 984);

    // cppcheck-suppress unreadVariable
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    [[maybe_unused]] const void* c = mem.allocate(64);
    EXPECT_EQ(mem.get_free_memory_size(), 920);

    // cppcheck-suppress unreadVariable
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    [[maybe_unused]] const void* d = mem.allocate(920);
    EXPECT_EQ(mem.get_free_memory_size(), 0);

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test aligned allocation ----------------------------------------------------------------------------------------

TEST(test_linear_memory, aligned_allocation) // NOLINT
{
    constexpr UST num_bytes  = 1024;
    constexpr UST alloc_size = 8;

    auto mem = LinearMemory();

    COUNT_NEW_AND_DELETE;

    mem.initialize(num_bytes);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    const void* a = mem.allocate(alloc_size, 64);
    EXPECT_TRUE(is_aligned<64>(a));

    UST exp_free_mem     = num_bytes - alloc_size;
    UST current_free_mem = mem.get_free_memory_size();
    EXPECT_LE(current_free_mem, exp_free_mem);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    const void* b = mem.allocate(alloc_size, 8);
    EXPECT_TRUE(is_aligned<8>(b));

    exp_free_mem     = current_free_mem - alloc_size;
    current_free_mem = mem.get_free_memory_size();
    EXPECT_LE(current_free_mem, exp_free_mem);
    EXPECT_GT(pointer_to_integer(b), pointer_to_integer(a));


    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    const void* c = mem.allocate(alloc_size, 32);
    EXPECT_TRUE(is_aligned<32>(c));

    exp_free_mem     = current_free_mem - alloc_size;
    current_free_mem = mem.get_free_memory_size();
    EXPECT_LE(current_free_mem, exp_free_mem);
    EXPECT_GT(pointer_to_integer(c), pointer_to_integer(b));

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test allocation exceptions -------------------------------------------------------------------------------------

TEST(test_linear_memory, allocation_exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory();

    mem.initialize(num_bytes);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-goto,hicpp-avoid-goto)
    EXPECT_THROW([[maybe_unused]] auto m = mem.allocate(num_bytes + 1), AllocationError);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);

    // cppcheck-suppress unreadVariable
    [[maybe_unused]] const void* a = mem.allocate(num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), 0);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-goto,hicpp-avoid-goto)
    EXPECT_THROW([[maybe_unused]] auto m = mem.allocate(1), AllocationError);
    EXPECT_EQ(mem.get_free_memory_size(), 0);
}


// --- test create ----------------------------------------------------------------------------------------------------

TEST(test_linear_memory, create) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory();

    COUNT_NEW_AND_DELETE;

    mem.initialize(num_bytes);


    const auto* a = mem.allocate_construct<UST>(num_bytes);

    UST exp_free_mem = num_bytes - sizeof(UST);

    EXPECT_EQ(*a, num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), exp_free_mem);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    const auto* b = mem.allocate_construct<F32>(std::numbers::pi_v<F32>);

    exp_free_mem -= sizeof(F32);

    EXPECT_EQ(*b, std::numbers::pi_v<F32>);
    EXPECT_EQ(mem.get_free_memory_size(), exp_free_mem);

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test create aligned --------------------------------------------------------------------------------------------

TEST(test_linear_memory, create_aligned) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory();

    COUNT_NEW_AND_DELETE;

    mem.initialize(num_bytes);

    const auto* a = mem.allocate_construct<AlignedStruct>();

    UST exp_free_mem = num_bytes - sizeof(AlignedStruct);

    EXPECT_TRUE(is_aligned(a, struct_alignment));
    EXPECT_LE(mem.get_free_memory_size(), exp_free_mem);

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test deallocation ----------------------------------------------------------------------------------------------

TEST(test_linear_memory, deallocation) // NOLINT
{
    constexpr UST num_bytes  = 1024;
    constexpr UST alloc_size = 36;

    auto mem = LinearMemory();

    COUNT_NEW_AND_DELETE;

    mem.initialize(num_bytes);


    void* a = mem.allocate(alloc_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    void* b = mem.allocate(alloc_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    void* c = mem.allocate(alloc_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    void* d = mem.allocate(alloc_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    constexpr UST exp_free_mem = num_bytes - 4 * alloc_size;
    EXPECT_EQ(mem.get_free_memory_size(), exp_free_mem);

    mem.deallocate(a, alloc_size);
    mem.deallocate(b, alloc_size);
    mem.deallocate(c, alloc_size);
    mem.deallocate(d, alloc_size);

    EXPECT_EQ(mem.get_free_memory_size(), exp_free_mem);

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test destroy ---------------------------------------------------------------------------------------------------

TEST(test_linear_memory, destroy) // NOLINT
{
    constexpr UST num_bytes     = 1024;
    UST           num_destroyed = 0;

    auto mem = LinearMemory();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto* a = mem.allocate_construct<DestructionTester>(num_destroyed);

    EXPECT_EQ(num_destroyed, 0);

    mem.destroy_deallocate(a);
    a = nullptr;

    EXPECT_EQ(num_destroyed, 1);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - sizeof(DestructionTester));


    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test deinitialization ------------------------------------------------------------------------------------------

TEST(test_linear_memory, deinitialization) // NOLINT
{
    constexpr UST num_bytes  = 1024;
    constexpr UST alloc_size = 36;

    COUNT_NEW_AND_DELETE;

    auto mem = LinearMemory();
    mem.initialize(num_bytes);

    void* a = mem.allocate(alloc_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    void* b = mem.allocate(alloc_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    mem.deallocate(a, alloc_size);
    mem.deallocate(b, alloc_size);

    mem.deinitialize();

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_EQ(mem.get_free_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 1);
}


// --- test deinitialization ------------------------------------------------------------------------------------------

TEST(test_linear_memory, deinitialization_exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory();
    mem.initialize(num_bytes);
    mem.deinitialize();

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-goto,hicpp-avoid-goto)
    EXPECT_THROW(mem.deinitialize(), Exception);

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
}


// --- test reset -----------------------------------------------------------------------------------------------------

TEST(test_linear_memory, reset) // NOLINT
{
    constexpr UST num_bytes  = 1024;
    constexpr UST alloc_size = 64;

    auto mem = LinearMemory();

    COUNT_NEW_AND_DELETE;

    mem.initialize(num_bytes);


    void* a = mem.allocate(alloc_size);
    void* b = mem.allocate(alloc_size);
    mem.deallocate(a, alloc_size);
    mem.deallocate(b, alloc_size);

    constexpr UST exp_free_mem = num_bytes - 2 * alloc_size;
    EXPECT_EQ(mem.get_free_memory_size(), exp_free_mem);

    mem.reset();

    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
    const void* c = mem.allocate(alloc_size);

    EXPECT_EQ(pointer_to_integer(a), pointer_to_integer(c));

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test get_allocator ---------------------------------------------------------------------------------------------

TEST(test_linear_memory, get_allocator) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto mem = LinearMemory();

    auto allocator = mem.get_allocator<F32>();

    EXPECT_TRUE((std::is_same_v<decltype(allocator), MemorySystemAllocator<F32, decltype(mem)>>) );
    EXPECT_EQ(&allocator.get_memory_system(), &mem);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test get_deleter -----------------------------------------------------------------------------------------------

TEST(test_linear_memory, get_deleter) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    auto mem = LinearMemory();

    auto deleter = mem.get_deleter<F32>();

    EXPECT_TRUE((std::is_same_v<decltype(deleter), MemorySystemDeleter<F32, decltype(mem)>>) );
    EXPECT_EQ(&deleter.get_memory_system(), &mem);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test with memory from buffer -----------------------------------------------------------------------------------

TEST(test_linear_memory, memory_from_buffer) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    constexpr UST                    num_bytes = 1024;
    std::array<std::byte, num_bytes> buffer    = {};

    auto deleter = []([[maybe_unused]] std::byte* unused)
    {
        // do notihing
    };

    auto mem = LinearMemory<void, decltype(deleter)>(deleter);

    mem.initialize(num_bytes, buffer.data());

    EXPECT_EQ(mem.get_memory_size(), num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
    EXPECT_TRUE(mem.is_initialized());

    F32* a = mem.allocate_construct<F32>(std::numbers::pi_v<F32>);

    EXPECT_EQ(*a, std::numbers::pi_v<F32>);
    EXPECT_EQ(pointer_to_integer(a), pointer_to_integer(buffer.data()));

    mem.destroy_deallocate(a);

    mem.deinitialize();


    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test with memory from other memory system ----------------------------------------------------------------------

TEST(test_linear_memory, memory_from_other_memory_system) // NOLINT
{
    COUNT_NEW_AND_DELETE;

    constexpr UST num_bytes_1 = 1024;
    constexpr UST num_bytes_2 = num_bytes_1 / 2;


    auto mem_1 = LinearMemory();
    mem_1.initialize(num_bytes_1);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    using DeleterType = LinearMemory<>::MemoryDeleterType<std::byte[]>;

    auto deleter = DeleterType(mem_1);
    auto mem_2   = LinearMemory<void, DeleterType>(deleter);

    void* mem_ptr = mem_1.allocate(num_bytes_2);
    mem_2.initialize(num_bytes_2, static_cast<std::byte*>(mem_ptr));

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
    EXPECT_EQ(mem_1.get_free_memory_size(), num_bytes_1 - num_bytes_2);
    EXPECT_EQ(mem_2.get_memory_size(), num_bytes_2);
    EXPECT_EQ(mem_2.get_free_memory_size(), num_bytes_2);
    EXPECT_TRUE(mem_2.is_initialized());

    F32* a = mem_2.allocate_construct<F32>(std::numbers::pi_v<F32>);

    EXPECT_EQ(*a, std::numbers::pi_v<F32>);
    EXPECT_EQ(pointer_to_integer(a), pointer_to_integer(mem_ptr));

    mem_2.destroy_deallocate(a);

    mem_2.deinitialize();

    EXPECT_EQ(mem_2.get_memory_size(), 0);
    EXPECT_EQ(mem_2.get_free_memory_size(), 0);
    EXPECT_FALSE(mem_2.is_initialized());


    // Next line would fail in debug mode if `mem_2` doesn't release the occupied memory correctly.
    mem_1.deinitialize();
}
