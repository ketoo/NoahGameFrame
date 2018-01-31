//
// Author: LUSHENG HUANG Created on 17/11/17.
//

#include "NFRedisClient.h"
#include "NFRedisClientSocket.h"

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

//1048576 = 1024 * 1024
#define NF_BUFFER_MAX_READ	1048576

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

int NFRedisClientSocket::Close()
{
    return 0;
}

bool NFRedisClientSocket::ReadLineFromBuff(std::string &line)
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
			return false;
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

int NFRedisClientSocket::GetLineNum()
{
	return mLineList.size();
}

bool NFRedisClientSocket::TryPredictType(char& eType)
{
	bool bFindType = false;
	while (!bFindType)
	{
		if (mLineList.size() <= 0)
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
		else
		{
			std::string& line = mLineList.front();
			eType = line.data()[0];
			bFindType = true;
		}
	}

	return true;
}

bool NFRedisClientSocket::ReadLine(std::string & line)
{
	bool bFindLine = false;
	while (!bFindLine)
	{
		if (mLineList.size() <= 0)
		{
			if (YieldFunction)
			{
				YieldFunction();
			}
			else
			{
				Execute();
			}
		}
		else
		{
			bFindLine = true;

			line = mLineList.front();
			mLineList.pop_front();
		}
	}
	
	return true;
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

	static char* mstrTempBuffData = nullptr;
	if (mstrTempBuffData == nullptr)
	{
		mstrTempBuffData = new char[NF_BUFFER_MAX_READ];
	}

	int nDataLen = len;
	if (len > NF_BUFFER_MAX_READ)
	{
		nDataLen = NF_BUFFER_MAX_READ;
	}

	if (evbuffer_remove(input, mstrTempBuffData, len) > 0)
	{
		pClientSocket->mstrBuff.append(mstrTempBuffData, len);
	}

	//push back as a new line
	bool b = true;
	while (b)
	{
		std::string line;
		if (pClientSocket->ReadLineFromBuff(line))
		{
			pClientSocket->mLineList.push_back(line);
		}
		else
		{
			b = false;
		}
	}
	
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
