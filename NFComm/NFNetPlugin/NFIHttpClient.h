////////////////////////////////////////////
//  Filename NFIHttpClient.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/07/05
//
////////////////////////////////////////////
#ifndef NFI_HTTP_CLIENT_H
#define NFI_HTTP_CLIENT_H

#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <iostream>
#include <map>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <assert.h>

#include "NFComm/NFPluginModule/NFGUID.h"

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
#include <windows.h>
#include <WinSock2.h>
#endif

typedef std::function<void(const int state_code, const std::string& strRespData, const std::string& strUserData)> HTTP_RESP_FUNCTOR;
typedef std::shared_ptr<HTTP_RESP_FUNCTOR> HTTP_RESP_FUNCTOR_PTR;

class NFIHttpClient
{
public:
	virtual ~NFIHttpClient() {}

    virtual bool Execute() = 0;

	virtual bool Init() = 0;

	virtual bool Final() = 0;

	virtual bool PerformGet(const std::string& strUri, HTTP_RESP_FUNCTOR_PTR pCB,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders) = 0;

	virtual bool PerformPost(const std::string& strUri, const std::string& strPostData, HTTP_RESP_FUNCTOR_PTR pCB,
		const std::string& strUserData,
		const std::map<std::string, std::string>& xHeaders) = 0;

};

#endif
