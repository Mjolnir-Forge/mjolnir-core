//! @file
//! exception.h
//!
//! @brief
//! Contains the definitions of various exception types and some macros for simple usage.


#pragma once

#include <cstring>
#include <stdexcept>

// === EXCEPTION MACROS ===============================================================================================

//! \addtogroup core
//! @{

//! @brief T
//! hrow an exception and automatically add the function that caused it.
//!
//! @param[in] message:
//! The exception message
#define THROW_EXCEPTION(message) throw mjolnir::Exception(__PRETTY_FUNCTION__, message)

//! @brief
//! Throw an exception if the passed condition is met.
//!
//! @param[in] condition:
//! If the passed condition validates as `true`, the exception is thrown.
//!
//! @param[in] message:
//! The exception message
#define THROW_EXCEPTION_IF(condition, message)                                                                         \
    if (condition) [[unlikely]]                                                                                        \
    THROW_EXCEPTION(message)

//! @}

// ====================================================================================================================


namespace mjolnir
{
//! \addtogroup core
//! @{

//! @brief Stores origion and message of an exception
class Exception : public std::runtime_error
{
public:
    Exception()           = delete;
    ~Exception() override = default;

    //! @brief
    //! Copy contstructor
    //!
    //! @param[in] other:
    //! Other instance that should be copied
    Exception(const Exception& other) = default;

    //! @brief
    //! Move constructor
    //!
    //! @param[in, out] other:
    //! Other instance that should be moved
    Exception(Exception&& other) = default;

    //! @brief
    //! Copy assignment operator
    //!
    //! @param[in] other:
    //! Other instance that should be copied
    //!
    //! @return Reference to class instance
    auto operator=(const Exception& other) -> Exception& = default;

    //! @brief
    //! Move assignment operator
    //!
    //! @param[in, out] other:
    //! Other instance that should be moved
    //!
    //! @return
    //! Reference to class instance
    auto operator=(Exception&& other) -> Exception& = default;


    //! @brief
    //! Constructor
    //!
    //! @param[in] origin:
    //! Name of the function that caused the exception
    //!
    //! @param[in] message:
    //! Exception message
    Exception(const std::string& origin, const std::string& message);
};

//! @}
} // namespace mjolnir


// ====================================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

inline Exception::Exception(const std::string& origin, const std::string& message)
    : std::runtime_error{std::string("[") + origin + std::string("] ") + message}
{
}

} // namespace mjolnir
