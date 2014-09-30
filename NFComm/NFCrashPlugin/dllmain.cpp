#include "NFComm/NFCore/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#endif



#pragma comment( lib, "Dbghelp.lib" )

#ifdef NF_DEBUG_MODE

#if NF_PLATFORM == NF_PLATFORM_WIN

#ifdef NF_DYNAMIC_PLUGIN
#pragma comment( lib, "NFCore_d.lib" )
#pragma comment( lib, "Theron_d.lib" )

#else
#pragma comment( lib, "NFCore_d.lib" )
#pragma comment( lib, "Theron_d.lib" )
#endif

//#pragma comment( lib, "Theron_d.lib" )
#pragma comment( lib, "libglog_d.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "NFCore_d.a" )
#pragma comment( lib, "libglog_d.a" )
#pragma comment( lib, "libtherond.a")
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#ifdef NF_DYNAMIC_PLUGIN
#pragma comment( lib, "NFCore.lib" )
#pragma comment( lib, "Theron.lib" )
#else
#pragma comment( lib, "NFCore.lib" )
#pragma comment( lib, "Theron.lib" )
#endif

//#pragma comment( lib, "Theron.lib" )
#pragma comment( lib, "libglog.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "NFCore.a" )
#pragma comment( lib, "libglog.a" )
#pragma comment( lib, "libtherond.a")
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#endif
