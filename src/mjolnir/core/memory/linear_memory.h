//! @file
//! memory/linear_memory.h
//!
//! @brief
//! Defines classes that deal with linear memory management


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/exception.h"
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/memory/definitions.h"
#include "mjolnir/core/memory/utility.h"
#include "mjolnir/core/utility/pointer_operations.h"

#include <cassert>
#include <cstddef>
#include <memory>


namespace mjolnir
{
template <typename, typename>
class LinearAllocator;
template <typename T_Type, MemorySystem>
class LinearDeleter;

// --- LinearMemory ---------------------------------------------------------------------------------------------------

//! \addtogroup core_memory
//! @{

//! @brief
//! A linear memory system
//!
//! @details
//! This memory system manages its memory in a linear fashion. Every allocation yields a pointer to the memory that
//! is located directly behind the previously allocated memory block. Memory can only be freed all at once and must be
//! done manually.
//!
//! @tparam T_Lock:
//! The type of lock that should be used for thread safety. If the type is set to `void`, the memory is not protected.
template <typename T_Lock = void, typename T_Deleter = DefaultMemoryDeleter>
class LinearMemory
{
    static_assert(std::is_same_v<T_Lock, void>, "Not implemented yet");

public:
    // template <typename T_Type>
    // using AllocatorType = LinearAllocator<T_Type, T_Lock>;

    //! @brief
    //! Compatible deleter type that can be used with `std::unique_ptr` etc.
    //!
    //! @tparam T_Type:
    //! Type of the object that should be deleted.
    template <typename T_Type>
    using DeleterType = LinearDeleter<T_Type, LinearMemory<T_Lock, T_Deleter>>;

    LinearMemory(const LinearMemory&)     = delete;
    LinearMemory(LinearMemory&&) noexcept = delete;
    ~LinearMemory()                       = default;
    auto operator=(const LinearMemory&) -> LinearMemory& = delete;
    auto operator=(LinearMemory&&) noexcept -> LinearMemory& = delete;


    //! @brief
    //! Construct a new instance
    //!
    //! @param[in] deleter:
    //! A deleter that is used to free the internal memory.
    explicit LinearMemory(T_Deleter deleter = DefaultMemoryDeleter());

    //! @brief
    //! Allocate a new memory block and return a pointer that points to it.
    //!
    //! @param[in] size:
    //! Size of the allocation
    //! @param[in] alignment:
    //! Required alignment of the memory
    //!
    //! @return
    //! Pointer to the newly allocated memory
    [[nodiscard]] auto allocate(UST size, UST alignment = 1) -> void*;


    //! @brief
    //! Create an instance of `T_Type` inside a newly allocated memory block and return the pointer to it.
    //!
    //! @tparam T_Type:
    //! The type that should be created
    //! @tparam T_Args:
    //! Types of the constructor arguments
    //!
    //! @param[in] args:
    //! Arguments that should be passed to the constructor of the created type.
    //!
    //! @return
    //! Pointer to the created instance of `T_Type`
    template <typename T_Type, typename... T_Args>
    [[nodiscard]] auto allocate_construct(T_Args&&... args) -> T_Type*;


    //! @brief
    //! Deallocate memory.
    //!
    //! @details
    //! In release builds this function does nothing. In debug builds some additional checks are performed.
    //!
    //! @param[in] ptr:
    //! Pointer to the memory that should be freed
    //! @param[in] size:
    //! Size of the memory that should be freed.
    //! @param[in] alignment:
    //! Alignment of the pointer.
    void deallocate([[maybe_unused]] void* ptr,
                    [[maybe_unused]] UST   size,
                    [[maybe_unused]] UST   alignment = 1) const noexcept;


    //! @brief
    //! Deinitialize the memory.
    //!
    //! @details
    //! Resets the internal variables and frees the memory.
    //!
    //! @exception Exception
    //! Memory is already deinitialized
    //! @exception Exception
    //! Memory is still in use (number of allocations != number of deallocations)
    void deinitialize();


    //! @brief
    //! Destroy the passed object and release its memory.
    //!
    //! @tparam T_Type
    //! Type of the passed object
    //!
    //! @param[in] pointer:
    //! Pointer to the object that should be destroyed
    template <typename T_Type>
    void destroy_deallocate(T_Type* pointer) const noexcept;


    //! @brief
    //! Get the size of the free memory.
    //!
    //! @details
    //! If the memory was not initialized using `initialize`, this method will return 0
    //!
    //! @return
    //! Size of the free memory
    [[nodiscard]] auto get_free_memory_size() const noexcept -> UST;


    //! @brief
    //! Get the size of the allocated memory.
    //!
    //! @details
    //! If the memory was not initialized using `initialize`, this method will return 0
    //!
    //! @return
    //! Size of the memory
    [[nodiscard]] auto get_memory_size() const noexcept -> UST;


    //! @brief
    //! Initialize the class.
    //!
    //! @details
    //! This function allocates memory from the heap that is further managed by the class.
    //!
    //! @param[in] size:
    //! Desired size of the internal memory.
    //!
    //! @exception Exception
    //! Memory is already initialized
    void initialize(UST size);


    //! @brief
    //! Initialize the class.
    //!
    //! @details
    //! This function passes the given memory to the class so that it can be used internally.
    //!
    //! @param[in] size:
    //! Size of the passed memory.
    //! @param[in] memory_ptr:
    //! Pointer to the memory that the class should use internally
    void initialize(UST size, std::byte* memory_ptr);


    //! @brief
    //! Return `true` if the memory is initialized and `false` otherwise.
    //!
    //! @return
    //! `true` or `false`
    [[nodiscard]] auto is_initialized() const noexcept -> bool;

    //! @brief
    //! Reset the internal memory
    //!
    //! @details
    //! Resets the internal pointer to the start of the memory block so that it can be reused. Only debug builds will
    //! check if the number of deallocations matches the number of allocations. In release builds the memory is reset
    //! without any further tests. So make sure none of the memory is used anymore.
    void reset() noexcept;


private:
    //! @brief
    //! Allocate a new memory block and return a pointer that points to it.
    //!
    //! @param[in] size:
    //! Size of the allocation
    //! @param[in] alignment:
    //! Required alignment of the memory
    //!
    //! @return
    //! Pointer to the newly allocated memory
    [[nodiscard]] auto allocate_internal(UST size, UST alignment) -> void*;


    //! @brief
    //! Initialize the memory.
    void deinitialize_internal();


    //! @brief
    //! Initialize the memory.
    void initialize_internal(UST size);

    //! @brief
    //! Get the start address of the internal memory
    [[nodiscard]] auto get_start_address() const noexcept -> UPT;


    UST m_memory_size  = {0};
    UPT m_current_addr = {0};
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    std::unique_ptr<std::byte[], T_Deleter> m_memory;

#ifndef NDEBUG
    mutable UST m_num_allocations = {0};
#endif
};


// --- LinearAllocator ------------------------------------------------------------------------------------------------

//! @brief
//! STL compatible allocator that uses linear memory.
//!
//! @tparam T_Type:
//! Type of the allocated object
//! @tparam T_Lock:
//! Set to `true` if the used linear memory is thread safe.
template <typename T_Type, typename T_Lock = void>
class LinearAllocator
{
public:
    //! \cond DO_NOT_DOCUMENT

    //! Required class members for allocators
    using value_type = T_Type; // NOLINT(readability-identifier-naming)

    template <typename T_OtherType>
    struct rebind // NOLINT(readability-identifier-naming)
    {
        using other = LinearAllocator<T_OtherType, T_Lock>; // NOLINT(readability-identifier-naming)
    };


    LinearAllocator()                           = delete;
    LinearAllocator(const LinearAllocator&)     = default;
    LinearAllocator(LinearAllocator&&) noexcept = default;
    ~LinearAllocator()                          = default;
    auto operator=(const LinearAllocator&) -> LinearAllocator& = default;
    auto operator=(LinearAllocator&&) noexcept -> LinearAllocator& = default;
    //! \endcond


    //! @brief
    //! Construct a new allocator with the passes `LinearMemory` instance
    //!
    //! @param[in] linear_memory:
    //! `LinearMemory` instance that provides the memory for the allocations
    explicit LinearAllocator(LinearMemory<T_Lock>& linear_memory) noexcept;


    //! @brief
    //! Construct a new allocator using the same `LinearMemory` instance as the passed allocator.
    //!
    //! @tparam T_OtherType
    //! Object type of the other allocator
    //!
    //! @param[in] other:
    //! Other allocator that provides a reference to the `LinearMemory` instance that should be used
    template <class T_OtherType>
    explicit LinearAllocator(const LinearAllocator<T_OtherType, T_Lock>& other) noexcept;


    //! @brief
    //! Allocate memory for one or more instances of the allocators `T_Type`.
    //!
    //! @param[in] num_instances:
    //! Specifies the number of instances of `T_Type` that should fit into the allocated memory
    //!
    //! @return
    //! Pointer to the allocated memory
    [[nodiscard]] auto allocate(UST num_instances) -> T_Type*;


    //! @brief
    //! Deallocates the memory of the passed pointer.
    //!
    //! @details
    //! Due to the mechanics of the internally used `LinearMemory` this function does nothing.
    //!
    //! @param[in] pointer:
    //! Pointer to the memory that should be freed
    //! @param[in] num_instances:
    //! Specifies how many instances of `T_Type` fitted into the allocated memory.
    void deallocate([[maybe_unused]] T_Type* pointer, UST num_instances);


private:
    LinearMemory<T_Lock>& m_memory;

    template <typename T_OtherType, typename T_OtherLock>
    friend class LinearAllocator;
};


//! @brief
//! STL compatible deleter that uses linear memory.
//!
//! @tparam T_Type:
//! Type of the allocated object
//! @tparam T_Lock:
//! Set to `true` if the used linear memory is thread safe.
template <typename T_Type, MemorySystem T_MemoryType>
class LinearDeleter
{
    // todo: generalize -> DefaultDeleter

public:
    //! \cond DO_NOT_DOCUMENT

    LinearDeleter()                         = delete;
    LinearDeleter(const LinearDeleter&)     = default;
    LinearDeleter(LinearDeleter&&) noexcept = default;
    ~LinearDeleter()                        = default;
    auto operator=(const LinearDeleter&) -> LinearDeleter& = default;
    auto operator=(LinearDeleter&&) noexcept -> LinearDeleter& = default;
    //! \endcond


    //! @brief
    //! Construct a new deleter with the passes `LinearMemory` instance.
    //!
    //! @param[in] linear_memory:
    //! `LinearMemory` that provided the memory for the opject that should be deleted
    explicit LinearDeleter(T_MemoryType& linear_memory) noexcept;

    //! @brief
    //! Destroy the object at the passed memory address and deallocate the memory.
    //!
    //! @param[in] pointer:
    //! Pointer to the object that should be destroyed and the memory that should be deallocated.
    void operator()(std::remove_extent_t<T_Type>* pointer) noexcept;


private:
    T_MemoryType& m_memory;
};


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
template <typename T_Lock, typename T_Deleter>
LinearMemory<T_Lock, T_Deleter>::LinearMemory(T_Deleter deleter) : m_memory{nullptr, deleter}
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
auto LinearMemory<T_Lock, T_Deleter>::allocate(UST size, UST alignment) -> void*
{
    return allocate_internal(size, alignment);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
template <typename T_Type, typename... T_Args>
auto LinearMemory<T_Lock, T_Deleter>::allocate_construct(T_Args&&... args) -> T_Type*
{
    // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
    return new (allocate(sizeof(T_Type), alignof(T_Type))) T_Type(std::forward<T_Args>(args)...);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
void LinearMemory<T_Lock, T_Deleter>::deallocate([[maybe_unused]] void* ptr,
                                                 [[maybe_unused]] UST   size,
                                                 [[maybe_unused]] UST   alignment) const noexcept
{
#ifndef NDEBUG
    UPT addr         = pointer_to_integer(ptr);
    UPT memory_start = get_start_address();

    // NOLINTNEXTLINE
    assert(addr >= memory_start && addr < memory_start + m_memory_size && "Pointer doesn't belong to this memory.");
    assert(m_num_allocations > 0 && "Deallocation was called to often"); // NOLINT

    --m_num_allocations;
#endif
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
void LinearMemory<T_Lock, T_Deleter>::deinitialize()
{
    deinitialize_internal();
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
template <typename T_Type>
void LinearMemory<T_Lock, T_Deleter>::destroy_deallocate(T_Type* pointer) const noexcept
{
    if (pointer)
    {
        pointer->~T_Type();
        deallocate(pointer, sizeof(T_Type), alignof(T_Type));
    }
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
auto LinearMemory<T_Lock, T_Deleter>::get_free_memory_size() const noexcept -> UST
{
    if (! m_memory)
        return 0;

    auto alloc_size = m_current_addr - get_start_address();
    return m_memory_size - alloc_size;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
[[nodiscard]] auto LinearMemory<T_Lock, T_Deleter>::get_memory_size() const noexcept -> UST
{
    if (m_memory)
        return m_memory_size;
    return 0;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
void LinearMemory<T_Lock, T_Deleter>::initialize(UST size)
{
    initialize_internal(size);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
void LinearMemory<T_Lock, T_Deleter>::initialize(UST size, std::byte* memory_ptr)
{
    THROW_EXCEPTION_IF(is_initialized(), Exception, "Memory is already initialized");
    THROW_EXCEPTION_IF(size == 0, Exception, "Memory size must be larger than 0.");

    m_memory_size = size;
    m_memory.reset(memory_ptr);
    m_current_addr = get_start_address();
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
[[nodiscard]] auto LinearMemory<T_Lock, T_Deleter>::is_initialized() const noexcept -> bool
{
    return m_memory != nullptr;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
void LinearMemory<T_Lock, T_Deleter>::reset() noexcept
{
    assert(m_num_allocations == 0 && "Memory still in use."); // NOLINT

    m_current_addr = get_start_address();
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
auto LinearMemory<T_Lock, T_Deleter>::allocate_internal(UST size, UST alignment) -> void*
{
    assert(size != 0 && "Allocated memory size is 0.");             // NOLINT
    assert(is_initialized() && "Stack memory is not initialized."); // NOLINT

    UPT allocated_addr = align_address(m_current_addr, alignment);
    UPT next_addr      = allocated_addr + size;

    THROW_EXCEPTION_IF(get_start_address() + m_memory_size < next_addr, AllocationError, "No more memory available.");

    m_current_addr = next_addr;

#ifndef NDEBUG
    ++m_num_allocations;
#endif

    return integer_to_pointer(allocated_addr);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
void LinearMemory<T_Lock, T_Deleter>::deinitialize_internal()
{
    THROW_EXCEPTION_IF(! is_initialized(), Exception, "Memory already deinitialized.");
    assert(m_num_allocations == 0 && "Memory still in use."); // NOLINT

    m_memory_size  = 0;
    m_memory       = nullptr;
    m_current_addr = 0;
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
void LinearMemory<T_Lock, T_Deleter>::initialize_internal(UST size)
{
    static_assert(std::is_same_v<T_Deleter, DefaultMemoryDeleter>,
                  "Function can only be used if the classes deleter type is the default deleter.");

    THROW_EXCEPTION_IF(is_initialized(), Exception, "Memory is already initialized");
    THROW_EXCEPTION_IF(size == 0, Exception, "Memory size must be larger than 0.");

    m_memory_size = size;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    m_memory       = std::make_unique<std::byte[]>(m_memory_size);
    m_current_addr = get_start_address();
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Lock, typename T_Deleter>
auto LinearMemory<T_Lock, T_Deleter>::get_start_address() const noexcept -> UPT
{
    return pointer_to_integer(m_memory.get());
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, typename T_Lock>
LinearAllocator<T_Type, T_Lock>::LinearAllocator(LinearMemory<T_Lock>& linear_memory) noexcept : m_memory{linear_memory}
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, typename T_Lock>
template <class T_OtherType>
LinearAllocator<T_Type, T_Lock>::LinearAllocator(const LinearAllocator<T_OtherType, T_Lock>& other) noexcept
    : m_memory{other.m_memory}
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, typename T_Lock>
[[nodiscard]] auto LinearAllocator<T_Type, T_Lock>::allocate(UST num_instances) -> T_Type*
{
    return static_cast<T_Type*>(m_memory.allocate(num_instances * sizeof(T_Type), alignof(T_Type)));
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, typename T_Lock>
void LinearAllocator<T_Type, T_Lock>::deallocate(T_Type* pointer, UST num_instances)
{
    m_memory.deallocate(pointer, num_instances * sizeof(T_Type), alignof(T_Type));
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemoryType>
// cppcheck-suppress constParameter
LinearDeleter<T_Type, T_MemoryType>::LinearDeleter(T_MemoryType& linear_memory) noexcept : m_memory(linear_memory)
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemoryType>
void LinearDeleter<T_Type, T_MemoryType>::operator()(std::remove_extent_t<T_Type>* pointer) noexcept
{
    m_memory.destroy_deallocate(pointer);
}


} // namespace mjolnir
