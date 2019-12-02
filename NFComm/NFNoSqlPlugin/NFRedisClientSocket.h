/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

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
#include <event2/bufferevent_struct.h>
#include <event2/event.h>

#include <hiredis/hiredis.h>


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
	bool ReConnect(const std::string& ip, const int port);
    int Close();
    int Write(const char *buf, size_t count);

    int Execute();

	bool IsConnect();
	redisReader* GetRedisReader();
protected:
	static void listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socklen, void* user_data);
	static void conn_readcb(struct bufferevent* bev, void* user_data);
	static void conn_writecb(struct bufferevent* bev, void* user_data);
	static void conn_eventcb(struct bufferevent* bev, short events, void* user_data);
	static void log_cb(int severity, const char* msg);

	string GetIP(const std::string& url);

private:
	struct event_base* base;
	struct bufferevent* bev;
	struct evconnlistener* listener;

	NF_NET_EVENT mNetStatus;
	redisReader* m_pRedisReader;
	NFINT64 fd_;
};


#endif //NFREDISPLUGIN_NFREDISCLIENTSOCKET_H
