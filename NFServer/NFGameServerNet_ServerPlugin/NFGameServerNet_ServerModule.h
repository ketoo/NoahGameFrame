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

    virtual void SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self);
	virtual void SendGroupMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const int nSceneID, const int nGroupID);
	virtual void SendGroupMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const int nSceneID, const int nGroupID, const NFGUID exceptID);

	virtual void SendMsgToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self);
	virtual void SendGroupMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const int nSceneID, const int nGroupID);
	virtual void SendGroupMsgPBToGate(const uint16_t nMsgID, const std::string& strMsg, const int nSceneID, const int nGroupID, const NFGUID exceptID);

    virtual bool AddPlayerGateInfo(const NFGUID& nRoleID, const NFGUID& nClientID, const int nGateID);
    virtual bool RemovePlayerGateInfo(const NFGUID& nRoleID);
    virtual NF_SHARE_PTR<GateBaseInfo> GetPlayerGateInfo(const NFGUID& nRoleID);

    virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfo(const int nGateID);
    virtual NF_SHARE_PTR<GateServerInfo> GetGateServerInfoBySockIndex(const NFSOCK nSockIndex);


protected:
    void OnSocketPSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet);
    void OnClientDisconnect(const NFSOCK nSockIndex);
    void OnClientConnected(const NFSOCK nSockIndex);

protected:
    void OnProxyServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnProxyServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
    void OnRefreshProxyServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
    void OnClientLeaveGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnClientSwapSceneProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnClientReqMoveProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnClientReqMoveImmuneProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnClientReqPosSyncProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);
	void OnClientEnterGameFinishProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen);

	void OnLagTestProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

    ///////////WORLD_START///////////////////////////////////////////////////////////////
    void OnTransWorld(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:
	void OnClientPropertyIntProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyFloatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyStringProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyObjectProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyVector2Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientPropertyVector3Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

	void OnClientAddRowProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRemoveRowProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientSwapRowProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordIntProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordFloatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordStringProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordObjectProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordVector2Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);
	void OnClientRecordVector3Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen);

protected:

	//////////////////////////////////////////


private:
    
    NFMapEx<NFGUID, GateBaseInfo> mRoleBaseData;

    //gateid,data
    NFMapEx<int, GateServerInfo> mProxyMap;

    //////////////////////////////////////////////////////////////////////////
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
