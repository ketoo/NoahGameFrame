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

#ifndef NF_GAMESERVER_SERVER_MODULE_H
#define NF_GAMESERVER_SERVER_MODULE_H

#include <memory>
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFISceneProcessModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFISceneModule.h"
#include "NFComm/NFPluginModule/NFIGameServerToWorldModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIGameServerNet_ServerModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFIThreadPoolModule.h"
#include "NFComm/NFPluginModule/NFISyncPosModule.h"
////////////////////////////////////////////////////////////////////////////



class NFGameServerNet_ServerModule
    : public NFIGameServerNet_ServerModule
{
public:
    NFGameServerNet_ServerModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

    virtual void SendMsgPBToGate(const uint16_t msgID, google::protobuf::Message& xMsg, const NFGUID& self);
	virtual void SendGroupMsgPBToGate(const uint16_t msgID, google::protobuf::Message& xMsg, const int sceneID, const int groupID);
	virtual void SendGroupMsgPBToGate(const uint16_t msgID, google::protobuf::Message& xMsg, const int sceneID, const int groupID, const NFGUID exceptID);

	virtual void SendMsgToGate(const uint16_t msgID, const std::string& msg, const NFGUID& self);
	virtual void SendGroupMsgPBToGate(const uint16_t msgID, const std::string& msg, const int sceneID, const int groupID);
	virtual void SendGroupMsgPBToGate(const uint16_t msgID, const std::string& msg, const int sceneID, const int groupID, const NFGUID exceptID);

    virtual bool AddPlayerGateInfo(const NFGUID& roleID, const NFGUID& clientID, const int gateID);
    virtual bool RemovePlayerGateInfo(const NFGUID& roleID);
    virtual NF_SHARE_PTR<GateBaseInfo> GetPlayerGateInfo(const NFGUID& roleID);

    virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfo(const int gateID);
    virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfoBySockIndex(const NFSOCK sockIndex);


protected:
    void OnSocketPSEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
    void OnClientDisconnect(const NFSOCK sockIndex);
    void OnClientConnected(const NFSOCK sockIndex);

protected:
    void OnProxyServerRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
    void OnProxyServerUnRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);
    void OnRefreshProxyServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

protected:
    void OnClientLeaveGameProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len);
	void OnClientSwapSceneProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len);
	void OnClientReqMoveProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len);
	void OnClientEnterGameFinishProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len);

	void OnLagTestProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

    ///////////WORLD_START///////////////////////////////////////////////////////////////
    void OnTransWorld(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len);

private:
    
    NFMapEx<NFGUID, GateBaseInfo> mRoleBaseData;

    //gateid,data
    NFMapEx<int, GateServerInfo> mProxyMap;

    //////////////////////////////////////////////////////////////////////////
	NFISyncPosModule* m_pSyncPosModule;
	NFIKernelModule* m_pKernelModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
    NFISceneProcessModule* m_pSceneProcessModule;
    NFIElementModule* m_pElementModule;
	NFINetModule* m_pNetModule;
	NFIEventModule* m_pEventModule;
	NFISceneModule* m_pSceneModule;
	NFINetClientModule* m_pNetClientModule;
	NFIScheduleModule* m_pScheduleModule;
    NFIThreadPoolModule* m_pThreadPoolModule;
};
#endif
