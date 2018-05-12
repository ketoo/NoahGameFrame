#ifdef _DEBUG

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "redisclient_d.lib" )
#pragma comment( lib, "NFCore_d.lib" )
#pragma comment( lib, "Theron_d.lib" )
#pragma comment( lib, "hiredis_d.lib") 
//#pragma comment( lib, "Win32_Interop_d.lib") 
#else
#pragma comment( lib, "redisclient_d.a" )
#pragma comment( lib, "libtherond.a")
#pragma comment( lib, "NFCore_d.a")
#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "redisclient.lib" )
#pragma comment( lib, "Theron.lib" )
#pragma comment( lib, "NFCore.lib" )

#else
#pragma comment( lib, "redisclient.a" )
#pragma comment( lib, "libtheron.a")
#pragma comment( lib, "NFCore.a")
#endif

#endif
