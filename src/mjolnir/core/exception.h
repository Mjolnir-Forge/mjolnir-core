//! @file
//! exception.h
//!
//! @brief
//! Contains the definitions of various exception types and some macros for simple usage.


#pragma once

#include <cstring>
#include <stdexcept>


namespace mjolnir
{
//! \addtogroup core
//! @{

//! @brief Stores origion and message of an exception
class Exception : public std::runtime_error
{

public:
    Exception() = delete;
    Exception(const Exception& other) = default;
    Exception(Exception&& other) = default;
    auto operator=(const Exception& other) ->Exception& = default;
    auto operator=(Exception&& other)->Exception& = default;
    virtual ~Exception() = default;

    //! @brief Constructor
    //! @param origin: Name of the function that caused the exception
    //! @param message: Exception message
    Exception(std::string origin, std::string message)
        : std::runtime_error{std::string("[") + origin + std::string("] ") + message}
    {
    }
};

//! @}
} // namespace mjolnir


// ====================================================================================================================
