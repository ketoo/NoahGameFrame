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
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_LINUX
#include <libkern/OSByteOrder.h>
#include <unistd.h>
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

	virtual bool PerformGet(const std::string& strUri, const std::string& strUserData, HTTP_RESP_FUNCTOR_PTR pCB) = 0;

	virtual bool PerformPost(const std::string& strUri, const std::string& strUserData, const std::string& strPostData, HTTP_RESP_FUNCTOR_PTR pCB) = 0;

};

#endif
