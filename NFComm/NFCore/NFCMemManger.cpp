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

#endif

#include "NFCMemManger.h"
#include "Dependencies/gperftools-2.1/src/gperftools/malloc_extension.h"

NFCMemManger::NFCMemManger()
{

}

NFCMemManger::~NFCMemManger()
{

}

void NFCMemManger::FreeMem()
{
    // »ØÊÕÄÚ´æ
    MallocExtension::instance()->ReleaseFreeMemory();
}
