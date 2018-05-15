#ifdef _DEBUG

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "libevent.lib" )
#pragma comment( lib, "hiredis_d.lib") 
#else

#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "libevent.lib" )
#pragma comment( lib, "hiredis.lib") 

#else

#endif

#endif
