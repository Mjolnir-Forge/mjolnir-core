//! @file
//! floating_point_vector_register_test_suite.h
//!
//! @brief
//! Contains the test suite for vector registers based on floating point types


#pragma once


#include "mjolnir/core/x86/definitions.h"
#include "mjolnir/core/x86/x86.h"
#include <gtest/gtest.h>


//! \addtogroup testing
//! @{

//!@brief
//! The test suite for all supported x86 vector registers based on floating point types.
//!
//! @tparam T_RegisterType
//! Template variable for the register type
template <mjolnir::x86::FloatVectorRegister T_RegisterType>
class FloatingPointVectorRegisterTestSuite : public ::testing::Test
{
};


//! \cond DO_NOT_DOCUMENT
using VectorRegisterTestTypes = ::testing::Types<__m128, __m128d, __m256, __m256d>; // NOLINT


//! The comma at the end of the typed test series call is necessary to suppress a warning. See the following link for
//! more information: https://github.com/google/googletest/issues/2271
// cppcheck-suppress syntaxError
TYPED_TEST_SUITE(FloatingPointVectorRegisterTestSuite, VectorRegisterTestTypes, );
//! \endcond


//! @}
