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


#include "NFGameServerToWorldModule.h"
#include "NFGameServerNet_ClientPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFPluginModule/NFINetClientModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFGameServerToWorldModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

	return true;
}

bool NFGameServerToWorldModule::Shut()
{

	return true;
}


bool NFGameServerToWorldModule::Execute()
{
	ServerReport();
	return true;
}

void NFGameServerToWorldModule::Register(NFINet* pNet)
{
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
				//const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				NFMsg::ServerInfoReportList xMsg;
				NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

				pData->set_server_id(nServerID);
				pData->set_server_name(strId);
				pData->set_server_cur_count(0);
				pData->set_server_ip(strIP);
				pData->set_server_port(nPort);
				pData->set_server_max_online(nMaxConnect);
				pData->set_server_state(NFMsg::EST_NARMAL);
				pData->set_server_type(nServerType);

				NF_SHARE_PTR<ConnectData> pServerData = m_pNetClientModule->GetServerNetInfo(pNet);
				if (pServerData)
				{
					int nTargetID = pServerData->nGameID;
					m_pNetClientModule->SendToServerByPB(nTargetID, NFMsg::EGameMsgID::GTW_GAME_REGISTERED, xMsg);

					m_pLogModule->LogInfo(NFGUID(0, pData->server_id()), pData->server_name(), "Register");
				}
			}
		}
	}
}

void NFGameServerToWorldModule::ServerReport()
{
	if (mLastReportTime + 10 > pPluginManager->GetNowTime())
	{
		return;
	}
	mLastReportTime = pPluginManager->GetNowTime();

	std::shared_ptr<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (pPluginManager->GetAppID() == nServerID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				NFMsg::ServerInfoReport reqMsg;

				reqMsg.set_server_id(nServerID);
				reqMsg.set_server_name(strId);
				reqMsg.set_server_cur_count(m_pKernelModule->GetOnLineCount());
				reqMsg.set_server_ip(strIP);
				reqMsg.set_server_port(nPort);
				reqMsg.set_server_max_online(nMaxConnect);
				reqMsg.set_server_state(NFMsg::EST_NARMAL);
				reqMsg.set_server_type(nServerType);

				m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::STS_SERVER_REPORT, reqMsg, NFGUID());
			}
		}
	}
}

bool NFGameServerToWorldModule::AfterInit()
{
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::STS_NET_INFO, this, &NFGameServerToWorldModule::OnServerInfoProcess);

	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_WORLD, this, &NFGameServerToWorldModule::TransPBToProxy);
	
	m_pNetClientModule->AddEventCallBack(NF_SERVER_TYPES::NF_ST_WORLD, this, &NFGameServerToWorldModule::OnSocketWSEvent);
	
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFGameServerToWorldModule::OnObjectClassEvent);

	m_pNetClientModule->ExpandBufferSize();

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();

		const int nCurAppID = pPluginManager->GetAppID();
		std::vector<std::string>::const_iterator itr =
			std::find_if(strIdList.begin(), strIdList.end(), [&](const std::string& strConfigId)
		{
			return nCurAppID == m_pElementModule->GetPropertyInt32(strConfigId, NFrame::Server::ServerID());
		});

		if (strIdList.end() == itr)
		{
			std::ostringstream strLog;
			strLog << "Cannot find current server, AppID = " << nCurAppID;
			m_pLogModule->LogError(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
			NFASSERT(-1, "Cannot find current server", __FILE__, __FUNCTION__);
			exit(0);
		}

		const int nCurArea = m_pElementModule->GetPropertyInt32(*itr, NFrame::Server::Area());

		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			const int nServerArea = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Area());
			if (nServerType == NF_SERVER_TYPES::NF_ST_WORLD && nCurArea == nServerArea)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				//const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				//const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				ConnectData xServerData;

				xServerData.nGameID = nServerID;
				xServerData.eServerType = (NF_SERVER_TYPES)nServerType;
				xServerData.strIP = strIP;
				xServerData.nPort = nPort;
				xServerData.strName = strId;

				m_pNetClientModule->AddServer(xServerData);
			}
		}
	}

	return true;
}

void NFGameServerToWorldModule::OnServerInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ServerInfoReportList xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	for (int i = 0; i < xMsg.server_list_size(); ++i)
	{
		const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

		//type
		ConnectData xServerData;

		xServerData.nGameID = xData.server_id();
		xServerData.strIP = xData.server_ip();
		xServerData.nPort = xData.server_port();
		xServerData.strName = xData.server_name();
		xServerData.nWorkLoad = xData.server_cur_count();
		xServerData.eServerType = (NF_SERVER_TYPES)xData.server_type();

		if (NF_SERVER_TYPES::NF_ST_WORLD == xServerData.eServerType)
		{
			m_pNetClientModule->AddServer(xServerData);
		}
		else if (NF_SERVER_TYPES::NF_ST_DB == xServerData.eServerType)
		{
			m_pNetClientModule->AddServer(xServerData);
		}
	}
}

void NFGameServerToWorldModule::OnSocketWSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	if (eEvent & NF_NET_EVENT_EOF)
	{
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
	}
	else  if (eEvent & NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED connected success", __FUNCTION__, __LINE__);
		Register(pNet);

	}
}

int NFGameServerToWorldModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
	if (strClassName == NFrame::Player::ThisName())
	{
		if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
		{
			SendOffline(self);
		}
		else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
		{
			SendOnline(self);
		}
	}

	return 0;
}

void NFGameServerToWorldModule::SendOnline(const NFGUID& self)
{
	if (m_pKernelModule->ExistObject(self))
	{
		NFMsg::RoleOnlineNotify xMsg;
		//const NFGUID& xClan = m_pKernelModule->GetPropertyObject(self, NFrame::Player::Clan_ID());
		const int& gateID = m_pKernelModule->GetPropertyInt(self, NFrame::Player::GateID());
		const std::string& playerName = m_pKernelModule->GetPropertyString(self, NFrame::Player::Name());
		const int bp = 0;//m_pKernelModule->GetPropertyInt(self, NFrame::Player::Cup());

		*xMsg.mutable_self() = NFINetModule::NFToPB(self);
		*xMsg.mutable_clan() = NFINetModule::NFToPB(NFGUID());
		xMsg.set_game(pPluginManager->GetAppID());
		xMsg.set_proxy(gateID);
		xMsg.set_name(playerName);
		xMsg.set_bp(bp);

		m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_ONLINE_NOTIFY, xMsg, self);
	}
}

void NFGameServerToWorldModule::SendOffline(const NFGUID& self)
{
	if (m_pKernelModule->ExistObject(self))
	{
		NFMsg::RoleOfflineNotify xMsg;

		//const NFGUID& xClan = m_pKernelModule->GetPropertyObject(self, NFrame::Player::Clan_ID());

		*xMsg.mutable_self() = NFINetModule::NFToPB(self);
		*xMsg.mutable_clan() = NFINetModule::NFToPB(NFGUID());
		xMsg.set_game(pPluginManager->GetAppID());
		xMsg.set_proxy(0);

		m_pNetClientModule->SendToAllServerByPB(NF_SERVER_TYPES::NF_ST_WORLD, NFMsg::ACK_OFFLINE_NOTIFY, xMsg, self);
	}
}

void NFGameServerToWorldModule::TransPBToProxy(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	std::string strData;
	if (!NFINetModule::ReceivePB( nMsgID, msg, nLen, strData, nPlayerID))
	{
		return;
	}

	m_pGameServerNet_ServerModule->SendMsgToGate(nMsgID, strData, nPlayerID);

	return;
}

void NFGameServerToWorldModule::TransmitToWorld(const int nHashKey, const int nMsgID, const google::protobuf::Message& xData)
{
	m_pNetClientModule->SendSuitByPB(NF_SERVER_TYPES::NF_ST_WORLD, nHashKey, nMsgID, xData);
}