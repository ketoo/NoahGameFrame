// ConsoleApplication1.cpp : Defines the entry point for the console application.
//



#ifndef __NFC_NET_H__
#define __NFC_NET_H__

#include "NFINet.h"
#include "NFIPacket.h"
#include <chrono>
#include <thread>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "NFCPacket.h"
#include <boost/function.hpp>

#pragma pack(push, 1)

enum NF_NET_EVENT
{
	NF_NET_EVENT_EOF = 0x10,	//掉线
	NF_NET_EVENT_ERROR = 0x20,//位置错误
	NF_NET_EVENT_TIMEOUT = 0x40,//连接超时
	NF_NET_EVENT_CONNECTED = 0x80,//连接成功(作为客户端)
};

class NFCNet;

class NetObject 
{
public:
	NetObject(NFCNet* pThis, int16_t nFd, sockaddr_in& addr, bufferevent* pBev)
	{
        m_pNet = pThis;

		nIndex = nFd;
		nBuffLen = 0;
		bev = pBev;
		memset(&sin, 0, sizeof(sin));
		sin = addr;

		mnErrorCount = 0;
	}

    ~NetObject()
    {
        if (bev)
        {
            free(bev);
        }
    }

	int AddBuff(const char* str, uint16_t nLen)
	{
		if (nBuffLen + nLen >= NF_MAX_SERVER_PACKET_SIZE)
		{
			IncreaseError(1000);
			return 0;
		}

		memcpy(strBuff + nBuffLen, str, nLen);
		nBuffLen += nLen;

		return nLen;
	}

	int CopyBuffTo(char* str, uint16_t nStart, uint16_t nLen)
	{
		if (nStart + nLen > NF_MAX_SERVER_PACKET_SIZE)
		{
			return 0;
		}

		memcpy(str, strBuff + nStart, nLen);

		return nLen;
	}

	int RemoveBuff(uint16_t nStart, uint16_t nLen)
	{
		//移除前面，则后面跟上
		if (nStart + nLen >= NF_MAX_SERVER_PACKET_SIZE)
		{
			return 0;
		}

		if (nLen > nBuffLen)
		{
			return 0;
		}

		if (nStart + nLen > nBuffLen)
		{
			return 0;
		}

		//把后面的挪到前面来
		memcpy(strBuff + nStart, strBuff + nStart + nLen, nBuffLen - nLen - nStart);

		nBuffLen -= nLen;
		return nLen;
	}

	char* GetBuff()
	{
		return strBuff;
	}

	int GetBuffLen() const
	{
		return nBuffLen;
	}

	int GetFd() const
	{
		return nIndex;
	}

	int GetErrorCount() const
	{
		return mnErrorCount;
	}

	int IncreaseError(const int nError = 1)
	{
		return mnErrorCount += nError;
	}

	bufferevent * GetBuffEvent()
	{
		return bev;
	}

    NFCNet* GetNet()
    {
        return m_pNet;
    }
private:
	uint16_t nIndex;
	sockaddr_in sin;
	bufferevent *bev;
	uint16_t nBuffLen;
	char strBuff[NF_MAX_SERVER_PACKET_SIZE];

	uint16_t mnErrorCount;
    NFCNet* m_pNet;
};

// NFINet* pNet = new NFCNet();
// //pNet->Initialization("42.62.64.76", 9001);
// pNet->Initialization(10000, 9001);
// while (1)
// {
//     std::this_thread::sleep_for(std::chrono::microseconds(1));
//     pNet->Execute(0.0f, 0.0f);
// }
// 
// pNet->Final();
typedef int(* ON_RECIVE_FUNC)(const NFIPacket& msg);
typedef int(* ON_EVENT_FUNC)(const uint16_t nSockIndex, const NF_NET_EVENT nEvent);

typedef boost::function<int(const NFIPacket& msg)> RECIEVE_FUNCTOR;
typedef std::shared_ptr<RECIEVE_FUNCTOR> RECIEVE_FUNCTOR_PTR;

typedef boost::function<int(const uint16_t nSockIndex, const NF_NET_EVENT nEvent)> EVENT_FUNCTOR;
typedef std::shared_ptr<EVENT_FUNCTOR> EVENT_FUNCTOR_PTR;

class NFCNet : public NFINet
{
public:

	NFCNet()
	{
		base = NULL;
		listener = NULL;
		signal_event = NULL;
		mCB = NULL;
		mECB = NULL;
		mstrIP = "";
		mnPort = 0;
		mnCpuCount = 0;
		mbServer = false;
		mbRuning = false;
        mbUsePacket = true;
		ev = NULL;
	};

	NFCNet(ON_RECIVE_FUNC cb, ON_EVENT_FUNC ecb)
	{
		base = NULL;
		listener = NULL;
		signal_event = NULL;
		mCB = cb;
		mECB = ecb;
		mstrIP = "";
		mnPort = 0;
		mnCpuCount = 0;
		mbServer = false;
		mbRuning = false;
        mbUsePacket = true;
		ev = NULL;
	};

    template<typename BaseType>
    NFCNet(BaseType* pBaseType, int (BaseType::*handleRecieve)(const NFIPacket&), int (BaseType::*handleEvent)(const uint16_t, const NF_NET_EVENT))
    {
        base = NULL;
        listener = NULL;
        signal_event = NULL;
        mRecvCB = boost::bind(handleRecieve, pBaseType, _1);
        mEventCB = boost::bind(handleEvent, pBaseType, _1, _2);
        mstrIP = "";
        mnPort = 0;
        mnCpuCount = 0;
        mbServer = false;
        mbRuning = false;
        mbUsePacket = true;
        ev = NULL;
    }

	virtual ~NFCNet(){};

public:
	virtual  bool Execute(const float fLasFrametime, const float fStartedTime);

	virtual  int Initialization(const char* strIP, const unsigned short nPort,const bool bUsePacket = true);
	virtual  int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4, const bool bUsePacket = true);

	virtual  bool Final();
    virtual  bool Reset();

	virtual bool SendMsg(const NFIPacket& msg, const uint16_t nSockIndex = 0, bool bBroadcast = false);
	virtual bool SendMsg(const uint16_t msgID, const char* msg, const uint16_t nLen, const uint16_t nSockIndex = 0, bool bBroadcast = false);

	virtual bool CloseSocket(const uint16_t nSockIndex);

	virtual bool AddBan(const uint16_t nSockIndex, const int32_t nTime = -1){return true;};
	virtual bool RemoveBan(const uint16_t nSockIndex){return true;};
	virtual void HeartPack();

private:
	virtual bool AddSocket(const uint16_t nSockIndex, NetObject* pObject);
	virtual bool Dismantle(NetObject* pObject);

	virtual int InitClientNet();
	virtual int InitServerNet();

	static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sa, int socklen, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
	static void signal_cb(evutil_socket_t sig, short events, void *user_data);
	static void time_cb(evutil_socket_t fd, short _event, void *argc);
private:
	//<fd,object>
	std::map<int, NetObject*> mmObject;
	int mnMaxConnect;
	std::string mstrIP;
	int mnPort;
	int mnCpuCount;
	bool mbServer;
	bool mbRuning;
    bool mbUsePacket;
	struct event_base *base;
	struct evconnlistener *listener;
	struct event *signal_event;
	//////////////////////////////////////////////////////////////////////////
	struct timeval tv;
	struct event* ev;
	//////////////////////////////////////////////////////////////////////////
	ON_RECIVE_FUNC mCB;
	ON_EVENT_FUNC mECB;

    // 暂时还未使用
    RECIEVE_FUNCTOR mRecvCB;
    EVENT_FUNCTOR mEventCB;

	std::string mstrPwd;


	//////////////////////////////////////////////////////////////////////////
};

#pragma pack(pop)

#endif