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
    NFCHttpServer(BaseType* pBaseType, bool (BaseType::*handleRecieve)(const NFHttpRequest& req), NFWebStatus (BaseType::*handleFilter)(const NFHttpRequest& req))
    {
        mxBase = NULL;
		mReceiveCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1);
		mFilter = std::bind(handleFilter, pBaseType, std::placeholders::_1);
    }

    virtual ~NFCHttpServer()
	{
		if (mxBase)
		{
			event_base_free(mxBase);
			mxBase = NULL;
		}
	};

    virtual bool Execute();

    virtual int InitServer(const unsigned short nPort);

    virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK");

private:
    static void listener_cb(struct evhttp_request* req, void* arg);

private:
    struct event_base* mxBase;
	HTTP_RECEIVE_FUNCTOR mReceiveCB;
	HTTP_FILTER_FUNCTOR mFilter;
};

#endif
