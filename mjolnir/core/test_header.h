//! @brief A test header
#pragma once

namespace mjolnir
{
//! @brief a function that returns 5
//! @details this is the detailed description
//! @return The number 5
auto a_function() -> int
{
    return 5;
}

//! @brief A function that was initially undocumented.
//! @return Always false
auto undocumented_function() -> bool
{
    return false;
}


//! @brief Some class.
//!
//! @details Not much more to say.
class test_class
{
public:
    int this_is_a_member = 1; //! < Some nice member
};
} // namespace mjolnir
