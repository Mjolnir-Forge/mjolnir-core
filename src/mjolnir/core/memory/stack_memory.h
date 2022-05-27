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
    //! @param[in] size_in_bytes
    StackMemory(UST size_in_bytes);
};


//! @}
} // namespace mjolnir


// ====================================================================================================================


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
    // CheckConstructionParameters();
}


// --------------------------------------------------------------------------------------------------------------------

} // namespace mjolnir
