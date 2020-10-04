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

auto undocumented_function() -> bool
{
    return false;
}
} // namespace mjolnir
