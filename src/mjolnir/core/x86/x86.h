//! @file
//! x86.h
//!
//! @brief
//! This header includes the correct x86 header depending on the operation system.


#pragma once

#if defined __GNUC__ && __GNUC__ >= 6
#    pragma GCC diagnostic ignored "-Wignored-attributes"
#endif


// see https://stackoverflow.com/a/22291538/6700329
#if defined(_MSC_VER)
#    include <intrin.h>
#elif defined(__GNUC__) && (defined(__x86_64__) || defined(__i386__))
#    include <x86intrin.h>
#else
static_assert(false, "Incompatible processor architecture or compiler");
#endif

