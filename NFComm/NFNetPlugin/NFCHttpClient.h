////////////////////////////////////////////
//  Filename NFCHttpClient.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/07/05
//
////////////////////////////////////////////
#ifndef NFC_HTTP_CLIENT_H
#define NFC_HTTP_CLIENT_H

#include "NFIHttpClient.h"

class HttpObject
{
public:
    HttpObject(NFIHttpClient* pNet, struct bufferevent* pBev,  HTTP_RESP_FUNCTOR_PTR pCB)
    {
        m_pBev = pBev;
        m_pHttpClient = pNet;
        m_pCB = pCB;
    }

    virtual ~HttpObject()
    {
    }

    bufferevent* m_pBev;
    NFIHttpClient* m_pHttpClient;
    HTTP_RESP_FUNCTOR_PTR m_pCB;
};


class NFCHttpClient : public NFIHttpClient
{
public:
    NFCHttpClient(int nRetry = 2, int nTimeoutSec = 2)
            : m_nRetry(nRetry), m_nTimeOut(nTimeoutSec)
    {
    }

    virtual ~NFCHttpClient()
    {};

public:
    virtual bool Execute();

    virtual bool Init();

    virtual bool Final();

    virtual bool PerformGet(const std::string& strUri, HTTP_RESP_FUNCTOR_PTR pCB,
                            const std::map<std::string, std::string>& xHeaders);

    virtual bool PerformPost(const std::string& strUri, const std::string& strPostData, HTTP_RESP_FUNCTOR_PTR pCB,
                             const std::map<std::string, std::string>& xHeaders);

private:
    static void OnHttpReqDone(struct evhttp_request* req, void* ctx);

    bool MakeRequest(const std::string& strUri,
                     HTTP_RESP_FUNCTOR_PTR pCB,
                     const std::string& strPostData,
                     const std::map<std::string, std::string>& xHeaders,
                     bool bPost = false);

private:
    std::string m_strUserAgent;
    struct event_base* m_pBase = nullptr;

    int m_nRetry = 2;
    int m_nTimeOut = 2;

#if NF_ENABLE_SSL
    SSL_CTX *			m_pSslCtx = nullptr;
#endif
};

#endif
