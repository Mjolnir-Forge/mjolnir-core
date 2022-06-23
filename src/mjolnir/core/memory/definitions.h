//! @file
//! memory/definitions.h
//!
//! @brief
//! Definitions and concepts tied to memory management


#pragma once


// === DECLARATIONS ===================================================================================================

#include "mjolnir/core/fundamental_types.h"

#include <concepts>
#include <memory>


namespace mjolnir
{
//! \addtogroup core_memory
//! @{

//! @brief
//! The default deleter type for all memory systems
// NOLINTNEXTLINE(cppcoreguidelines-avoid-c-arrays,hicpp-avoid-c-arrays,modernize-avoid-c-arrays)
using DefaultMemoryDeleter = std::default_delete<std::byte[]>;

//! @brief
//! Concept for a memory system
//!
//! @tparam T_Type
//! Type
// clang-format off
template <typename T_Type>
concept MemorySystem = requires(T_Type t, void* ptr, UST size, UST alignment)
{
    {t.allocate(size, alignment)} -> std::same_as<void*>;
    {t.deallocate(ptr, size, alignment)} -> std::same_as<void>;
};
// clang-format on

// template <typename T_Type, typename T_Object_Type>
// concept TypedMemorySystem = MemorySystem<T_Type> && requires(T_Type t, T_Object_Type* ptr)
//{
//    {t.destroy_deallocate(ptr)};
//};


// template <typename T_Type>
// concept


//! @}
} // namespace mjolnir
