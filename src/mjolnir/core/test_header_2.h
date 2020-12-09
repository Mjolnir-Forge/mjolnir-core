//! @file
//! test_header_2.h
//!
//! @brief
//! A second test header
//!
//! \addtogroup core
//! @{


#pragma once

#include "mjolnir/core/fundamental_types.h"

//! @brief
//! Just another class
class AnotherClass
{
public:
    //! @brief
    //! Just another function
    //!
    //! @param [in] arg
    //! Some number
    //!
    //! @return
    //! `true` if the number is larger than 4
    static auto another_function(int arg) -> bool
    {
        return arg > 4;
    }

    //! @brief
    //! A function that contains an error that should be found by cppcheck
    //!
    //! @return
    //! Some crap
    static auto cpp_check_error() -> char*
    {
        // static char a[10];
        // a[10] = 0;
        // return a;
        return nullptr;
    }
};
namespace mjolnir
{
//! @brief
//! Just another free function
//!
//! @return
//! Some crap
auto tiny() -> UST;
} // namespace mjolnir

//! @}
