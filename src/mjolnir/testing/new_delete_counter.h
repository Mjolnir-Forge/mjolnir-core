//! @file
//! new_delete_counter.h
//!
//! @brief
//! Contains a class that keeps track of the number of new and delete calls.


#pragma once


// === MACROS =========================================================================================================

#if defined(_MSC_VER)
#    include <malloc.h>
#    define ALIGNED_ALLOC(alignment, size) _aligned_malloc(size, alignment)
#    define ALIGNED_FREE _aligned_free
#elif defined(__GNUC__)
#    include <cstdlib>
#    define ALIGNED_ALLOC(alignment, size) std::aligned_alloc(alignment, size) // NOLINT(cppcoreguidelines-macro-usage)
#    define ALIGNED_FREE std::free                                             // NOLINT(cppcoreguidelines-macro-usage)
#else
static_assert(false, "Incompatible compiler");
#endif


#ifndef DISABLE_NEW_DELETE_COUNTER


//! @brief
//! Assert that the number of delete calls are equal to the passed number
//!
//! @details
//! Requires a variable with name `new_delete_counter` and the type `NewDeleteCounter` that is valid in the current
//! scope. The macro `COUNT_NEW_AND_DELETE` performs the required initialization. This macro should only be
//! used in a test using `gtest.h` and relies on `ASSERT_EQ`. The test is passed if the number of delete calls since the
//! macro `COUNT_NEW_AND_DELETE` was used is equal to the passed number. Otherwise, the test fails immediately.
//!
//!
//! If `DISABLE_NEW_DELETE_COUNTER` is defined, this macro does nothing.
//!
//! @param num_delete_exp:
//! Expected number of delete calls
#    define ASSERT_NUM_DELETE_EQ(num_delete_exp) ASSERT_EQ(new_delete_counter.get_num_delete_calls(), num_delete_exp)


// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! Check if the number of new and delete calls are equal to the passed numbers
//!
//! @details
//! Requires a variable with name `new_delete_counter` and the type `NewDeleteCounter` that is valid in the current
//! scope. The macro `COUNT_NEW_AND_DELETE` performs the required initialization. This macro should only be
//! used in a test using `gtest.h` and relies on `EXPECT_EQ`. The test is passed if the number of new and delete calls
//! since the macro `COUNT_NEW_AND_DELETE` was used are equal to the passed numbers. Otherwise, the test fails.
//!
//!
//! If `DISABLE_NEW_DELETE_COUNTER` is defined, this macro does nothing.
//!
//! @param num_new_exp:
//! Expected number of new calls
//! @param num_delete_exp:
//! Expected number of delete calls
#    define ASSERT_NUM_NEW_AND_DELETE_EQ(num_new_exp, num_delete_exp)                                                  \
        ASSERT_NUM_NEW_EQ(num_new_exp);                                                                                \
        ASSERT_NUM_DELETE_EQ(num_delete_exp)


// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! Assert the number of new calls are equal to the passed number
//!
//! @details
//! Requires a variable with name `new_delete_counter` and the type `NewDeleteCounter` that is valid in the current
//! scope. The macro `COUNT_NEW_AND_DELETE` performs the required initialization. This macro should only be
//! used in a test using `gtest.h` and relies on `ASSERT_EQ`. The test is passed if the number of new calls since the
//! macro `COUNT_NEW_AND_DELETE` was used is equal to the passed number. Otherwise, the test fails immediately.
//!
//! If `DISABLE_NEW_DELETE_COUNTER` is defined, this macro does nothing.
//!
//! @param num_new_exp:
//! Expected number of new calls
#    define ASSERT_NUM_NEW_EQ(num_new_exp) ASSERT_EQ(new_delete_counter.get_num_new_calls(), num_new_exp)


// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! Creates a new `NewDeleteCounter` and stores it in a variable with a specific name that is needed by other macros.
//!
//! @details
//! If `DISABLE_NEW_DELETE_COUNTER` is defined, this macro does nothing.
#    define COUNT_NEW_AND_DELETE auto new_delete_counter = NewDeleteCounter()


// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! Check if the number of delete calls are equal to the passed number
//!
//! @details
//! Requires a variable with name `new_delete_counter` and the type `NewDeleteCounter` that is valid in the current
//! scope. The macro `COUNT_NEW_AND_DELETE` performs the required initialization. This macro should only be
//! used in a test using `gtest.h` and relies on `EXPECT_EQ`. The test is passed if the number of delete calls since the
//! macro `COUNT_NEW_AND_DELETE` was used is equal to the passed number. Otherwise, the test fails.
//!
//!
//! If `DISABLE_NEW_DELETE_COUNTER` is defined, this macro does nothing.
//!
//! @param num_delete_exp:
//! Expected number of delete calls
//!
//! @remark
//! In case that a check fails, `EXPECT_EQ` will perform a lot of allocations. In result, all subsequent checks will not
//! only fail too but the presented number of calls will also be significantly higher than expected.
#    define EXPECT_NUM_DELETE_EQ(num_delete_exp) EXPECT_EQ(new_delete_counter.get_num_delete_calls(), num_delete_exp)


// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! Check if the number of new and delete calls are equal to the passed numbers
//!
//! @details
//! Requires a variable with name `new_delete_counter` and the type `NewDeleteCounter` that is valid in the current
//! scope. The macro `COUNT_NEW_AND_DELETE` performs the required initialization. This macro should only be
//! used in a test using `gtest.h` and relies on `EXPECT_EQ`. The test is passed if the number of new and delete calls
//! since the macro `COUNT_NEW_AND_DELETE` was used are equal to the passed numbers. Otherwise, the test fails.
//!
//!
//! If `DISABLE_NEW_DELETE_COUNTER` is defined, this macro does nothing.
//!
//! @param num_new_exp:
//! Expected number of new calls
//! @param num_delete_exp:
//! Expected number of delete calls
//!
//! @remark
//! In case that a check fails, `EXPECT_EQ` will perform a lot of allocations. In result, all subsequent checks will not
//! only fail too but the presented number of calls will also be significantly higher than expected.
#    define EXPECT_NUM_NEW_AND_DELETE_EQ(num_new_exp, num_delete_exp)                                                  \
        EXPECT_NUM_NEW_EQ(num_new_exp);                                                                                \
        EXPECT_NUM_DELETE_EQ(num_delete_exp)


// --------------------------------------------------------------------------------------------------------------------

//! @brief
//! Check if the number of new calls are equal to the passed number
//!
//! @details
//! Requires a variable with name `new_delete_counter` and the type `NewDeleteCounter` that is valid in the current
//! scope. The macro `COUNT_NEW_AND_DELETE` performs the required initialization. This macro should only be
//! used in a test using `gtest.h` and relies on `EXPECT_EQ`. The test is passed if the number of new calls since the
//! macro `COUNT_NEW_AND_DELETE` was used is equal to the passed number. Otherwise, the test fails.
//!
//! If `DISABLE_NEW_DELETE_COUNTER` is defined, this macro does nothing.
//!
//! @param num_new_exp:
//! Expected number of new calls
//!
//! @remark
//! In case that a check fails, `EXPECT_EQ` will perform a lot of allocations. In result, all subsequent checks will not
//! only fail too but the presented number of calls will also be significantly higher than expected.
#    define EXPECT_NUM_NEW_EQ(num_new_exp) EXPECT_EQ(new_delete_counter.get_num_new_calls(), num_new_exp)


#else
#    define ASSERT_NUM_DELETE_EQ(num_delete_exp)
#    define ASSERT_NUM_NEW_AND_DELETE_EQ(num_new_exp, num_delete_exp)
#    define ASSERT_NUM_NEW_EQ(num_new_exp)
#    define COUNT_NEW_AND_DELETE
#    define EXPECT_NUM_DELETE_EQ(num_delete_exp)
#    define EXPECT_NUM_NEW_AND_DELETE_EQ(num_new_exp, num_delete_exp)
#    define EXPECT_NUM_NEW_EQ(num_new_exp)
#endif


// === DECLARATIONS ===================================================================================================


#include "mjolnir/core/fundamental_types.h"

#include <atomic>
#include <iostream>


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
//! Define `DISABLE_NEW_DELETE_COUNTER` if you like to run them on files that include this class.
class NewDeleteCounter
{
public:
    NewDeleteCounter();
    NewDeleteCounter(const NewDeleteCounter&) = delete;
    NewDeleteCounter(NewDeleteCounter&&)      = delete;
    ~NewDeleteCounter()                       = default;

    auto operator=(const NewDeleteCounter&) -> NewDeleteCounter& = delete;
    auto operator=(NewDeleteCounter&&) -> NewDeleteCounter& = delete;


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
    //! Prints the number of new and delete calls since the construction of the class instance
    void print_num_calls() const noexcept;


private:
    //! @brief
    //! Increases the delete counter
    static void increase_total_delete_calls() noexcept;


    //! @brief
    //! Increases the new counter
    static void increase_total_new_calls() noexcept;


    //! @brief
    //! Return `-1` if `DISABLE_NEW_DELETE_COUNTER` is defined and the passed value otherwise.
    //!
    //! @param[in] value:
    //! Value that should be returned
    //!
    //! @return
    //! Passed value or -1
    [[nodiscard]] static auto return_value([[maybe_unused]] I32 value) noexcept -> I32;


    std::atomic<I32> m_num_new_at_construction = -1;
    std::atomic<I32> m_num_del_at_construction = -1;

    inline static std::atomic<I32> m_num_new_global = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)
    inline static std::atomic<I32> m_num_del_global = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)


#ifndef DISABLE_NEW_DELETE_COUNTER
    friend auto ::operator new(std::size_t size) -> void*;
    friend auto ::operator new(std::size_t size, std::align_val_t al) -> void*;
    friend auto ::operator new(std::size_t size, const std::nothrow_t&) noexcept -> void*;
    friend auto ::operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept -> void*;
    friend auto ::operator new[](std::size_t size) -> void*;
    friend auto ::operator new[](std::size_t size, std::align_val_t al) -> void*;
    friend auto ::operator new[](std::size_t size, const std::nothrow_t&) noexcept -> void*;
    friend auto ::operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept -> void*;
    friend void ::operator delete(void* ptr) noexcept;
    friend void ::operator delete(void* ptr, const std::nothrow_t&) noexcept;
    friend void ::operator delete[](void* ptr) noexcept;
    friend void ::operator delete[](void* ptr, const std::nothrow_t&) noexcept;
    friend void ::operator delete(void* ptr, std::align_val_t al) noexcept;
    friend void ::operator delete[](void* ptr, std::align_val_t al) noexcept;
    friend void ::operator delete(void* ptr, std::align_val_t al, const std::nothrow_t& tag) noexcept;
    friend void ::operator delete[](void* ptr, std::align_val_t al, const std::nothrow_t& tag) noexcept;
#    ifdef __cpp_sized_deallocation
    friend void ::operator delete(void* ptr, std::size_t sz) noexcept;
    friend void ::operator delete[](void* ptr, std::size_t sz) noexcept;
    friend void ::operator delete(void* ptr, std::size_t sz, std::align_val_t al) noexcept;
    friend void ::operator delete[](void* ptr, std::size_t sz, std::align_val_t al) noexcept;
#    endif // __cpp_sized_deallocation
#endif     // DISABLE_NEW_DELETE_COUNTER
};


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

inline NewDeleteCounter::NewDeleteCounter()
    : m_num_new_at_construction(m_num_new_global.load())
    , m_num_del_at_construction(m_num_del_global.load())
{
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto NewDeleteCounter::get_num_delete_calls() const noexcept -> I32
{
    return return_value(m_num_del_global - m_num_del_at_construction);
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto NewDeleteCounter::get_num_new_calls() const noexcept -> I32
{
    return return_value(m_num_new_global - m_num_new_at_construction);
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto NewDeleteCounter::get_total_num_delete_calls() noexcept -> I32
{
    return return_value(m_num_del_global);
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto NewDeleteCounter::get_total_num_new_calls() noexcept -> I32
{
    return return_value(m_num_new_global);
}


// --------------------------------------------------------------------------------------------------------------------

// NOLINTNEXTLINE(readability-convert-member-functions-to-static)
inline void NewDeleteCounter::print_num_calls() const noexcept
{
#ifndef DISABLE_NEW_DELETE_COUNTER
    std::cout << "Number of new calls    : " << get_num_new_calls() << std::endl;
    std::cout << "Number of delete calls : " << get_num_delete_calls() << std::endl;
#else
    std::cout << "Global new counter disabled." << std::endl;
#endif // DISABLE_NEW_DELETE_COUNTER
}


// --------------------------------------------------------------------------------------------------------------------

inline void NewDeleteCounter::increase_total_delete_calls() noexcept
{
    ++m_num_del_global;
}


// --------------------------------------------------------------------------------------------------------------------

inline void NewDeleteCounter::increase_total_new_calls() noexcept
{
    ++m_num_new_global;
}


// --------------------------------------------------------------------------------------------------------------------

[[nodiscard]] inline auto NewDeleteCounter::return_value([[maybe_unused]] I32 value) noexcept -> I32
{
#ifndef DISABLE_NEW_DELETE_COUNTER
    return value;
#else
    return -1;
#endif // DISABLE_NEW_DELETE_COUNTER
}


// --------------------------------------------------------------------------------------------------------------------


} // namespace mjolnir


// === Overloads of new ===============================================================================================


//! \cond DO_NOT_DOCUMENT

#ifndef DISABLE_NEW_DELETE_COUNTER
auto operator new(std::size_t size) -> void*
{
    void* p = malloc(size);
    if (! p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE

    mjolnir::NewDeleteCounter::increase_total_new_calls();
    return p;
}

auto operator new[](std::size_t size) -> void*
{
    void* p = malloc(size);
    if (! p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE

    mjolnir::NewDeleteCounter::increase_total_new_calls();
    return p;
}

auto operator new(std::size_t size, const std::nothrow_t&) noexcept -> void*
{
    mjolnir::NewDeleteCounter::increase_total_new_calls();
    return malloc(size);
}

auto operator new[](std::size_t size, const std::nothrow_t&) noexcept -> void*
{
    mjolnir::NewDeleteCounter::increase_total_new_calls();
    return malloc(size);
}

auto operator new(std::size_t size, std::align_val_t al) -> void*
{
    void* p = ALIGNED_ALLOC(static_cast<size_t>(al), size);
    if (! p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE

    mjolnir::NewDeleteCounter::increase_total_new_calls();
    return p;
}

auto operator new[](std::size_t size, std::align_val_t al) -> void*
{
    void* p = ALIGNED_ALLOC(static_cast<size_t>(al), size);
    if (! p)
        throw std::bad_alloc(); // LCOV_EXCL_LINE

    mjolnir::NewDeleteCounter::increase_total_new_calls();
    return p;
}

auto operator new(std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept -> void*
{
    mjolnir::NewDeleteCounter::increase_total_new_calls();
    return ALIGNED_ALLOC(static_cast<size_t>(al), size);
}

auto operator new[](std::size_t size, std::align_val_t al, const std::nothrow_t&) noexcept -> void*
{
    mjolnir::NewDeleteCounter::increase_total_new_calls();
    return ALIGNED_ALLOC(static_cast<size_t>(al), size);
}

void operator delete(void* ptr) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete[](void* ptr) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete(void* ptr, const std::nothrow_t&) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete[](void* ptr, const std::nothrow_t&) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    free(ptr);
}


#    ifdef __cpp_sized_deallocation
void operator delete(void* ptr, [[maybe_unused]] std::size_t sz) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t sz) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete(void* ptr, [[maybe_unused]] std::size_t sz, [[maybe_unused]] std::align_val_t al) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    free(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t sz, [[maybe_unused]] std::align_val_t al) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    free(ptr);
}
#    endif // __cpp_sized_deallocation

void operator delete(void* ptr, [[maybe_unused]] std::align_val_t al) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    ALIGNED_FREE(ptr);
}

void operator delete[](void* ptr, [[maybe_unused]] std::align_val_t al) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    ALIGNED_FREE(ptr);
}

void operator delete(void*                                  ptr,
                     [[maybe_unused]] std::align_val_t      al,
                     [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    ALIGNED_FREE(ptr);
}

void operator delete[](void*                                  ptr,
                       [[maybe_unused]] std::align_val_t      al,
                       [[maybe_unused]] const std::nothrow_t& tag) noexcept
{
    mjolnir::NewDeleteCounter::increase_total_delete_calls();
    ALIGNED_FREE(ptr);
}

#endif // DISABLE_NEW_DELETE_COUNTER

//! \endcond
