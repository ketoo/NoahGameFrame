/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

#ifndef NF_HTTP_CLIENT_H
#define NF_HTTP_CLIENT_H


#include "NFIHttpClient.h"
#include "NFIHttpServer.h"
#include "NFIHttpServer.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <io.h>
#include <fcntl.h>
#ifndef S_ISDIR
#define S_ISDIR(x) (((x) & S_IFMT) == S_IFDIR)
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
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/http.h>
#include <event2/thread.h>
#include <event2/event_compat.h>
#include <event2/bufferevent_struct.h>
#include <event2/http_struct.h>
#include <event2/event.h>

//it would be a pool
class HttpObject
{
public:
    HttpObject(NFIHttpClient* pNet, struct bufferevent* pBev, HTTP_RESP_FUNCTOR_PTR pCB, NFGUID id, const std::string aMemo ="")
    {
        m_pBev = pBev;
        m_pHttpClient = pNet;
        m_pCB = pCB;

		mID = id;
        strMemo = aMemo;
    }

    virtual ~HttpObject()
    {
    }

	NFGUID mID;
    bufferevent* m_pBev;
    NFIHttpClient* m_pHttpClient;
    HTTP_RESP_FUNCTOR_PTR m_pCB;
    std::string strMemo;
};


class NFHttpClient : public NFIHttpClient
{
public:
    NFHttpClient(int nRetry = 2, int nTimeoutSec = 30)
            : m_nRetry(nRetry), m_nTimeOut(nTimeoutSec)
    {
    }

    virtual ~NFHttpClient()
    {};

public:
    virtual bool Execute();

    virtual bool Init();

    virtual bool Final();

    virtual bool DoGet(const std::string& strUri, HTTP_RESP_FUNCTOR_PTR pCB,
                            const std::map<std::string, std::string>& xHeaders, const NFGUID id = NFGUID());

    virtual bool DoPost(const std::string& strUri, const std::string& strPostData, const std::string& strMemoData, HTTP_RESP_FUNCTOR_PTR pCB,
                             const std::map<std::string, std::string>& xHeaders, const NFGUID id = NFGUID());

private:
    static void OnHttpReqDone(struct evhttp_request* req, void* ctx);

    bool MakeRequest(const std::string& strUri,
					HTTP_RESP_FUNCTOR_PTR pCB,
                    const std::string& strPostData,
                    const std::map<std::string, std::string>& xHeaders,
					const NFHttpType eHttpType,
                    const std::string& strMemoData,
                    const NFGUID id = NFGUID());

private:
    std::string m_strUserAgent;
    struct event_base* m_pBase = nullptr;

    int m_nRetry = 2;
    int m_nTimeOut = 30;

    std::list<HttpObject*> mlHttpObject;

#if NF_ENABLE_SSL
    SSL_CTX *			m_pSslCtx = nullptr;
#endif
};

#endif
