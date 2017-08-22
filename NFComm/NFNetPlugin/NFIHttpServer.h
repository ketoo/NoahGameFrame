
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

#include "NFComm/NFPluginModule/NFPlatform.h"

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
#else
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#endif

enum NFWebStatus
{
	WEB_OK = 200,
	WEB_ERROR = 404,
	WEB_TIMEOUT = 503,
};

class NFHttpRequest
{
public:
	void* req;
	int nCommand;
	std::string url;
};

class NFHttpHeader
{

};

class NFHttpBody
{

};
//it should be
//typedef std::function<void(const NFHttpRequest& req, const int nCommand, const std::string& strData, const std::string& strUrl)> HTTP_RECEIVE_FUNCTOR;
typedef std::function<void(const NFHttpRequest& req, const std::string& strCommand, const std::string& strUrl)> HTTP_RECEIVE_FUNCTOR;
typedef std::shared_ptr<HTTP_RECEIVE_FUNCTOR> HTTP_RECEIVE_FUNCTOR_PTR;

class NFIHttpServer
{
public:
	virtual bool Execute() = 0;
	virtual int InitServer(const unsigned short nPort) = 0;
	virtual bool Final() = 0;

	virtual bool ResponseMsg(const NFHttpRequest & req, const std::string& strMsg, NFWebStatus code, const std::string& strReason = "OK") = 0;
	virtual bool ResponseFile(const NFHttpRequest& req, const std::string& strPath, const std::string& strFileName) = 0;

};

#endif
