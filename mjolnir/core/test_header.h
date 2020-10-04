//! @brief A test header
#pragma once

namespace mjolnir
{
//! @brief a function that returns 5
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
} // namespace mjolnir
