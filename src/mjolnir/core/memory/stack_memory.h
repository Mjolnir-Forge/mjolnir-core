//! @file
//! memory/stack_memory.h
//!
//! @brief
//! Defines a management class for stack based memory


#pragma once

#include "mjolnir/core/fundamental_types.h"

#include <memory>


namespace mjolnir
{
//! \addtogroup core_memory
//! @{

//! @brief
//! A stack based memory system
//!
//! @tparam t_thread_safe:
//! If `true`, a thread safety mechanism is added.
template <bool t_thread_safe = false>
class StackMemory
{
    UST                   m_memory_size;
    UST                   m_num_allocations;
    U8*                   m_current_memory_ptr;
    std::unique_ptr<U8[]> m_memory;


public:
    StackMemory()                   = delete;
    StackMemory(const StackMemory&) = delete;
    StackMemory(StackMemory&&)      = delete;
    StackMemory& operator=(const StackMemory&) = delete;
    StackMemory& operator=(StackMemory&&) = delete;
    ~StackMemory()                        = default;

    //! @brief
    //! Construct a new instance with the specified memory size.
    //!
    //! @param[in] size_in_bytes:
    //! The desired size of the memory stack. Note that the memory is not allocated until the `initialize` method is
    //! called.
    explicit StackMemory(UST size_in_bytes);

private:
public:
    //! @brief
    //! Get the size of the allocated memory.
    //!
    //! @details
    //! If the memory was not initialized using `initialize`, this method will return 0
    //!
    //! @return
    //! Size of the memory
    [[nodiscard]] auto get_memory_size() const noexcept -> UST;
};


//! @}
} // namespace mjolnir


// ====================================================================================================================


#include "mjolnir/core/exception.h"


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
StackMemory<t_thread_safe>::StackMemory(UST size_in_bytes)
    : m_memory_size{size_in_bytes}
    , m_num_allocations{0}
    , m_current_memory_ptr{nullptr}
    , m_memory{nullptr}
{
    THROW_EXCEPTION_IF(m_memory_size < 1, Exception, "Memory size can't be 0.");
}


// --------------------------------------------------------------------------------------------------------------------

template <bool t_thread_safe>
[[nodiscard]] auto StackMemory<t_thread_safe>::get_memory_size() const noexcept -> UST
{
    if (m_memory)
        return m_memory_size;
    return 0;
}


} // namespace mjolnir
