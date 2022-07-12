//! @file
//! math/linear_algebra/vector_products.h
//!
//! @brief
//! Functions to calculate various vector products.


#pragma once

#include "mjolnir/core/definitions.h"
#include "mjolnir/core/fundamental_types.h"
#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/element_summation.h"
#include "mjolnir/core/x86/intrinsics.h"
#include "mjolnir/core/x86/permutation.h"

#include <array>

// === DECLARATIONS ===================================================================================================

namespace mjolnir
{
//! \addtogroup core_math
//! @{


//! @brief
//! Calculate the dot product of 2 vectors
//!
//! @tparam t_size:
//! Size of the vector
//! @tparam T_Type:
//! Data type of the vector elements and the returned value
//!
//! @param[in] lhs:
//! Data of the vector on the left-hand side of the operator
//! @param[in] rhs:
//! Data of the vector on the right-hand side of the operator
//!
//! @return
//! Dot product
template <UST t_size, Number T_Type>
[[nodiscard]] constexpr auto dot_product(const std::array<T_Type, t_size>& lhs, const std::array<T_Type, t_size>& rhs)
        -> T_Type;


template <UST t_size, x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] auto dot_product(T_RegisterType lhs, T_RegisterType rhs) -> x86::ElementType<T_RegisterType>;

//! @}
} // namespace mjolnir


// === DEFINITIONS ====================================================================================================


namespace mjolnir
{
// --------------------------------------------------------------------------------------------------------------------

template <UST t_size, Number T_Type>
[[nodiscard]] constexpr auto dot_product(const std::array<T_Type, t_size>& lhs, const std::array<T_Type, t_size>& rhs)
        -> T_Type
{
    T_Type result = 0;
    for (UST i = 0; i < t_size; ++i)
        result += lhs[i] * rhs[i];

    return result;
}


// --------------------------------------------------------------------------------------------------------------------


template <UST t_size, x86::FloatVectorRegister T_RegisterType>
[[nodiscard]] auto dot_product(T_RegisterType lhs, T_RegisterType rhs) -> x86::ElementType<T_RegisterType>
{
    using namespace x86;

    auto product = mm_mul(lhs, rhs);
    return element_sum_first_n<t_size>(product);
}


} // namespace mjolnir
