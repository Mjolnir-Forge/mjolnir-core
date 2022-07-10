//! @file
//! memory_test_classes.h
//!
//! @brief
//! Contains structs and classes that are frequently used during memory related tests.


#pragma once


// === DECLARATIONS ===================================================================================================


#include "mjolnir/core/fundamental_types.h"


namespace mjolnir
{
//! \addtogroup testing
//! @{


//! @brief Alignment requirement of the `AlignedStruct`
constexpr UST struct_alignment = 32;


//! @brief
//! Struct that is smaller in size than its alignment requirement.
//!
//! @details
//! This is useful to test if a memory system handles alignment requirements correctly.
struct alignas(struct_alignment) AlignedStruct
{
    //! \cond DO_NOT_DOCUMENT
    I64 m_member_a = 0;
    I64 m_member_b = 0;
    //! \endcond
};


//! @brief
//! Class that increases an external counter variable during destruction.
//!
//! @details
//! This class is used to test if a custom deleter calls the destructor during deletion. It takes a reference to an
//! integer during construction and increases it by 1 if the class gets destroyed.
class DestructionTester
{
public:
    //! \cond DO_NOT_DOCUMENT
    DestructionTester()                             = delete;
    DestructionTester(const DestructionTester&)     = default;
    DestructionTester(DestructionTester&&) noexcept = default;
    auto operator=(const DestructionTester&) -> DestructionTester& = delete;
    auto operator=(DestructionTester&&) noexcept -> DestructionTester& = delete;
    //! \endcond

    //! @brief
    //! Constructor
    //!
    //! @param[in] destruction_count:
    //! Reference to an integer that is increased if the destructor of this class is called
    explicit DestructionTester(UST& destruction_count);

    //! @brief
    //! Destructor
    //!
    //! @details
    //! Increases the referenced integer
    ~DestructionTester();

private:
    UST& m_destruction_count;
};

//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

inline DestructionTester::DestructionTester(UST& destruction_count) : m_destruction_count{destruction_count}
{
}


// --------------------------------------------------------------------------------------------------------------------

inline DestructionTester::~DestructionTester()
{
    ++m_destruction_count;
};

} // namespace mjolnir
