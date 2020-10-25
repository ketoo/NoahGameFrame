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

#ifndef NF_NET_MODULE_H
#define NF_NET_MODULE_H

#include "NFNet.h"
#include <iostream>
#include <iosfwd>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFCore/NFQueue.hpp"
#include "NFComm/NFCore/NFPerformance.hpp"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"


#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif

#include "NFComm/NFMessageDefine/NFDefine.pb.h"

#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif

#if NF_PLATFORM != NF_PLATFORM_WIN
#include "NFComm/NFCore/NFException.hpp"
#endif

class NFNetModule : public NFINetModule
{
public:
    NFNetModule(NFIPluginManager* p);

    virtual ~NFNetModule();

	virtual bool Init();
	virtual bool AfterInit();

    //as client
    virtual void Initialization(const char* ip, const unsigned short nPort);

    //as server
    virtual int Initialization(const unsigned int nMaxClient, const unsigned short nPort, const int nCpuCount = 4);

    virtual unsigned int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20) override;

    virtual void RemoveReceiveCallBack(const int msgID);

    virtual bool AddReceiveCallBack(const int msgID, const NET_RECEIVE_FUNCTOR_PTR& cb);

    virtual bool AddReceiveCallBack(const NET_RECEIVE_FUNCTOR_PTR& cb);

    virtual bool AddEventCallBack(const NET_EVENT_FUNCTOR_PTR& cb);

    virtual bool Execute();


    virtual bool SendMsgWithOutHead(const int msgID, const std::string& msg, const NFSOCK sockIndex);
    virtual bool SendMsgToAllClientWithOutHead(const int msgID, const std::string& msg);

	virtual bool SendMsgPB(const uint16_t msgID, const google::protobuf::Message& xData, const NFSOCK sockIndex);
	virtual bool SendMsgPB(const uint16_t msgID, const google::protobuf::Message& xData, const NFSOCK sockIndex, const NFGUID id);
	virtual bool SendMsg(const uint16_t msgID, const std::string& xData, const NFSOCK sockIndex);
	virtual bool SendMsg(const uint16_t msgID, const std::string& xData, const NFSOCK sockIndex, const NFGUID id);

    virtual bool SendMsgPBToAllClient(const uint16_t msgID, const google::protobuf::Message& xData);

    virtual bool SendMsgPB(const uint16_t msgID, const google::protobuf::Message& xData, const NFSOCK sockIndex, const std::vector<NFGUID>* pClientIDList);
    virtual bool SendMsgPB(const uint16_t msgID, const std::string& strData, const NFSOCK sockIndex,  const std::vector<NFGUID>* pClientIDList);

    virtual NFINet* GetNet();

protected:
    void OnReceiveNetPack(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

    void OnSocketNetEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    void KeepAlive();

private:

    unsigned int mnBufferSize;
    NFINet* m_pNet;
    NFINT64 nLastTime;
	std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>> mxReceiveCallBack;
    std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBackList;
    std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;

	NFILogModule* m_pLogModule;
};

#endif