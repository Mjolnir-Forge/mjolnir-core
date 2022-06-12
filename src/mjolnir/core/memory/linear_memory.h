//! @file
//! memory/linear_memory.h
//!
//! @brief
//! Defines classes that deal with linear memory management


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/fundamental_types.h"

#include <cstddef>
#include <memory>


namespace mjolnir
{
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
//! @tparam t_thread_safe:
//! If `true`, a thread safety mechanism is added.
template <bool t_thread_safe = false>
class LinearMemory
{
public:
    LinearMemory()                        = delete;
    LinearMemory(const LinearMemory&)     = delete;
    LinearMemory(LinearMemory&&) noexcept = delete;
    ~LinearMemory()                       = default;
    auto operator=(const LinearMemory&) -> LinearMemory& = delete;
    auto operator=(LinearMemory&&) noexcept -> LinearMemory& = delete;


    //! @brief
    //! Construct a new instance with the specified memory size.
    //!
    //! @param[in] size_in_bytes:
    //! The desired size of the memory stack. Note that the memory is not allocated until the `initialize` method is
    //! called.
    //!
    //! @exception Exception
    //! If the passed size is `0`
    explicit LinearMemory(UST size_in_bytes);


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
    //! Initialize the memory.
    //!
    //! @exception Exception
    //! Memory is already initialized
    void initialize();


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
    void initialize_internal();

    //! @brief
    //! Get the start address of the internal memory
    [[nodiscard]] auto get_start_address() const noexcept -> UPT;


    UST m_memory_size;
    UPT m_current_addr = {0};
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    std::unique_ptr<std::byte[]> m_memory = {nullptr};

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
//! @tparam t_thread_safe:
//! Set to `true` if the used linear memory is thread safe.
template <typename T_Type, bool t_thread_safe = false>
class LinearAllocator
{
public:
    //! \cond DO_NOT_DOCUMENT

    //! Required class members for allocators
    using value_type = T_Type; // NOLINT(readability-identifier-naming)

    template <typename T_OtherType>
    struct rebind // NOLINT(readability-identifier-naming)
    {
        using other = LinearAllocator<T_OtherType, t_thread_safe>; // NOLINT(readability-identifier-naming)
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
    explicit LinearAllocator(LinearMemory<t_thread_safe>& linear_memory) noexcept;

    //! @brief
    //! Construct a new allocator using the same `LinearMemory` instance as the passed allocator.
    //!
    //! @tparam T_OtherType
    //! Object type of the other allocator
    //!
    //! @param[in] other:
    //! Other allocator that provides a reference to the `LinearMemory` instance that should be used
    template <class T_OtherType>
    explicit LinearAllocator(const LinearAllocator<T_OtherType, t_thread_safe>& other) noexcept;

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
    LinearMemory<t_thread_safe>& m_memory;

    template <typename T_OtherType, bool t_other_thread_safe>
    friend class LinearAllocator;
};


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================

#include "mjolnir/core/exception.h"
#include "mjolnir/core/memory/utility.h"
#include "mjolnir/core/utility/pointer_operations.h"

#include <cassert>


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
LinearMemory<t_thread_safe>::LinearMemory(UST size_in_bytes) : m_memory_size{size_in_bytes}
{
    THROW_EXCEPTION_IF(m_memory_size < 1, Exception, "Memory size can't be 0.");
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
auto LinearMemory<t_thread_safe>::allocate(UST size, UST alignment) -> void*
{
    return allocate_internal(size, alignment);
}

template <bool t_thread_safe>
void LinearMemory<t_thread_safe>::deallocate([[maybe_unused]] void* ptr,
                                             [[maybe_unused]] UST   size,
                                             [[maybe_unused]] UST   alignment) const noexcept
{
#ifndef NDEBUG
    UPT addr         = pointer_to_integer(ptr);
    UPT memory_start = get_start_address();

    // NOLINTNEXTLINE
    assert(addr >= memory_start && addr < memory_start + m_memory_size && "Pointer doesn't belong to this memory.");

    --m_num_allocations;
#endif
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
void LinearMemory<t_thread_safe>::deinitialize()
{
    deinitialize_internal();
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
auto LinearMemory<t_thread_safe>::get_free_memory_size() const noexcept -> UST
{
    if (! m_memory)
        return 0;

    auto alloc_size = m_current_addr - get_start_address();
    return m_memory_size - alloc_size;
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
[[nodiscard]] auto LinearMemory<t_thread_safe>::get_memory_size() const noexcept -> UST
{
    if (m_memory)
        return m_memory_size;
    return 0;
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
void LinearMemory<t_thread_safe>::initialize()
{
    initialize_internal();
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
[[nodiscard]] auto LinearMemory<t_thread_safe>::is_initialized() const noexcept -> bool
{
    return m_memory != nullptr;
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
void LinearMemory<t_thread_safe>::reset() noexcept
{
    assert(m_num_allocations == 0 && "Memory still in use."); // NOLINT

    m_current_addr = get_start_address();
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
auto LinearMemory<t_thread_safe>::allocate_internal(UST size, UST alignment) -> void*
{
    assert(size != 0 && "Allocated memory size is 0.");             // NOLINT
    assert(is_initialized() && "Stack memory is not initialized."); // NOLINT

    UPT allocated_addr = align_address(m_current_addr, alignment);
    UPT next_addr      = allocated_addr + size;

    THROW_EXCEPTION_IF(get_start_address() + m_memory_size < next_addr, Exception, "No more memory available.");

    m_current_addr = next_addr;

#ifndef NDEBUG
    ++m_num_allocations;
#endif

    return integer_to_pointer(allocated_addr);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
void LinearMemory<t_thread_safe>::deinitialize_internal()
{
    THROW_EXCEPTION_IF(! is_initialized(), Exception, "Memory already deinitialized.");
    assert(m_num_allocations == 0 && "Memory still in use."); // NOLINT

    m_memory       = nullptr;
    m_current_addr = 0;
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
void LinearMemory<t_thread_safe>::initialize_internal()
{
    THROW_EXCEPTION_IF(is_initialized(), Exception, "Memory is already initialized");

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    m_memory       = std::make_unique<std::byte[]>(m_memory_size);
    m_current_addr = get_start_address();
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
auto LinearMemory<t_thread_safe>::get_start_address() const noexcept -> UPT
{
    return pointer_to_integer(m_memory.get());
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, bool t_thread_safe>
LinearAllocator<T_Type, t_thread_safe>::LinearAllocator(LinearMemory<t_thread_safe>& linear_memory) noexcept
    : m_memory{linear_memory}
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, bool t_thread_safe>
template <class T_OtherType>
LinearAllocator<T_Type, t_thread_safe>::LinearAllocator(
        const LinearAllocator<T_OtherType, t_thread_safe>& other) noexcept
    : m_memory{other.m_memory}
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, bool t_thread_safe>
[[nodiscard]] auto LinearAllocator<T_Type, t_thread_safe>::allocate(UST num_instances) -> T_Type*
{
    return static_cast<T_Type*>(m_memory.allocate(num_instances * sizeof(T_Type), alignof(T_Type)));
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, bool t_thread_safe>
void LinearAllocator<T_Type, t_thread_safe>::deallocate(T_Type* pointer, UST num_instances)
{
    m_memory.deallocate(pointer, num_instances * sizeof(T_Type), alignof(T_Type));
}

} // namespace mjolnir
