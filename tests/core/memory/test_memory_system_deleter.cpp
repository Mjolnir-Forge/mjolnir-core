#if defined(_MSC_VER)
#    pragma warning(disable : 4324) // Some objects trigger this warning more or less on purpose during alignment tests
#endif

#include "mjolnir/core/exception.h"
#include "mjolnir/core/memory/linear_memory.h"
#include "mjolnir/core/memory/memory_system_deleter.h"
#include "mjolnir/core/utility/pointer_operations.h"
#include "mjolnir/testing/new_delete_counter.h"
#include <gtest/gtest.h>

#include <memory>


// === SETUP ==========================================================================================================

using namespace mjolnir;


// --- test suite -----------------------------------------------------------------------------------------------------

template <class T_Type>
class DeleterTestSuite : public ::testing::Test
{
};
using DeleterTestTypes = ::testing::Types<LinearMemory<>>;
// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(DeleterTestSuite, DeleterTestTypes, ); // NOLINT


// --- test structs ---------------------------------------------------------------------------------------------------

constexpr UST struct_alignment = 32;
struct alignas(struct_alignment) AlignedStruct
{
    I64 m_member_a = 0;
    I64 m_member_b = 0;
};


//! This class is used to test if a custom deleter calls the destructor during deletion. It takes a reference to an
//! integer during construction and increases it by 1 if the class gets destroyed.
class DestructionTester
{
public:
    DestructionTester()                             = delete;
    DestructionTester(const DestructionTester&)     = default;
    DestructionTester(DestructionTester&&) noexcept = default;
    auto operator=(const DestructionTester&) -> DestructionTester& = delete;
    auto operator=(DestructionTester&&) noexcept -> DestructionTester& = delete;
    ~DestructionTester()
    {
        ++m_destruction_count;
    }

    explicit DestructionTester(UST& destruction_count) : m_destruction_count{destruction_count} {};


private:
    UST& m_destruction_count;
};


// === TESTS ==========================================================================================================


// todo:
// - alignment test
// - std::shared_ptr test


// --- test constructor -----------------------------------------------------------------------------------------------

TYPED_TEST(DeleterTestSuite, constructor) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    [[maybe_unused]] auto deleter = MemorySystemDeleter<F32, decltype(mem)>(mem);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test get_memory_system -----------------------------------------------------------------------------------------

TYPED_TEST(DeleterTestSuite, get_memory_system) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto deleter = MemorySystemDeleter<F32, decltype(mem)>(mem);

    auto& mem_ref = deleter.get_memory_system();

    EXPECT_TRUE((std::is_same_v<decltype(mem_ref), decltype(mem)&>) );

    UST original_addr  = pointer_to_integer(&mem);
    UST reference_addr = pointer_to_integer(&mem_ref);

    EXPECT_EQ(reference_addr, original_addr);

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test as_type ---------------------------------------------------------------------------------------------------

TYPED_TEST(DeleterTestSuite, as_type) // NOLINT
{
    constexpr UST num_bytes = 1024;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto deleter = MemorySystemDeleter<F32, decltype(mem)>(mem);

    auto deleter_other = deleter.template as_type<UST>();

    EXPECT_TRUE((std::is_same_v<decltype(deleter_other), MemorySystemDeleter<UST, decltype(mem)>>) );
    EXPECT_EQ(&deleter.get_memory_system(), &deleter_other.get_memory_system());

    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test call deleter ----------------------------------------------------------------------------------------------

TYPED_TEST(DeleterTestSuite, call_deleter) // NOLINT
{
    constexpr UST num_bytes     = 1024;
    UST           num_destroyed = 0;


    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto deleter = MemorySystemDeleter<DestructionTester, decltype(mem)>(mem);

    auto* ptr = mem.template allocate_construct<DestructionTester>(num_destroyed);

    EXPECT_EQ(num_destroyed, 0);

    deleter(ptr);
    ptr = nullptr;

    EXPECT_EQ(num_destroyed, 1);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - sizeof(DestructionTester));


    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}


// --- test std::unique_ptr -------------------------------------------------------------------------------------------

TYPED_TEST(DeleterTestSuite, std_unique_ptr) // NOLINT
{
    constexpr UST num_bytes     = 1024;
    UST           num_destroyed = 0;

    auto mem = TypeParam();
    mem.initialize(num_bytes);

    COUNT_NEW_AND_DELETE;

    auto deleter = MemorySystemDeleter<DestructionTester, decltype(mem)>(mem);

    auto* ptr   = mem.template allocate_construct<DestructionTester>(num_destroyed);
    auto  u_ptr = std::unique_ptr<DestructionTester, decltype(deleter)>(ptr, deleter);
    ptr         = nullptr;

    EXPECT_EQ(num_destroyed, 0);

    u_ptr.reset(nullptr);

    EXPECT_EQ(num_destroyed, 1);
    EXPECT_EQ(mem.get_free_memory_size(), num_bytes - sizeof(DestructionTester));


    ASSERT_NUM_NEW_AND_DELETE_EQ(0, 0);
}
