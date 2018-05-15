//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"
#include "NFRedisClientSocket.h"

#if NF_PLATFORM == NF_PLATFORM_WIN

#ifndef LIBEVENT_SRC
#pragma  comment(lib,"libevent.lib")
#pragma  comment(lib,"libevent_core.lib")
#endif
#elif NF_PLATFORM == NF_PLATFORM_APPLE
#include <arpa/inet.h>
#endif

//1048576 = 1024 * 1024
#define NF_BUFFER_MAX_READ	1048576

NFRedisClientSocket::NFRedisClientSocket()
{
	mNetStatus = NF_NET_EVENT::NF_NET_EVENT_NONE;
	m_pRedisReader = redisReaderCreate();
}

NFRedisClientSocket::~NFRedisClientSocket()
{
	if (m_pRedisReader)
	{
		redisReaderFree(m_pRedisReader);
	}
}

int64_t NFRedisClientSocket::Connect(const std::string &ip, const int port)
{
	struct sockaddr_in addr;

#ifdef _MSC_VER
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	if (evutil_inet_pton(AF_INET, ip.c_str(), &addr.sin_addr) <= 0)
	{
		printf("inet_pton");
		return -1;
	}

	base = event_base_new();
	if (base == NULL)
	{
		printf("event_base_new ");
		return -1;
	}

	bev = bufferevent_socket_new(base, -1, BEV_OPT_CLOSE_ON_FREE);
	if (bev == NULL)
	{
		printf("bufferevent_socket_new ");
		return -1;
	}

	int bRet = bufferevent_socket_connect(bev, (struct sockaddr*)&addr, sizeof(addr));
	if (0 != bRet)
	{
		//int nError = GetLastError();
		printf("bufferevent_socket_connect error");
		return -1;
	}

	fd = bufferevent_getfd(bev);

	bufferevent_setcb(bev, conn_readcb, conn_writecb, conn_eventcb, this);
	bufferevent_enable(bev, EV_READ | EV_WRITE);

	event_set_log_callback(&NFRedisClientSocket::log_cb);

	return fd;
}

int NFRedisClientSocket::Execute()
{
	if (base)
	{
		event_base_loop(base, EVLOOP_ONCE | EVLOOP_NONBLOCK);
	}

	return 0;
}

bool NFRedisClientSocket::IsConnect()
{
	return mNetStatus == NF_NET_EVENT::NF_NET_EVENT_CONNECTED;
}

redisReader * NFRedisClientSocket::GetRedisReader()
{
	return m_pRedisReader;
}

int NFRedisClientSocket::Close()
{
    return 0;
}

int NFRedisClientSocket::Write(const char *buf, size_t count)
{
	if (buf == NULL || count <= 0)
	{
		return 0;
	}

	if (bev == nullptr || mNetStatus != NF_NET_EVENT::NF_NET_EVENT_CONNECTED)
	{
		return -1;
	}

	return bufferevent_write(bev, buf, count);
}

void NFRedisClientSocket::listener_cb(evconnlistener * listener, evutil_socket_t fd, sockaddr * sa, int socklen, void * user_data)
{
	
}

void NFRedisClientSocket::conn_readcb(bufferevent * bev, void * user_data)
{
	NFRedisClientSocket* pClientSocket = (NFRedisClientSocket*)user_data;

	struct evbuffer* input = bufferevent_get_input(bev);
	if (!input)
	{
		return;
	}

	size_t len = evbuffer_get_length(input);
	unsigned char *pData = evbuffer_pullup(input, len);
	redisReaderFeed(pClientSocket->m_pRedisReader, (const char *)pData, len);
	evbuffer_drain(input, len);
}

void NFRedisClientSocket::conn_writecb(bufferevent * bev, void * user_data)
{
}

void NFRedisClientSocket::conn_eventcb(bufferevent * bev, short events, void * user_data)
{
	NFRedisClientSocket* pClientSocket = (NFRedisClientSocket*)user_data;
	if (events & BEV_EVENT_CONNECTED)
	{
		pClientSocket->mNetStatus = NF_NET_EVENT::NF_NET_EVENT_CONNECTED;
	}
	else if (events & BEV_EVENT_TIMEOUT)
	{
		pClientSocket->mNetStatus = NF_NET_EVENT::NF_NET_EVENT_TIMEOUT;
	}
	else
	{
		pClientSocket->mNetStatus = NF_NET_EVENT::NF_NET_EVENT_ERROR;
	}
}

void NFRedisClientSocket::log_cb(int severity, const char * msg)
{
}
