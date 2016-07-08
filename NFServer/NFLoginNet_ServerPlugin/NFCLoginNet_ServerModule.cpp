// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCLoginNet_ServerModule.h"
#include "NFLoginNet_ServerPlugin.h"

const std::string PROPERTY_ACCOUNT = "Account";
const std::string PROPERTY_VERIFIED = "Verified";

bool NFCLoginNet_ServerModule::Init()
{
	m_pNetModule = NF_NEW NFINetModule(pPluginManager);
	return true;
}

bool NFCLoginNet_ServerModule::Shut()
{
	return true;
}

bool NFCLoginNet_ServerModule::BeforeShut()
{
	return true;
}

bool NFCLoginNet_ServerModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLoginLogicModule = pPluginManager->FindModule<NFILoginLogicModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLoginToMasterModule = pPluginManager->FindModule<NFILoginToMasterModule>();
	m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>();


	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_STS_HEART_BEAT, this, &NFCLoginNet_ServerModule::OnHeartBeat);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LOGIN, this, &NFCLoginNet_ServerModule::OnLoginProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LOGOUT, this, &NFCLoginNet_ServerModule::OnLogOut);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CONNECT_WORLD, this, &NFCLoginNet_ServerModule::OnSelectWorldProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_WORLD_LIST, this, &NFCLoginNet_ServerModule::OnViewWorldProcess);
	m_pNetModule->AddReceiveCallBack(this, &NFCLoginNet_ServerModule::InvalidMessage);

	m_pNetModule->AddEventCallBack(this, &NFCLoginNet_ServerModule::OnSocketClientEvent);

	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName;
		for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
		{
			const int nServerType = m_pElementModule->GetPropertyInt(strConfigName, "Type");
			const int nServerID = m_pElementModule->GetPropertyInt(strConfigName, "ServerID");
			if (nServerType == NF_SERVER_TYPES::NF_ST_LOGIN && pPluginManager->AppID() == nServerID)
			{
				const int nPort = m_pElementModule->GetPropertyInt(strConfigName, "Port");
				const int nMaxConnect = m_pElementModule->GetPropertyInt(strConfigName, "MaxOnline");
				const int nCpus = m_pElementModule->GetPropertyInt(strConfigName, "CpuCount");

				m_pUUIDModule->SetIdentID(nServerID);

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

int NFCLoginNet_ServerModule::OnSelectWorldResultsProcess(const int nWorldID, const NFGUID xSenderID, const int nLoginID, const std::string& strAccount, const std::string& strWorldIP, const int nWorldPort, const std::string& strWorldKey)
{
	NF_SHARE_PTR<int> xFD = mxClientIdent.GetElement(xSenderID);
	if (xFD)
	{
		NFMsg::AckConnectWorldResult xMsg;
		xMsg.set_world_id(nWorldID);
		xMsg.mutable_sender()->CopyFrom(NFINetModule::NFToPB(xSenderID));
		xMsg.set_login_id(nLoginID);
		xMsg.set_account(strAccount);
		xMsg.set_world_ip(strWorldIP);
		xMsg.set_world_port(nWorldPort);
		xMsg.set_world_key(strWorldKey);

		m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD, xMsg, *xFD);
	}

	return 0;
}

bool NFCLoginNet_ServerModule::Execute()
{
	return m_pNetModule->Execute();
}

void NFCLoginNet_ServerModule::OnClientConnected(const int nAddress)
{
	NetObject* pObject = m_pNetModule->GetNet()->GetNetObject(nAddress);
	if (pObject)
	{
		NFGUID xIdent = m_pUUIDModule->CreateGUID();
		pObject->SetClientID(xIdent);
		mxClientIdent.AddElement(xIdent, NF_SHARE_PTR<int>(NF_NEW int(nAddress)));
	}
}

void NFCLoginNet_ServerModule::OnClientDisconnect(const int nAddress)
{
	NetObject* pObject = m_pNetModule->GetNet()->GetNetObject(nAddress);
	if (pObject)
	{
		NFGUID xIdent = pObject->GetClientID();
		mxClientIdent.RemoveElement(xIdent);
	}
}

void NFCLoginNet_ServerModule::OnLoginProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqAccountLogin xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NetObject* pNetObject = m_pNetModule->GetNet()->GetNetObject(nSockIndex);
	if (pNetObject)
	{
		//还没有登录过
		if (pNetObject->GetConnectKeyState() == 0)
		{
			int nState = m_pLoginLogicModule->OnLoginProcess(pNetObject->GetClientID(), xMsg.account(), xMsg.password());
			if (0 != nState)
			{
				std::ostringstream strLog;
				strLog << "Check password failed, Account = " << xMsg.account() << " Password = " << xMsg.password();
				m_pLogModule->LogNormal(NFILogModule::NLL_ERROR_NORMAL, NFGUID(0, nSockIndex), strLog, __FUNCTION__, __LINE__);

				NFMsg::AckEventResult xMsg;
				xMsg.set_event_code(NFMsg::EGEC_ACCOUNTPWD_INVALID);

				m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xMsg, nSockIndex);
				return;
			}

			pNetObject->SetConnectKeyState(1);
			pNetObject->SetAccount(xMsg.account());

			NFMsg::AckEventResult xData;
			xData.set_event_code(NFMsg::EGEC_ACCOUNT_SUCCESS);

			m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_LOGIN, xData, nSockIndex);

			m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "Login successed :", xMsg.account().c_str());
		}
	}
}

void NFCLoginNet_ServerModule::OnSelectWorldProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqConnectWorld xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NetObject* pNetObject = m_pNetModule->GetNet()->GetNetObject(nSockIndex);
	if (!pNetObject)
	{
		return;
	}

	//没登录过
	if (pNetObject->GetConnectKeyState() <= 0)
	{
		return;
	}

	NFMsg::ReqConnectWorld xData;
	xData.set_world_id(xMsg.world_id());
	xData.set_login_id(pPluginManager->AppID());
	xData.mutable_sender()->CopyFrom(NFINetModule::NFToPB(pNetObject->GetClientID()));
	xData.set_account(pNetObject->GetAccount());

	m_pLoginToMasterModule->GetClusterModule()->SendSuitByPB(pNetObject->GetAccount(), NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xData);//here has a problem to be solve
}

void NFCLoginNet_ServerModule::OnSocketClientEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
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
	else  if (eEvent == NF_NET_EVENT_CONNECTED)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
		OnClientConnected(nSockIndex);
	}
}

void NFCLoginNet_ServerModule::SynWorldToClient(const int nFD)
{
	NFMsg::AckServerList xData;
	xData.set_type(NFMsg::RSLT_WORLD_SERVER);

	NFMapEx<int, NFMsg::ServerInfoReport>& xWorldMap = m_pLoginToMasterModule->GetWorldMap();
	NFMsg::ServerInfoReport* pWorldData = xWorldMap.FirstNude();
	while (pWorldData)
	{
		NFMsg::ServerInfo* pServerInfo = xData.add_info();

		pServerInfo->set_name(pWorldData->server_name());
		pServerInfo->set_status(pWorldData->server_state());
		pServerInfo->set_server_id(pWorldData->server_id());
		pServerInfo->set_wait_count(0);

		pWorldData = xWorldMap.NextNude();
	}


	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_WORLD_LIST, xData, nFD);
}

void NFCLoginNet_ServerModule::OnViewWorldProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqServerList xMsg;
	if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	if (xMsg.type() == NFMsg::RSLT_WORLD_SERVER)
	{
		SynWorldToClient(nSockIndex);
	}
}

void NFCLoginNet_ServerModule::OnHeartBeat(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
}

void NFCLoginNet_ServerModule::OnLogOut(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
}

void NFCLoginNet_ServerModule::InvalidMessage(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	printf("NFNet || 非法消息:unMsgID=%d\n", nMsgID);
}


