////////////////////////////////////////////
//  Filename NFIHttpClient.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/07/05
//
////////////////////////////////////////////
#ifndef NFI_HTTP_CLIENT_H
#define NFI_HTTP_CLIENT_H

#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <iostream>
#include <map>

#include "NFComm/NFPluginModule/NFGUID.h"

#ifndef _MSC_VER
#include <netinet/in.h>
#ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <assert.h>
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

#if NF_ENABLE_SSL

#include <event2/bufferevent_ssl.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
#include <openssl/rand.h>

#endif

#if NF_PLATFORM == NF_PLATFORM_WIN
//#ifdef _MSC_VER
#include <windows.h>
#include <WinSock2.h>
//#elseifdef _APPLE_
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_APPLE_IOS
#include <libkern/OSByteOrder.h>
#else
#include <unistd.h>
#endif

#pragma pack(push, 1)

typedef std::function<void(const int state_code, const std::string& strRespData, const std::string& strUserData)> HTTP_RESP_FUNCTOR;
typedef std::shared_ptr<HTTP_RESP_FUNCTOR> HTTP_RESP_FUNCTOR_PTR;

class NFIHttpClient;

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

class NFIHttpClient
{
public:
	virtual ~NFIHttpClient() {}

    //need to call this function every frame to drive network library
    virtual bool Execute() = 0;

	virtual bool Initialization(const std::string& strUserAgent) = 0;

	virtual bool Final() = 0;

};

#pragma pack(pop)

#endif
