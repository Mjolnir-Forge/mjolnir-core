//! @file
//! memory/memory_system_allocator.h
//!
//! @brief
//! Defines an STL compatible allocator class for the memory systems of this library


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/memory/definitions.h"


namespace mjolnir
{
//! \addtogroup core_memory
//! @{


//! @brief
//! STL compatible allocator for memory systems.
//!
//! @tparam T_Type:
//! Type of the allocated object
//! @tparam T_MemorySystem:
//! The memory system type.
template <typename T_Type, MemorySystem T_MemorySystem>
class MemorySystemAllocator
{
public:
    //! @brief
    //! The Type of the objects that are allocated by the allocator.
    using ValueType = T_Type;


    //! \cond DO_NOT_DOCUMENT

    //! Required class members for allocators
    using value_type = T_Type; // NOLINT(readability-identifier-naming)

    template <typename T_OtherType>
    struct rebind // NOLINT(readability-identifier-naming)
    {
        using other = MemorySystemAllocator<T_OtherType, T_MemorySystem>; // NOLINT(readability-identifier-naming)
    };


    MemorySystemAllocator()                                 = delete;
    MemorySystemAllocator(const MemorySystemAllocator&)     = default;
    MemorySystemAllocator(MemorySystemAllocator&&) noexcept = default;
    ~MemorySystemAllocator()                                = default;
    auto operator=(const MemorySystemAllocator&) -> MemorySystemAllocator& = default;
    auto operator=(MemorySystemAllocator&&) noexcept -> MemorySystemAllocator& = default;
    //! \endcond


    //! @brief
    //! Construct a new allocator with the passed `LinearMemory` instance
    //!
    //! @param[in] linear_memory:
    //! `LinearMemory` instance that provides the memory for the allocations
    explicit MemorySystemAllocator(T_MemorySystem& linear_memory) noexcept;


    //! @brief
    //! Construct a new allocator using the same `LinearMemory` instance as the passed allocator.
    //!
    //! @tparam T_OtherType
    //! Object type of the other allocator
    //!
    //! @param[in] other:
    //! Other allocator that provides a reference to the `LinearMemory` instance that should be used
    template <class T_OtherType>
    explicit MemorySystemAllocator(const MemorySystemAllocator<T_OtherType, T_MemorySystem>& other) noexcept;


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
    //! Get an allocator with different value type that uses the same memory system.
    //!
    //! @tparam T_OtherType:
    //! Type of the objects that should be allocated by the new allocator
    //!
    //! @return
    //! New allocator that uses the same memory system
    template <typename T_OtherType>
    [[nodiscard]] auto as_type() const noexcept -> MemorySystemAllocator<T_OtherType, T_MemorySystem>;


    //! @brief
    //! Deallocates the memory of the passed pointer.
    //!
    //! @param[in] pointer:
    //! Pointer to the memory that should be freed
    //! @param[in] num_instances:
    //! Specifies how many instances of `T_Type` fitted into the allocated memory.
    void deallocate([[maybe_unused]] T_Type* pointer, UST num_instances);


    //! @brief
    //! Get a reference to the memory system that is used by the allocator.
    //!
    //! @return
    //! Memory system that is used by the allocator
    [[nodiscard]] auto get_memory_system() const noexcept -> T_MemorySystem&;


private:
    T_MemorySystem& m_memory;

    template <typename, MemorySystem>
    friend class MemorySystemAllocator;
};


//! @brief
//! Comparison operator `==` for two `MemorySystemAllocator` instances.
//!
//! @details
//! The function returns `true` if both allocators use the same memory system instance and `false` otherwise.
//!
//! @tparam T_TypeLHS:
//! Value type of the allocator on the operators left-hand side.
//! @tparam T_TypeRHS:
//! Value type of the allocator on the operators right-hand side.
//! @tparam T_MemorySystemLHS:
//! The memory system type of the allocator on the operators left-hand side.
//! @tparam T_MemorySystemRHS:
//! The memory system type of the allocator on the operators right-hand side.
//!
//! @param[in] lhs:
//! Allocator on the left-hand side of the operator
//! @param[in] rhs:
//! Allocator on the right-hand side of the operator
//!
//! @return
//! `true` or `false`
template <typename T_TypeLHS, typename T_TypeRHS, MemorySystem T_MemorySystemLHS, MemorySystem T_MemorySystemRHS>
[[nodiscard]] constexpr auto
operator==([[maybe_unused]] const MemorySystemAllocator<T_TypeLHS, T_MemorySystemLHS>& lhs,
           [[maybe_unused]] const MemorySystemAllocator<T_TypeRHS, T_MemorySystemRHS>& rhs) noexcept -> bool;


//! @brief
//! Comparison operator `!=` for two `MemorySystemAllocator` instances.
//!
//! @details
//! The function returns `false` if both allocators use the same memory system instance and `true` otherwise.
//!
//! @tparam T_TypeLHS:
//! Value type of the allocator on the operators left-hand side.
//! @tparam T_TypeRHS:
//! Value type of the allocator on the operators right-hand side.
//! @tparam T_MemorySystemLHS:
//! The memory system type of the allocator on the operators left-hand side.
//! @tparam T_MemorySystemRHS:
//! The memory system type of the allocator on the operators right-hand side.
//!
//! @param[in] lhs:
//! Allocator on the left-hand side of the operator
//! @param[in] rhs:
//! Allocator on the right-hand side of the operator
//!
//! @return
//! `true` or `false`
template <typename T_TypeLHS, typename T_TypeRHS, MemorySystem T_MemorySystemLHS, MemorySystem T_MemorySystemRHS>
[[nodiscard]] constexpr auto
operator!=([[maybe_unused]] const MemorySystemAllocator<T_TypeLHS, T_MemorySystemLHS>& lhs,
           [[maybe_unused]] const MemorySystemAllocator<T_TypeRHS, T_MemorySystemRHS>& rhs) noexcept -> bool;


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
MemorySystemAllocator<T_Type, T_MemorySystem>::MemorySystemAllocator(T_MemorySystem& linear_memory) noexcept
    : m_memory{linear_memory}
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
template <class T_OtherType>
MemorySystemAllocator<T_Type, T_MemorySystem>::MemorySystemAllocator(
        const MemorySystemAllocator<T_OtherType, T_MemorySystem>& other) noexcept
    : m_memory{other.m_memory}
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
[[nodiscard]] auto MemorySystemAllocator<T_Type, T_MemorySystem>::allocate(UST num_instances) -> T_Type*
{
    return static_cast<T_Type*>(m_memory.allocate(num_instances * sizeof(T_Type), alignof(T_Type)));
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
template <typename T_OtherType>
[[nodiscard]] auto MemorySystemAllocator<T_Type, T_MemorySystem>::as_type() const noexcept
        -> MemorySystemAllocator<T_OtherType, T_MemorySystem>
{
    return MemorySystemAllocator<T_OtherType, T_MemorySystem>(m_memory);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
void MemorySystemAllocator<T_Type, T_MemorySystem>::deallocate(T_Type* pointer, UST num_instances)
{
    m_memory.deallocate(pointer, num_instances * sizeof(T_Type), alignof(T_Type));
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
[[nodiscard]] auto MemorySystemAllocator<T_Type, T_MemorySystem>::get_memory_system() const noexcept -> T_MemorySystem&
{
    return m_memory;
}


// --------------------------------------------------------------------------------------------------------------------


template <typename T_TypeLHS, typename T_TypeRHS, MemorySystem T_MemorySystemLHS, MemorySystem T_MemorySystemRHS>
[[nodiscard]] constexpr auto
operator==([[maybe_unused]] const MemorySystemAllocator<T_TypeLHS, T_MemorySystemLHS>& lhs,
           [[maybe_unused]] const MemorySystemAllocator<T_TypeRHS, T_MemorySystemRHS>& rhs) noexcept -> bool
{
    if constexpr (std::is_same_v<T_MemorySystemLHS, T_MemorySystemRHS>)
        return &(lhs.get_memory_system()) == &(rhs.get_memory_system());
    else
        return false;
}


// --------------------------------------------------------------------------------------------------------------------


template <typename T_TypeLHS, typename T_TypeRHS, MemorySystem T_MemorySystemLHS, MemorySystem T_MemorySystemRHS>
[[nodiscard]] constexpr auto
operator!=([[maybe_unused]] const MemorySystemAllocator<T_TypeLHS, T_MemorySystemLHS>& lhs,
           [[maybe_unused]] const MemorySystemAllocator<T_TypeRHS, T_MemorySystemRHS>& rhs) noexcept -> bool
{
    return ! (lhs == rhs);
}

} // namespace mjolnir
