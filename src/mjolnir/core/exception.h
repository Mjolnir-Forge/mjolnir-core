//! @file
//! exception.h
//!
//! @brief
//! Contains the definitions of various exception types and some macros for simple usage.


#pragma once

#include <cstring>
#include <stdexcept>

// === EXCEPTION MACROS ===============================================================================================


#if defined(_MSC_VER)
#    define FUNCTION_SIG __FUNCSIG__
#elif defined(__GNUC__)
// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-array-to-pointer-decay,hicpp-no-array-decay)
#    define FUNCTION_SIG __PRETTY_FUNCTION__ // NOLINT(cppcoreguidelines-macro-usage)
#else
static_assert(false, "Incompatible compiler");
#endif


//! \addtogroup core
//! @{

//! @brief
//! Throw an exception and automatically add the function that caused it.
//!
//! @param[in] exception_type:
//! The exception type
//! @param[in] message:
//! The exception message
#define THROW_EXCEPTION(exception_type, message) throw exception_type(FUNCTION_SIG, message)

//! @brief
//! Throw an exception if the passed condition is met.
//!
//! @param[in] condition:
//! If the passed condition validates as `true`, the exception is thrown.
//! @param[in] exception_type:
//! The exception type
//! @param[in] message:
//! The exception message
#define THROW_EXCEPTION_IF(condition, exception_type, message)                                                         \
    if (condition) [[unlikely]] /* NOLINT(clang-diagnostic-unknown-attributes) */                                      \
    THROW_EXCEPTION(exception_type, message)


//! @}

// ====================================================================================================================


namespace mjolnir
{
//! \addtogroup core
//! @{

//! @brief
//! Stores origin and message of an exception
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
