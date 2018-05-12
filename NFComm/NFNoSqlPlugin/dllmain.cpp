#ifdef _DEBUG

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "hiredis_d.lib") 
#pragma comment( lib, "Win32_Interop_d.lib") 
#else

#endif

#else

#if NF_PLATFORM == NF_PLATFORM_WIN
#pragma comment( lib, "ws2_32" )
#pragma comment( lib, "hiredis.lib") 
#pragma comment( lib, "Win32_Interop.lib") 

#else

#endif

#endif
