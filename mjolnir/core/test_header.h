//! @defgroup core Mjolnir Core Module
//!
//! @file
//! test_header.h
//!
//! @brief
//! A test header

#pragma once

namespace mjolnir
{
//! @brief
//! A function that returns 5  \ingroup core
//!
//! @details
//! This is the detailed description. Maybe you are interested in the `TestClass`
//!
//! @return
//! The number 5
//!
//! @todo
//! Remember that you can use TODOS!!!
inline auto a_function() -> int
{
    constexpr int returned_number = 5;
    return returned_number;
}


//! @brief
//! A function that was initially undocumented.
//!
//! @return
//! Always false
//!
//! @deprecated
//! It was undocumented because it is deprecated
inline auto undocumented_function() -> bool
{
    return false;
}


//! @brief
//! Some class. \ingroup core
//!
//! @details
//! Not much more to say.
//!
//! @test
//! Test this class
class TestClass
{
    int m_private_member = 2; //!< shouldn't show up in the official documentation

public:
    //! @brief
    //! Do something funny.
    //!
    //! @details
    //! Well... it's not so funny at all... [the undocumented function](@ref undocumented_function()) is
    //! funnier. Here is a cpp code snippet:
    //!
    //! ~~~ cpp
    //! int a = 3;
    //! int b = 3 - a;
    //! ~~~
    //!
    //! -# some enumeration
    //! -# enumeration item 2
    //!
    //! @param [in] a:
    //! The first parameter
    //! @param [in] b:
    //! The second parameter
    //!
    //! @return
    //! The funny result
    [[nodiscard]] auto some_funny_function(int a, int b) const -> int
    {
        return m_private_member + a + b;
    }

    //! @brief
    //! Be boring
    //!
    //! @details
    //!  This function is boring and doesn't do much at all
    //!
    //! @return
    //! A boring result
    //!
    //! @bug
    //! Is there a bug somewhere?
    [[nodiscard]] auto boring_function() const -> int
    {
        return m_private_member;
    }
};
} // namespace mjolnir
