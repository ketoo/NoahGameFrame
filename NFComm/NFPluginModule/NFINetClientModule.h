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


#ifndef NFI_CLUSTER_CLIENT_MODULE_H
#define NFI_CLUSTER_CLIENT_MODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFINetModule.h"
#include "NFComm/NFCore/NFConsistentHash.hpp"

enum ConnectDataState
{
	DISCONNECT,
    CONNECTING,
	NORMAL,
    RECONNECT,
};

struct ConnectData
{
	ConnectData()
	{
		nGameID = 0;
		nPort = 0;
		strName = "";
		strIP = "";
		eServerType = NF_ST_NONE;
		eState = ConnectDataState::DISCONNECT;
		mnLastActionTime = 0;
		nWorkLoad = 0;
	}

	int nGameID;
	NF_SERVER_TYPES eServerType;
	std::string strIP;
	int nPort;
	int nWorkLoad;
	std::string strName;
	ConnectDataState eState;
	NFINT64 mnLastActionTime;

	NF_SHARE_PTR<NFINetModule> mxNetModule;
};

class NFINetClientModule : public NFIModule
{
public:
    enum EConstDefine
    {
        EConstDefine_DefaultWeith = 500,
    };

	template<typename BaseType>
	bool AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID, BaseType* pBase, void (BaseType::*handleRecieve)(const NFSOCK, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

		AddReceiveCallBack(eType, nMsgID, functorPtr);

		return true;
	}

	template<typename BaseType>
	int AddReceiveCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handleRecieve)(const NFSOCK, const int, const char*, const uint32_t))
	{
		NET_RECEIVE_FUNCTOR functor = std::bind(handleRecieve, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		NET_RECEIVE_FUNCTOR_PTR functorPtr(new NET_RECEIVE_FUNCTOR(functor));

		AddReceiveCallBack(eType, functorPtr);

		return true;
	}

	template<typename BaseType>
	bool AddEventCallBack(const NF_SERVER_TYPES eType, BaseType* pBase, void (BaseType::*handler)(const NFSOCK, const NF_NET_EVENT, NFINet*))
	{
		NET_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		NET_EVENT_FUNCTOR_PTR functorPtr(new NET_EVENT_FUNCTOR(functor));

		AddEventCallBack(eType, functorPtr);

		return true;
	}

	virtual int AddReceiveCallBack(const NF_SERVER_TYPES eType, NET_RECEIVE_FUNCTOR_PTR functorPtr) = 0;
	virtual int AddReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID, NET_RECEIVE_FUNCTOR_PTR functorPtr) = 0;
	virtual int AddEventCallBack(const NF_SERVER_TYPES eType, NET_EVENT_FUNCTOR_PTR functorPtr) = 0;

	virtual void RemoveReceiveCallBack(const NF_SERVER_TYPES eType, const uint16_t nMsgID) = 0;
	////////////////////////////////////////////////////////////////////////////////

	virtual void AddServer(const ConnectData& xInfo) = 0;
	virtual int ExpandBufferSize(const unsigned int size = 1024 * 1024 * 20) = 0;

	////////////////////////////////////////////////////////////////////////////////
	virtual void SendByServerIDWithOutHead(const int nServerID, const uint16_t nMsgID, const std::string& strData) = 0;

	virtual void SendByServerID(const int nServerID, const uint16_t nMsgID, const std::string& strData) = 0;
	virtual void SendByServerID(const int nServerID, const uint16_t nMsgID, const std::string& strData, const NFGUID id) = 0;

	virtual void SendToAllServerWithOutHead(const uint16_t nMsgID, const std::string& strData) = 0;

	virtual void SendToAllServer(const uint16_t nMsgID, const std::string& strData) = 0;
	virtual void SendToAllServer(const uint16_t nMsgID, const std::string& strData, const NFGUID id) = 0;

	virtual void SendToAllServerWithOutHead(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const std::string& strData) = 0;

	virtual void SendToAllServer(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const std::string& strData) = 0;
	virtual void SendToAllServer(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const std::string& strData, const NFGUID id) = 0;

	virtual void SendToServerByPB(const int nServerID, const uint16_t nMsgID, const google::protobuf::Message& xData) = 0;
	virtual void SendToServerByPB(const int nServerID, const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id) = 0;

	virtual void SendToAllServerByPB(const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id) = 0;
	virtual void SendToAllServerByPB(const NF_SERVER_TYPES eType, const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id) = 0;

	////////////////////////////////////////////////////////////////////////////////

	virtual void SendBySuitWithOutHead(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const std::string& strData) = 0;

	virtual void SendBySuit(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const std::string& strData) = 0;
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const std::string& strData, const NFGUID id) = 0;

	virtual void SendBySuitWithOutHead(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const std::string& strData) = 0;

	virtual void SendBySuit(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const std::string& strData) = 0;
	virtual void SendBySuit(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const std::string& strData, const NFGUID id) = 0;

	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const google::protobuf::Message& xData) = 0;
	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const std::string& strHashKey, const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id) = 0;

	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const google::protobuf::Message& xData) = 0;
	virtual void SendSuitByPB(const NF_SERVER_TYPES eType, const int nHashKey32, const uint16_t nMsgID, const google::protobuf::Message& xData, const NFGUID id) = 0;

	////////////////////////////////////////////////////////////////////////////////

	virtual NFMapEx<int, ConnectData>& GetServerList() = 0;
	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NF_SERVER_TYPES eType) = 0;
	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const int nServerID) = 0;
	virtual NF_SHARE_PTR<ConnectData> GetServerNetInfo(const NFINet* pNet) = 0;
};
#endif
