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

//! \addtogroup core_memory
//! @{





//! @brief
//! STL compatible deleter for memory systems.
//!
//! @tparam T_Type:
//! Type of the objects that are deleted by this class
//! @tparam T_MemorySystem:
//! The memory system type.
template <typename T_Type, MemorySystem T_MemorySystem>
class MemorySystemDeleter
{
public:
    //! @brief
    //! The Type of the objects that are deleted by the deleter.
    using ValueType = T_Type;

    //! \cond DO_NOT_DOCUMENT
    MemorySystemDeleter()                               = delete;
    MemorySystemDeleter(const MemorySystemDeleter&)     = default;
    MemorySystemDeleter(MemorySystemDeleter&&) noexcept = default;
    ~MemorySystemDeleter()                              = default;
    auto operator=(const MemorySystemDeleter&) -> MemorySystemDeleter& = default;
    auto operator=(MemorySystemDeleter&&) noexcept -> MemorySystemDeleter& = default;
    //! \endcond


    //! @brief
    //! Construct a new deleter with the passes `LinearMemory` instance.
    //!
    //! @param[in] memory_system:
    //! Memory system that provided the memory for the object that should be deleted
    explicit MemorySystemDeleter(T_MemorySystem& memory_system) noexcept;


    //! @brief
    //! Destroy the object at the passed memory address and deallocate the memory.
    //!
    //! @param[in] pointer:
    //! Pointer to the object that should be destroyed and the memory that should be deallocated.
    void operator()(std::remove_extent_t<T_Type>* pointer) noexcept;


    //! @brief
    //! Get a deleter for another type that uses the same memory system.
    //!
    //! @tparam T_OtherType:
    //! Type of the objects that should be deleted by the new deleter
    //!
    //! @return
    //! New deleter that uses the same memory system
    template <typename T_OtherType>
    [[nodiscard]] auto as_type() const noexcept -> MemorySystemDeleter<T_OtherType, T_MemorySystem>;


    //! @brief
    //! Get a reference to the memory system that is used by the deleter
    //!
    //! @return
    //! Memory system that is used by the deleter
    [[nodiscard]] auto get_memory_system() const noexcept -> T_MemorySystem&;


private:
    T_MemorySystem& m_memory;
};


//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
// cppcheck-suppress constParameter
MemorySystemDeleter<T_Type, T_MemorySystem>::MemorySystemDeleter(T_MemorySystem& memory_system) noexcept
    : m_memory(memory_system)
{
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
void MemorySystemDeleter<T_Type, T_MemorySystem>::operator()(std::remove_extent_t<T_Type>* pointer) noexcept
{
    m_memory.destroy_deallocate(pointer);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
template <typename T_OtherType>
[[nodiscard]] auto MemorySystemDeleter<T_Type, T_MemorySystem>::as_type() const noexcept
        -> MemorySystemDeleter<T_OtherType, T_MemorySystem>
{
    return MemorySystemDeleter<T_OtherType, T_MemorySystem>(m_memory);
}


// --------------------------------------------------------------------------------------------------------------------

template <typename T_Type, MemorySystem T_MemorySystem>
[[nodiscard]] auto MemorySystemDeleter<T_Type, T_MemorySystem>::get_memory_system() const noexcept -> T_MemorySystem&
{
    return m_memory;
}


} // namespace mjolnir
