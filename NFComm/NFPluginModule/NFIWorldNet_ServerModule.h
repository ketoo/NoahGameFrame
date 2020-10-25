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


#ifndef NFI_WORLDNET_SERVERMODULE_H
#define NFI_WORLDNET_SERVERMODULE_H

#include <iostream>
#include "NFIModule.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

class NFIWorldNet_ServerModule
    : public NFIModule
{
public:
	class IPlayerWorldCache
	{

	};

	class PlayerData
	{
	public:
		PlayerData(const NFGUID id)
		{
			self = id;
			gameID = 0;
			gateID = 0;
            bp = 0;

			onLine = false;
		}

		virtual ~PlayerData()
		{

		}

		void OnLine(const int gameSvrID, const int gateSvrID)
		{
			gameID = gameSvrID;
			gateID = gateSvrID;
			onLine = true;
		}

		void OffLine()
		{
			gameID = 0;
			gateID = 0;
			onLine = false;
		}

		bool onLine;
		int gameID = 0;
		int gateID = 0;
		NFGUID self;
		NFGUID team;
		NFGUID clan;
		int bp = 0;
		std::string name;
		std::string headIcon;

		int level = 0;
		int gold = 0;
		int diamond = 0;

        NFMapEx<int, IPlayerWorldCache> playerWorldCache;
	};

	virtual bool IsPrimaryWorldServer() = 0;
	virtual int GetWorldAreaID() = 0;

	virtual void OnServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len) = 0;

	virtual bool SendMsgToGame(const int gameID, const int msgID, const std::string& xData) = 0;
	virtual bool SendMsgToGame(const int gameID, const int msgID, const google::protobuf::Message& xData) = 0;

    virtual bool SendMsgToGamePlayer(const NFGUID nPlayer, const int msgID, const std::string& xData) = 0;
    virtual bool SendMsgToGamePlayer(const NFGUID nPlayer, const int msgID, const google::protobuf::Message& xData) = 0;
	virtual bool SendMsgToGamePlayer(const NFDataList& argObjectVar, const int msgID, google::protobuf::Message& xData) = 0;

	virtual NF_SHARE_PTR<ServerData> GetSuitProxyToEnter() = 0;
	virtual NF_SHARE_PTR<ServerData> GetSuitGameToEnter(const int arg) = 0;
    virtual const std::vector<NFGUID>& GetOnlinePlayers() = 0;

    virtual NF_SHARE_PTR<PlayerData> GetPlayerData(const NFGUID& id) = 0;


	template<typename BaseType>
	bool AddCallBackForPlayerOnline(BaseType* pBase, void (BaseType::*handleReceiver)(const NFGUID self))
	{
		auto functor = std::bind(handleReceiver, pBase, std::placeholders::_1);
		std::shared_ptr<std::function<void(const NFGUID)>> cb(NF_NEW std::function<void(const NFGUID)>(functor));
		return AddOnLineReceiveCallBack(cb);
	}
	template<typename BaseType>
	bool AddCallBackForPlayerOffline(BaseType* pBase, void (BaseType::*handleReceiver)(const NFGUID self))
	{
		auto functor = std::bind(handleReceiver, pBase, std::placeholders::_1);
		std::shared_ptr<std::function<void(const NFGUID)>> cb(NF_NEW std::function<void(const NFGUID)>(functor));
		return AddOffLineReceiveCallBack(cb);
	}
protected:
	virtual bool AddOnLineReceiveCallBack(std::shared_ptr<std::function<void(const NFGUID)>> cb) = 0;
	virtual bool AddOffLineReceiveCallBack(std::shared_ptr<std::function<void(const NFGUID)>> cb) = 0;
};

#endif