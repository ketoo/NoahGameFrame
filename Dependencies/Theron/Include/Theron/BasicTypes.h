// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_BASICTYPES_H
#define THERON_BASICTYPES_H


/**
\file BasicTypes.h
\brief Defines basic standard types in a hopefully cross-platform manner.
*/


#include <Theron/Defines.h>


#if THERON_BOOST

// Boost provides this header.
#include <boost/cstdint.hpp>

#elif THERON_CPP11

// This header is assumed to be available in C++11 builds.
#include <cstdint>

#elif THERON_GCC

// This header is typically available in GCC builds.
#include <inttypes.h>

#elif THERON_MSVC

// These are packaged with Theron, and provide stdint functionality for MSVC.
#include <Standard/stdint.h>
#include <Standard/inttypes.h>

#else

// Failing all else, we hope this C99 header is available.
#include <inttypes.h>

#endif


namespace Theron
{


#if THERON_BOOST

// In Boost builds we reuse the convenient Boost integer types.
typedef boost::uint8_t uint8_t;
typedef boost::uint32_t uint32_t;
typedef boost::int32_t int32_t;
typedef boost::uint64_t uint64_t;

// Boost doesn't define uintptr_t so we have to define it ourselves.
#if THERON_64BIT
typedef unsigned long long uintptr_t;
#else
typedef unsigned long uintptr_t;
#endif

#else

// Promote the global namespace types into the Theron namespace.
typedef ::uint8_t uint8_t;
typedef ::uint32_t uint32_t;
typedef ::int32_t int32_t;
typedef ::uint64_t uint64_t;
typedef ::uintptr_t uintptr_t;

#endif


}


#endif // THERON_BASICTYPES_H

