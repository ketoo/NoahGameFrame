#include "NFComm/NFPluginModule/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "Dbghelp.lib" )
#endif


#ifdef NF_DEBUG_MODE

#if NF_PLATFORM == NF_PLATFORM_WIN

#ifdef NF_DYNAMIC_PLUGIN
#pragma comment( lib, "NFCore_d.lib" )


#else
#pragma comment( lib, "NFCore_d.lib" )

#endif

//
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "NFCore_d.a" )

#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#ifdef NF_DYNAMIC_PLUGIN
#pragma comment( lib, "NFCore.lib" )

#else
#pragma comment( lib, "NFCore.lib" )
#endif

//
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "NFCore.a" )

#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#endif
