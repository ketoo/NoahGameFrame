
#ifndef NFI_HTTP_NET_H
#define NFI_HTTP_NET_H

#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <iostream>
#include <map>

#ifndef _MSC_VER
#include <netinet/in.h>
# ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif
#include <sys/socket.h>
#endif

#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/thread.h>
#include <event2/event_compat.h>
#include <assert.h>

#ifdef _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#endif

typedef std::function<void(struct evhttp_request *req, const std::string& strCommand, const std::string& strUrl)> HTTPNET_RECEIVE_FUNCTOR;
typedef std::shared_ptr<HTTPNET_RECEIVE_FUNCTOR> HTTPNET_RECEIVE_FUNCTOR_PTR;

class _NFExport NFIHttpNet
{
public:
	virtual bool Execute() = 0;
	virtual int InitServer(const unsigned short nPort) = 0;
	virtual bool Final() = 0;
};

#endif
