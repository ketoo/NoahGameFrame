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
#ifndef NF_WORLDNET_SERVER_MODULE_H
#define NF_WORLDNET_SERVER_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIWorldLogicModule.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIWorldNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFILoginNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIWorldToMasterModule.h"
#include "NFComm/NFPluginModule/NFIThreadPoolModule.h"

class NFWorldNet_ServerModule
    : public NFIWorldNet_ServerModule
{
public:
    NFWorldNet_ServerModule(NFIPluginManager* p)
    {
        m_bIsExecute = true;
        pPluginManager = p;
        mnLastCheckTime = pPluginManager->GetNowTime();
    }

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    virtual bool AfterInit();
	virtual void OnServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

	virtual bool IsPrimaryWorldServer();
	virtual int GetWorldAreaID();

	virtual bool SendMsgToGame(const int gameID, const int msgID, const std::string& xData);
	virtual bool SendMsgToGame(const int gameID, const int msgID, const google::protobuf::Message& xData);

    virtual bool SendMsgToGamePlayer(const NFGUID nPlayer, const int msgID, const std::string& xData);
    virtual bool SendMsgToGamePlayer(const NFGUID nPlayer, const int msgID, const google::protobuf::Message& xData);
    virtual bool SendMsgToGamePlayer(const NFDataList& argObjectVar, const int msgID, google::protobuf::Message& xData);

    virtual NF_SHARE_PTR<ServerData> GetSuitProxyToEnter();
	virtual NF_SHARE_PTR<ServerData> GetSuitGameToEnter(const int arg);

    virtual int GetPlayerGameID(const NFGUID self);
    virtual const std::vector<NFGUID>& GetOnlinePlayers();

    virtual NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> GetPlayerData(const NFGUID& id);

protected:
	virtual bool AddOnLineReceiveCallBack(std::shared_ptr<std::function<void(const NFGUID)>> cb);
	virtual bool AddOffLineReceiveCallBack(std::shared_ptr<std::function<void(const NFGUID)>> cb);
protected:

    void OnSocketEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);

    void OnClientDisconnect(const NFSOCK nAddress);
    void OnClientConnected(const NFSOCK nAddress);


    void OnOnlineProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
    void OnOfflineProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

    void OnTransmitServerReport(const NFSOCK nFd, const int msgId, const char *buffer, const uint32_t len);
    void ServerReport(int reportServerId, NFMsg::EServerState serverStatus);

protected:

    void OnGameServerRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
    void OnGameServerUnRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
    void OnRefreshGameServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

    void OnProxyServerRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
    void OnProxyServerUnRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
    void OnRefreshProxyServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

	void OnDBServerRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnDBServerUnRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
	void OnRefreshDBServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

	void SynGameToProxy();
    void SynGameToProxy(const NFSOCK nFD);

	void SynWorldToProxy();
	void SynWorldToProxy(const NFSOCK nFD);

	void SynWorldToGame();
	void SynWorldToGame(const NFSOCK nFD);

	void SynWorldToDB();
	void SynWorldToDB(const NFSOCK nFD);

	void SynDBToGame();
	void SynDBToGame(const NFSOCK nFD);

    void LogGameServer();

private:
	int mAreaID = 0;

	std::vector<std::shared_ptr<std::function<void(const NFGUID)>>> mPlayerOnLineCallBackFunc;
	std::vector<std::shared_ptr<std::function<void(const NFGUID)>>> mPlayerOffLineCallBackFunc;

	NFMapEx<NFGUID, PlayerData> mPlayersData;
    NFINT64 mnLastCheckTime;

    //serverid,data
	NFConsistentHashMapEx<int, ServerData> mWorldMap;
	NFConsistentHashMapEx<int, ServerData> mGameMap;
	NFConsistentHashMapEx<int, ServerData> mProxyMap;
	NFConsistentHashMapEx<int, ServerData> mDBMap;

    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
    NFIKernelModule* m_pKernelModule;
    NFILogModule* m_pLogModule;
	NFINetModule* m_pNetModule;
	NFINetClientModule* m_pNetClientModule;
    NFIThreadPoolModule* m_pThreadPoolModule;
};

#endif
