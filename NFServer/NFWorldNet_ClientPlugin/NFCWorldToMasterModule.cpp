// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCWorldToMasterModule.h"
#include "NFWorldNet_ClientPlugin.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFMessageDefine/NFMsgPreGame.pb.h"

bool NFCWorldToMasterModule::Init()
{

    return true;
}

bool NFCWorldToMasterModule::Shut()
{
	return true;
}

bool NFCWorldToMasterModule::AfterInit()
{
	
	m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
	m_pWorldLogicModule = pPluginManager->FindModule<NFIWorldLogicModule>("NFCWorldLogicModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");

	assert(NULL != m_pEventProcessModule);
	assert(NULL != m_pWorldLogicModule);
	assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogModule);

	m_pEventProcessModule->AddEventCallBack(NFGUID(), NFED_ON_CLIENT_SELECT_SERVER_RESULTS, this, &NFCWorldToMasterModule::OnSelectServerResultsEvent);

	NFIClusterClientModule::Bind(this, &NFCWorldToMasterModule::OnReciveMSPack, &NFCWorldToMasterModule::OnSocketMSEvent);

	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName; 
		for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
		{
			const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
			if (nServerType == NF_SERVER_TYPES::NF_ST_MASTER)
			{
				const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
				const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
				const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
				const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
				const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

				ConnectData xServerData;

				xServerData.nGameID = nServerID;
				xServerData.eServerType = (NF_SERVER_TYPES)nServerType;
				xServerData.strIP = strIP;
				xServerData.nPort = nPort;
				xServerData.strName = strName;

				NFIClusterClientModule::AddServer(xServerData);
			}
		}
	}

	return true;
}


bool NFCWorldToMasterModule::Execute()
{
	return NFIClusterClientModule::Execute();
}

void NFCWorldToMasterModule::Register(NFINet* pNet)
{
	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName; 
		for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
		{
			const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
			if (nServerType == NF_SERVER_TYPES::NF_ST_WORLD && pPluginManager->AppID() == nServerID)
			{
				const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
				const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
				const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
				const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
				const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

				NFMsg::ServerInfoReportList xMsg;
				NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

				pData->set_server_id(nServerID);
				pData->set_server_name(strName);
				pData->set_server_cur_count(0);
				pData->set_server_ip(strIP);
				pData->set_server_port(nPort);
				pData->set_server_max_online(nMaxConnect);
				pData->set_server_state(NFMsg::EST_NARMAL);
				pData->set_server_type(nServerType);

				NF_SHARE_PTR<ConnectData> pServerData = GetServerNetInfo(pNet);
				if (pServerData)
				{
					int nTargetID = pServerData->nGameID;
					SendToServerByPB(nTargetID, NFMsg::EGameMsgID::EGMI_MTL_WORLD_REGISTERED, xMsg);

					m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "Register");
				}
			}
		}
	}
}

void NFCWorldToMasterModule::RefreshWorldInfo()
{

}

int NFCWorldToMasterModule::OnSelectServerProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqConnectWorld xMsg;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return 0;
	}

    NFCDataList var;
    var << xMsg.world_id() << NFINetModule::PBToNF(xMsg.sender())  << xMsg.login_id() << xMsg.account();
    m_pEventProcessModule->DoEvent(NFGUID(), NFED_ON_CLIENT_SELECT_SERVER, var);

    return 0;
}

int NFCWorldToMasterModule::OnSelectServerResultsEvent(const NFGUID& object, const int nEventID, const NFIDataList& var)
{
    if (var.GetCount() != 7
        || !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_OBJECT,
		TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_STRING,
		TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
    {
		return 0;
    }

    const int nWorldID = var.Int(0);
    const NFGUID xClientIdent = var.Object(1);
    const int nLoginID = var.Int(2);
    const std::string& strAccount = var.String(3);
    const std::string& strWorldAddress = var.String(4);
    const int nPort = var.Int(5);
    const std::string& strKey = var.String(6);

    NFMsg::AckConnectWorldResult xMsg;

    xMsg.set_world_id(nWorldID);
    xMsg.set_login_id(nLoginID);
    xMsg.set_world_port(nPort);
    xMsg.mutable_sender()->CopyFrom(NFINetModule::NFToPB(xClientIdent));
    xMsg.set_account(strAccount);
    xMsg.set_world_ip(strWorldAddress);
    xMsg.set_world_key(strKey);

	SendSuitByPB(NFMsg::EGMI_ACK_CONNECT_WORLD, xMsg);

    return 0;
}

int NFCWorldToMasterModule::OnKickClientProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqKickFromWorld xMsg;
	if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return 0;
	}

    //T人,下线
    NFCDataList var;
    var << xMsg.world_id() << xMsg.account();
    m_pEventProcessModule->DoEvent(NFGUID(), NFED_ON_KICK_FROM_SERVER, var);
    return 0;
}

void NFCWorldToMasterModule::OnReciveMSPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	switch (nMsgID)
	{
	case NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD:
		OnSelectServerProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	case NFMsg::EGameMsgID::EGMI_REQ_KICK_CLIENT_INWORLD:
		OnKickClientProcess(nSockIndex, nMsgID, msg, nLen);
		break;

	default:
		printf("NFNet || 非法消息:unMsgID=%d\n", nMsgID);
		break;
	}
}

void NFCWorldToMasterModule::OnSocketMSEvent( const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        Register(pNet);
    }
}

void NFCWorldToMasterModule::OnClientDisconnect( const int nAddress )
{

}

void NFCWorldToMasterModule::OnClientConnected( const int nAddress )
{

}

bool NFCWorldToMasterModule::BeforeShut()
{
    return true;
}

void NFCWorldToMasterModule::LogServerInfo( const std::string& strServerInfo )
{
	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), strServerInfo, "");
}
