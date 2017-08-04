// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCMasterNet_ServerModule.h"
#include "NFMasterNet_ServerPlugin.h"
#include "Dependencies/rapidjson/document.h"
#include "Dependencies/rapidjson/writer.h"
#include "Dependencies/rapidjson/stringbuffer.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

NFCMasterNet_ServerModule::~NFCMasterNet_ServerModule()
{
}

bool NFCMasterNet_ServerModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

	return true;
}

bool NFCMasterNet_ServerModule::Shut()
{
	return true;
}

void NFCMasterNet_ServerModule::OnWorldRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "WorldRegistered");
	}


	SynWorldToLogin();
}

void NFCMasterNet_ServerModule::OnWorldUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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


		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "WorldUnRegistered");
	}

	SynWorldToLogin();
}

void NFCMasterNet_ServerModule::OnRefreshWorldInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "RefreshWorldInfo");

	}

	SynWorldToLogin();
}

void NFCMasterNet_ServerModule::OnLoginRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "LoginRegistered");
	}

	SynWorldToLogin();
}

void NFCMasterNet_ServerModule::OnLoginUnRegisteredProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "LoginUnRegistered");

	}
}

void NFCMasterNet_ServerModule::OnRefreshLoginInfoProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "RefreshLoginInfo");

	}
}

void NFCMasterNet_ServerModule::OnSelectWorldProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xMsg, pServerData->nFD);
}

bool NFCMasterNet_ServerModule::Execute()
{
	LogGameServer();

	return true;
}

void NFCMasterNet_ServerModule::OnSelectServerResultProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

	
	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD, xMsg, pServerData->nFD);
}

bool NFCMasterNet_ServerModule::AfterInit()
{
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_STS_HEART_BEAT, this, &NFCMasterNet_ServerModule::OnHeartBeat);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_WTM_WORLD_REGISTERED, this, &NFCMasterNet_ServerModule::OnWorldRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_WTM_WORLD_UNREGISTERED, this, &NFCMasterNet_ServerModule::OnWorldUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_WTM_WORLD_REFRESH, this, &NFCMasterNet_ServerModule::OnRefreshWorldInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_LTM_LOGIN_REGISTERED, this, &NFCMasterNet_ServerModule::OnLoginRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_LTM_LOGIN_UNREGISTERED, this, &NFCMasterNet_ServerModule::OnLoginUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_LTM_LOGIN_REFRESH, this, &NFCMasterNet_ServerModule::OnRefreshLoginInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CONNECT_WORLD, this, &NFCMasterNet_ServerModule::OnSelectWorldProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_CONNECT_WORLD, this, &NFCMasterNet_ServerModule::OnSelectServerResultProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_STS_SERVER_REPORT, this, &NFCMasterNet_ServerModule::OnServerReport);
	m_pNetModule->AddReceiveCallBack(this, &NFCMasterNet_ServerModule::InvalidMessage);

	m_pNetModule->AddEventCallBack(this, &NFCMasterNet_ServerModule::OnSocketEvent);
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
				const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
				const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

				int nRet = m_pNetModule->Initialization(nMaxConnect, nPort, nCpus);
				if (nRet < 0)
				{
					std::ostringstream strLog;
					strLog << "Cannot init server net, Port = " << nPort;
					m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NULL_OBJECT, strLog, __FUNCTION__, __LINE__);
					NFASSERT(nRet, "Cannot init server net", __FILE__, __FUNCTION__);
					exit(0);
				}
				NFMsg::ServerInfoReport reqMsg;

				reqMsg.set_server_id(nServerID);
				reqMsg.set_server_name(strName);
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

void NFCMasterNet_ServerModule::OnSocketEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
{
	//std::cout << "OnSocketEvent::thread id=" << GetCurrentThreadId() << std::endl;

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
	else  if (eEvent == NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
		OnClientConnected(nSockIndex);
	}
}

void NFCMasterNet_ServerModule::OnClientDisconnect(const NFSOCK nAddress)
{
	
	NF_SHARE_PTR<ServerData> pServerData = mWorldMap.First();
	while (pServerData)
	{
		if (nAddress == pServerData->nFD)
		{
			pServerData->pData->set_server_state(NFMsg::EST_CRASH);
			pServerData->nFD = 0;

			SynWorldToLogin();
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

void NFCMasterNet_ServerModule::OnClientConnected(const NFSOCK nAddress)
{
}

void NFCMasterNet_ServerModule::SynWorldToLogin()
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
		m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, pServerData->nFD);

		pServerData = mLoginMap.Next();
	}
}

void NFCMasterNet_ServerModule::LogGameServer()
{
	if (mnLastLogTime + 10 > GetPluginManager()->GetNowTime())
	{
		return;
	}

	mnLastLogTime = GetPluginManager()->GetNowTime();

	//////////////////////////////////////////////////////////////////////////

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "Begin Log WorldServer Info", "");

	NF_SHARE_PTR<ServerData> pGameData = mWorldMap.First();
	while (pGameData)
	{
		std::ostringstream stream;
		stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " << NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

		pGameData = mWorldMap.Next();
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "End Log WorldServer Info", "");

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "Begin Log LoginServer Info", "");

	//////////////////////////////////////////////////////////////////////////
	pGameData = mLoginMap.First();
	while (pGameData)
	{
		std::ostringstream stream;
		stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " << NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

		pGameData = mLoginMap.Next();
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "End Log LoginServer Info", "");

}

void NFCMasterNet_ServerModule::OnHeartBeat(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
}

void NFCMasterNet_ServerModule::InvalidMessage(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	printf("NFNet || unMsgID=%d\n", nMsgID);
}

void NFCMasterNet_ServerModule::OnServerReport(const NFSOCK nFd, const int msgId, const char* buffer, const uint32_t nLen)
{
	NFGUID xGUID;
	NFMsg::ServerInfoReport msg;
	if (!m_pNetModule->ReceivePB(msgId, buffer, nLen, msg, xGUID))
	{
		return;
	}

	std::shared_ptr<ServerData> pServerData;
	if (msg.server_type() == NF_SERVER_TYPES::NF_ST_LOGIN)
	{
		pServerData = mLoginMap.GetElement(msg.server_id());
		if (!pServerData)
		{
			pServerData = std::shared_ptr<ServerData>(new ServerData());
			mLoginMap.AddElement(msg.server_id(), pServerData);
		}
	}
	else if (msg.server_type() == NF_SERVER_TYPES::NF_ST_WORLD)
	{
		pServerData = mWorldMap.GetElement(msg.server_id());
		if (!pServerData)
		{
			pServerData = std::shared_ptr<ServerData>(new ServerData());
			mWorldMap.AddElement(msg.server_id(), pServerData);
		}
	}
	else if (msg.server_type() == NF_SERVER_TYPES::NF_ST_PROXY)
	{
		pServerData = mProxyMap.GetElement(msg.server_id());
		if (!pServerData)
		{
			pServerData = std::shared_ptr<ServerData>(new ServerData());
			mProxyMap.AddElement(msg.server_id(), pServerData);
		}
	}
	else if (msg.server_type() == NF_SERVER_TYPES::NF_ST_GAME)
	{
		pServerData = mGameMap.GetElement(msg.server_id());
		if (!pServerData)
		{
			pServerData = std::shared_ptr<ServerData>(new ServerData());
			mGameMap.AddElement(msg.server_id(), pServerData);
		}
	}
	else if (msg.server_type() == NF_SERVER_TYPES::NF_ST_AI)
	{
		pServerData = mGameMap.GetElement(msg.server_id());
		if (!pServerData)
		{
			pServerData = std::shared_ptr<ServerData>(new ServerData());
			mGameMap.AddElement(msg.server_id(), pServerData);
		}
	}

	//udate status
	pServerData->nFD = nFd;
	*(pServerData->pData) = msg;

	//std::cout << "ServerReport:"<<msg.server_name() << std::endl;

}

std::string NFCMasterNet_ServerModule::GetServersStatus()
{
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
		/*
		rapidjson::Value server_info_ext(rapidjson::kArrayType);
		for (int i = 0; i < pServerData->pData->server_info_list_ext().key_size();i++)
		{
			rapidjson::Value extKeyValue(rapidjson::kObjectType);
			extKeyValue.AddMember(rapidjson::Value(pServerData->pData->server_info_list_ext().key(i).c_str(), allocator), rapidjson::Value(pServerData->pData->server_info_list_ext().value(i).c_str(), allocator), allocator);
			server_info_ext.PushBack(extKeyValue, allocator);
		}
		server.AddMember("info_ext", server_info_ext, allocator);
		*/
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
		/*
		rapidjson::Value server_info_ext(rapidjson::kArrayType);
		for (int i = 0; i < pServerData->pData->server_info_list_ext().key_size();i++)
		{
			rapidjson::Value extKeyValue(rapidjson::kObjectType);
			extKeyValue.AddMember(rapidjson::Value(pServerData->pData->server_info_list_ext().key(i).c_str(), allocator), rapidjson::Value(pServerData->pData->server_info_list_ext().value(i).c_str(), allocator), allocator);
			server_info_ext.PushBack(extKeyValue, allocator);
		}
		server.AddMember("info_ext", server_info_ext, allocator);
		*/
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
		/*
		rapidjson::Value server_info_ext(rapidjson::kArrayType);
		for (int i = 0; i < pServerData->pData->server_info_list_ext().key_size();i++)
		{
			rapidjson::Value extKeyValue(rapidjson::kObjectType);
			extKeyValue.AddMember(rapidjson::Value(pServerData->pData->server_info_list_ext().key(i).c_str(), allocator), rapidjson::Value(pServerData->pData->server_info_list_ext().value(i).c_str(), allocator), allocator);
			server_info_ext.PushBack(extKeyValue, allocator);
		}
		server.AddMember("info_ext", server_info_ext, allocator);
		*/
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

		/*
		rapidjson::Value server_info_ext(rapidjson::kArrayType);
		for (int i = 0; i < pServerData->pData->server_info_list_ext().key_size();i++)
		{
			rapidjson::Value extKeyValue(rapidjson::kObjectType);
			extKeyValue.AddMember(rapidjson::Value(pServerData->pData->server_info_list_ext().key(i).c_str(), allocator), rapidjson::Value(pServerData->pData->server_info_list_ext().value(i).c_str(), allocator), allocator);
			server_info_ext.PushBack(extKeyValue, allocator);
		}
		server.AddMember("info_ext", server_info_ext, allocator);
		*/
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
		/*
		rapidjson::Value server_info_ext(rapidjson::kArrayType);
		for (int i = 0; i < pServerData->pData->server_info_list_ext().key_size();i++)
		{
			rapidjson::Value extKeyValue(rapidjson::kObjectType);
			extKeyValue.AddMember(rapidjson::Value(pServerData->pData->server_info_list_ext().key(i).c_str(), allocator), rapidjson::Value(pServerData->pData->server_info_list_ext().value(i).c_str(), allocator), allocator);
			server_info_ext.PushBack(extKeyValue, allocator);
		}
		server.AddMember("info_ext", server_info_ext, allocator);
		*/
		games.PushBack(server, allocator);
		pServerData = mGameMap.Next();
	}
	root.AddMember("games", games, allocator);

	rapidjson::StringBuffer jsonBuf;
	rapidjson::Writer<rapidjson::StringBuffer> jsonWriter(jsonBuf);
	root.Accept(jsonWriter);
	return jsonBuf.GetString();
}

