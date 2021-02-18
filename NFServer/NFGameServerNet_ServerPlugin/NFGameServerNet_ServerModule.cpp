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


#include "NFGameServerNet_ServerModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFKernelPlugin/NFSceneModule.h"

bool NFGameServerNet_ServerModule::Init()
{
	this->pPluginManager->SetAppType(NF_SERVER_TYPES::NF_ST_GAME);

	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pThreadPoolModule = pPluginManager->FindModule<NFIThreadPoolModule>();
	m_pSyncPosModule = pPluginManager->FindModule<NFISyncPosModule>();

	return true;
}

bool NFGameServerNet_ServerModule::AfterInit()
{

	m_pNetModule->AddReceiveCallBack(NFMsg::PTWG_PROXY_REFRESH, this, &NFGameServerNet_ServerModule::OnRefreshProxyServerInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::PTWG_PROXY_REGISTERED, this, &NFGameServerNet_ServerModule::OnProxyServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::PTWG_PROXY_UNREGISTERED, this, &NFGameServerNet_ServerModule::OnProxyServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_LEAVE_GAME, this, &NFGameServerNet_ServerModule::OnClientLeaveGameProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_SWAP_SCENE, this, &NFGameServerNet_ServerModule::OnClientSwapSceneProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_ENTER_GAME_FINISH, this, &NFGameServerNet_ServerModule::OnClientEnterGameFinishProcess);
	

	//EGMI_ACK_RECORD_CLEAR = 228,
	//EGMI_ACK_RECORD_SORT = 229,

	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_MOVE, this, &NFGameServerNet_ServerModule::OnClientReqMoveProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_LAG_TEST, this, &NFGameServerNet_ServerModule::OnLagTestProcess);

	m_pNetModule->AddEventCallBack(this, &NFGameServerNet_ServerModule::OnSocketPSEvent);


	/////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int serverType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int serverID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (serverType == NF_SERVER_TYPES::NF_ST_GAME && pPluginManager->GetAppID() == serverID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				const int maxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				//const std::string& name = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				//const std::string& ip = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());
				int nRet = m_pNetModule->Initialization(maxConnect, nPort, nCpus);
				if (nRet < 0)
				{
					std::ostringstream strLog;
					strLog << "Cannot init server net, Port = " << nPort;
					m_pLogModule->LogError(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
					NFASSERT(nRet, "Cannot init server net", __FILE__, __FUNCTION__);
					exit(0);
				}
			}
		}
	}

	return true;
}

bool NFGameServerNet_ServerModule::Shut()
{

	return true;
}

bool NFGameServerNet_ServerModule::Execute()
{
	return true;
}

void NFGameServerNet_ServerModule::OnSocketPSEvent(const NFSOCK sockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	if (eEvent & NF_NET_EVENT_EOF)
	{
		m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_EOF Connection closed", __FUNCTION__, __LINE__);
		OnClientDisconnect(sockIndex);
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
		m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_ERROR Got an error on the connection", __FUNCTION__, __LINE__);
		OnClientDisconnect(sockIndex);
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
		m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_TIMEOUT read timeout", __FUNCTION__, __LINE__);
		OnClientDisconnect(sockIndex);
	}
	else  if (eEvent & NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogInfo(NFGUID(0, sockIndex), "NF_NET_EVENT_CONNECTED connected success", __FUNCTION__, __LINE__);
		OnClientConnected(sockIndex);
	}
}

void NFGameServerNet_ServerModule::OnClientDisconnect(const NFSOCK nAddress)
{
	int serverID = 0;
	NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.First();
	while (pServerData)
	{
		if (nAddress == pServerData->xServerData.nFD)
		{
			serverID = pServerData->xServerData.pData->server_id();
			break;
		}

		pServerData = mProxyMap.Next();
	}

	mProxyMap.RemoveElement(serverID);
}

void NFGameServerNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{

}

void NFGameServerNet_ServerModule::OnClientLeaveGameProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	NFGUID nPlayerID;
	NFMsg::ReqLeaveGameServer xMsg;
	if (!m_pNetModule->ReceivePB( msgID, msg, len, xMsg, nPlayerID))
	{
		return;
	}

	if (nPlayerID.IsNull())
	{
		return;
	}

	m_pKernelModule->SetPropertyInt(nPlayerID, NFrame::IObject::Connection(), 0);

	m_pKernelModule->DestroyObject(nPlayerID);

	RemovePlayerGateInfo(nPlayerID);
}

void NFGameServerNet_ServerModule::OnClientEnterGameFinishProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS( msgID, msg, len, NFMsg::ReqAckEnterGameSuccess);
	m_pKernelModule->DoEvent(nPlayerID, NFrame::Player::ThisName(), CLASS_OBJECT_EVENT::COE_CREATE_CLIENT_FINISH, NFDataList::Empty());
	
	m_pNetModule->SendMsgPB(NFMsg::ACK_ENTER_GAME_FINISH, xMsg, sockIndex, nPlayerID);
}

void NFGameServerNet_ServerModule::OnLagTestProcess(const NFSOCK sockIndex, const int msgID, const char * msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS(msgID, msg, len, NFMsg::ReqAckLagTest);
	this->SendMsgPBToGate(NFMsg::ACK_GAME_LAG_TEST, xMsg, nPlayerID);
}

void NFGameServerNet_ServerModule::OnClientSwapSceneProcess(const NFSOCK sockIndex, const int msgID, const char *msg, const uint32_t len)
{
	CLIENT_MSG_PROCESS(msgID, msg, len, NFMsg::ReqAckSwapScene)

	const NFMsg::ESceneType sceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt(std::to_string(xMsg.scene_id()), NFrame::Scene::Type());
	const int nowSceneID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::SceneID());
	const int nowGroupID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::GroupID());

	if (sceneType == NFMsg::ESceneType::NORMAL_SCENE)
	{
		const NFVector3& pos = m_pSceneModule->GetRelivePosition(xMsg.scene_id());
		m_pSceneProcessModule->RequestEnterScene(pObject->Self(), xMsg.scene_id(), 1, 0, pos, NFDataList::Empty());
	}
	else if (sceneType == NFMsg::ESceneType::SINGLE_CLONE_SCENE)
	{
		const NFVector3& pos = m_pSceneModule->GetRelivePosition(xMsg.scene_id());
		m_pSceneProcessModule->RequestEnterScene(pObject->Self(), xMsg.scene_id(), m_pKernelModule->RequestGroupScene(xMsg.scene_id()), 0, pos, NFDataList::Empty());
	}
}

void NFGameServerNet_ServerModule::OnClientReqMoveProcess(const NFSOCK sockIndex, const int msgID, const char *msg,  const uint32_t len)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(msgID, msg, len, NFMsg::ReqAckPlayerPosSync)

	if (xMsg.sync_unit_size() > 0)
	{
		NFMsg::PosSyncUnit* syncUnit = xMsg.mutable_sync_unit(0);
		if (syncUnit)
		{
			const NFGUID& xMover = NFINetModule::PBToNF(syncUnit->mover());
			if (xMover != nPlayerID)
			{
				const NFGUID masterID = m_pKernelModule->GetPropertyObject(xMover, NFrame::NPC::MasterID());
				if (masterID != nPlayerID)
				{
					m_pLogModule->LogError(xMover, "Message come from player " + nPlayerID.ToString());
					return;
				}
				return;
			}
			else
			{
				const int sceneID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::Player::SceneID());
				const int groupID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::Player::GroupID());

				PosSyncUnit posSyncUnit;

				posSyncUnit.mover = xMover;
				posSyncUnit.pos = NFINetModule::PBToNF(syncUnit->pos());
				posSyncUnit.orientation = NFINetModule::PBToNF(syncUnit->orientation());
				posSyncUnit.status = syncUnit->status();
				posSyncUnit.type = syncUnit->type();

				m_pSyncPosModule->RequireMove(NFGUID(sceneID, groupID), posSyncUnit);
				m_pKernelModule->SetPropertyVector3(nPlayerID, NFrame::IObject::Position(), posSyncUnit.pos);
				//this->SendGroupMsgPBToGate(NFMsg::ACK_MOVE, xMsg, sceneID, groupID);
			}
		}
	}
}

void NFGameServerNet_ServerModule::OnProxyServerRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!NFINetModule::ReceivePB( msgID, msg, len, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<GateServerInfo>(NF_NEW GateServerInfo());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->xServerData.nFD = sockIndex;
		*(pServerData->xServerData.pData) = xData;

		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");
	}

	return;
}

void NFGameServerNet_ServerModule::OnProxyServerUnRegisteredProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB( msgID, msg, len, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mProxyMap.RemoveElement(xData.server_id());


		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "Proxy UnRegistered");
	}

	return;
}

void NFGameServerNet_ServerModule::OnRefreshProxyServerInfoProcess(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB( msgID, msg, len, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<GateServerInfo>(NF_NEW GateServerInfo());
			mProxyMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->xServerData.nFD = sockIndex;
		*(pServerData->xServerData.pData) = xData;

		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");
	}

	return;
}

void NFGameServerNet_ServerModule::SendMsgPBToGate(const uint16_t msgID, google::protobuf::Message& xMsg, const NFGUID& self)
{
	NF_SHARE_PTR<GateBaseInfo> pData = mRoleBaseData.GetElement(self);
	if (pData)
	{
		NF_SHARE_PTR<GateServerInfo> pProxyData = mProxyMap.GetElement(pData->gateID);
		if (pProxyData)
		{
			m_pNetModule->SendMsgPB(msgID, xMsg, pProxyData->xServerData.nFD, pData->xClientID);
		}
	}
}

void NFGameServerNet_ServerModule::SendMsgToGate(const uint16_t msgID, const std::string& msg, const NFGUID& self)
{
	NF_SHARE_PTR<GateBaseInfo> pData = mRoleBaseData.GetElement(self);
	if (pData)
	{
		NF_SHARE_PTR<GateServerInfo> pProxyData = mProxyMap.GetElement(pData->gateID);
		if (pProxyData)
		{
			m_pNetModule->SendMsg(msgID, msg, pProxyData->xServerData.nFD, pData->xClientID);
		}
	}
}

void NFGameServerNet_ServerModule::SendGroupMsgPBToGate(const uint16_t msgID, google::protobuf::Message & xMsg, const int sceneID, const int groupID)
{
	//care: batch
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			this->SendMsgPBToGate(msgID, xMsg, xObject);
		}
	}
}

void NFGameServerNet_ServerModule::SendGroupMsgPBToGate(const uint16_t msgID, google::protobuf::Message & xMsg, const int sceneID, const int groupID, const NFGUID exceptID)
{
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			if (xObject != exceptID)
			{
				this->SendMsgPBToGate(msgID, xMsg, xObject);
			}
		}
	}
}

void NFGameServerNet_ServerModule::SendGroupMsgPBToGate(const uint16_t msgID, const std::string & msg, const int sceneID, const int groupID)
{
	//care: batch
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			this->SendMsgToGate(msgID, msg, xObject);
		}
	}
}

void NFGameServerNet_ServerModule::SendGroupMsgPBToGate(const uint16_t msgID, const std::string & msg, const int sceneID, const int groupID, const NFGUID exceptID)
{
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(sceneID, groupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			if (xObject != exceptID)
			{
				this->SendMsgToGate(msgID, msg, xObject);
			}
		}
	}
}

bool NFGameServerNet_ServerModule::AddPlayerGateInfo(const NFGUID& roleID, const NFGUID& clientID, const int gateID)
{
    if (gateID <= 0)
    {
        
        return false;
    }

    if (clientID.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<NFGameServerNet_ServerModule::GateBaseInfo> pBaseData = mRoleBaseData.GetElement(roleID);
    if (nullptr != pBaseData)
    {
        
        m_pLogModule->LogError(clientID, "player is exist, cannot enter game", __FUNCTION__, __LINE__);
        return false;
    }

    NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(gateID);
    if (nullptr == pServerData)
    {
        return false;
    }

    if (!pServerData->xRoleInfo.insert(std::make_pair(roleID, pServerData->xServerData.nFD)).second)
    {
        return false;
    }

    if (!mRoleBaseData.AddElement(roleID, NF_SHARE_PTR<GateBaseInfo>(NF_NEW GateBaseInfo(gateID, clientID))))
    {
        pServerData->xRoleInfo.erase(roleID) ;
        return false;
    }

    return true;
}

bool NFGameServerNet_ServerModule::RemovePlayerGateInfo(const NFGUID& roleID)
{
    NF_SHARE_PTR<GateBaseInfo> pBaseData = mRoleBaseData.GetElement(roleID);
    if (nullptr == pBaseData)
    {
        return false;
    }

    mRoleBaseData.RemoveElement(roleID);

    NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(pBaseData->gateID);
    if (nullptr == pServerData)
    {
        return false;
    }

    pServerData->xRoleInfo.erase(roleID);
    return true;
}

NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo> NFGameServerNet_ServerModule::GetPlayerGateInfo(const NFGUID& roleID)
{
    return mRoleBaseData.GetElement(roleID);
}

NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> NFGameServerNet_ServerModule::GetGateServerInfo(const int gateID)
{
    return mProxyMap.GetElement(gateID);
}

NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> NFGameServerNet_ServerModule::GetGateServerInfoBySockIndex(const NFSOCK sockIndex)
{
    int gateID = -1;
    NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.First();
    while (pServerData)
    {
        if (sockIndex == pServerData->xServerData.nFD)
        {
            gateID = pServerData->xServerData.pData->server_id();
            break;
        }

        pServerData = mProxyMap.Next();
    }

    if (gateID == -1)
    {
        return nullptr;
    }

    return pServerData;
}

void NFGameServerNet_ServerModule::OnTransWorld(const NFSOCK sockIndex, const int msgID, const char* msg, const uint32_t len)
{
	std::string msgData;
	NFGUID nPlayer;
	int64_t nHasKey = 0;
	if (NFINetModule::ReceivePB( msgID, msg, len, msgData, nPlayer))
	{
		nHasKey = nPlayer.nData64;
	}

	m_pNetClientModule->SendBySuitWithOutHead(NF_SERVER_TYPES::NF_ST_WORLD, nHasKey, msgID, std::string(msg, len));
}