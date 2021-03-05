/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

#ifndef NF_NET_H
#define NF_NET_H

#include <set>
#include <algorithm>
#include <thread>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/thread.h>
#include <event2/event_compat.h>
#include "NFINet.h"
#include "Dependencies/concurrentqueue/concurrentqueue.h"

#pragma pack(push, 1)

class NFNet : public NFINet
{
public:
    NFNet()
    {
        mxBase = NULL;
        listener = NULL;

        mstrIP = "";
        mnPort = 0;
        mnCpuCount = 0;
        mbServer = false;
        mbWorking = false;

        mnSendMsgTotal = 0;
        mnReceiveMsgTotal = 0;

		mnBufferSize = 0;
        mbTCPStream = false;
    }

    template<typename BaseType>
    NFNet(BaseType* pBaseType, void (BaseType::*handleReceive)(const NFSOCK, const int, const char*, const uint32_t), void (BaseType::*handleEvent)(const NFSOCK, const NF_NET_EVENT, NFINet*), bool tcpStream = false)
    {
        mxBase = NULL;
        listener = NULL;

        mRecvCB = std::bind(handleReceive, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        mEventCB = std::bind(handleEvent, pBaseType, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        mstrIP = "";
        mnPort = 0;
        mnCpuCount = 0;
        mbServer = false;
        mbWorking = false;
        
        mnSendMsgTotal = 0;
        mnReceiveMsgTotal = 0;

		mnBufferSize = 0;
        mbTCPStream = tcpStream;
    }
    
    virtual ~NFNet() {};

public:
    virtual bool Execute() override ;

    virtual void Initialization(const char* ip, const unsigned short nPort) override ;
    virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4) override ;
	virtual unsigned int ExpandBufferSize(const unsigned int size) override;

    virtual bool Final() override ;

    virtual bool SendMsg(const char* msg, const size_t len, const NFSOCK sockIndex) override ;

    virtual bool SendMsgWithOutHead(const int16_t msgID, const char* msg, const size_t len, const NFSOCK sockIndex) override ;

	bool SendMsgToAllClient(const char* msg, const size_t len) override;

    virtual bool SendMsgToAllClientWithOutHead(const int16_t msgID, const char* msg, const size_t len) override ;


    virtual bool CloseNetObject(const NFSOCK sockIndex) override ;
    virtual bool AddNetObject(const NFSOCK sockIndex, NetObject* pObject) override ;
    virtual NetObject* GetNetObject(const NFSOCK sockIndex) override ;

    virtual bool IsServer() override ;
    virtual bool Log(int severity, const char* msg) override ;

private:
	bool SendMsgWithOutHead(const int16_t msgID, const char* msg, const size_t len, const std::list<NFSOCK>& fdList);

    bool SendMsg(const char* msg, const size_t len, const std::list<NFSOCK>& fdList);


private:
    void ExecuteClose();
    bool CloseSocketAll();

    bool Dismantle(NetObject* pObject);


    int InitClientNet();
    int InitServerNet();
    void CloseObject(const NFSOCK sockIndex);

    static void listener_cb(struct evconnlistener* listener, evutil_socket_t fd, struct sockaddr* sa, int socklen, void* user_data);
    static void conn_readcb(struct bufferevent* bev, void* user_data);
    static void conn_writecb(struct bufferevent* bev, void* user_data);
    static void conn_eventcb(struct bufferevent* bev, short events, void* user_data);
    static void log_cb(int severity, const char* msg);
    static void event_fatal_cb(int err);

protected:
    int DeCode(const char* strData, const uint32_t ulen, NFMsgHead& xHead);
    int EnCode(const uint16_t umsgID, const char* strData, const uint32_t unDataLen, std::string& strOutData);

private:
    //<fd,object>

	//std::multiset<NetObject*> mLiveBeatMap;

	//Use share pointer replace C-style pointer
    std::map<NFSOCK, NetObject*> mmObject;
    std::vector<NFSOCK> mvRemoveObject;

    int mnMaxConnect;
    std::string mstrIP;
    int mnPort;
    int mnCpuCount;
	bool mbServer;

    unsigned int mnBufferSize;

    bool mbWorking;
    bool mbTCPStream;

    int64_t mnSendMsgTotal;
    int64_t mnReceiveMsgTotal;

    struct event_base* mxBase;
    struct evconnlistener* listener;
    //////////////////////////////////////////////////////////////////////////

    NET_RECEIVE_FUNCTOR mRecvCB;
    NET_EVENT_FUNCTOR mEventCB;

    //1: async thread to process net event & msg and main thread to process logic business(decode binary data to message object)
    //2: pass a functor when startup net module to decode binary data to message object with async thread
    struct NetEvent
	{
		NF_NET_EVENT event;
		int fd = 0;
		//std::string* data;
		char* data = nullptr;
		int len = 0;

		void* dataObject = nullptr;
	};
	moodycamel::ConcurrentQueue<NetEvent> msgQueue;
    //////////////////////////////////////////////////////////////////////////
};

#pragma pack(pop)

#endif
