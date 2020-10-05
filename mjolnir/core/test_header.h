//! @brief A test header
#pragma once

namespace mjolnir
{
//! @brief a function that returns 5
//! @details this is the detailed description
//! @return The number 5
inline auto a_function() -> int
{
    constexpr int returned_number = 5;
    return returned_number;
}

//! @brief A function that was initially undocumented.
//! @return Always false
inline auto undocumented_function() -> bool
{
    return false;
}


//! @brief Some class.
//!
//! @details Not much more to say.
class TestClass
{
    int m_private_member = 2; //!< shouldn't show up in the official documentation

public:
    //! @brief Do something funny.
    //!
    //! @details Well... it's not so funny at all...
    //!
    //! @param [in] a: The first parameter
    //! @param [in] b: The second parameter
    //!
    //! @return The funny result
    [[nodiscard]] auto some_funny_function(int a, int b) const -> int
    {
        return m_private_member + a + b;
    }
};
} // namespace mjolnir
