#include "NFComm/NFPluginModule/NFPlatform.h"


#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma  comment(lib,"ws2_32.lib")
#pragma  comment(lib,"wsock32.lib")
#pragma  comment(lib, "Wldap32.lib")
#endif


#pragma comment( lib, "Dbghelp.lib" )

#ifdef NF_DEBUG_MODE

#if NF_PLATFORM == NF_PLATFORM_WIN

#ifdef NF_DYNAMIC_PLUGIN
#pragma  comment(lib, "libcurld.lib")
#pragma comment( lib, "libprotobuf_d.lib" )
#pragma comment( lib, "NFMessageDefine_d.lib" )

#else
#pragma  comment(lib, "libcurl.lib")
#pragma comment( lib, "libprotobuf.lib" )
#pragma comment( lib, "NFMessageDefine.lib" )
#endif

//#pragma comment( lib, "Theron_d.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "libcurl.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#ifdef NF_DYNAMIC_PLUGIN
#pragma  comment(lib, "libcurld.lib")
#pragma comment( lib, "libprotobuf_d.lib" )
#pragma comment( lib, "NFMessageDefine_d.lib" )
#else
#pragma  comment(lib, "libcurl.lib")
#pragma comment( lib, "libprotobuf.lib" )
#pragma comment( lib, "NFMessageDefine.lib" )
#endif

//#pragma comment( lib, "Theron.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "libcurl.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#endif
