// Copyright (C) by Ashton Mason. See LICENSE.txt for licensing information.
#ifndef THERON_DEFINES_H
#define THERON_DEFINES_H


/**
\file Defines.h
\brief Global user-configurable defines.

This file defines, in one place, all the defines which can be defined in order
to override default options within Theron. Some of them enable or disable debugging
functionality, others abstract away platform-specific detail such as code inlining
and variable alignment, and still others control aspects of the operation of Theron.

\note The intention is that most users will be able to leave these defines at their default
values, hardly knowing they're there. But if you do need to override one of them then
the best way to do it is to define the define globally in your build (for example as
a compiler option), rather than by editing this file. The definitions of the defines
in this file are all conditional on the define having not already been defined, so can
be easily overridden just by defining the defines yourself globally in your build.
*/


/**
\def THERON_VERSION

\brief Exposes the version number of the Theron release in use.

This define is defined automatically and matches the major and minor version numbers
used to name official releases. Its definition can't be overridden by the user.
*/


#define THERON_VERSION "6.00"


/**
\def THERON_WINDOWS

\brief Controls the use of Windows features.

This define is defined automatically if not predefined by the user. When automatically defined,
it is defined as 1 if the Windows platform is detected, and 0 otherwise.

In particular this define controls the use of Windows thread and atomic functionality.
If Windows functionality is available then it is used in preference to other APIs.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_WINDOWS)
#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32) || defined(_WIN64) || defined(WINDOWS) || defined(_WIN32_CE) || defined(__CYGWIN__)
#define THERON_WINDOWS 1
#else
#define THERON_WINDOWS 0
#endif
#endif // THERON_WINDOWS


/**
\def THERON_MSVC

\brief Enables the use of Microsoft Visual C++ features.

This define is defined automatically if not predefined by the user. When automatically defined,
it is defined as 1 if the Microsoft Visual C++ compiler is detected, and 0 otherwise.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_MSVC)
#if defined(_MSC_VER)
#define THERON_MSVC 1
#else
#define THERON_MSVC 0
#endif
#endif // THERON_MSVC


/**
\def THERON_GCC

\brief Enables the use of GCC-specific features.

This define is defined automatically if not predefined by the user. When automatically defined,
it is defined as 1 if a version of the GCC compiler is detected, and 0 otherwise.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_GCC)
#if defined(__GNUC__)
#define THERON_GCC 1
#else
#define THERON_GCC 0
#endif
#endif // THERON_GCC


/**
\def THERON_64BIT

\brief Enables the use of 64-bit environment features.

This define is defined automatically if not predefined by the user. When automatically
defined, it is defined as 1 if a 64-bit environment is detected, as 0 if a 32-bit environment
is detected, or as 1 by default if the environment isn't recognized.

Pointers are assumed to be 8 bytes wide when it is defined as 1 and 4 bytes wide when it
is defined as 0.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_64BIT)
#if THERON_WINDOWS

#if defined(_WIN64)
#define THERON_64BIT 1
#else
#define THERON_64BIT 0
#endif

#elif THERON_GCC

#if defined(__x86_64__) || defined(__ppc64__)
#define THERON_64BIT 1
#else
#define THERON_64BIT 0
#endif

#else

// Assume 64-bit environment by default.
#define THERON_64BIT 1

#endif
#endif // THERON_64BIT


/**
\def THERON_DEBUG

\brief Controls generation of code for asserts, allocation checking, etc.

This is a global define that decides default values for several other
defines that individually control aspects of code generation related to debugging.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.

If not defined, then THERON_DEBUG defaults to 1 in GCC builds when NDEBUG is not
defined, and to 1 in other builds (including Visual C++) when _DEBUG is defined.
Otherwise it defaults to 0, disabling debug-related code generation.

If THERON_DEBUG is defined as 1, then \ref THERON_ENABLE_ASSERTS defaults to 1,
\ref THERON_FORCEINLINE defaults to 'inline', and \ref
THERON_ENABLE_DEFAULTALLOCATOR_CHECKS defaults to 1. Otherwise those defines
default to 0, forced inlining, and 0, respectively.
*/


#if !defined(THERON_DEBUG)
#if THERON_MSVC
#if defined(_DEBUG)
#define THERON_DEBUG 1
#else
#define THERON_DEBUG 0
#endif
#elif THERON_GCC
#if defined(NDEBUG)
#define THERON_DEBUG 0
#else
#define THERON_DEBUG 1
#endif
#else
#if defined(_DEBUG)
#define THERON_DEBUG 1
#else
#define THERON_DEBUG 0
#endif
#endif
#endif // THERON_DEBUG


/**
\def THERON_ENABLE_ASSERTS

\brief Enables generation of code for asserts within Theron.

Defaults to 0, disabling asserts, when \ref THERON_DEBUG is 0.
Defaults to 1, enabling asserts, when \ref THERON_DEBUG is 1.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_ENABLE_ASSERTS)
#if THERON_DEBUG
#define THERON_ENABLE_ASSERTS 1
#else
#define THERON_ENABLE_ASSERTS 0
#endif
#endif


/**
\def THERON_BOOST

\brief Controls whether Boost functionality is used.

If THERON_BOOST is defined as 1 then Boost is used, including using boost::thread
for thread support and boost::atomic (packaged with Theron) for atomic support.

This define is defined automatically if not predefined by the user. When automatically
defined, it is defined as 0, disabling the use of Boost. In Windows builds the Windows API is used
instead, and in GCC builds Linux/POSIX API is used instead (pthreads). Note that the pthreads support
is slow due to emulating atomic operations with locks, therefore it is preferable to use Boost or
C++11 if available.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


/**
\def THERON_CPP11

\brief Controls whether C++11 features are used.

If THERON_CPP11 is defined as 1 then C++11 features are used, including using std::thread
for thread support and std::atomic for atomic support. This requires either a C++ compiler with
built-in support for these C++11 features (eg. a recent version of GCC such as 4.6 or later) or
support provided by some third-party library (eg. JustThread).

This define is defined automatically if not predefined by the user. When automatically
defined, it is defined as 0, disabling the use of C++11. In Windows builds the Windows API is used
instead, and in GCC builds Linux/POSIX API is used instead (pthreads). Note that the pthreads support
is slow due to emulating atomic operations with locks, therefore it is preferable to use Boost or
C++11 if available.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


/**
\def THERON_POSIX

\brief Controls whether POSIX features are used.

If THERON_POSIX is defined as 1 then POSIX features may be used, including using POSIX threads
(pthreads) as the underlying thread support.

This define is defined automatically if not predefined by the user. When automatically
defined, it is defined as 1 unless \ref THERON_MSVC is enabled, enabling the use of POSIX features
as a fallback with non-MSVC compilers if no alternative API is available. Note that the pthreads
support is slow due to emulating atomic operations with locks, therefore it is preferable to use
Boost or C++11 features if available.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


// Support for legacy defines - use THERON_BOOST or THERON_CPP11 instead!
#if defined(THERON_USE_BOOST_THREADS) || defined(THERON_USE_STD_THREADS)
#if THERON_USE_BOOST_THREADS
#define THERON_BOOST 1
#define THERON_CPP11 0
#elif THERON_USE_STD_THREADS
#define THERON_BOOST 0
#define THERON_CPP11 1
#endif
#endif


// Default definitions - but don't replace any user-defined definitions.
#if !defined(THERON_BOOST)
#define THERON_BOOST 0
#endif

#if !defined(THERON_CPP11)
#define THERON_CPP11 0
#endif

#if !defined(THERON_POSIX)
#if THERON_MSVC
#define THERON_POSIX 0
#else
#define THERON_POSIX 1
#endif
#endif


/**
\def BOOST_THREAD_BUILD_LIB

\brief Controls whether boost::thread is built as a static library or dynamic/shared library.

Unless specified otherwise, Theron defaults to building boost::thread as a library.
Somewhat bizarrely Boost defaults to dynamically linked under GCC without this define
(at least in Boost 1.47).
*/


#if THERON_BOOST
#if !defined(BOOST_THREAD_BUILD_DLL) && !defined(BOOST_THREAD_BUILD_LIB)
#define BOOST_THREAD_BUILD_LIB 1
#endif //
#endif // THERON_BOOST


/**
\def THERON_FORCEINLINE

\brief Controls force-inlining of core functions within Theron.

This define defines the keyword used for inlined function definitions.
It is defined automatically if not predefined by the user. When automatically
defined, in release builds (when \ref THERON_DEBUG is 0), it defaults to forced inlining
in MSVC and GCC builds. In debug builds and on all other platforms, it defaults to the
inline keyword, which is required on functions defined within headers.

Many functions within Theron are force-inlined via compiler keywords, to avoid
function call overhead, resulting in a significant speedup over optionally inlined code.
The use of forced inlining does however lead to some code bloat. Therefore it may
be desirable to turn it off in some builds. Moreover, it makes debugging difficult when
enabled, so it is useful to be able to disable it - and for it to be disabled
by default - in debug builds.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_FORCEINLINE)
#if THERON_DEBUG
#define THERON_FORCEINLINE inline
#else // THERON_DEBUG
#if THERON_MSVC
#define THERON_FORCEINLINE __forceinline
#elif THERON_GCC
#define THERON_FORCEINLINE inline __attribute__((always_inline))
#else
#define THERON_FORCEINLINE inline
#endif
#endif // THERON_DEBUG
#endif // THERON_FORCEINLINE


/**
\def THERON_NOINLINE

\brief Used to prevent functions from being inlined.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_NOINLINE)
#if THERON_MSVC
#define THERON_NOINLINE __declspec(noinline)
#elif THERON_GCC
#define THERON_NOINLINE __attribute__((noinline))
#else
#define THERON_NOINLINE
#endif
#endif // THERON_NOINLINE


/**
\def THERON_ENABLE_DEFAULTALLOCATOR_CHECKS

\brief Enables debug checking of allocations in the \ref Theron::DefaultAllocator "DefaultAllocator".

This define controls the use of debugging checks in the default allocator used within
Theron. It is defined automatically if not predefined by the user. When automatically
defined, it is defined as 0 when \ref THERON_DEBUG is 0, and 1 when \ref THERON_DEBUG is 1,
so that debug checks are enabled only in debug builds.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_ENABLE_DEFAULTALLOCATOR_CHECKS)
// Support THERON_ENABLE_SIMPLEALLOCATOR_CHECKS as a legacy synonym.
#if defined(THERON_ENABLE_SIMPLEALLOCATOR_CHECKS)
#define THERON_ENABLE_DEFAULTALLOCATOR_CHECKS THERON_ENABLE_SIMPLEALLOCATOR_CHECKS
#else
#if THERON_DEBUG
#define THERON_ENABLE_DEFAULTALLOCATOR_CHECKS 1
#else
#define THERON_ENABLE_DEFAULTALLOCATOR_CHECKS 0
#endif // THERON_DEBUG
#endif // defined(THERON_ENABLE_SIMPLEALLOCATOR_CHECKS)
#endif // THERON_ENABLE_DEFAULTALLOCATOR_CHECKS


/**
\def THERON_ENABLE_MESSAGE_REGISTRATION_CHECKS

\brief Controls run-time reporting of unregistered message types.

This define controls run-time reporting of unregistered message types within
Theron. It is defined automatically if not predefined by the user. When automatically
defined, it is defined as 0, disabling the runtime checks.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.

Message types can be registered using the \ref THERON_REGISTER_MESSAGE macro, which notifies
Theron of the unique name of a message type. Registering the message types sent within an
application causes Theron's internal type ID system to be used to identity messages when they
arrive at an actor, instead of the built-in C++ typeid system.

This has two useful effects: Firstly, it avoids calling the slow C++ dynamic_cast
operator, and instead performs a simple check on the unique name associated with each message type
(a fast pointer comparison). Secondly, it allows the C++ compiler's built-in Run-Time Type
Information (RTTI) system to be disabled, which avoids storing compiler-generated type IDs in
classes.

However this efficiency comes with a catch: once we start registering message types, we
have to remember to consistently always register *every* message type used by the application.
If any of the message types are omitted, then errors may ensue (usually caught by asserts,
in debug builds).

To make this easier, define \ref THERON_ENABLE_MESSAGE_REGISTRATION_CHECKS as 1 in your local
build. This enables run-time error reports that helpfully detect message types that haven't been
registered. The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.

Of course, you should only enable this checking if you are actually intending to register your
message types! If you don't care about the overheads of RTTI and dynamic_cast, then it's perfectly
fine (and a lot simpler) to just not register *any* message types at all -- and leave this define
disabled -- and then everything will work as normal.

\note Note that unregistered message types are reported by asserts, so are only active if assert
code is also enabled via \ref THERON_ENABLE_ASSERTS.
*/


#if !defined(THERON_ENABLE_MESSAGE_REGISTRATION_CHECKS)
#define THERON_ENABLE_MESSAGE_REGISTRATION_CHECKS 0
#endif // THERON_ENABLE_MESSAGE_REGISTRATION_CHECKS


/**
\def THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS

\brief Enables reporting of undelivered and unhandled messages, which is enabled by default.

This define controls reporting of messages that are either not delivered (because no entity
exists at the address to which the message is sent, in which case \ref Theron::Actor::Send "Actor::Send"
and \ref Theron::Framework::Send "Framework::Send" return false), or not handled by the actor to
which they were delivered (because the actor has no message handlers registered for the message type
and no default message handler).

Such messages are passed to a \ref Theron::Framework::SetFallbackHandler "fallback handler"
registered with the \ref Theron::Framework "Framework" concerned (that within which the message was sent,
in the case of undelivered messages, and that containing the unresponsive actor, in the case of
unhandled messages). The default fallback handler registered with each framework, which is
used unless replaced explicitly by a custom user-specified fallback handler, reports such
messages to stderr using printf, and asserts, if asserts are enabled with \ref THERON_ENABLE_ASSERTS.

This define controls the activation of the default fallback handler. If defined as 1, the
default fallback handler reports failures. If defined as 0, it does nothing, and unhandled
messages are silently unreported. Defining \ref THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS to 0
effectively turns off the checking. If the intention is to replace the reporting mechanism, then
use \ref Theron::Framework::SetFallbackHandler "SetFallbackHandler" to replace the default handler
entirely with a custom handler implementation.

Defaults to 1 (enabled). Set this to 0 to disable the reporting.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS)
#define THERON_ENABLE_UNHANDLED_MESSAGE_CHECKS 1
#endif


/**
\def THERON_ENABLE_BUILD_CHECKS

\brief Enables automatic checking of build consistency between the Theron library and client code.

This define controls the use of automatic checks that compare the build settings used to build the
Theron library with those used to build any client code in which Framework objects are
constructed. The checks are performed at framework construction time and compare generated
strings that characterize the build settings in the library and (inlined within) client code.

Because Theron includes a significant amount of inlined code which gets generated within the
client executable rather than within the Theron library itself, it is important that Theron
code is generated consistently across these two potentially different environments. Many of the
defines defined in Theron/Defines.h affect code generation in non-trivial ways, and it can
be unsafe to mix code built with different definitions of these defines. In particular, some of
the most important defines are, by default, defined differently in debug and release builds.
For example, mixing a debug build of the Theron library with a release build of Theron-based
client code, or vice versa, will almost always result in hard-to-find errors.

Defaults to 1 (enabled). Set this to 0 to disable the checking if you wish to mix different
builds and know for certain that it is safe to do so.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_ENABLE_BUILD_CHECKS)
#define THERON_ENABLE_BUILD_CHECKS 1
#endif


/**
\def THERON_ENABLE_COUNTERS

\brief Controls availability of per-framework counters that record the occurrence of scheduling events.

This define controls the availability of a set of per-framework event counters.
If the value of the define is non-zero then the counters are incremented when the counted
events occur. If the value of the define is zero then the counters are not incremented, and cannot
be queried. The incrementing of the counters incurs some small overhead that makes
a measurable difference in synthetic benchmarks, but is probably not significant in real code.

Defaults to 0 (disabled). Define this as 1 to enable counters.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.

\see Theron::Framework::GetNumCounters
\see Theron::Framework::GetCounterValue
*/


#if !defined(THERON_ENABLE_COUNTERS)
#define THERON_ENABLE_COUNTERS 0
#endif


/**
\def THERON_CACHELINE_ALIGNMENT

\brief Describes the size of a cache line on the target platform.

This define is used internally to align allocated structures to cache-line boundaries.
By default it is set to 64 bytes, the size of a cache line on many systems.

The value of \ref THERON_CACHELINE_ALIGNMENT can be overridden by defining it globally in the build
(in the makefile using -D, or in the project preprocessor settings in Visual Studio).

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.

By defining THERON_CACHELINE_ALIGNMENT globally within your own project build, you can
override the default value and specify a different size, either to optimize allocations for
a system with a different cache-line width, or else to disable cache-line alignment completely
by specifying a harmless alignment such as 4 bytes.

\note Changing this value is unlikely to provide any benefit unless you know for sure
that you need to change it.
*/


#if !defined(THERON_CACHELINE_ALIGNMENT)
#define THERON_CACHELINE_ALIGNMENT 64
#endif


/**
\def THERON_NUMA

\brief Enables NUMA support.

This define controls the use of NUMA (Non-Uniform Memory Architecture) libraries to
implement support for NUMA-aware thread affinities.

It is defined automatically if not predefined by the user. When automatically
defined, it defaults to 1 in Windows builds and 0 in all other builds.

Enabling NUMA support can lead to substantial performance improvements on NUMA hardware,
for example some dual-socket Opteron systems.

NUMA support is included within recent versions of Windows. On all other builds
on all other platforms THERON_NUMA is disabled by default, and must be explicitly
defined as 1 in order to enable NUMA support. NUMA support is currently only
implemented in Windows and GCC builds. In non-Windows GCC builds NUMA support depends
on the numa library.

In GCC builds, for example on Linux, you may need to install the libnuma and
libnuma-dev package distributions of the libnuma library in order to build
Theron with NUMA support. Because libnuma-dev may not be installed on many
systems, THERON_NUMA defaults to undefined in non-Windows builds even though
NUMA runtime support may be available via libnuma.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_NUMA)
#if THERON_WINDOWS
#define THERON_NUMA 1
#else
#define THERON_NUMA 0
#endif
#endif


/**
\def THERON_XS

\brief Enables the use of Crossroads.IO (the XS library) to implement network support for remote actors.

This define controls the use of the XS network library to implement support for remote actors.
Specifically, message-based communication between actors in Frameworks located in different
processes or on different hosts.

It is defined automatically if not predefined by the user. When automatically
defined, it currently defaults to 0 in all builds, disabling the use of the XS library.

\note Network support is currently in a state of development and testing.

The default definition can be overridden by defining it globally in the build - either
via the makefile command line options, on the GCC command line using -D, or in the project
preprocessor settings in Visual Studio.
*/


#if !defined(THERON_XS)
#define THERON_XS 0
#endif


#endif // THERON_DEFINES_H

