//! @file
//! concepts.h
//!
//! @brief
//! Contains x86 specific concepts


#pragma once

#include "mjolnir/core/utility/type.h"
#include "mjolnir/core/x86/x86.h"


namespace mjolnir::x86
{
//! \addtogroup core_x86
//! @{

//! @brief
//! Concept for a x86 vector register
//!
//! @tparam T_Type
//! Type
template <typename T_Type>
concept VectorRegister = is_any_of<T_Type, __m128, __m128d, __m128i, __m256, __m256d, __m256i>();

//! @brief
//! Concept for a x86 vector register that has floating-point elements.
//!
//! @tparam T_Type
//! Type
template <typename T_Type>
concept FloatVectorRegister = is_any_of<T_Type, __m128, __m128d, __m256, __m256d>();



//! @}
} // namespace mjolnir::x86

