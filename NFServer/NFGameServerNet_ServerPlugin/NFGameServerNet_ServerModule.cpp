/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

bool NFGameServerNet_ServerModule::Init()
{
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

	return true;
}

bool NFGameServerNet_ServerModule::AfterInit()
{

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REFRESH, this, &NFGameServerNet_ServerModule::OnRefreshProxyServerInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_REGISTERED, this, &NFGameServerNet_ServerModule::OnProxyServerRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_PTWG_PROXY_UNREGISTERED, this, &NFGameServerNet_ServerModule::OnProxyServerUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ENTER_GAME, this, &NFGameServerNet_ServerModule::OnClientEnterGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_GAME, this, &NFGameServerNet_ServerModule::OnClientLeaveGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ROLE_LIST, this, &NFGameServerNet_ServerModule::OnReqRoleListProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_ROLE, this, &NFGameServerNet_ServerModule::OnCreateRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_DELETE_ROLE, this, &NFGameServerNet_ServerModule::OnDeleteRoleGameProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SWAP_SCENE, this, &NFGameServerNet_ServerModule::OnClientSwapSceneProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ENTER_GAME_FINISH, this, &NFGameServerNet_ServerModule::OnClientEnterGameFinishProcess);
	
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_INT, this, &NFGameServerNet_ServerModule::OnClientPropertyIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_FLOAT, this, &NFGameServerNet_ServerModule::OnClientPropertyFloatProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_STRING, this, &NFGameServerNet_ServerModule::OnClientPropertyStringProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_OBJECT, this, &NFGameServerNet_ServerModule::OnClientPropertyObjectProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_VECTOR2, this, &NFGameServerNet_ServerModule::OnClientPropertyVector2Process);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_PROPERTY_VECTOR3, this, &NFGameServerNet_ServerModule::OnClientPropertyVector3Process);
	
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_ADD_ROW, this, &NFGameServerNet_ServerModule::OnClientAddRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_REMOVE_ROW, this, &NFGameServerNet_ServerModule::OnClientRemoveRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_SWAP_ROW, this, &NFGameServerNet_ServerModule::OnClientSwapRowProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_INT, this, &NFGameServerNet_ServerModule::OnClientRecordIntProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_FLOAT, this, &NFGameServerNet_ServerModule::OnClientRecordFloatProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_STRING, this, &NFGameServerNet_ServerModule::OnClientRecordStringProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_OBJECT, this, &NFGameServerNet_ServerModule::OnClientRecordObjectProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_VECTOR2, this, &NFGameServerNet_ServerModule::OnClientRecordVector2Process);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_RECORD_VECTOR3, this, &NFGameServerNet_ServerModule::OnClientRecordVector3Process);

	//EGMI_ACK_RECORD_CLEAR = 228,
	//EGMI_ACK_RECORD_SORT = 229,

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_CLAN, this, &NFGameServerNet_ServerModule::OnClanTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_CLAN, this, &NFGameServerNet_ServerModule::OnClanTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_CLAN, this, &NFGameServerNet_ServerModule::OnClanTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_OPR_CLAN, this, &NFGameServerNet_ServerModule::OnClanTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_CLAN, this, &NFGameServerNet_ServerModule::OnClanTransWorld);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_CREATE_CHATGROUP, this, &NFGameServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_JOIN_CHATGROUP, this, &NFGameServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_LEAVE_CHATGROUP, this, &NFGameServerNet_ServerModule::OnTransWorld);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_SUBSCRIPTION_CHATGROUP, this, &NFGameServerNet_ServerModule::OnTransWorld);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_MOVE, this, &NFGameServerNet_ServerModule::OnClientReqMoveProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_MOVE_IMMUNE, this, &NFGameServerNet_ServerModule::OnClientReqMoveImmuneProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_STATE_SYNC, this, &NFGameServerNet_ServerModule::OnClientReqStateSyncProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_POS_SYNC, this, &NFGameServerNet_ServerModule::OnClientReqPosSyncProcess);

	m_pNetModule->AddReceiveCallBack(NFMsg::EGEC_REQ_LAG_TEST, this, &NFGameServerNet_ServerModule::OnLagTestProcess);

	m_pNetModule->AddEventCallBack(this, &NFGameServerNet_ServerModule::OnSocketPSEvent);


	/////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (nServerType == NF_SERVER_TYPES::NF_ST_GAME && pPluginManager->GetAppID() == nServerID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				//const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				//const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				int nRet = m_pNetModule->Initialization(nMaxConnect, nPort, nCpus);
				if (nRet < 0)
				{
					std::ostringstream strLog;
					strLog << "Cannot init server net, Port = " << nPort;
					m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
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

void NFGameServerNet_ServerModule::OnSocketPSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	if (eEvent & NF_NET_EVENT_EOF)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else  if (eEvent & NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connected success", __FUNCTION__, __LINE__);
		OnClientConnected(nSockIndex);
	}
}

void NFGameServerNet_ServerModule::OnClientDisconnect(const NFSOCK nAddress)
{
	int nServerID = 0;
	NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.First();
	while (pServerData)
	{
		if (nAddress == pServerData->xServerData.nFD)
		{
			nServerID = pServerData->xServerData.pData->server_id();
			break;
		}

		pServerData = mProxyMap.Next();
	}

	mProxyMap.RemoveElement(nServerID);
}

void NFGameServerNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{

}

void NFGameServerNet_ServerModule::OnClientEnterGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
                                                             const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqEnterGameServer xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFGUID nRoleID = NFINetModule::PBToNF(xMsg.id());

	if (m_pKernelModule->GetObject(nRoleID))
	{
		//it should be rebind with proxy's netobject
		m_pKernelModule->DestroyObject(nRoleID);
	}

	//////////////////////////////////////////////////////////////////////////

	NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo>  pGateInfo = GetPlayerGateInfo(nRoleID);
	if (nullptr != pGateInfo)
	{
		RemovePlayerGateInfo(nRoleID);
	}

	NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> pGateServerinfo = GetGateServerInfoBySockIndex(nSockIndex);
	if (nullptr == pGateServerinfo)
	{
		return;
	}

	int nGateID = -1;
	if (pGateServerinfo->xServerData.pData)
	{
		nGateID = pGateServerinfo->xServerData.pData->server_id();
	}

	if (nGateID < 0)
	{
		return;
	}

	if (!AddPlayerGateInfo(nRoleID, nClientID, nGateID))
	{
		return;
	}

	int nSceneID = 1;
	NFDataList var;
	var.AddString(NFrame::Player::Name());
	var.AddString(xMsg.name());

	var.AddString(NFrame::Player::GateID());
	var.AddInt(nGateID);

	var.AddString(NFrame::Player::GameID());
	var.AddInt(pPluginManager->GetAppID());

	NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->CreateObject(nRoleID, nSceneID, 0, NFrame::Player::ThisName(), "", var);
	if (NULL == pObject)
	{
		//mRoleBaseData
		//mRoleFDData
		return;
	}

	const NFVector3& pos = pObject->GetPropertyVector3(NFrame::IObject::Position());
	if (!pos.IsZero())
	{
		m_pSceneModule->RequestEnterScene(pObject->Self(), nSceneID, 1, 0, pos, NFDataList());
	}
	else
	{
		const NFVector3& pos = m_pSceneModule->GetRelivePosition(nSceneID, 0);
		m_pSceneModule->RequestEnterScene(pObject->Self(), nSceneID, 1, 0, pos, NFDataList());
	}
}

void NFGameServerNet_ServerModule::OnClientLeaveGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
                                                             const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqLeaveGameServer xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	if (nPlayerID.IsNull())
	{
		return;
	}

	if (m_pKernelModule->GetObject(nPlayerID))
	{
		//if the player now in the match, we wouldn't destroy him
		//or if the player now fighting in the clan scene with other clan, we wouldn't destroy him too
		const int sceneID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::SceneID());
		const int groupID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::GroupID());
		const NFGUID matchID = m_pSceneModule->GetPropertyObject(sceneID, groupID, NFrame::Group::MatchID());
		if (matchID.IsNull())
		{
			m_pKernelModule->DestroyObject(nPlayerID);
		}
		else
		{
			//add schedule
			//single mode or multi-player mode
			//if (m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Group::MatchPlayers()) <= 1)
			{
				m_pKernelModule->DestroyObject(nPlayerID);
			}
			/*
			else
			{
				m_pKernelModule->SetPropertyObject(nPlayerID, NFrame::Player::GateID(), NFGUID());
				m_pScheduleModule->AddSchedule(nPlayerID, "DestroyPlayerOnTime", this, &NFGameServerNet_ServerModule::DestroyPlayerByTime, 100.0f, -1);
			}
			*/
		}
	}

	RemovePlayerGateInfo(nPlayerID);
}


void NFGameServerNet_ServerModule::OnReqRoleListProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,
                                                         const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqRoleList xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	xAckRoleLiteInfoList.set_account(xMsg.account());
	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
}

void NFGameServerNet_ServerModule::OnCreateRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nClientID;
	NFMsg::ReqCreateRole xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nClientID))
	{
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	xAckRoleLiteInfoList.set_account(xMsg.account());

	NFMsg::RoleLiteInfo* pData = xAckRoleLiteInfoList.add_char_data();
	pData->mutable_id()->CopyFrom(NFINetModule::NFToPB(m_pKernelModule->CreateGUID()));
	pData->set_career(xMsg.career());
	pData->set_sex(xMsg.sex());
	pData->set_race(xMsg.race());
	pData->set_noob_name(xMsg.noob_name());
	pData->set_game_id(xMsg.game_id());
	pData->set_role_level(1);
	pData->set_delete_time(0);
	pData->set_reg_time(0);
	pData->set_last_offline_time(0);
	pData->set_last_offline_ip(0);
	pData->set_view_record("");

	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nClientID);
}

void NFGameServerNet_ServerModule::OnDeleteRoleGameProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqDeleteRole xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NFMsg::AckRoleLiteInfoList xAckRoleLiteInfoList;
	xAckRoleLiteInfoList.set_account(xMsg.account());

	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ROLE_LIST, xAckRoleLiteInfoList, nSockIndex, nPlayerID);
}

void NFGameServerNet_ServerModule::OnClientEnterGameFinishProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckEnterGameSuccess);
	m_pKernelModule->DoEvent(nPlayerID, NFrame::Player::ThisName(), CLASS_OBJECT_EVENT::COE_CREATE_CLIENT_FINISH, NFDataList());
	
	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_ENTER_GAME_FINISH, xMsg, nSockIndex, nPlayerID);
}

void NFGameServerNet_ServerModule::OnLagTestProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckLagTest);
	this->SendMsgPBToGate(NFMsg::EGEC_ACK_GAME_LAG_TEST, xMsg, nPlayerID);
}

void NFGameServerNet_ServerModule::OnClientSwapSceneProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckSwapScene)

	const NFMsg::ESceneType sceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt(std::to_string(xMsg.scene_id()), NFrame::Scene::Type());
	const int homeSceneID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::HomeSceneID());
	const int nowSceneID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::SceneID());
	const int nowGroupID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::GroupID());
	const NFGUID matchID = m_pSceneModule->GetPropertyObject(nowSceneID, nowGroupID, NFrame::Group::MatchID());
	const NFMsg::ESceneType nowSceneType = (NFMsg::ESceneType)m_pElementModule->GetPropertyInt(std::to_string(nowSceneID), NFrame::Scene::Type());

	if (!matchID.IsNull()
		&& sceneType == NFMsg::ESceneType::SCENE_HOME
		&& xMsg.scene_id() == homeSceneID)
	{
		//fighting now, want to end the fight
		const NFVector3& pos = m_pSceneModule->GetRelivePosition(homeSceneID, 0);
		m_pSceneProcessModule->RequestEnterScene(pObject->Self(), homeSceneID, -1, 0, pos, NFDataList());

		return;
	}
		
	if (sceneType == NFMsg::ESceneType::SCENE_NORMAL)
	{
		const NFVector3& pos = m_pSceneModule->GetRelivePosition(xMsg.scene_id(), 0);
		m_pSceneProcessModule->RequestEnterScene(pObject->Self(), xMsg.scene_id(), 1, 0, pos, NFDataList());
	}
	
	/*
	if (nowSceneType == NFMsg::ESceneType::SCENE_HOME)
	{
		//fighting now, want to end the fight
		const NFVector3& pos = m_pSceneModule->GetRelivePosition(homeSceneID, 0);
		m_pSceneProcessModule->RequestEnterScene(pObject->Self(), homeSceneID, -1, pos, NFDataList());
	}
	else
	{
		if (sceneType == NFMsg::ESceneType::SCENE_HOME)
		{
			//
			const NFVector3& pos = m_pSceneModule->GetRelivePosition(xMsg.scene_id(), 0);
			m_pSceneProcessModule->RequestEnterScene(pObject->Self(), xMsg.scene_id(), -1, pos, NFDataList());
		}
	}
	*/
}

void NFGameServerNet_ServerModule::OnClientReqMoveProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg,  const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckPlayerMove)


	const NFGUID& xMover = NFINetModule::PBToNF(xMsg.mover());
	if (xMover == nPlayerID)
	{
		const int nSceneID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::Player::SceneID());
		const int nGroupID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::Player::GroupID());

		this->SendGroupMsgPBToGate(NFMsg::EGMI_ACK_MOVE, xMsg, nSceneID, nGroupID);
	}
	else
	{
		const NFGUID xAIOwnerID = m_pKernelModule->GetPropertyObject(xMover, NFrame::NPC::AIOwnerID());
		if (xAIOwnerID == nPlayerID)
		{

			NFMsg::Vector3 vPos = xMsg.target_pos(0);

			NFVector3 v;
			v.SetX(vPos.x());
			v.SetY(vPos.y());
			v.SetZ(vPos.z());

			//m_pKernelModule->SetPropertyVector3(xMover, NFrame::IObject::Position(), v);
			const int nSceneID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::Player::SceneID());
			const int nGroupID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::Player::GroupID());

			this->SendGroupMsgPBToGate(NFMsg::EGMI_ACK_MOVE, xMsg, nSceneID, nGroupID);
		}
	}

}

void NFGameServerNet_ServerModule::OnClientReqMoveImmuneProcess(const NFSOCK nSockIndex, const int nMsgID,
                                                                 const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckPlayerMove)

	const NFGUID& self = NFINetModule::PBToNF(xMsg.mover());
	const NFGUID& xOwnerID = m_pKernelModule->GetPropertyObject(self, NFrame::NPC::AIOwnerID());
	if (self == nPlayerID
		|| xOwnerID == nPlayerID)
	{
		const int nSceneID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::SceneID());
		const int nGroupID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GroupID());

		if (xMsg.target_pos_size() > 0)
		{
			NFMsg::Vector3 vPos = xMsg.target_pos(0);

			NFVector3 v;
			v.SetX(vPos.x());
			v.SetY(vPos.y());
			v.SetZ(vPos.z());

			m_pKernelModule->SetPropertyVector3(self, NFrame::IObject::Position(), v);
		}


		this->SendGroupMsgPBToGate(NFMsg::EGMI_ACK_MOVE_IMMUNE, xMsg, nSceneID, nGroupID);
	}
}

void NFGameServerNet_ServerModule::OnClientReqStateSyncProcess(const NFSOCK nSockIndex, const int nMsgID,
                                                                const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckPlayerMove)
	//only the player can send the message to the back-end
	//the monter use the require move message to sync the position between different view
	const NFGUID& xMover = NFINetModule::PBToNF(xMsg.mover());
	if (xMover != nPlayerID)
	{
		//const NFGUID xMasterID = m_pKernelModule->GetPropertyObject(xMover, NFrame::NPC::MasterID());
		//if (xMasterID != nPlayerID)
		//{
		//	m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, xMover, "Message come from player ", nPlayerID.ToString());
		//	return;
		//}
		return;
	}

	const int nSceneID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::IObject::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::IObject::GroupID());

	if (xMsg.target_pos_size() > 0)
	{
		NFMsg::Vector3 vPos = xMsg.target_pos(0);

		NFVector3 v;
		v.SetX(vPos.x());
		v.SetY(vPos.y());
		v.SetZ(vPos.z());

		m_pKernelModule->SetPropertyVector3(xMover, NFrame::IObject::Position(), v);
	}

	this->SendGroupMsgPBToGate(NFMsg::EGMI_ACK_STATE_SYNC, xMsg, nSceneID, nGroupID);
}

void NFGameServerNet_ServerModule::OnClientReqPosSyncProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ReqAckPlayerPosSync)
	//only the player can send the message to the back-end
	//the monter use the require move message to sync the position between different view
	const NFGUID& xMover = NFINetModule::PBToNF(xMsg.mover());
	if (xMover != nPlayerID)
	{
		const NFGUID xMasterID = m_pKernelModule->GetPropertyObject(xMover, NFrame::NPC::MasterID());
		if (xMasterID != nPlayerID)
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, xMover, "Message come from player ", nPlayerID.ToString());
			return;
		}
		return;
	}

	const int nSceneID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::IObject::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt32(xMover, NFrame::IObject::GroupID());

	NFVector3 v;
	v.SetX(xMsg.position().x());
	v.SetY(xMsg.position().y());
	v.SetZ(xMsg.position().z());

	m_pKernelModule->SetPropertyVector3(xMover, NFrame::IObject::Position(), v);

	this->SendGroupMsgPBToGate(NFMsg::EGMI_ACK_POS_SYNC, xMsg, nSceneID, nGroupID);
}

void NFGameServerNet_ServerModule::OnClientPropertyIntProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyInt)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyInt &xPropertyInt = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xPropertyInt.property_name());
		if (pProperty)
		{
			//judge upload then set value
			//GM
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client int set " + xPropertyInt.property_name(), xPropertyInt.data(), __FUNCTION__, __LINE__);
				pProperty->SetInt(xPropertyInt.data());
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client int set Upload error", xPropertyInt.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client int set Property error", xPropertyInt.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameServerNet_ServerModule::OnClientPropertyFloatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyFloat)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyFloat &xPropertyFloat = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xPropertyFloat.property_name());
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client float set " + xPropertyFloat.property_name(), xPropertyFloat.data(), __FUNCTION__, __LINE__);
				pProperty->SetFloat(xPropertyFloat.data());
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client float set Upload error", xPropertyFloat.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client float set Property error", xPropertyFloat.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameServerNet_ServerModule::OnClientPropertyStringProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyString)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyString &xPropertyString = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xPropertyString.property_name());
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client string set " + xPropertyString.property_name(), xPropertyString.data(), __FUNCTION__, __LINE__);
				pProperty->SetString(xPropertyString.data());
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client string set Upload error", xPropertyString.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client string set Property error", xPropertyString.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameServerNet_ServerModule::OnClientPropertyObjectProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyObject)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		const NFMsg::PropertyObject &xPropertyObject = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xPropertyObject.property_name());
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				NFGUID xID = NFINetModule::PBToNF(xPropertyObject.data());
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client object set " + xPropertyObject.property_name(), xID.ToString(), __FUNCTION__, __LINE__);
				pProperty->SetObject(xID);
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Upload error", xPropertyObject.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Property error", xPropertyObject.property_name(), __FUNCTION__, __LINE__);
		}
	}
}


void NFGameServerNet_ServerModule::OnClientPropertyVector2Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyVector2)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		NFMsg::PropertyVector2 xProperty = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xProperty.property_name());
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				NFVector2 vVec2 = NFINetModule::PBToNF(xProperty.data());
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client object set " + xProperty.property_name(), vVec2.ToString(), __FUNCTION__, __LINE__);
				pProperty->SetVector2(vVec2);
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Upload error", xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Property error", xProperty.property_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameServerNet_ServerModule::OnClientPropertyVector3Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectPropertyVector3)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	for (int i = 0; i < xMsg.property_list_size(); i++)
	{
		NFMsg::PropertyVector3 xProperty = xMsg.property_list().Get(i);
		NF_SHARE_PTR<NFIProperty> pProperty = pObject->GetPropertyManager()->GetElement(xProperty.property_name());
		if (pProperty)
		{
			//judge upload then set value
			if (pProperty->GetUpload() || nGMLevel > 0)
			{
				NFVector3 vVec3 = NFINetModule::PBToNF(xProperty.data());
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client object set " + xProperty.property_name(), vVec3.ToString(), __FUNCTION__, __LINE__);
				pProperty->SetVector3(vVec3);
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Upload error", xProperty.property_name(), __FUNCTION__, __LINE__);
			}
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client object set Property error", xProperty.property_name(), __FUNCTION__, __LINE__);
		}
	}
}


void NFGameServerNet_ServerModule::OnClientAddRowProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordAddRow)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client add row record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (pRecord->GetUpload() || nGMLevel > 0)
	{
		for (int i = 0; i < xMsg.row_data_size(); i++)
		{
			const NFMsg::RecordAddRowStruct &xAddRowStruct = xMsg.row_data().Get(i);
			int row = xAddRowStruct.row();

			std::map<int, NFData> colDataMap;
			for (int j = 0; j < xAddRowStruct.record_int_list_size(); j++)
			{
				const NFMsg::RecordInt &xRecordInt = xAddRowStruct.record_int_list().Get(j);
				NFData data;
				data.SetInt(xRecordInt.data());
				colDataMap.insert(std::map<int, NFData>::value_type(xRecordInt.col(), data));
			}
			for (int j = 0; j < xAddRowStruct.record_float_list_size(); j++)
			{
				const NFMsg::RecordFloat &xRecordFloat = xAddRowStruct.record_float_list().Get(j);
				NFData data;
				data.SetFloat(xRecordFloat.data());
				colDataMap.insert(std::map<int, NFData>::value_type(xRecordFloat.col(), data));
			}
			for (int j = 0; j < xAddRowStruct.record_string_list_size(); j++)
			{
				const NFMsg::RecordString &xRecordString = xAddRowStruct.record_string_list().Get(j);
				NFData data;
				data.SetString(xRecordString.data());
				colDataMap.insert(std::map<int, NFData>::value_type(xRecordString.col(), data));
			}
			for (int j = 0; j < xAddRowStruct.record_object_list_size(); j++)
			{
				const NFMsg::RecordObject &xRecordObject = xAddRowStruct.record_object_list().Get(j);
				NFData data;
				data.SetObject(NFINetModule::PBToNF(xRecordObject.data()));
				colDataMap.insert(std::map<int, NFData>::value_type(xRecordObject.col(), data));
			}

			NFDataList xDataList;
			for (int j = 0; j < colDataMap.size(); j++)
			{
				if (colDataMap.find(j) != colDataMap.end())
				{
					xDataList.Append(colDataMap[j]);
				}
				else
				{
					m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID,
											"Upload From Client add row record error", xMsg.record_name(), __FUNCTION__,
											__LINE__);
					return;
				}
			}

			if (pRecord->AddRow(row, xDataList) >= 0)
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client add row record",
										xMsg.record_name(), __FUNCTION__, __LINE__);
			}
			else
			{
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID,
										"Upload From Client add row record error", xMsg.record_name(), __FUNCTION__,
										__LINE__);
			}
		}
	}
}

void NFGameServerNet_ServerModule::OnClientRemoveRowProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordRemove)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client remove row record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
    {
		for (int i = 0; i < xMsg.remove_row_size(); i++)
        {
			if (pRecord->Remove(xMsg.remove_row().Get(i)))
            {
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID,
										"Upload From Client remove row record", xMsg.record_name(), __FUNCTION__,
										__LINE__);
			}
			else
            {
				m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID,
										"Upload From Client remove row record error", xMsg.record_name(), __FUNCTION__,
										__LINE__);
			}
		}
	}
}

void NFGameServerNet_ServerModule::OnClientSwapRowProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordSwap)


	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());


	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.origin_record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client swap row record error", xMsg.origin_record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (!pRecord->GetUpload() || nGMLevel > 0)
	{

		if (pRecord->SwapRowInfo(xMsg.row_origin(), xMsg.row_target()))
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL,
									nPlayerID,
									"Upload From Client swap row record",
									xMsg.origin_record_name(),
									__FUNCTION__,
									__LINE__);
		}
		else
		{
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL,
									nPlayerID,
									"Upload From Client swap row record error",
									xMsg.origin_record_name(),
									__FUNCTION__,
									__LINE__);
		}
	}
}

void NFGameServerNet_ServerModule::OnClientRecordIntProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordInt)

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client int set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());


	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordInt &xRecordInt = xMsg.property_list().Get(i);
			pRecord->SetInt(xRecordInt.row(), xRecordInt.col(), xRecordInt.data());
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL,
									nPlayerID,
									"Upload From Client int set record",
									xMsg.record_name(),
									__FUNCTION__,
									__LINE__);
		}
	}

}


void NFGameServerNet_ServerModule::OnClientRecordFloatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordFloat)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client float set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordFloat &xRecordFloat = xMsg.property_list().Get(i);
			pRecord->SetFloat(xRecordFloat.row(), xRecordFloat.col(), xRecordFloat.data());
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL,
									nPlayerID,
									"Upload From Client float set record",
									xMsg.record_name(),
									__FUNCTION__,
									__LINE__);
		}
	}
	
}

void NFGameServerNet_ServerModule::OnClientRecordStringProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordString)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client String set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordString &xRecordString = xMsg.property_list().Get(i);
			pRecord->SetString(xRecordString.row(), xRecordString.col(), xRecordString.data());
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL,
									nPlayerID,
									"Upload From Client String set record",
									xMsg.record_name(),
									__FUNCTION__,
									__LINE__);
		}
	}
	
}

void NFGameServerNet_ServerModule::OnClientRecordObjectProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordObject)


	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client Object set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordObject &xRecordObject = xMsg.property_list().Get(i);
			pRecord->SetObject(xRecordObject.row(), xRecordObject.col(), NFINetModule::PBToNF(xRecordObject.data()));
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client Object set record", xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameServerNet_ServerModule::OnClientRecordVector2Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordVector2)

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client vector2 set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}
	if (pRecord->GetUpload() || nGMLevel > 0)
	{
		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordVector2 &xRecordVector2 = xMsg.property_list().Get(i);
			pRecord->SetVector2(xRecordVector2.row(), xRecordVector2.col(), NFINetModule::PBToNF(xRecordVector2.data()));
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client vector2 set record", xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}
}

void NFGameServerNet_ServerModule::OnClientRecordVector3Process(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ObjectRecordVector3)

	NF_SHARE_PTR<NFIRecord> pRecord = pObject->GetRecordManager()->GetElement(xMsg.record_name());
	if (!pRecord)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nPlayerID, "Upload From Client vector3 set record error", xMsg.record_name(), __FUNCTION__, __LINE__);
		return;
	}

	const std::string& strAccount = pObject->GetPropertyString(NFrame::Player::Account());
	const int nGMLevel = m_pElementModule->GetPropertyInt(strAccount, NFrame::GM::Level());

	if (pRecord->GetUpload() || nGMLevel > 0)
	{

		for (int i = 0; i < xMsg.property_list_size(); i++)
		{
			const NFMsg::RecordVector3 &xRecordVector3 = xMsg.property_list().Get(i);
			pRecord->SetVector3(xRecordVector3.row(), xRecordVector3.col(), NFINetModule::PBToNF(xRecordVector3.data()));
			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nPlayerID, "Upload From Client vector3 set record", xMsg.record_name(), __FUNCTION__, __LINE__);
		}
	}
}

int NFGameServerNet_ServerModule::DestroyPlayerByTime(const NFGUID & self, const std::string & name, const float fIntervalTime, const int nCount)
{
	const int nSceneID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::SceneID());
	const int nGroupID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GroupID());
	const NFGUID gateID = m_pKernelModule->GetPropertyObject(self, NFrame::Player::GateID());

	const NFGUID& matchID = m_pSceneModule->GetPropertyObject(nSceneID, nGroupID, NFrame::Group::MatchID());

	if (matchID.IsNull() && gateID.IsNull())
	{
		m_pKernelModule->DestroyObject(self);
	}

	return 0;
}

void NFGameServerNet_ServerModule::OnProxyServerRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!NFINetModule::ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
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

		pServerData->xServerData.nFD = nSockIndex;
		*(pServerData->xServerData.pData) = xData;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");
	}

	return;
}

void NFGameServerNet_ServerModule::OnProxyServerUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
		mProxyMap.RemoveElement(xData.server_id());


		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy UnRegistered");
	}

	return;
}

void NFGameServerNet_ServerModule::OnRefreshProxyServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
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

		pServerData->xServerData.nFD = nSockIndex;
		*(pServerData->xServerData.pData) = xData;

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "Proxy Registered");
	}

	return;
}

void NFGameServerNet_ServerModule::SendMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message& xMsg, const NFGUID& self)
{
	NF_SHARE_PTR<GateBaseInfo> pData = mRoleBaseData.GetElement(self);
	if (pData)
	{
		NF_SHARE_PTR<GateServerInfo> pProxyData = mProxyMap.GetElement(pData->nGateID);
		if (pProxyData)
		{
			m_pNetModule->SendMsgPB(nMsgID, xMsg, pProxyData->xServerData.nFD, pData->xClientID);
		}
	}
}

void NFGameServerNet_ServerModule::SendMsgToGate(const uint16_t nMsgID, const std::string& strMsg, const NFGUID& self)
{
	NF_SHARE_PTR<GateBaseInfo> pData = mRoleBaseData.GetElement(self);
	if (pData)
	{
		NF_SHARE_PTR<GateServerInfo> pProxyData = mProxyMap.GetElement(pData->nGateID);
		if (pProxyData)
		{
			m_pNetModule->SendMsg(nMsgID, strMsg, pProxyData->xServerData.nFD, pData->xClientID);
		}
	}
}

void NFGameServerNet_ServerModule::SendGroupMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message & xMsg, const int nSceneID, const int nGroupID)
{
	//care: batch
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			this->SendMsgPBToGate(nMsgID, xMsg, xObject);
		}
	}
}

void NFGameServerNet_ServerModule::SendGroupMsgPBToGate(const uint16_t nMsgID, google::protobuf::Message & xMsg, const int nSceneID, const int nGroupID, const NFGUID exceptID)
{
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			if (xObject != exceptID)
			{
				this->SendMsgPBToGate(nMsgID, xMsg, xObject);
			}
		}
	}
}

void NFGameServerNet_ServerModule::SendGroupMsgPBToGate(const uint16_t nMsgID, const std::string & strMsg, const int nSceneID, const int nGroupID)
{
	//care: batch
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			this->SendMsgToGate(nMsgID, strMsg, xObject);
		}
	}
}

void NFGameServerNet_ServerModule::SendGroupMsgPBToGate(const uint16_t nMsgID, const std::string & strMsg, const int nSceneID, const int nGroupID, const NFGUID exceptID)
{
	NFDataList xList;
	if (m_pKernelModule->GetGroupObjectList(nSceneID, nGroupID, xList, true))
	{
		for (int i = 0; i < xList.GetCount(); ++i)
		{
			NFGUID xObject = xList.Object(i);
			if (xObject != exceptID)
			{
				this->SendMsgToGate(nMsgID, strMsg, xObject);
			}
		}
	}
}

bool NFGameServerNet_ServerModule::AddPlayerGateInfo(const NFGUID& nRoleID, const NFGUID& nClientID, const int nGateID)
{
    if (nGateID <= 0)
    {
        
        return false;
    }

    if (nClientID.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<NFGameServerNet_ServerModule::GateBaseInfo> pBaseData = mRoleBaseData.GetElement(nRoleID);
    if (nullptr != pBaseData)
    {
        
        m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, nClientID, "player is exist, cannot enter game", "", __FUNCTION__, __LINE__);
        return false;
    }

    NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(nGateID);
    if (nullptr == pServerData)
    {
        return false;
    }

    if (!pServerData->xRoleInfo.insert(std::make_pair(nRoleID, pServerData->xServerData.nFD)).second)
    {
        return false;
    }

    if (!mRoleBaseData.AddElement(nRoleID, NF_SHARE_PTR<GateBaseInfo>(NF_NEW GateBaseInfo(nGateID, nClientID))))
    {
        pServerData->xRoleInfo.erase(nRoleID) ;
        return false;
    }

    return true;
}

bool NFGameServerNet_ServerModule::RemovePlayerGateInfo(const NFGUID& nRoleID)
{
    NF_SHARE_PTR<GateBaseInfo> pBaseData = mRoleBaseData.GetElement(nRoleID);
    if (nullptr == pBaseData)
    {
        return false;
    }

    mRoleBaseData.RemoveElement(nRoleID);

    NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.GetElement(pBaseData->nGateID);
    if (nullptr == pServerData)
    {
        return false;
    }

    pServerData->xRoleInfo.erase(nRoleID);
    return true;
}

NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateBaseInfo> NFGameServerNet_ServerModule::GetPlayerGateInfo(const NFGUID& nRoleID)
{
    return mRoleBaseData.GetElement(nRoleID);
}

NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> NFGameServerNet_ServerModule::GetGateServerInfo(const int nGateID)
{
    return mProxyMap.GetElement(nGateID);
}

NF_SHARE_PTR<NFIGameServerNet_ServerModule::GateServerInfo> NFGameServerNet_ServerModule::GetGateServerInfoBySockIndex(const NFSOCK nSockIndex)
{
    int nGateID = -1;
    NF_SHARE_PTR<GateServerInfo> pServerData = mProxyMap.First();
    while (pServerData)
    {
        if (nSockIndex == pServerData->xServerData.nFD)
        {
            nGateID = pServerData->xServerData.pData->server_id();
            break;
        }

        pServerData = mProxyMap.Next();
    }

    if (nGateID == -1)
    {
        return nullptr;
    }

    return pServerData;
}

void NFGameServerNet_ServerModule::OnTransWorld(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	std::string strMsg;
	NFGUID nPlayer;
	int64_t nHasKey = 0;
	if (NFINetModule::ReceivePB( nMsgID, msg, nLen, strMsg, nPlayer))
	{
		nHasKey = nPlayer.nData64;
	}

	m_pNetClientModule->SendBySuitWithOutHead(NF_SERVER_TYPES::NF_ST_WORLD, nHasKey, nMsgID, std::string(msg, nLen));
}

void NFGameServerNet_ServerModule::OnClanTransWorld(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	switch (nMsgID)
	{
		case NFMsg::EGMI_REQ_CREATE_CLAN:
		case NFMsg::EGMI_REQ_JOIN_CLAN:
		{
			std::string strMsg;
			NFGUID nPlayer;
			if (NFINetModule::ReceivePB(nMsgID, msg, nLen, strMsg, nPlayer) && !nPlayer.IsNull())
			{
				NFGUID xClanID = m_pKernelModule->GetPropertyObject(nPlayer, NFrame::Player::Clan_ID());
				if (xClanID.IsNull())
				{
					int nHashKey = nPlayer.nHead64;
					m_pNetClientModule->SendBySuitWithOutHead(NF_SERVER_TYPES::NF_ST_WORLD, nHashKey, nMsgID, std::string(msg, nLen));
				}
			}
		}
			break;
		case NFMsg::EGMI_REQ_LEAVE_CLAN:
		case NFMsg::EGMI_REQ_OPR_CLAN:
		{

		}
			break;
		case NFMsg::EGMI_REQ_SEARCH_CLAN:
		{

		}
			break;
		default:
			break;
	}

	CLIENT_MSG_PROCESS(nMsgID, msg, nLen, NFMsg::ObjectRecordVector2)

	std::string strMsg;
	NFGUID nPlayer;
	int nHasKey = 0;
	if (NFINetModule::ReceivePB(nMsgID, msg, nLen, strMsg, nPlayer))
	{
		nHasKey = nPlayer.nData64;
		m_pNetClientModule->SendBySuitWithOutHead(NF_SERVER_TYPES::NF_ST_WORLD, nHasKey, nMsgID, std::string(msg, nLen));
	}
}

