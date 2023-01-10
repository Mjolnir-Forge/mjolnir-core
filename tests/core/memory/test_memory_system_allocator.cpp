#if defined(_MSC_VER)
#    pragma warning(disable : 4324) // Some objects trigger this warning more or less on purpose during alignment tests
#endif

#include "mjolnir/core/memory/linear_memory.h"
#include "mjolnir/core/memory/memory_system_allocator.h"
#include "mjolnir/core/utility/pointer_operations.h"
#include "mjolnir/testing/memory/memory_test_classes.h"
#include "mjolnir/testing/new_delete_counter.h"
#include <gtest/gtest.h>


// === SETUP ==========================================================================================================

using namespace mjolnir;


// --- test suite -----------------------------------------------------------------------------------------------------

template <class T_Type>
class AllocatorTestSuite : public ::testing::Test
{
};
using AllocatorTestTypes = ::testing::Types<LinearMemory<>>;
// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(AllocatorTestSuite, AllocatorTestTypes, ); // NOLINT


// === TESTS ==========================================================================================================


// --- test constructor -----------------------------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, constructor) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    [[maybe_unused]] auto allocator = MemorySystemAllocator<F32, TypeParam>(mem);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test constructor (other value type) ----------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, constructor_allocator_other_value_type) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto allocator = MemorySystemAllocator<F32, TypeParam>(mem);

    auto allocator_other_type = MemorySystemAllocator<UST, TypeParam>(allocator);

    EXPECT_EQ(&allocator_other_type.get_memory_system(), &allocator.get_memory_system());

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test get_memory_system -----------------------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, get_memory_system) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto allocator = MemorySystemAllocator<F32, TypeParam>(mem);

    auto& mem_ref = allocator.get_memory_system();

    EXPECT_TRUE((std::is_same_v<decltype(mem_ref), decltype(mem)&>) );

    UST original_addr  = pointer_to_integer(&mem);
    UST reference_addr = pointer_to_integer(&mem_ref);

    EXPECT_EQ(reference_addr, original_addr);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test as_type ---------------------------------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, as_type) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto allocator            = MemorySystemAllocator<F32, TypeParam>(mem);
    auto allocator_other_type = allocator.template as_type<UST>();

    EXPECT_TRUE((std::is_same_v<decltype(allocator_other_type), MemorySystemAllocator<UST, TypeParam>>) );
    EXPECT_EQ(&allocator_other_type.get_memory_system(), &allocator.get_memory_system());

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test allocate --------------------------------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, allocate) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto allocator = MemorySystemAllocator<F32, TypeParam>(mem);

    auto* a = allocator.allocate(1);
    *a      = static_cast<F32>(num_bytes);

    EXPECT_TRUE((std::is_same_v<decltype(a), F32*>) );
    EXPECT_EQ(*a, static_cast<F32>(num_bytes));
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - sizeof(F32));

    [[maybe_unused]] auto* b = allocator.allocate(3);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - 4 * sizeof(F32));

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test deallocate ------------------------------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, deallocate) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto allocator = MemorySystemAllocator<F32, TypeParam>(mem);

    auto* a = allocator.allocate(1);
    auto* b = allocator.allocate(3);

    allocator.deallocate(a, 1);
    allocator.deallocate(b, 3);

    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - 4 * sizeof(F32));

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test std::vector -----------------------------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, std_vector) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto allocator       = MemorySystemAllocator<F32, TypeParam>(mem);
    auto vec             = std::vector<F32, MemorySystemAllocator<F32, TypeParam>>(0, allocator);
    UST  exp_memory_size = mem.get_free_memory_size();

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
    /*
    EXPECT_EQ(vec[0], 1.F);
    EXPECT_EQ(vec[1], 2.F);
    EXPECT_EQ(vec[2], 3.F);

    vec.clear();
    vec.shrink_to_fit();

    EXPECT_EQ(mem.get_free_memory_size(), exp_memory_size);
    */
    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test std::vector with aligned object ---------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, std_vector_aligned_object) // NOLINT
{
    constexpr UST num_bytes    = 1024;
    constexpr UST num_elements = 5;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto allocator = MemorySystemAllocator<AlignedStruct, TypeParam>(mem);
    auto vec       = std::vector<AlignedStruct, MemorySystemAllocator<AlignedStruct, TypeParam>>(0, allocator);

    for (UST i = 0; i < num_elements; ++i)
        vec.emplace_back();

    for (UST i = 0; i < num_elements; ++i)
        EXPECT_TRUE(is_aligned(&vec[i], struct_alignment));

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}

// --- test std::map --------------------------------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, std_map) // NOLINT
{
    using AllocatorType = MemorySystemAllocator<std::pair<const UST, F32>, TypeParam>;

    constexpr UST num_bytes    = 1024;
    constexpr UST num_elements = 5;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

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

    UST minimal_alloc_size = sizeof(typename AllocatorType::value_type) * num_elements;
    EXPECT_LT(mem.get_free_memory_size(), num_bytes - minimal_alloc_size);

    map.clear();

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test std::map (aligned object) ---------------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, std_map_aligned_object) // NOLINT
{
    using AllocatorType = MemorySystemAllocator<std::pair<const UST, AlignedStruct>, TypeParam>;

    constexpr UST num_bytes    = 1024;
    constexpr UST num_elements = 5;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto allocator = AllocatorType(mem);
    auto map       = std::map<UST, AlignedStruct, std::less<>, AllocatorType>(allocator);

    for (UST i = 0; i < num_elements; ++i)
        map.emplace(i, AlignedStruct());


    for (auto const& [key, val] : map)
        EXPECT_TRUE(is_aligned(&val, struct_alignment));

    map.clear();

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test std::shared_ptr -------------------------------------------------------------------------------------------

TYPED_TEST(AllocatorTestSuite, std_shared_ptr) // NOLINT
{
    using AllocatorType = MemorySystemAllocator<DestructionTester, TypeParam>;

    constexpr UST num_bytes     = 1024;
    UST           num_destroyed = 0;


    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto allocator = AllocatorType(mem);

    {
        std::shared_ptr<DestructionTester> s_ptr_1;
        {
            auto s_ptr_2 = std::allocate_shared<DestructionTester, AllocatorType>(allocator, num_destroyed);

            s_ptr_1 = s_ptr_2;
        }
        EXPECT_EQ(num_destroyed, 0);
    }
    EXPECT_EQ(num_destroyed, 1);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}
