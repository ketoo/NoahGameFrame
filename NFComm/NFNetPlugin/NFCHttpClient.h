////////////////////////////////////////////
//  Filename NFCHttpClient.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/07/05
//
////////////////////////////////////////////
#ifndef NFC_HTTP_CLIENT_H
#define NFC_HTTP_CLIENT_H

#include "NFIHttpClient.h"

#define DEFAULT_USER_AGENT "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Mobile Safari/537.36"

class HttpObject
{
public:
	HttpObject(NFIHttpClient* pNet, struct bufferevent* pBev, const std::string& strUserData, HTTP_RESP_FUNCTOR_PTR pCB)
	{
		m_pHttpClient = pNet;
		m_pBev = pBev;
		m_strUserData = strUserData;
		m_pCB = pCB;
	}

	virtual ~HttpObject()
	{
	}

	void SetUri(const std::string& strUri) {
		m_strUri = strUri;
	}

	bufferevent*		m_pBev;
	std::string			m_strUserData;
	NFIHttpClient*		m_pHttpClient;
	HTTP_RESP_FUNCTOR_PTR m_pCB;
	std::string			m_strUri;
};


class NFCHttpClient : public NFIHttpClient
{
public:
	NFCHttpClient(int nRetry = 2, int nTimeoutSec = 2)
		:m_nRetry(nRetry)
		, m_nTimeOut(nTimeoutSec)
	{
	}

	virtual ~NFCHttpClient() {};

public:
	virtual bool Execute();

	virtual bool Init();

	virtual bool Final();

	virtual bool PerformGet(const std::string& strUri, const std::string& strUserData, HTTP_RESP_FUNCTOR_PTR pCB);

	virtual bool PerformPost(const std::string& strUri, const std::string& strUserData, const std::string& strPostData, HTTP_RESP_FUNCTOR_PTR pCB);

private:
	static void OnHttpReqDone(struct evhttp_request *req, void *ctx);

	bool MakeRequest(const std::string& strUri,
		const std::string& strUserData,
		const std::string& strPostData,
		HTTP_RESP_FUNCTOR_PTR pCB,
		bool bPost = false);

private:
	std::string			m_strUserAgent;
	struct event_base*	m_pBase = nullptr;

	int					m_nRetry = 2;
	int					m_nTimeOut = 2;

#if NF_ENABLE_SSL
	SSL_CTX *			m_pSslCtx = nullptr;
#endif
};

#endif
