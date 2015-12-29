#include "NFComm/NFPluginModule/NFPlatform.h"

#ifdef NF_DEBUG_MODE

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma  comment(lib,"ws2_32.lib")
#pragma  comment(lib,"wsock32.lib")
#pragma  comment(lib, "Wldap32.lib")
#pragma comment( lib, "Dbghelp.lib" )
#pragma comment( lib, "libcurld.lib" )
#pragma comment( lib, "libprotobuf_d.lib" )
#pragma comment( lib, "NFMessageDefine_d.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "libcurld.a" )
#pragma comment( lib, "libprotobuf_d.a" )
#pragma comment( lib, "NFMessageDefine_d.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma  comment(lib,"ws2_32.lib")
#pragma  comment(lib,"wsock32.lib")
#pragma  comment(lib, "Wldap32.lib")
#pragma comment( lib, "Dbghelp.lib" )
#pragma comment( lib, "libcurl.lib" )
#pragma comment( lib, "libprotobuf.lib" )
#pragma comment( lib, "NFMessageDefine.lib" )
#elif NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID
#pragma comment( lib, "libcurld.a" )
#pragma comment( lib, "libprotobuf_d.a" )
#pragma comment( lib, "NFMessageDefine.a" )
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#endif

#endif