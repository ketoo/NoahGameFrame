// -------------------------------------------------------------------------
//    @FileName         ：    NFCNet.h
//    @Author           ：    LvSheng.Huang
//    @Date             ：    2013-12-15
//    @Module           ：    NFIPacket
//    @Desc             :     CNet
// -------------------------------------------------------------------------

#ifndef __NFC_NET_H__
#define __NFC_NET_H__

#include "NFINet.h"
#include "NFIPacket.h"
#include <chrono>
#include <thread>

#if NF_PLATFORM == NF_PLATFORM_WIN
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "NFCPacket.h"
#include <functional>
#include <memory>
#include <list>

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
	NetObject(NFCNet* pThis, int32_t nFd, sockaddr_in& addr, bufferevent* pBev)
	{
        m_pNet = pThis;

		nIndex = nFd;
		bev = pBev;
		memset(&sin, 0, sizeof(sin));
		sin = addr;

		mnErrorCount = 0;
	}

    ~NetObject()
    {
    }

	int AddBuff(const char* str, uint32_t nLen)
	{
        mstrBuff.append(str, nLen);

		return mstrBuff.length();
	}

	int CopyBuffTo(char* str, uint32_t nStart, uint32_t nLen)
	{
		if (nStart + nLen > mstrBuff.length())
		{
			return 0;
		}

		memcpy(str, mstrBuff.data() + nStart, nLen);

		return nLen;
	}

	int RemoveBuff(uint32_t nStart, uint32_t nLen)
	{
        if (nStart < 0)
        {
            return 0;
        }

		//移除前面，则后面跟上
		if (nStart + nLen > mstrBuff.length())
		{
			return 0;
		}

		//把后面的挪到前面来
		mstrBuff.erase(nStart, nLen);

        std::cout << "剩余长度:" << mstrBuff.length() << std::endl;

		return mstrBuff.length();
	}

	const char* GetBuff()
	{
		return mstrBuff.data();
	}

	int GetBuffLen() const
	{
		return mstrBuff.length();
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
	uint32_t nIndex;
	sockaddr_in sin;
	bufferevent *bev;
    std::string mstrBuff;

	uint16_t mnErrorCount;
    NFCNet* m_pNet;
};


typedef std::function<int(const NFIPacket& msg)> RECIEVE_FUNCTOR;
typedef std::shared_ptr<RECIEVE_FUNCTOR> RECIEVE_FUNCTOR_PTR;

typedef std::function<int(const uint32_t nSockIndex, const NF_NET_EVENT nEvent)> EVENT_FUNCTOR;
typedef std::shared_ptr<EVENT_FUNCTOR> EVENT_FUNCTOR_PTR;

class NFCNet : public NFINet
{
public:
    template<typename BaseType>
    NFCNet(NFIMsgHead::NF_Head nHeadLength, BaseType* pBaseType, int (BaseType::*handleRecieve)(const NFIPacket&), int (BaseType::*handleEvent)(const uint16_t, const NF_NET_EVENT))
    {
        base = NULL;
        listener = NULL;

        mRecvCB = std::bind(handleRecieve, pBaseType, std::placeholders::_1);
        mEventCB = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2);
        mstrIP = "";
        mnPort = 0;
        mnCpuCount = 0;
        mbServer = false;
        ev = NULL;
        mnHeadLength = nHeadLength;
    }

	virtual ~NFCNet(){};

public:
	virtual  bool Execute(const float fLasFrametime, const float fStartedTime);

	virtual  int Initialization(const char* strIP, const unsigned short nPort);
	virtual  int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4);

	virtual  bool Final();
    virtual  bool Reset();

	virtual bool SendMsg(const NFIPacket& msg, const uint32_t nSockIndex = 0, bool bBroadcast = false);
	virtual bool SendMsg(const char* msg, const uint32_t nLen, const uint32_t nSockIndex = 0, bool bBroadcast = false);

	virtual bool CloseSocket(const uint32_t nSockIndex);

	virtual bool AddBan(const uint32_t nSockIndex, const int32_t nTime = -1){return true;};
	virtual bool RemoveBan(const uint32_t nSockIndex){return true;};
	virtual void HeartPack();
    virtual NFIMsgHead::NF_Head GetHeadLen(){return mnHeadLength;};

private:
    virtual bool CloseSocketAll();

	virtual bool AddSocket(const uint32_t nSockIndex, NetObject* pObject);
	virtual bool Dismantle(NetObject* pObject);

	virtual int InitClientNet();
	virtual int InitServerNet();

	static void listener_cb(struct evconnlistener *listener, evutil_socket_t fd,struct sockaddr *sa, int socklen, void *user_data);
	static void conn_readcb(struct bufferevent *bev, void *user_data);
	static void conn_writecb(struct bufferevent *bev, void *user_data);
	static void conn_eventcb(struct bufferevent *bev, short events, void *user_data);
	static void time_cb(evutil_socket_t fd, short _event, void *argc);
private:
	//<fd,object>
	std::map<int, NetObject*> mmObject;
    
    NFIMsgHead::NF_Head mnHeadLength;

	int mnMaxConnect;
	std::string mstrIP;
	int mnPort;
	int mnCpuCount;
	bool mbServer;
    bool mbUsePacket;//是否使用我们的包
	struct event_base *base;
	struct evconnlistener *listener;
	//////////////////////////////////////////////////////////////////////////
	struct timeval tv;
	struct event* ev;
	//////////////////////////////////////////////////////////////////////////


    // 暂时还未使用
    RECIEVE_FUNCTOR mRecvCB;
    EVENT_FUNCTOR mEventCB;

	std::string mstrPwd;


	//////////////////////////////////////////////////////////////////////////
};

#pragma pack(pop)

#endif