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
public:
    int this_is_a_member = 1; //!< Some nice member
};
} // namespace mjolnir
