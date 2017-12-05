//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#ifndef NFREDISPLUGIN_NFREDISCLIENTSOCKET_H
#define NFREDISPLUGIN_NFREDISCLIENTSOCKET_H

#include <cstring>
#include <cerrno>
#include <cstdio>
#include <csignal>
#include <cstdint>
#include <iostream>
#include <map>
#include <vector>
#include <functional>
#include <memory>
#include <list>
#include <vector>
#include <atomic>

#include "NFComm/NFPluginModule/NFGUID.h"

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <WinSock2.h>
#include <windows.h>
#pragma comment(lib, "Ws2_32.lib")
#elif NF_PLATFORM == NF_PLATFORM_APPLE || NF_PLATFORM == NF_PLATFORM_LINUX || NF_PLATFORM == NF_PLATFORM_ANDROID

#if NF_PLATFORM == NF_PLATFORM_APPLE

#include <libkern/OSByteOrder.h>

#endif

#include <netinet/in.h>

#ifdef _XOPEN_SOURCE_EXTENDED
#  include <arpa/inet.h>
# endif

#include <sys/socket.h>
#include <unistd.h>

#endif

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/thread.h>
#include <event2/event_compat.h>
#include "event2/bufferevent_struct.h"
#include "event2/event.h"


class NFRedisClientSocket
{
public:
	enum NF_NET_EVENT
	{
		NF_NET_EVENT_NONE = 0,
		NF_NET_EVENT_EOF = 0x10,
		NF_NET_EVENT_ERROR = 0x20,
		NF_NET_EVENT_TIMEOUT = 0x40,
		NF_NET_EVENT_CONNECTED = 0x80,
	};

	NFRedisClientSocket();
	virtual ~NFRedisClientSocket();

	int64_t Connect(const std::string& ip, const int port);
    int Close();
    int Write(const char *buf, int count);

    bool ReadN(char *buf, int count);
    bool ReadLine(std::string& line);

    int ClearBuff();
    int BuffLength();
    int Execute();

    /////test
    void AddBuff(const std::string& buff);

protected:
	static void listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socklen, void* user_data);
	static void conn_readcb(struct bufferevent* bev, void* user_data);
	static void conn_writecb(struct bufferevent* bev, void* user_data);
	static void conn_eventcb(struct bufferevent* bev, short events, void* user_data);
	static void log_cb(int severity, const char* msg);

private:
	struct event_base* base;
	struct bufferevent* bev;
	struct evconnlistener* listener;
	NF_NET_EVENT mNetStatus;
    int64_t fd;
    std::string mstrBuff;
};


#endif //NFREDISPLUGIN_NFREDISCLIENTSOCKET_H
