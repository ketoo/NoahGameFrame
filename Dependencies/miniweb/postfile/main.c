#include <windows.h>
#include <stdio.h>
#include "httpclient.h"

int main(int argc, char* argv[])
{
	WSADATA wsaData;
	if (argc <= 2) return -1;
	if ( WSAStartup( MAKEWORD( 2, 2 ), &wsaData ) ) {
		return 0;
	}

	//PostFileStream(argv[1], argv[2]);
	PostFile(argv[1], "file", argv[2]);

	WSACleanup( );
	getchar();
	return 0;
}