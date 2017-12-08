//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <WS2tcpip.h>
#include <winsock2.h>
#pragma  comment(lib,"Ws2_32.lib")
#ifndef LIBEVENT_SRC
#pragma  comment(lib,"libevent.lib")
#pragma  comment(lib,"libevent_core.lib")
#endif
#elif NF_PLATFORM == NF_PLATFORM_APPLE
#include <arpa/inet.h>
#endif


#include "NFRedisClient.h"
#include "NFRedisClientSocket.h"

NFRedisClientSocket::NFRedisClientSocket()
{
	mNetStatus = NF_NET_EVENT::NF_NET_EVENT_CONNECTED;
}

NFRedisClientSocket::~NFRedisClientSocket()
{

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

void NFRedisClientSocket::AddBuff(const std::string & buff)
{
}

int NFRedisClientSocket::Close()
{
    return 0;
}

bool NFRedisClientSocket::ReadLine(std::string &line)
{
	line.clear();

	bool bFindLine = false;
	int len = -1;
	const char* p = mstrBuff.data();

	while (!bFindLine)
	{
		for (int i = 0; i < mstrBuff.length(); ++i)
		{
			p++;
			if (i < (mstrBuff.length() - 1) && *p == '\r' && *(p + 1) == '\n')
			{
				len = i;
				bFindLine = true;
				break;
			}
		}

		if (!bFindLine)
		{
			//yield
			if (YieldFunction)
			{
				YieldFunction();
			}
			else
			{
				Execute();
			}
		}
	}

	int nTotalLen = len + 1 + NFREDIS_SIZEOF_CRLF;
	line.append(mstrBuff.data(), nTotalLen);
	mstrBuff.erase(0, nTotalLen);

	return true;
}

int NFRedisClientSocket::Write(const char *buf, int count)
{
	if (buf == NULL || count <= 0)
	{
		return 0;
	}

	if (NULL != bev && mNetStatus == NF_NET_EVENT::NF_NET_EVENT_CONNECTED)
	{
		return bufferevent_write(bev, buf, count);
	}

    return 0;
}

bool NFRedisClientSocket::ReadN(char *buf, int count)
{
	while (count > mstrBuff.length())
	{
		//yeild
		if (YieldFunction)
		{
			YieldFunction();
		}
		else
		{
			Execute();
		}
	}

	if (mstrBuff.length() >= count)
	{
		memcpy(buf, mstrBuff.data(), count);
		mstrBuff.erase(0, count);

		return true;
	}

	return false;
}

int NFRedisClientSocket::ClearBuff()
{
    mstrBuff = "";

	return 0;
}

int NFRedisClientSocket::BuffLength()
{
	return mstrBuff.length();
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

	//////////////////////////////////////////////////////////////////////////


	char* strMsg = new char[len];

	if (evbuffer_remove(input, strMsg, len) > 0)
	{
		pClientSocket->mstrBuff.append(strMsg, len);
	}

	delete[] strMsg;
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
