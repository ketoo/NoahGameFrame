////////////////////////////////////////////
//  Filename NFIHttpClient.h
//  Copyright (C) 2017  Stonexin
//  CreateTime 2017/07/05
//
////////////////////////////////////////////
#ifndef NFI_HTTP_SERVER_H
#define NFI_HTTP_SERVER_H

#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <assert.h>

#include "NFINet.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#else
#include <sys/stat.h>
#include <sys/socket.h>
#include <signal.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#endif

enum NFWebStatus
{
    WEB_OK = 200,
	WEB_AUTH = 401,
	WEB_ERROR = 404,
    WEB_TIMEOUT = 503,
};


enum NFHttpType
{
	NF_HTTP_REQ_GET = 1 << 0,
	NF_HTTP_REQ_POST = 1 << 1,
	NF_HTTP_REQ_HEAD = 1 << 2,
	NF_HTTP_REQ_PUT = 1 << 3,
	NF_HTTP_REQ_DELETE = 1 << 4,
	NF_HTTP_REQ_OPTIONS = 1 << 5,
	NF_HTTP_REQ_TRACE = 1 << 6,
	NF_HTTP_REQ_CONNECT = 1 << 7,
	NF_HTTP_REQ_PATCH = 1 << 8
};

class NFHttpRequest
{
public:
    void* req;
	std::string url;
	std::string path;
	std::string remoteHost;
	NFHttpType type;
	std::string body;//when using post
	std::map<std::string, std::string> params;//when using get
	std::map<std::string, std::string> headers;
};

//it should be
typedef std::function<bool(const NFHttpRequest& req)> HTTP_RECEIVE_FUNCTOR;
typedef std::shared_ptr<HTTP_RECEIVE_FUNCTOR> HTTP_RECEIVE_FUNCTOR_PTR;

typedef std::function<NFWebStatus(const NFHttpRequest& req)> HTTP_FILTER_FUNCTOR;
typedef std::shared_ptr<HTTP_FILTER_FUNCTOR> HTTP_FILTER_FUNCTOR_PTR;

class NFIHttpServer
{
public:
	template<typename BaseType>
	void AddFilter(BaseType* pBaseType, NFWebStatus (BaseType::*handleRecieve)(const NFHttpRequest& req))
	{
		HTTP_FILTER_FUNCTOR functor = std::bind(handleRecieve, pBaseType, std::placeholders::_1);
		HTTP_FILTER_FUNCTOR_PTR functorPtr(new HTTP_FILTER_FUNCTOR(functor));
		return AddFilter(functorPtr);
	}

    virtual bool Execute() = 0;

    virtual int InitServer(const unsigned short nPort) = 0;

	virtual bool Final() = 0;

    virtual bool ResponseMsg(const NFHttpRequest& req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK") = 0;

	virtual void AddFilter(const HTTP_FILTER_FUNCTOR_PTR& ptr) = 0;

};

#endif
