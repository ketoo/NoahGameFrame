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

#ifndef NF_NET_CLIENT_MODULE_H
#define NF_NET_CLIENT_MODULE_H

#include <iostream>
#include <iosfwd>
#include "NFComm/NFCore/NFQueue.hpp"
#include "NFComm/NFCore/NFConsistentHash.hpp"
#include "NFComm/NFMessageDefine/NFDefine.pb.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

class NFNetClientModule : public NFINetClientModule
{
public:
    NFNetClientModule(NFIPluginManager* p);

    virtual bool Init();

    virtual bool AfterInit();

    virtual bool BeforeShut();

    virtual bool Shut();

    virtual bool Execute();

    virtual void AddServer(const ConnectData& xInfo);

    virtual int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20);

    virtual int AddReceiveCallBack(const NF_SERVER_TYPES eType, NET_RECEIVE_FUNCTOR_PTR functorPtr);

    virtual int AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID, NET_RECEIVE_FUNCTOR_PTR functorPtr);

    virtual int AddEventCallBack(const NF_SERVER_TYPES eType, NET_EVENT_FUNCTOR_PTR functorPtr);

    virtual void RemoveReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID);

    ////////////////////////////////////////////////////////////////////////////////
	virtual void SendByServerIDWithOutHead(const int nServerID, const uint16_t nMsgID, const std::string& strData);

	virtual void SendByServerID(const int nServerID, const uint16_t nMsgID, const std::string& strData);
	virtual void SendByServerID(const int nServerID, const uint16_t nMsgID, const std::string& strData, const NFGUID id);

	virtual void SendToAllServerWithOutHead(const uint16_t nMsgID, const std::string& strData);

	virtual void SendToAllServer(const uint16_t nMsgID, const std::string& strData);
	virtual void SendToAllServer(const uint16_t nMsgID, const std::string& strData, const NFGUID id);

	virtual void SendToAllServerWithOutHead(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const std::string& strData);

	virtual void SendToAllServer(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const std::string& strData);
	virtual void SendToAllServer(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const std::string& strData, const NFGUID id);

	virtual void SendToServerByPB(const int nServerID, const uint16_t nMsgID, const google::protobuf::Message& xData);
	virtual void SendToServerByPB(const int nServerID, const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id);

    virtual void SendToAllServerByPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id);

    virtual void SendToAllServerByPB(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id);

    ////////////////////////////////////////////////////////////////////////////////

	virtual void SendBySuitWithOutHead(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const std::string& strData);

	virtual void SendBySuit(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const std::string& strData);
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const std::string& strData, const NFGUID id);

	virtual void SendBySuitWithOutHead(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const std::string& strData);

	virtual void SendBySuit(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const std::string& strData);
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const std::string& strData, const NFGUID id);

	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const google::protobuf::Message& xData);
	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id);

	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const google::protobuf::Message& xData);
	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id);

    ////////////////////////////////////////////////////////////////////////////////

    virtual NFMapEx<int, ConnectData>& GetServerList();

    virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NF_SERVER_TYPES eType);

    virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const int nServerID);

    virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NFINet* pNet);

protected:

    void InitCallBacks(NF_SHARE_PTR<ConnectData> pServerData);

    void ProcessExecute();

private:
    void LogServerInfo();

    void KeepState(NF_SHARE_PTR<ConnectData> pServerData);

    void OnSocketEvent(const NFSOCK fd, const NF_NET_EVENT eEvent, NFINet* pNet);

    int OnConnected(const NFSOCK fd, NFINet* pNet);

    int OnDisConnected(const NFSOCK fd, NFINet* pNet);

    void ProcessAddNetConnect();

private:
	int64_t mnLastActionTime;
	int64_t mnBufferSize;
    //server_id, server_data
    NFConsistentHashMapEx<int, ConnectData> mxServerMap;
    //server_type, server_id, server_data
    NFMapEx<int, NFConsistentHashMapEx<int, ConnectData>> mxServerTypeMap;

    std::list<ConnectData> mxTempNetList;

    struct CallBack
    {
        //call back
		//std::map<int, NET_RECEIVE_FUNCTOR_PTR> mxReceiveCallBack;
		std::map<int, std::list<NET_RECEIVE_FUNCTOR_PTR>> mxReceiveCallBack;
        std::list<NET_EVENT_FUNCTOR_PTR> mxEventCallBack;
        std::list<NET_RECEIVE_FUNCTOR_PTR> mxCallBackList;
    };

    NFMapEx<int, CallBack> mxCallBack;

	NFILogModule* m_pLogModule;
};

#endif
