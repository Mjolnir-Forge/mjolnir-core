//! @file
//! heap_allocation_counter.h
//!
//! @brief
//! Contains a class that keeps track of the number of heap allocations.


#pragma once


#include "mjolnir/core/fundamental_types.h"

#include <atomic>
#include <iostream>


#if defined(_MSC_VER)
#    include <malloc.h>
#    define ALIGNED_ALLOC(alignment, size) _aligned_malloc(size, alignment)
#    define ALIGNED_FREE _aligned_free
#elif defined(__GNUC__)
#    include <cstdlib>
#    define ALIGNED_ALLOC(alignment, size) std::aligned_alloc(alignment, size)
#    define ALIGNED_FREE free
#else
static_assert(false, "Incompatible compiler");
#endif


namespace mjolnir
{
//! \addtogroup testing
//! @{


//! @brief
//! Counts the number of new and delete calls since the instance was created.
//!
//! @details
//! This header overrides the default implementations of `new` and `delete` and should only be used in tests.
//!
//! @remark
//! The necessary overloads of `new` and `delete` interfere with valgrind and other tools that override those operators.
//! Define `DISABLE_HEAP_ALLOCATION_COUNTER` if you like to run them on files that include this class.
class HeapAllocationCounter
{
    std::atomic<I32> m_num_new_at_construction    = -1;
    std::atomic<I32> m_num_delete_at_construction = -1;

    static std::atomic<I32> m_num_new_global;
    static std::atomic<I32> m_num_delete_global;


#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
    friend void* ::operator new(std::size_t size);
    friend void* ::operator new(std::size_t size, std::align_val_t al);
    friend void* ::operator new(std::size_t size, const std::nothrow_t&) noexcept;
    friend void* ::operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept;
    friend void* ::operator new[](std::size_t size);
    friend void* ::operator new[](std::size_t size, std::align_val_t al);
    friend void* ::operator new[](std::size_t size, const std::nothrow_t&) noexcept;
    friend void* ::operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept;
    friend void :: operator delete(void* ptr) noexcept;
    friend void :: operator delete(void* ptr, const std::nothrow_t&) noexcept;
    friend void :: operator delete[](void* ptr) noexcept;
    friend void :: operator delete[](void* ptr, const std::nothrow_t&) noexcept;
    friend void :: operator delete(void* ptr, std::align_val_t al) noexcept;
    friend void :: operator delete[](void* ptr, std::align_val_t al) noexcept;
    friend void :: operator delete(void* ptr, std::align_val_t al, const std::nothrow_t& tag) noexcept;
    friend void :: operator delete[](void* ptr, std::align_val_t al, const std::nothrow_t& tag) noexcept;
#    ifdef __cpp_sized_deallocation
    friend void ::operator delete(void* ptr, std::size_t sz) noexcept;
    friend void ::operator delete[](void* ptr, std::size_t sz) noexcept;
    friend void ::operator delete(void* ptr, std::size_t sz, std::align_val_t al) noexcept;
    friend void ::operator delete[](void* ptr, std::size_t sz, std::align_val_t al) noexcept;
#    endif // __cpp_sized_deallocation
#endif     // DISABLE_HEAP_ALLOCATION_COUNTER


public:
    HeapAllocationCounter();
    HeapAllocationCounter(const HeapAllocationCounter&) = delete;
    HeapAllocationCounter(HeapAllocationCounter&&)      = delete;
    ~HeapAllocationCounter()                            = default;

    auto operator=(const HeapAllocationCounter&) -> HeapAllocationCounter& = delete;
    auto operator=(HeapAllocationCounter&&) -> HeapAllocationCounter& = delete;


private:
    //! @brief
    //! Increases the delete counter
    static void increase_total_delete_calls() noexcept;


    //! @brief
    //! Increases the new counter
    static void increase_total_new_calls() noexcept;


    //! @brief
    //! Return `-1` if `DISABLE_HEAP_ALLOCATION_COUNTER` is defined and the passed value otherwise.
    //!
    //! @param[in] value:
    //! Value that should be returned
    //!
    //! @return
    //! Passed value or -1
    [[nodiscard]] static auto return_value([[maybe_unused]] I32 value) noexcept -> I32;


public:
    //! @brief
    //! Gets the number of delete calls since the construction of the instance.
    //!
    //! @return
    //! Number of delete calls since the construction of the instance
    [[nodiscard]] auto get_num_delete_calls() const noexcept -> I32;


    //! @brief
    //! Gets the number of new calls since the construction of the instance.
    //!
    //! @return
    //! Number of new calls since the construction of the instance
    [[nodiscard]] auto get_num_new_calls() const noexcept -> I32;


    //! @brief
    //! Gets the total number of delete calls of the program.
    //!
    //! @return
    //! Total number of delete calls of the program
    [[nodiscard]] static auto get_total_num_delete_calls() noexcept -> I32;


    //! @brief
    //! Gets the total number of new calls of the program.
    //!
    //! @return
    //! Total number of new calls of the program
    [[nodiscard]] static auto get_total_num_new_calls() noexcept -> I32;


    //! @brief
    //! Return `true` if the number of delete calls is equal to the passed value and `false` otherwise.
    //!
    //! @param[in] exp_num_delete:
    //! The expected number of delete calls
    //!
    //! @return
    //! `true` or `false`
    //!
    //! @remark
    //! Returns always `true` if `DISABLE_HEAP_ALLOCATION_COUNTER` is defined
    [[nodiscard]] auto is_num_delete_calls_equal_to([[maybe_unused]] I32 exp_num_delete) const noexcept -> bool;


    //! @brief
    //! Return `true` if the number of new calls is equal to the passed value and `false` otherwise.
    //!
    //! @param[in] exp_num_new:
    //! The expected number of new calls
    //!
    //! @return
    //! `true` or `false`
    //!
    //! @remark
    //! Returns always `true` if `DISABLE_HEAP_ALLOCATION_COUNTER` is defined
    [[nodiscard]] auto is_num_new_calls_equal_to([[maybe_unused]] I32 exp_num_new) const noexcept -> bool;


    //! @brief
    //! Prints the number of new and delete calls since the construction of the class instance
    void print_num_calls() const noexcept;
};


//! @}
} // namespace mjolnir


// ====================================================================================================================


namespace mjolnir
{
std::atomic<I32> HeapAllocationCounter::m_num_new_global    = 0;
std::atomic<I32> HeapAllocationCounter::m_num_delete_global = 0;


// --------------------------------------------------------------------------------------------------------------------

HeapAllocationCounter::HeapAllocationCounter()
    : m_num_new_at_construction(m_num_new_global.load())
    , m_num_delete_at_construction(m_num_delete_global.load())
{
}


// --------------------------------------------------------------------------------------------------------------------

inline void HeapAllocationCounter::increase_total_delete_calls() noexcept
{
    ++m_num_delete_global;
}


// --------------------------------------------------------------------------------------------------------------------

inline void HeapAllocationCounter::increase_total_new_calls() noexcept
{
    ++m_num_new_global;
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto HeapAllocationCounter::return_value(I32 value) noexcept -> I32
{
#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
    return value;
#else
    return -1;
#endif // DISABLE_HEAP_ALLOCATION_COUNTER
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto HeapAllocationCounter::get_num_delete_calls() const noexcept -> I32
{
    return return_value(m_num_delete_global - m_num_delete_at_construction);
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto HeapAllocationCounter::get_num_new_calls() const noexcept -> I32
{
    return return_value(m_num_new_global - m_num_new_at_construction);
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto HeapAllocationCounter::get_total_num_delete_calls() noexcept -> I32
{
    return return_value(m_num_delete_global);
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto HeapAllocationCounter::get_total_num_new_calls() noexcept -> I32
{
    return return_value(m_num_new_global);
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] auto
HeapAllocationCounter::is_num_delete_calls_equal_to([[maybe_unused]] I32 exp_num_delete) const noexcept -> bool
{
#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
    return get_num_delete_calls() == exp_num_delete;
#else
    return true;
#endif // DISABLE_HEAP_ALLOCATION_COUNTER
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] auto HeapAllocationCounter::is_num_new_calls_equal_to([[maybe_unused]] I32 exp_num_new) const noexcept
        -> bool
{
#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
    return get_num_new_calls() == exp_num_new;
#else
    return true;
#endif // DISABLE_HEAP_ALLOCATION_COUNTER
}


// --------------------------------------------------------------------------------------------------------------------

inline void HeapAllocationCounter::print_num_calls() const noexcept
{
#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
    std::cout << "Number of new calls    : " << get_num_new_calls() << std::endl;
    std::cout << "Number of delete calls : " << get_num_delete_calls() << std::endl;
#else
    std::cout << "Global new counter disabled." << std::endl;
#endif // DISABLE_HEAP_ALLOCATION_COUNTER
}


// --------------------------------------------------------------------------------------------------------------------


} // namespace mjolnir


// === Overloads of new ===============================================================================================


//! \cond DO_NOT_DOCUMENT

#ifndef DISABLE_HEAP_ALLOCATION_COUNTER
void* operator new(std::size_t size)
{
    void* p = malloc(size);
    if (! p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE

    mjolnir::HeapAllocationCounter::increase_total_new_calls();
    return p;
}

void* operator new[](std::size_t size)
{
    void* p = malloc(size);
    if (! p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE

    mjolnir::HeapAllocationCounter::increase_total_new_calls();
    return p;
}

void* operator new(std::size_t size, const std::nothrow_t&) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_new_calls();
    return malloc(size);
}

void* operator new[](std::size_t size, const std::nothrow_t&) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_new_calls();
    return malloc(size);
}

void* operator new(std::size_t size, std::align_val_t al)
{
    void* p = ALIGNED_ALLOC(static_cast<size_t>(al), size);
    if (! p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE

    mjolnir::HeapAllocationCounter::increase_total_new_calls();
    return p;
}

void* operator new[](std::size_t size, std::align_val_t al)
{
    void* p = ALIGNED_ALLOC(static_cast<size_t>(al), size);
    if (! p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE

    mjolnir::HeapAllocationCounter::increase_total_new_calls();
    return p;
}

void* operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_new_calls();
    return ALIGNED_ALLOC(static_cast<size_t>(al), size);
}

void* operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_new_calls();
    return ALIGNED_ALLOC(static_cast<size_t>(al), size);
}

void operator delete(void* ptr) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    free(ptr);
}


#    ifdef __cpp_sized_deallocation
void operator delete(void* ptr, [[maybe_unused]] std::size_t sz) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t sz) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] std::size_t sz, [[maybe_unused]] std::align_val_t al) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t sz, [[maybe_unused]] std::align_val_t al) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    free(ptr);
}
#    endif // __cpp_sized_deallocation

void operator delete(void* ptr, [[maybe_unused]] std::align_val_t al) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    ALIGNED_FREE(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::align_val_t al) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    ALIGNED_FREE(ptr);
}

void operator delete(void*                                  ptr,
                     [[maybe_unused]] std::align_val_t      al,
                     [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    ALIGNED_FREE(ptr);
}

void operator delete[](void*                                  ptr,
                       [[maybe_unused]] std::align_val_t      al,
                       [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
    mjolnir::HeapAllocationCounter::increase_total_delete_calls();
    ALIGNED_FREE(ptr);
}

#endif // DISABLE_HEAP_ALLOCATION_COUNTER

//! \endcond
