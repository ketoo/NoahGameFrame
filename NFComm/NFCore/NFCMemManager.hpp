// -------------------------------------------------------------------------
//    @FileName         :    NFCMemManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCMemManager
//
// -------------------------------------------------------------------------
#ifndef NFC_NFCMEMMANGER_H
#define NFC_NFCMEMMANGER_H

#include "NFSingleton.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"

//#define  NF_USE_TCMALLOC

#ifdef NF_USE_TCMALLOC
#include "Dependencies/gperftools-2.1/src/gperftools/malloc_extension.h"
#endif

#ifdef NF_USE_TCMALLOC

#ifdef NF_DEBUG_MODE

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "libtcmalloc_minimal_d.lib" )
#pragma comment(linker, "/include:__tcmalloc")
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "libtcmalloc_minimal.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "libtcmalloc_minimal.lib" )
#pragma comment(linker, "/include:__tcmalloc")
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "libtcmalloc_minimal.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#endif // NF_USE_TCMALLOC

#endif

class NFCMemManager: public NFSingleton<NFCMemManager>
{
public:
	NFCMemManager()
	{

	}

	~NFCMemManager()
	{

	}


public:
	void FreeMem()
	{

#ifdef NF_USE_TCMALLOC

		MallocExtension::instance()->ReleaseFreeMemory();
#endif

	}

private:

};

#endif