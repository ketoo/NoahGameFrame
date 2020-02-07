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
#include "NFMasterNet_ServerModule.h"
#include "NFMasterNet_ServerPlugin.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFMasterNet_ServerModule::~NFMasterNet_ServerModule()
{
}

bool NFMasterNet_ServerModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

	return true;
}

bool NFMasterNet_ServerModule::Shut()
{
	return true;
}

void NFMasterNet_ServerModule::OnWorldRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
		NF_SHARE_PTR<ServerData> pServerData = mWorldMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
			mWorldMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->nFD = nSockIndex;
		*(pServerData->pData) = xData;

		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "WorldRegistered");
	}


	SynWorldToLoginAndWorld();
}

void NFMasterNet_ServerModule::OnWorldUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
		mWorldMap.RemoveElement(xData.server_id());


		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "WorldUnRegistered");
	}

	SynWorldToLoginAndWorld();
}

void NFMasterNet_ServerModule::OnRefreshWorldInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
		NF_SHARE_PTR<ServerData> pServerData = mWorldMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
			mWorldMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->nFD = nSockIndex;
		*(pServerData->pData) = xData;

		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "RefreshWorldInfo");

	}

	SynWorldToLoginAndWorld();
}

void NFMasterNet_ServerModule::OnLoginRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
		NF_SHARE_PTR<ServerData> pServerData = mLoginMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
			mLoginMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->nFD = nSockIndex;
		*(pServerData->pData) = xData;

		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "LoginRegistered");
	}

	SynWorldToLoginAndWorld();
}

void NFMasterNet_ServerModule::OnLoginUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

		mLoginMap.RemoveElement(xData.server_id());

		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "LoginUnRegistered");

	}
}

void NFMasterNet_ServerModule::OnRefreshLoginInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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
		NF_SHARE_PTR<ServerData> pServerData = mLoginMap.GetElement(xData.server_id());
		if (!pServerData)
		{
			pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
			mLoginMap.AddElement(xData.server_id(), pServerData);
		}

		pServerData->nFD = nSockIndex;
		*(pServerData->pData) = xData;

		m_pLogModule->LogInfo(NFGUID(0, xData.server_id()), xData.server_name(), "RefreshLoginInfo");

	}
}

void NFMasterNet_ServerModule::OnSelectWorldProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqConnectWorld xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.GetElement(xMsg.world_id());
	if (!pServerData)
	{
		return;
	}

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::REQ_CONNECT_WORLD, xMsg, pServerData->nFD);
}

bool NFMasterNet_ServerModule::Execute()
{
	LogGameServer();

	return true;
}

void NFMasterNet_ServerModule::OnSelectServerResultProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::AckConnectWorldResult xMsg;
	if (!m_pNetModule->ReceivePB( nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NF_SHARE_PTR<ServerData> pServerData = mLoginMap.GetElement(xMsg.login_id());
	if (!pServerData)
	{
		return;
	}

	
	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::ACK_CONNECT_WORLD, xMsg, pServerData->nFD);
}

bool NFMasterNet_ServerModule::AfterInit()
{
	m_pNetModule->AddReceiveCallBack(NFMsg::STS_HEART_BEAT, this, &NFMasterNet_ServerModule::OnHeartBeat);
	m_pNetModule->AddReceiveCallBack(NFMsg::WTM_WORLD_REGISTERED, this, &NFMasterNet_ServerModule::OnWorldRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::WTM_WORLD_UNREGISTERED, this, &NFMasterNet_ServerModule::OnWorldUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::WTM_WORLD_REFRESH, this, &NFMasterNet_ServerModule::OnRefreshWorldInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::LTM_LOGIN_REGISTERED, this, &NFMasterNet_ServerModule::OnLoginRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::LTM_LOGIN_UNREGISTERED, this, &NFMasterNet_ServerModule::OnLoginUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::LTM_LOGIN_REFRESH, this, &NFMasterNet_ServerModule::OnRefreshLoginInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_CONNECT_WORLD, this, &NFMasterNet_ServerModule::OnSelectWorldProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::ACK_CONNECT_WORLD, this, &NFMasterNet_ServerModule::OnSelectServerResultProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::STS_SERVER_REPORT, this, &NFMasterNet_ServerModule::OnServerReport);

	m_pNetModule->AddReceiveCallBack(this, &NFMasterNet_ServerModule::InvalidMessage);

	m_pNetModule->AddEventCallBack(this, &NFMasterNet_ServerModule::OnSocketEvent);
	m_pNetModule->ExpandBufferSize();

	NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
	if (xLogicClass)
	{
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

			const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
			const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
			if (nServerType == NF_SERVER_TYPES::NF_ST_MASTER && pPluginManager->GetAppID() == nServerID)
			{
				const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
				const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
				const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::ID());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				int nRet = m_pNetModule->Initialization(nMaxConnect, nPort, nCpus);
				if (nRet < 0)
				{
					std::ostringstream strLog;
					strLog << "Cannot init server net, Port = " << nPort;
					m_pLogModule->LogError(NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
					NFASSERT(nRet, "Cannot init server net", __FILE__, __FUNCTION__);
					exit(0);
				}
				NFMsg::ServerInfoReport reqMsg;

				reqMsg.set_server_id(nServerID);
				reqMsg.set_server_name(strId);
				reqMsg.set_server_cur_count(0);
				reqMsg.set_server_ip(strIP);
				reqMsg.set_server_port(nPort);
				reqMsg.set_server_max_online(nMaxConnect);
				reqMsg.set_server_state(NFMsg::EST_NARMAL);
				reqMsg.set_server_type(nServerType);

				auto pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
				*(pServerData->pData) = reqMsg;
				mMasterMap.AddElement(nServerID, pServerData);
			}
		}
	}

	return true;
}

void NFMasterNet_ServerModule::OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	//std::cout << "OnSocketEvent::thread id=" << GetCurrentThreadId() << std::endl;

	if (eEvent & NF_NET_EVENT_EOF)
	{
		m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF Connection closed", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else if (eEvent & NF_NET_EVENT_ERROR)
	{
		m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR Got an error on the connection", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else if (eEvent & NF_NET_EVENT_TIMEOUT)
	{
		m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT read timeout", __FUNCTION__, __LINE__);
		OnClientDisconnect(nSockIndex);
	}
	else  if (eEvent & NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogInfo(NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED connected success", __FUNCTION__, __LINE__);
		OnClientConnected(nSockIndex);
	}
}

void NFMasterNet_ServerModule::OnClientDisconnect(const NFSOCK nAddress)
{
	
	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		if (nAddress == pServerData->nFD)
		{
			pServerData->pData->set_server_state(NFMsg::EST_CRASH);
			pServerData->nFD = 0;

			SynWorldToLoginAndWorld();
			return;
		}

		pServerData = mWorldMap.Next();
	}

	//////////////////////////////////////////////////////////////////////////

	int nServerID = 0;
	pServerData = mLoginMap.First();
	while (pServerData)
	{
		if (nAddress == pServerData->nFD)
		{
			nServerID = pServerData->pData->server_id();
			break;
		}

		pServerData = mLoginMap.Next();
	}

	mLoginMap.RemoveElement(nServerID);

}

void NFMasterNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{
}

void NFMasterNet_ServerModule::SynWorldToLoginAndWorld()
{
	NFMsg::ServerInfoReportList xData;

	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReport* pData = xData.add_server_list();
		*pData = *(pServerData->pData);

		pServerData = mWorldMap.Next();
	}

	//loginserver
	pServerData = mLoginMap.First();
	while (pServerData)
	{
		m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::STS_NET_INFO, xData, pServerData->nFD);

		pServerData = mLoginMap.Next();
	}

	//world server
	pServerData = mWorldMap.First();
	while (pServerData)
	{
		NFMsg::ServerInfoReportList xWorldData;
		const int nCurArea = m_pElementModule->GetPropertyInt(pServerData->pData->server_name(), NFrame::Server::Area());

		for (int i = 0; i < xData.server_list_size(); ++i)
		{
			const NFMsg::ServerInfoReport& xServerInfo = xData.server_list(i);
			//it must be the same area			
			const int nAreaID = m_pElementModule->GetPropertyInt(xServerInfo.server_name(), NFrame::Server::Area());
			if (nAreaID == nCurArea)
			{
				NFMsg::ServerInfoReport* pData = xWorldData.add_server_list();
				*pData = *(pServerData->pData);
			}
		}

		m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::STS_NET_INFO, xWorldData, pServerData->nFD);

		pServerData = mWorldMap.Next();
	}
	
}

void NFMasterNet_ServerModule::LogGameServer()
{
	if (mnLastLogTime + 60 > GetPluginManager()->GetNowTime())
	{
		return;
	}

	mnLastLogTime = GetPluginManager()->GetNowTime();

	//////////////////////////////////////////////////////////////////////////

	m_pLogModule->LogInfo(NFGUID(), "Begin Log WorldServer Info", "");

	NF_SHARE_PTR<ServerData> pGameData = mWorldMap.First();
	while (pGameData)
	{
		std::ostringstream stream;
		stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " << NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;
		m_pLogModule->LogInfo(NFGUID(), stream);

		pGameData = mWorldMap.Next();
	}

	m_pLogModule->LogInfo(NFGUID(), "End Log WorldServer Info", "");

	m_pLogModule->LogInfo(NFGUID(), "Begin Log LoginServer Info", "");

	//////////////////////////////////////////////////////////////////////////
	pGameData = mLoginMap.First();
	while (pGameData)
	{
		std::ostringstream stream;
		stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " << NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;
		m_pLogModule->LogInfo(NFGUID(), stream);

		pGameData = mLoginMap.Next();
	}

	m_pLogModule->LogInfo(NFGUID(), "End Log LoginServer Info", "");

}

void NFMasterNet_ServerModule::OnHeartBeat(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
}

void NFMasterNet_ServerModule::InvalidMessage(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	printf("NFNet || unMsgID=%d\n", nMsgID);
}

void NFMasterNet_ServerModule::OnServerReport(const NFSOCK nFd, const int msgId, const char* buffer, const uint32_t nLen)
{
	NFGUID xGUID;
	NFMsg::ServerInfoReport msg;
	if (!m_pNetModule->ReceivePB(msgId, buffer, nLen, msg, xGUID))
	{
		return;
	}

	std::shared_ptr<ServerData> pServerData;
	switch (msg.server_type())
	{
		case NF_SERVER_TYPES::NF_ST_LOGIN:
		{
			pServerData = mLoginMap.GetElement(msg.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<ServerData>(new ServerData());
				mLoginMap.AddElement(msg.server_id(), pServerData);
			}
		}
		break;
		case NF_SERVER_TYPES::NF_ST_WORLD:
		{
			pServerData = mWorldMap.GetElement(msg.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<ServerData>(new ServerData());
				mWorldMap.AddElement(msg.server_id(), pServerData);
			}
		}
		break;
		case NF_SERVER_TYPES::NF_ST_PROXY:
		{
			pServerData = mProxyMap.GetElement(msg.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<ServerData>(new ServerData());
				mProxyMap.AddElement(msg.server_id(), pServerData);
			}
		}
		break;
		case NF_SERVER_TYPES::NF_ST_GAME:
		{
			pServerData = mGameMap.GetElement(msg.server_id());
			if (!pServerData)
			{
				pServerData = std::shared_ptr<ServerData>(new ServerData());
				mGameMap.AddElement(msg.server_id(), pServerData);
			}
		}
		break;

		default:
		{
			//m_pLogModule->LogError("UNKNOW SERVER TYPE", msg.server_type());
		}
		break;
	}

	//udate status
	if (pServerData)
	{
		pServerData->nFD = nFd;
		*(pServerData->pData) = msg;
	}
}

std::string NFMasterNet_ServerModule::GetServersStatus()
{
	/*
	rapidjson::Document doc;
	rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
	rapidjson::Value root(rapidjson::kObjectType);

	root.AddMember("code", 0, allocator);
	root.AddMember("errMsg", "", allocator);
	root.AddMember("nowTime", pPluginManager->GetNowTime(), allocator);

	rapidjson::Value master(rapidjson::kArrayType);
	std::shared_ptr<ServerData> pServerData = mMasterMap.First();
	while (pServerData)
	{
		rapidjson::Value server(rapidjson::kObjectType);
		server.AddMember("serverId", pServerData->pData->server_id(), allocator);
		server.AddMember("servrName", rapidjson::Value(pServerData->pData->server_name().c_str(), allocator), allocator);
		server.AddMember("ip", rapidjson::Value(pServerData->pData->server_ip().c_str(), allocator), allocator);
		server.AddMember("port", pServerData->pData->server_port(), allocator);
		server.AddMember("onlineCount", pServerData->pData->server_cur_count(), allocator);
		server.AddMember("status", (int)pServerData->pData->server_state(), allocator);
		
		master.PushBack(server, allocator);
		pServerData = mMasterMap.Next();
	}
	root.AddMember("master", master, allocator);

	rapidjson::Value logins(rapidjson::kArrayType);
	pServerData = mLoginMap.First();
	while (pServerData)
	{
		rapidjson::Value server(rapidjson::kObjectType);
		server.AddMember("serverId", pServerData->pData->server_id(), allocator);
		server.AddMember("servrName", rapidjson::Value(pServerData->pData->server_name().c_str(), allocator), allocator);
		server.AddMember("ip", rapidjson::Value(pServerData->pData->server_ip().c_str(), allocator), allocator);
		server.AddMember("port", pServerData->pData->server_port(), allocator);
		server.AddMember("onlineCount", pServerData->pData->server_cur_count(), allocator);
		server.AddMember("status", (int)pServerData->pData->server_state(), allocator);
		
		logins.PushBack(server, allocator);
		pServerData = mLoginMap.Next();
	}
	root.AddMember("logins", logins, allocator);

	rapidjson::Value worlds(rapidjson::kArrayType);
	pServerData = mWorldMap.First();
	while (pServerData.get())
	{
		rapidjson::Value server(rapidjson::kObjectType);
		server.AddMember("serverId", pServerData->pData->server_id(), allocator);
		server.AddMember("servrName", rapidjson::Value(pServerData->pData->server_name().c_str(), allocator), allocator);
		server.AddMember("ip", rapidjson::Value(pServerData->pData->server_ip().c_str(), allocator), allocator);
		server.AddMember("port", pServerData->pData->server_port(), allocator);
		server.AddMember("onlineCount", pServerData->pData->server_cur_count(), allocator);
		server.AddMember("status", (int)pServerData->pData->server_state(), allocator);
		
		worlds.PushBack(server, allocator);
		pServerData = mWorldMap.Next();
	}
	root.AddMember("worlds", worlds, allocator);

	rapidjson::Value proxys(rapidjson::kArrayType);
	pServerData = mProxyMap.First();
	while (pServerData.get())
	{
		rapidjson::Value server(rapidjson::kObjectType);
		server.AddMember("serverId", pServerData->pData->server_id(), allocator);
		server.AddMember("servrName", rapidjson::Value(pServerData->pData->server_name().c_str(), allocator), allocator);
		server.AddMember("ip", rapidjson::Value(pServerData->pData->server_ip().c_str(), allocator), allocator);
		server.AddMember("port", pServerData->pData->server_port(), allocator);
		server.AddMember("onlineCount", pServerData->pData->server_cur_count(), allocator);
		server.AddMember("status", (int)pServerData->pData->server_state(), allocator);

		proxys.PushBack(server, allocator);
		pServerData = mProxyMap.Next();
	}
	root.AddMember("proxys", proxys, allocator);

	rapidjson::Value games(rapidjson::kArrayType);
	pServerData = mGameMap.First();
	while (pServerData.get())
	{
		rapidjson::Value server(rapidjson::kObjectType);
		server.AddMember("serverId", pServerData->pData->server_id(), allocator);
		server.AddMember("servrName", rapidjson::Value(pServerData->pData->server_name().c_str(), allocator), allocator);
		server.AddMember("ip", rapidjson::Value(pServerData->pData->server_ip().c_str(), allocator), allocator);
		server.AddMember("port", pServerData->pData->server_port(), allocator);
		server.AddMember("onlineCount", pServerData->pData->server_cur_count(), allocator);
		server.AddMember("status", (int)pServerData->pData->server_state(), allocator);
		
		games.PushBack(server, allocator);
		pServerData = mGameMap.Next();
	}
	root.AddMember("games", games, allocator);

	rapidjson::StringBuffer jsonBuf;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuf);
	root.Accept(jsonWriter);

	return jsonBuf.GetString();

	*/
	return "";
}

