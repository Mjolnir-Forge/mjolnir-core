#include "mjolnir/core/exception.h"
#include "mjolnir/core/memory/linear_memory.h"
#include "mjolnir/core/utility/pointer_operations.h"
#include "mjolnir/testing/new_delete_counter.h"
#include <gtest/gtest.h>

#include <memory>


//#if defined(_MSC_VER)
//#    define SUPPRESS_MSVC_WARNING(number) #    pragma warning(suppress : number)
//#else
//#    define SUPPRESS_MSVC_WARNING(number)
//#endif


// === SETUP ==========================================================================================================

using namespace mjolnir;


constexpr UST struct_alignment = 32;
struct alignas(struct_alignment) AlignedStruct
{
    I64 m_member_a = 0;
    I64 m_member_b = 0;
    I64 m_member_c = 0;
    I64 m_member_d = 0;
};


// === TESTS ==========================================================================================================

// todo -> use templated tests where it makes sense

// ~~~ LinearMemory ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// --- test construction ----------------------------------------------------------------------------------------------

TEST(test_linear_memory, construction) // NOLINT
{
    constexpr UST num_bytes = 1024;

    COUNT_NEW_AND_DELETE;

    auto mem = LinearMemory(num_bytes);

    EXPECT_EQ(mem.get_memory_size(), 0);
    EXPECT_EQ(mem.get_free_memory_size(), 0);
    EXPECT_FALSE(mem.is_initialized());
    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test construction exceptions -----------------------------------------------------------------------------------

TEST(test_linear_memory, construction_exceptions) // NOLINT
{
    EXPECT_THROW(LinearMemory(0), Exception); // NOLINT
}


// --- test initialization --------------------------------------------------------------------------------------------

TEST(test_linear_memory, initialization) // NOLINT
{
    constexpr UST num_bytes = 1024;

    COUNT_NEW_AND_DELETE;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    EXPECT_EQ(mem.get_memory_size(), num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
    EXPECT_TRUE(mem.is_initialized());
    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// --- test initialization exceptions ---------------------------------------------------------------------------------

TEST(test_linear_memory, initialization_exceptions) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    EXPECT_THROW(mem.initialize(), Exception); // NOLINT

    EXPECT_EQ(mem.get_memory_size(), num_bytes);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
    EXPECT_TRUE(mem.is_initialized());
}


// --- test allocation ------------------------------------------------------------------------------------------------

TEST(test_linear_memory, allocation) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory(num_bytes);

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


// --- test aligned allocation ----------------------------------------------------------------------------------------

TEST(test_linear_memory, aligned_allocation) // NOLINT
{
    constexpr UST num_bytes  = 1024;
    constexpr UST alloc_size = 8;

    auto mem = LinearMemory(num_bytes);

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
    EXPECT_GT(pointer_to_integer(b), pointer_to_integer(a));


    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    void* c = mem.allocate(alloc_size, 32);
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

    auto mem = LinearMemory(num_bytes);

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

// --- test deallocation ----------------------------------------------------------------------------------------------

TEST(test_linear_memory, deallocation) // NOLINT
{
    constexpr UST num_bytes  = 1024;
    constexpr UST alloc_size = 36;

    auto mem = LinearMemory(num_bytes);

    COUNT_NEW_AND_DELETE;

    mem.initialize();


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


// --- test deinitialization ------------------------------------------------------------------------------------------

TEST(test_linear_memory, deinitialization) // NOLINT
{
    constexpr UST num_bytes  = 1024;
    constexpr UST alloc_size = 36;

    COUNT_NEW_AND_DELETE;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

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

    auto mem = LinearMemory(num_bytes);
    mem.initialize();
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

    auto mem = LinearMemory(num_bytes);

    COUNT_NEW_AND_DELETE;

    mem.initialize();


    void* a = mem.allocate(alloc_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    void* b = mem.allocate(alloc_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    mem.deallocate(a, alloc_size);
    mem.deallocate(b, alloc_size);

    constexpr UST exp_free_mem = num_bytes - 2 * alloc_size;
    EXPECT_EQ(mem.get_free_memory_size(), exp_free_mem);

    mem.reset();

    EXPECT_EQ(mem.get_free_memory_size(), num_bytes);
    void* c = mem.allocate(alloc_size); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)

    EXPECT_EQ(pointer_to_integer(a), pointer_to_integer(c));

    ASSERT_NUM_NEW_AND_DELETE_EQ(1, 0);
}


// ~~~ LinearAllocator ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// --- test constructor (number of bytes) -----------------------------------------------------------------------------

TEST(test_linear_allocator, constructor_num_bytes) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    COUNT_NEW_AND_DELETE;

    // cppcheck-suppress unreadVariable
    [[maybe_unused]] auto allocator = LinearAllocator<F32>(mem);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test constructor (other value type) ----------------------------------------------------------------------------

TEST(test_linear_allocator, constructor_allocator_other_value_type) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    COUNT_NEW_AND_DELETE;

    auto allocator = LinearAllocator<F32>(mem);

    // cppcheck-suppress unreadVariable
    [[maybe_unused]] auto allocator_other_type = LinearAllocator<UST>(allocator);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test allocate --------------------------------------------------------------------------------------------------

TEST(test_linear_allocator, allocate) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    COUNT_NEW_AND_DELETE;

    auto allocator = LinearAllocator<F32>(mem);

    auto* a = allocator.allocate(1);
    *a      = static_cast<F32>(num_bytes);

    EXPECT_TRUE((std::is_same_v<decltype(a), F32*>) );
    EXPECT_EQ(*a, static_cast<F32>(num_bytes));
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - sizeof(F32));

    // cppcheck-suppress unreadVariable
    [[maybe_unused]] auto* b = allocator.allocate(3);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - 4 * sizeof(F32));

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test deallocate ------------------------------------------------------------------------------------------------

TEST(test_linear_allocator, deallocate) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    COUNT_NEW_AND_DELETE;

    auto allocator = LinearAllocator<F32>(mem);

    auto* a = allocator.allocate(1);
    auto* b = allocator.allocate(3);

    allocator.deallocate(a, 1);
    allocator.deallocate(b, 3);

    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - 4 * sizeof(F32));

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test std::vector -----------------------------------------------------------------------------------------------

TEST(test_linear_allocator, std_vector) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    COUNT_NEW_AND_DELETE;

    auto allocator = LinearAllocator<F32>(mem);
    auto vec       = std::vector<F32, LinearAllocator<F32>>(0, allocator);

    UST exp_memory_size = mem.get_free_memory_size();

    vec.reserve(1);
    vec.push_back(1.F);

    exp_memory_size -= sizeof(F32);

    EXPECT_EQ(mem.get_free_memory_size(), exp_memory_size);
    EXPECT_EQ(vec[0], 1.F);

    vec.reserve(3);
    vec.push_back(2.F); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    vec.push_back(3.F); // NOLINT(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    exp_memory_size -= 3 * sizeof(F32);

    EXPECT_EQ(mem.get_free_memory_size(), exp_memory_size);
    EXPECT_EQ(vec[0], 1.F);
    EXPECT_EQ(vec[1], 2.F);
    EXPECT_EQ(vec[2], 3.F);

    vec.clear();
    vec.shrink_to_fit();

    EXPECT_EQ(mem.get_free_memory_size(), exp_memory_size);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test std::vector with aligned object ---------------------------------------------------------------------------

TEST(test_linear_allocator, std_vector_aligned_object) // NOLINT
{
    constexpr UST num_bytes    = 1024;
    constexpr UST num_elements = 5;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    COUNT_NEW_AND_DELETE;

    auto allocator = LinearAllocator<AlignedStruct>(mem);
    auto vec       = std::vector<AlignedStruct, LinearAllocator<AlignedStruct>>(0, allocator);

    for (UST i = 0; i < num_elements; ++i)
        vec.emplace_back();

    for (UST i = 0; i < num_elements; ++i)
        EXPECT_TRUE(is_aligned(&vec[i], alignof(AlignedStruct)));

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test std::map --------------------------------------------------------------------------------------------------

TEST(test_linear_allocator, std_map) // NOLINT
{
    using AllocatorType = LinearAllocator<std::pair<const UST, F32>>;

    constexpr UST num_bytes    = 1024;
    constexpr UST num_elements = 5;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    COUNT_NEW_AND_DELETE;

    auto allocator = AllocatorType(mem);
    auto map       = std::map<UST, F32, std::less<>, AllocatorType>(allocator);

    for (UST i = 0; i < num_elements; ++i)
        map.emplace(i, static_cast<F32>(i));

    for (UST i = 0; i < num_elements; ++i)
    {
        const auto& entry = map.find(i);
        EXPECT_NE(entry, map.end());
        EXPECT_EQ(entry->second, static_cast<F32>(i));
    }

    UST minimal_alloc_size = sizeof(AllocatorType::value_type) * num_elements;
    EXPECT_LT(mem.get_free_memory_size(), num_bytes - minimal_alloc_size);

    map.clear();

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test std::map (aligned object) ---------------------------------------------------------------------------------

TEST(test_linear_allocator, std_map_aligned_object) // NOLINT
{
    using AllocatorType = LinearAllocator<std::pair<const UST, AlignedStruct>>;

    constexpr UST num_bytes    = 1024;
    constexpr UST num_elements = 5;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    COUNT_NEW_AND_DELETE;

    auto allocator = AllocatorType(mem);
    auto map       = std::map<UST, AlignedStruct, std::less<>, AllocatorType>(allocator);

    for (UST i = 0; i < num_elements; ++i)
    {
#if defined(_MSC_VER)
#    pragma warning(suppress : C4324)
#endif
        map.emplace(i, AlignedStruct());
    }

    for (auto const& [key, val] : map)
        EXPECT_TRUE(is_aligned(&val, alignof(AlignedStruct)));

    map.clear();

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// ~~~ LinearDeleter $$~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// --- test std::map --------------------------------------------------------------------------------------------------

TEST(test_linear_deleter, std_unique_ptr) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = LinearMemory(num_bytes);
    mem.initialize();

    COUNT_NEW_AND_DELETE;

    auto deleter = LinearDeleter<F32>(mem);

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-magic-numbers,readability-magic-numbers)
    F32* ptr = new (mem.allocate(sizeof(F32), alignof(F32))) F32(2.); // NOLINT(cppcoreguidelines-owning-memory)
    EXPECT_EQ(*ptr, 2.0);

    auto u_ptr = std::unique_ptr<F32, LinearDeleter<F32>>(ptr, deleter);

    u_ptr.reset(nullptr);

    mem.deallocate(ptr, 1);

    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - sizeof(F32));


    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}
