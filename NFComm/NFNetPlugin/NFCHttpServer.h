#ifndef NFC_HTTP_SERVER_H
#define NFC_HTTP_SERVER_H

#include "NFIHttpServer.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
#endif
#ifndef LIBEVENT_SRC
#pragma comment( lib, "libevent.lib")
#endif

#else

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <dirent.h>
#include <atomic>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#endif

#include <event2/bufferevent.h>
#include "event2/bufferevent_struct.h"
#include "event2/event.h"
#include <event2/http.h>
#include <event2/buffer.h>
#include <event2/util.h>
#include <event2/keyvalq_struct.h>



class NFCHttpServer : public NFIHttpServer
{
public:
	NFCHttpServer()
	{
	}

	template<typename BaseType>
	NFCHttpServer(BaseType* pBaseType, void (BaseType::*handleRecieve)(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl))
	{
		base = NULL;
		mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		mPort = 0;
	}
	
	virtual ~NFCHttpServer() {};

	virtual bool Execute();
	virtual int InitServer(const unsigned short nPort);
	virtual bool Final();

	virtual bool ResponseMsg(const NFHttpRequest & req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK");
	
	virtual bool ResponseFile(const NFHttpRequest& req, const std::string& strPath, const std::string& strFileName);

private:

	bool ResponseFile(const NFHttpRequest& req, const int fd, struct stat st, const std::string& strType);

	static void listener_cb(struct evhttp_request *req, void *arg);
	static std::vector<std::string> Split(const std::string& str, std::string delim);

private:
	int mPort;
	struct event_base* base;
	HTTP_RECEIVE_FUNCTOR mRecvCB;
};

#endif
