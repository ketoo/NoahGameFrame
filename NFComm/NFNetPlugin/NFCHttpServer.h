/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: Stonexin
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef NFC_HTTP_SERVER_H
#define NFC_HTTP_SERVER_H

#include "NFIHttpServer.h"
#include "NFComm/NFCore/NFException.hpp"

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
		mnIndex = 0;
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

	NFHttpRequest* AllowHttpRequest();

private:
    struct event_base* mxBase;
	HTTP_RECEIVE_FUNCTOR mReceiveCB;
	HTTP_FILTER_FUNCTOR mFilter;

	int64_t mnIndex;
	std::map<int64_t, NFHttpRequest*> mxHttpRequestMap;
	std::list<NFHttpRequest*> mxHttpRequestPool;
};

#endif
