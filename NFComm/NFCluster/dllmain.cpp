// dllmain.cpp : Defines the entry point for the DLL application.
//#include "stdafx.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

#ifdef _DEBUG

#if NF_PLATFORM == NF_PLATFORM_WIN32
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "RakNet_LibStatic_Debug_Win32.lib" )
#pragma comment( lib, "NFCore_d.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "NFCore_d.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN32
#pragma comment( lib, "NFCore.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "NFCore.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#endif
