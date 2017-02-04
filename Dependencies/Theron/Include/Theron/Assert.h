// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DETAIL_DEBUG_ASSERT_H
#define THERON_DETAIL_DEBUG_ASSERT_H


/**
\file Assert.h
\brief Defines assert and fail macro's for debugging and error reporting.
*/


#include <Theron/Defines.h>


/**
\def THERON_ASSERT

\brief Theron assert macro.

This define is defined automatically if not predefined by the user. When automatically
defined, it is defined differently depending on the value of \ref THERON_ENABLE_ASSERTS.
When THERON_ENABLE_ASSERTS evaluates to non-zero, THERON_ASSERT is defined as a test of
the condition parameter, followed by a call to Theron::Detail::Fail if the condition
evaluates as false. Otherwise, it is defined away to nothing.

The default definition can be overridden by defining it globally in the build.
If so then it must always be defined before inclusion of any Theron headers,
in every compilation unit.
*/


/**
\def THERON_ASSERT_MSG

\brief Theron assert macro.

This define is defined automatically if not predefined by the user. When automatically
defined, it is defined differently depending on the value of \ref THERON_ENABLE_ASSERTS.
When THERON_ENABLE_ASSERTS evaluates to non-zero, THERON_ASSERT_MSG is defined as a test of
the condition parameter, followed by a call to Theron::Detail::Fail with the provided
message if the condition evaluates as false. Otherwise, it is defined away to nothing.

The default definition can be overridden by defining it globally in the build.
If so then it must always be defined before inclusion of any Theron headers,
in every compilation unit.
*/


/**
\def THERON_FAIL

\brief Theron error macro.

This define is defined automatically if not predefined by the user. When automatically
defined, it is defined differently depending on the value of \ref THERON_ENABLE_ASSERTS.
When THERON_ENABLE_ASSERTS evaluates to non-zero, THERON_FAIL is defined as an unconditional
call to Theron::Detail::Fail. Otherwise, it is defined away to nothing.

The default definition can be overridden by defining it globally in the build.
If so then it must always be defined before inclusion of any Theron headers,
in every compilation unit.
*/


/**
\def THERON_FAIL_MSG

\brief Theron error macro.

This define is defined automatically if not predefined by the user. When automatically
defined, it is defined differently depending on the value of \ref THERON_ENABLE_ASSERTS.
When THERON_ENABLE_ASSERTS evaluates to non-zero, THERON_FAIL_MSG is defined as an
unconditional call to Theron::Detail::Fail with the provided message parameter.
Otherwise, it is defined away to nothing.

The default definition can be overridden by defining it globally in the build.
If so then it must always be defined before inclusion of any Theron headers,
in every compilation unit.
*/


#if THERON_ENABLE_ASSERTS

#include <stdio.h>
#include <assert.h>

#ifndef THERON_ASSERT
#define THERON_ASSERT(condition)                if (!(condition)) Theron::Detail::Fail(__FILE__, __LINE__); else { }
#endif // THERON_ASSERT

#ifndef THERON_ASSERT_MSG
#define THERON_ASSERT_MSG(condition, msg)       if (!(condition)) Theron::Detail::Fail(__FILE__, __LINE__, msg); else { }
#endif // THERON_ASSERT_MSG

#ifndef THERON_FAIL
#define THERON_FAIL()                           Theron::Detail::Fail(__FILE__, __LINE__)
#endif // THERON_FAIL

#ifndef THERON_FAIL_MSG
#define THERON_FAIL_MSG(msg)                    Theron::Detail::Fail(__FILE__, __LINE__, msg)
#endif // THERON_ASSERT

#else

#ifndef THERON_ASSERT
#define THERON_ASSERT(condition)
#endif // THERON_ASSERT

#ifndef THERON_ASSERT_MSG
#define THERON_ASSERT_MSG(condition, msg)
#endif // THERON_ASSERT_MSG

#ifndef THERON_FAIL
#define THERON_FAIL()
#endif // THERON_FAIL

#ifndef THERON_FAIL_MSG
#define THERON_FAIL_MSG(msg)
#endif // THERON_FAIL_MSG

#endif // THERON_ENABLE_ASSERTS



namespace Theron
{
namespace Detail
{


#if THERON_ENABLE_ASSERTS
/**
Reports an internal application or system failure.
A message describing the failure is printed to stderr.
\param file The name of the file in which the failure occurred.
\param line The line number at which the failure occurred.
\param message A message describing the failure.
*/
inline void Fail(const char *const file, const unsigned int line, const char *const message = 0)
{
    fprintf(stderr, "FAIL in %s (%d)", file, line);
    if (message)
    {
        fprintf(stderr, ": %s", message);
    }

    fprintf(stderr, "\n");
    assert(false);
}
#endif // THERON_ENABLE_ASSERTS


} // namespace Detail
} // namespace Theron


#endif // THERON_DETAIL_DEBUG_ASSERT_H

