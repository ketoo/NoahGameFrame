////////////////////////////////////////////
//  Filename NFCHttpClient.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/07/05
//
////////////////////////////////////////////
#ifndef NFC_HTTP_CLIENT_H
#define NFC_HTTP_CLIENT_H

#include "NFIHttpClient.h"

#pragma pack(push, 1)


#define DEFAULT_USER_AGENT "Mozilla/5.0 (Linux; Android 6.0; Nexus 5 Build/MRA58N) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3071.115 Mobile Safari/537.36"

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

	virtual bool Initialization(const std::string& strUserAgent = std::string(DEFAULT_USER_AGENT));

	virtual bool Final();

	template<typename BaseType>
	bool PerformGet(const std::string& strUri, const std::string& strUserData,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		return PerformGet(strUri, strUserData, pd);
	}

	template<typename BaseType>
	bool PerformPost(const std::string& strUri, const std::string& strUserData, const std::string& strPostData,
		BaseType* pBase,
		void (BaseType::*handleRecieve)(const int state_code, const std::string& strRespData, const std::string& strUserData))
	{
		HTTP_RESP_FUNCTOR_PTR pd(new HTTP_RESP_FUNCTOR(std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
		return PerformPost(strUri, strUserData, strPostData, pd);
	}

private:
	static void OnHttpReqDone(struct evhttp_request *req, void *ctx);
	bool MakeRequest(const std::string& strUri,
		const std::string& strUserData,
		const std::string& strPostData,
		HTTP_RESP_FUNCTOR_PTR pCB,
		bool bPost = false);

	bool PerformGet(const std::string& strUri, const std::string& strUserData, HTTP_RESP_FUNCTOR_PTR pCB);

	bool PerformPost(const std::string& strUri, const std::string& strUserData, const std::string& strPostData, HTTP_RESP_FUNCTOR_PTR pCB);

private:
	std::string			m_strUserAgent;
	struct event_base*	m_pBase = nullptr;
#if NF_ENABLE_SSL
	SSL_CTX *			m_pSslCtx = nullptr;
#endif
	int					m_nRetry = 2;
	int					m_nTimeOut = 2;
};

#pragma pack(pop)

#endif
