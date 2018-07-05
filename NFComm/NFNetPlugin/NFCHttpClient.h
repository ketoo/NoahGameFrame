////////////////////////////////////////////
//  Filename NFCHttpClient.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/07/05
//
////////////////////////////////////////////
#ifndef NFC_HTTP_CLIENT_H
#define NFC_HTTP_CLIENT_H

#include "NFIHttpClient.h"
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
    HttpObject(NFIHttpClient* pNet, struct bufferevent* pBev, HTTP_RESP_FUNCTOR_PTR pCB, NFGUID id)
    {
        m_pBev = pBev;
        m_pHttpClient = pNet;
        m_pCB = pCB;

		mID = id;
    }

    virtual ~HttpObject()
    {
    }

	NFGUID mID;
    bufferevent* m_pBev;
    NFIHttpClient* m_pHttpClient;
    HTTP_RESP_FUNCTOR_PTR m_pCB;
};


class NFCHttpClient : public NFIHttpClient
{
public:
    NFCHttpClient(int nRetry = 2, int nTimeoutSec = 30)
            : m_nRetry(nRetry), m_nTimeOut(nTimeoutSec)
    {
    }

    virtual ~NFCHttpClient()
    {};

public:
    virtual bool Execute();

    virtual bool Init();

    virtual bool Final();

    virtual bool DoGet(const std::string& strUri, HTTP_RESP_FUNCTOR_PTR pCB,
                            const std::map<std::string, std::string>& xHeaders, const NFGUID id = NFGUID());

    virtual bool DoPost(const std::string& strUri, const std::string& strPostData, HTTP_RESP_FUNCTOR_PTR pCB,
                             const std::map<std::string, std::string>& xHeaders, const NFGUID id = NFGUID());

private:
    static void OnHttpReqDone(struct evhttp_request* req, void* ctx);

    bool MakeRequest(const std::string& strUri,
					HTTP_RESP_FUNCTOR_PTR pCB,
                    const std::string& strPostData,
                    const std::map<std::string, std::string>& xHeaders,
					const NFHttpType eHttpType,
					const NFGUID id = NFGUID());

private:
    std::string m_strUserAgent;
    struct event_base* m_pBase = nullptr;

    int m_nRetry = 2;
    int m_nTimeOut = 30;

    static std::list<HttpObject*> mlHttpObject;

#if NF_ENABLE_SSL
    SSL_CTX *			m_pSslCtx = nullptr;
#endif
};

#endif
