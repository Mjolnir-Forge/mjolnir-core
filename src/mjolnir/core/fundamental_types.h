//! @file
//! fundamental_types.h
//!
//! @brief
//! Defines the fundamental data types.
//!
//! \addtogroup core
//! @{

#pragma once


#include <climits>
#include <cstddef>
#include <cstdint>


namespace mjolnir
{
using I8  = std::int8_t;   //!< 8 bit signed integer type
using I16 = std::int16_t;  //!< 16 bit signed integer type
using I32 = std::int32_t;  //!< 32 bit signed integer type
using I64 = std::int64_t;  //!< 64 bit signed integer type
using U8  = std::uint8_t;  //!< 8 bit unsigned integer type
using U16 = std::uint16_t; //!< 16 bit unsigned integer type
using U32 = std::uint32_t; //!< 32 bit unsigned integer type
using U64 = std::uint64_t; //!< 64 bit unsigned integer type
using UST = std::size_t;   //!< Unsigned integer type that is returned by `sizeof` operations
using F32 = float;         //!< 32 bit floating point type
using F64 = double;        //!< 64 bit floating point type


constexpr UST float_bit_size  = 32; //!< Required size of a `float` in bits
constexpr UST double_bit_size = 64; //!< Required size of a `double` in bits

static_assert(sizeof(float) * CHAR_BIT == float_bit_size, "Incopatible architecture, 'float' size isn't 32 bit");
static_assert(sizeof(double) * CHAR_BIT == double_bit_size, "Incopatible architecture, 'double' size isn't 64 bit");

} // namespace mjolnir


//! @}
