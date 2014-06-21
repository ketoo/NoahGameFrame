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

typedef std::function<int(const NFIPacket& msg)> RECIEVE_FUNCTOR;
typedef std::shared_ptr<RECIEVE_FUNCTOR> RECIEVE_FUNCTOR_PTR;

typedef std::function<int(const int nSockIndex, const NF_NET_EVENT nEvent)> EVENT_FUNCTOR;
typedef std::shared_ptr<EVENT_FUNCTOR> EVENT_FUNCTOR_PTR;

class NFCNet : public NFINet
{
public:
    template<typename BaseType>
    NFCNet(NFIMsgHead::NF_Head nHeadLength, BaseType* pBaseType, int (BaseType::*handleRecieve)(const NFIPacket&), int (BaseType::*handleEvent)(const int, const NF_NET_EVENT))
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

	virtual bool SendMsg(const NFIPacket& msg, const int nSockIndex = 0, bool bBroadcast = false);
	virtual bool SendMsg(const char* msg, const uint32_t nLen, const int nSockIndex = 0, bool bBroadcast = false);

    virtual bool CloseNetObject(const int nSockIndex);
    virtual NetObject* GetNetObject(const int nSockIndex);
    virtual bool AddNetObject(const int nSockIndex, NetObject* pObject);

	virtual bool AddBan(const int nSockIndex, const int32_t nTime = -1){return true;};
	virtual bool RemoveBan(const int nSockIndex){return true;};
	virtual void HeartPack();
    virtual NFIMsgHead::NF_Head GetHeadLen(){return mnHeadLength;};

private:
    virtual bool CloseSocketAll();

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