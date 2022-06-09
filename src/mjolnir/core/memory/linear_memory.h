//! @file
//! memory/stack_memory.h
//!
//! @brief
//! Defines a management class for stack based memory


#pragma once

#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/utility/pointer_operations.h"

#include <cassert>
#include <cstddef>
#include <memory>


namespace mjolnir
{
//! \addtogroup core_memory
//! @{

//! @brief
//! A stack based memory system
//!
//! @details
//! This memory system manages its memory in a stack based fashion. Every allocation yields a pointer to the memory that
//! is located directly behind the previously allocated memory block. Memory is only freed if the passed pointer during
//! deallocated points to the last allocated memory or if the intenal allocation counter becomes 0. In the latter case,
//! all memory is freed at once. As a consequence, deallocations should happen in the opposite order than the
//! allocations took place (last in, first out). Otherwise, not all memory can be freed until the number of
//! deallocations matches the number of allocations or the memory is flushed by calling `flush`.
//!
//! If you know, that deallocations will occur in random order, you can disable the mechanism to free the memory of the
//! last allocated memory block setting the template parameter `t_free_last` to `false`. This increases the speed of the
//! deallocations.
//!
//! @tparam t_free_last:
//! If `true`, memory is freed in case the passed pointer during deallocation matches the pointer to the last allocated
//! memory block.
//! @tparam t_thread_safe:
//! If `true`, a thread safety mechanism is added.
//!
//! @todo
//! set `t_free_last` default to `true`
template <bool t_thread_safe = false>
class LinearMemory
{
    UST m_memory_size;
    UST m_num_allocations = {0};
    UPT m_current_addr    = {0};


    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    std::unique_ptr<std::byte[]> m_memory = {nullptr};


public:
    LinearMemory()                    = delete;
    LinearMemory(const LinearMemory&) = delete;
    LinearMemory(LinearMemory&&)      = delete;
    ~LinearMemory()                   = default;
    auto operator=(const LinearMemory&) -> LinearMemory& = delete;
    auto operator=(LinearMemory&&) -> LinearMemory& = delete;


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


public:
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
};


//! @}
} // namespace mjolnir


// ====================================================================================================================


#include "mjolnir/core/exception.h"


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
auto LinearMemory<t_thread_safe>::allocate_internal(UST size, UST alignment) -> void*
{
    assert(size != 0 && "Allocated memory size is 0.");             // NOLINT
    assert(is_initialized() && "Stack memory is not initialized."); // NOLINT
    // todo -> implement assert
    // assert(IsPowerOf2(alignment), "Alignment must be a power of 2.");


    UPT allocated_addr = (m_current_addr + (alignment - 1)) & -alignment;
    THROW_EXCEPTION_IF(pointer_to_integer(m_memory.get()) + m_memory_size < allocated_addr + size,
                       Exception,
                       "No more memory available.");

    m_current_addr = allocated_addr + size;

    ++m_num_allocations;

    return integer_to_pointer(allocated_addr);
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
void LinearMemory<t_thread_safe>::deinitialize_internal()
{
    THROW_EXCEPTION_IF(! is_initialized(), Exception, "Memory already deinitialized.");
    THROW_EXCEPTION_IF(m_num_allocations > 0, Exception, "Can't deinitialize. Memory still in use.");

    m_memory       = nullptr;
    m_current_addr = 0;
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
void LinearMemory<t_thread_safe>::initialize_internal()
{
    THROW_EXCEPTION_IF(is_initialized(), Exception, "Memory is already initialized");

    // NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
    m_memory          = std::make_unique<std::byte[]>(m_memory_size);
    m_num_allocations = 0;
    m_current_addr    = pointer_to_integer(m_memory.get());
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
auto LinearMemory<t_thread_safe>::allocate(UST size, UST alignment) -> void*
{
    return allocate_internal(size, alignment);
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

    auto alloc_size = m_current_addr - pointer_to_integer(m_memory.get());
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


} // namespace mjolnir
