/*
    Copyright 2009 - 2019 LvSheng.Huang

   This source file is part of NoahGameFrame/NoahFrame.
   NoahGameFrame/NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#ifndef NF_PLATFORM_H
#define NF_PLATFORM_H

#define NF_PLATFORM_WIN 1
#define NF_PLATFORM_LINUX 2
#define NF_PLATFORM_APPLE 3
#define NF_PLATFORM_SYMBIAN 4
#define NF_PLATFORM_APPLE_IOS 5
#define NF_PLATFORM_ANDROID 6

#define NF_COMPILER_MSVC 1
#define NF_COMPILER_GNUC 2
#define NF_COMPILER_BORL 3
#define NF_COMPILER_WINSCW 4
#define NF_COMPILER_GCCE 5

#define NF_ENDIAN_LITTLE 1
#define NF_ENDIAN_BIG 2

//#define NF_ENABLE_SSL 1

#if NF_ENABLE_SSL
#define EVENT__HAVE_OPENSSL
#endif

#define NF_ARCHITECTURE_32 1
#define NF_ARCHITECTURE_64 2

/* Finds the compiler type and version.
*/
#if defined( __GCCE__ )
#   define NF_COMPILER NF_COMPILER_GCCE
#   define NF_COMP_VER _MSC_VER
//# include <staticlibinit_gcce.h> // This is a GCCE toolchain workaround needed when compiling with GCCE
#elif defined( __WINSCW__ )
#   define NF_COMPILER NF_COMPILER_WINSCW
#   define NF_COMP_VER _MSC_VER
#elif defined( _MSC_VER )
#   define NF_COMPILER NF_COMPILER_MSVC
#   define NF_COMP_VER _MSC_VER
#elif defined( __GNUC__ )
#   define NF_COMPILER NF_COMPILER_GNUC
#   define NF_COMP_VER (((__GNUC__)*100) + \
                        (__GNUC_MINOR__*10) + \
                        __GNUC_PATCHLEVEL__)

#elif defined( __BORLANDC__ )
#   define NF_COMPILER NF_COMPILER_BORL
#   define NF_COMP_VER __BCPLUSPLUS__
#   define __FUNCTION__ __FUNC__
#else
#   pragma error "No known compiler. Abort! Abort!"

#endif

/* See if we can use __forceinline or if we need to use __inline instead */
#if NF_COMPILER == NF_COMPILER_MSVC
#   if NF_COMP_VER >= 1200
#       define FORCEINLINE __forceinline
#   endif
#elif defined(__MINGW32__)
#   if !defined(FORCEINLINE)
#       define FORCEINLINE __inline
#   endif
#else
#   define FORCEINLINE __inline
#endif

/* Finds the current platform */
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined( __WIN32__ ) || defined( _WIN32 ) || defined(_WINDOWS) || defined(WIN) || defined(_WIN64) || defined( __WIN64__ )
#   define NF_PLATFORM NF_PLATFORM_WIN
//////////////////////////////////////////////////////////////////////////
#elif defined( __APPLE_CC__) || defined(__APPLE__) || defined(__OSX__)
// Device                                                     Simulator
// Both requiring OS version 4.0 or greater
#   if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 40000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 40000
#       define NF_PLATFORM NF_PLATFORM_APPLE_IOS
#   else
#       define NF_PLATFORM NF_PLATFORM_APPLE
#   endif
//////////////////////////////////////////////////////////////////////////
#elif defined(linux) && defined(__arm__)
// TODO: This is NOT the correct way to detect the Tegra 2 platform but it works for now.
// It doesn't appear that GCC defines any platform specific macros.
#   define NF_PLATFORM NF_PLATFORM_TEGRA2
#elif defined(__ANDROID__)
#   define NF_PLATFORM NF_PLATFORM_ANDROID
//////////////////////////////////////////////////////////////////////////
#elif defined( __native_client__ )
#   define NF_PLATFORM NF_PLATFORM_NACL
#   ifndef NF_STATIC_LIB
#       error NF must be built as static for NaCl (NF_STATIC=true in cmake)
#   endif
#   ifdef NF_BUILD_RENDERSYSTEM_D3D9
#       error d3d9 is nor supported on NaCl (ONF_BUILD_RENDERSYSTEM_D3D9 false in cmake)
#   endif
#   ifdef NF_BUILD_RENDERSYSTEM_GL
#       error gl is nor supported on NaCl (ONF_BUILD_RENDERSYSTEM_GL=false in cmake)
#   endif
#   ifndef NF_BUILD_RENDERSYSTEM_GLES2
#       error GLES2 render system is needed for NaCl (ONF_BUILD_RENDERSYSTEM_GLES2=false in cmake)
#   endif
#else
#   define NF_PLATFORM NF_PLATFORM_LINUX
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/* Find the arch type */
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) || defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define NF_ARCH_TYPE NF_ARCHITECTURE_64
#else
#   define NF_ARCH_TYPE NF_ARCHITECTURE_32
#endif

// For generating compiler warnings - should work on any compiler
// As a side note, if you start your message with 'Warning: ', the MSVC
// IDE actually does catch a warning :)
#define NF_QUOTE_INPLACE(x) # x
#define NF_QUOTE(x) NF_QUOTE_INPLACE(x)
#define NF_WARN( x )  message( __FILE__ "(" QUOTE( __LINE__ ) ") : " x "\n" )

//----------------------------------------------------------------------------
// Windows Settings
#if NF_PLATFORM == NF_PLATFORM_WIN

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#pragma warning(disable:4091) 
#include <Windows.h>
#define NF_EXPORT extern "C"  __declspec(dllexport)

#include <Dbghelp.h>
// If we're not including this from a client build, specify that the stuff
// should get exported. Otherwise, import it.
#   if defined( NF_STATIC_LIB )
// Linux compilers don't have symbol import/export directives.
#       define _NFExport
#       define _NFPrivate
#   else
#       if defined( NF_NONCLIENT_BUILD )
#           define _NFExport __declspec( dllexport )
#       else
#           if defined( __MINGW32__ )
#               define _NFExport
#           else
#               define _NFExport __declspec( dllimport )
#           endif
#       endif
#       define _NFPrivate
#   endif
// Win32 compilers use _DEBUG for specifying debug builds.
// for MinGW, we set DEBUG
#   if defined(_DEBUG) || defined(DEBUG)
#       define NF_DEBUG_MODE 1
#   endif

// Disable unicode support on MingW for GCC 3, poorly supported in stdlibc++
// STLPORT fixes this though so allow if found
// MinGW C++ Toolkit supports unicode and sets the define __MINGW32_TOOLBOX_UNICODE__ in _mingw.h
// GCC 4 is also fine
#if defined(__MINGW32__)
# if NF_COMP_VER < 400
#  if !defined(_STLPORT_VERSION)
#   include<_mingw.h>
#   if defined(__MINGW32_TOOLBOX_UNICODE__) || NF_COMP_VER > 345
#    define NF_UNICODE_SUPPORT 1
#   else
#    define NF_UNICODE_SUPPORT 0
#   endif
#  else
#   define NF_UNICODE_SUPPORT 1
#  endif
# else
#  define NF_UNICODE_SUPPORT 1
# endif
#else
#  define NF_UNICODE_SUPPORT 1
#endif

#endif // NF_PLATFORM == NF_PLATFORM_WIN
//----------------------------------------------------------------------------
// Android Settings
/*
#if NF_PLATFORM == NF_PLATFORM_ANDROID
#   define _NFExport
#   define NF_UNICODE_SUPPORT 1
#   define NF_DEBUG_MODE 0
#   define _NFPrivate
#     define CLOCKS_PER_SEC  1000
//  pragma def were found here: http://www.inf.pucrs.br/~eduardob/disciplinas/SistEmbarcados/Mobile/Nokia/Tools/Carbide_vs/WINSCW/Help/PDF/C_Compilers_Reference_3.2.pdf
#     pragma warn_unusedarg off
#     pragma warn_emptydecl off
#     pragma warn_possunwant off
// A quick define to overcome different names for the same function
#   define stricmp strcasecmp
#   ifdef DEBUG
#       define NF_DEBUG_MODE 1
#   else
#       define NF_DEBUG_MODE 0
#   endif
#endif
*/
//----------------------------------------------------------------------------
// Linux/Apple/iOs/Android/Symbian/Tegra2/NaCl Settings
#if NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS || NF_PLATFORM == NF_PLATFORM_ANDROID || NF_PLATFORM == NF_PLATFORM_TEGRA2 || NF_PLATFORM == NF_PLATFORM_NACL

//#include <syswait.h>

// Enable GCC symbol visibility
#   if defined( NF_GCC_VISIBILITY )
#       define _NFExport  __attribute__ ((visibility("default")))
#       define _NFPrivate __attribute__ ((visibility("hidden")))
#   else
#       define _NFExport
#       define _NFPrivate
#   endif

// A quick define to overcome different names for the same function
#   define stricmp strcasecmp

// Unlike the Win32 compilers, Linux compilers seem to use DEBUG for when
// specifying a debug build.
// (??? this is wrong, on Linux debug builds aren't marked in any way unless
// you mark it yourself any way you like it -- zap ???)
#   if defined(_DEBUG) || defined(DEBUG)
#       define NF_DEBUG_MODE 1
#   endif

// Always enable unicode support for the moment
// Perhaps disable in old versions of gcc if necessary
#define NF_UNICODE_SUPPORT 1
#define MAX_PATH 255

#define NF_EXPORT extern "C" __attribute ((visibility("default")))

#endif


//----------------------------------------------------------------------------
// Endian Settings
// check for BIG_ENDIAN config flag, set NF_ENDIAN correctly
#ifdef NF_CONFIG_BIG_ENDIAN
#    define NF_ENDIAN NF_ENDIAN_BIG
#else
#    define NF_ENDIAN NF_ENDIAN_LITTLE
#endif

#include <stdint.h>
#include <chrono>
#include <functional>
#include <assert.h>
#include <string>
#include <map>
#include <iostream>

// Integer formats of fixed bit width
typedef uint32_t NFUINT32;
typedef uint16_t NFUINT16;
typedef uint8_t NFUINT8;
typedef int32_t NFINT32;
typedef int16_t NFINT16;
typedef int8_t NFINT8;
typedef uint64_t NFUINT64;
typedef int64_t NFINT64;
typedef int64_t NFSOCK;

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <crtdbg.h>
#define NFASSERT(exp_, msg_, file_, func_)  \
    std::string strInfo("Message:");        \
    strInfo += msg_ + std::string(" don't exist or some warning") + std::string("\n\nFile:") + std::string(file_) + std::string("\n Function:") + func_; \
    MessageBoxA(0, strInfo.c_str(), ("Error_"#exp_), MB_RETRYCANCEL | MB_ICONERROR); \
    assert(0);
#else
#define NFASSERT(exp_, msg_, file_, func_)
#endif


//#define GOOGLE_GLOG_DLL_DECL=

///////////////////////////////////////////////////////////////
#include <string>
#include <algorithm>
#include <cmath>
#include <time.h>
#include <sstream>
#include <stdio.h>
#include <common/lexical_cast.hpp>

#ifndef _MSC_VER
#include <sys/time.h>
#include <unistd.h>
#define EPOCHFILETIME 11644473600000000ULL
#else
#include <WinSock2.h>
#include <Windows.h>
#define NOMINMAX
#ifdef min
#undef min
#endif
#ifdef max
#undef max
#endif

#include <time.h>
#include <process.h>
#define EPOCHFILETIME 11644473600000000Ui64
#endif

#define ELPP_DISABLE_DEFAULT_CRASH_HANDLING

#if NF_PLATFORM == NF_PLATFORM_WIN
#define NFSPRINTF sprintf_s
#define NFSTRICMP stricmp
#define NFSLEEP(s) Sleep(s) //millisecond
#define NFGetPID() lexical_cast<std::string>(getpid())
#else
#define NFSPRINTF snprintf
#define NFSTRICMP strcasecmp
#define NFSLEEP(s) usleep(s*1000) //millisecond
#define NFGetPID() lexical_cast<std::string>(getpid())
#endif

#if NF_PLATFORM == NF_PLATFORM_WIN
#ifndef NF_DYNAMIC_PLUGIN
//#define NF_DYNAMIC_PLUGIN 1
#endif
#endif

//using tcmalloc
#if NF_PLATFORM != NF_PLATFORM_WIN
#ifndef NF_USE_TCMALLOC
//#define NF_USE_TCMALLOC 1
#endif
#endif


#define GET_CLASS_NAME(className) (#className)

#define NF_SHARE_PTR std::shared_ptr
#define NF_NEW new


template<typename DTYPE>
bool NF_StrTo(const std::string& strValue, DTYPE& nValue)
{
    try
    {
        nValue = lexical_cast<DTYPE>(strValue);
        return true;
    }
    catch (...)
    {
        return false;
    }

    return false;
}

inline bool IsZeroFloat(const float fValue, float epsilon = 1e-6)
{
    return std::abs(fValue) <= epsilon;
}

inline bool IsZeroDouble(const double dValue, double epsilon = 1e-15)
{
    return std::abs(dValue) <= epsilon;
}

inline int64_t NFGetTimeMSEx()
{
	return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//millisecond
inline int64_t NFGetTimeMS()
{
	return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

//second
inline int64_t NFGetTimeS()
{
    return NFGetTimeMS() / 1000;
}


#endif
