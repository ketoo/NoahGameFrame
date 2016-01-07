// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyServerNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyServerNet_ClientModule
//
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCProxyServerToGameModule.h"
#include "NFProxyServerNet_ClientPlugin.h"
#include "NFComm/NFCore/NFIHeartBeatManager.h"
#include "NFComm/NFCore/NFCHeartBeatManager.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

bool NFCProxyServerToGameModule::Init()
{
    return true;
}

bool NFCProxyServerToGameModule::Shut()
{
    //Final();
    //Clear();
    return true;
}

bool NFCProxyServerToGameModule::Execute()
{
	return NFIClusterClientModule::Execute();
}


bool NFCProxyServerToGameModule::AfterInit()
{

    m_pEventProcessModule = pPluginManager->FindModule<NFIEventProcessModule>("NFCEventProcessModule");
    m_pProxyLogicModule = dynamic_cast<NFIProxyLogicModule*>(pPluginManager->FindModule("NFCProxyLogicModule"));
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pProxyServerNet_ServerModule = dynamic_cast<NFIProxyServerNet_ServerModule*>(pPluginManager->FindModule("NFCProxyServerNet_ServerModule"));
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
	m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	
    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pProxyLogicModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pProxyServerNet_ServerModule);
    assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pLogicClassModule);
	
	NFIClusterClientModule::Bind(this, &NFCProxyServerToGameModule::OnReciveGSPack, &NFCProxyServerToGameModule::OnSocketGSEvent);

	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName; 
		for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
		{
			const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_GAME)
			{
				const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
				const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
				const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
				const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
				const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

				ConnectData xServerData;

				xServerData.nGameID = nServerID;
				xServerData.eServerType = (NF_SERVER_TYPE)nServerType;
				xServerData.strIP = strIP;
				xServerData.nPort = nPort;
				xServerData.strName = strName;

				NFIClusterClientModule::AddServer(xServerData);
			}
		}
	}

    return true;
}

void NFCProxyServerToGameModule::OnReciveGSPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	switch (nMsgID)
	{
	case NFMsg::EGMI_ACK_ENTER_GAME:
	    OnAckEnterGame(nSockIndex, nMsgID, msg, nLen);
	    break;
	default:
	    m_pProxyServerNet_ServerModule->Transpond(nSockIndex, nMsgID, msg, nLen);
	    break;
	}
}

void NFCProxyServerToGameModule::OnSocketGSEvent( const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet )
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
	else  if (eEvent == NF_NET_EVENT_CONNECTED)
	{
	    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
	    Register(pNet);
	}
}

void NFCProxyServerToGameModule::Register(NFINet* pNet)
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
            if (nServerType == NF_SERVER_TYPES::NF_ST_PROXY && pPluginManager->AppID() == nServerID)
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
					SendToServerByPB(nTargetID, NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED, xMsg);

					m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "Register");
				}
			}
		}
	}
}

void NFCProxyServerToGameModule::OnAckEnterGame(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::AckEventResult xData;
    if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xData, nPlayerID))
    {
        return;
    }

    if (xData.event_code() == NFMsg::EGEC_ENTER_GAME_SUCCESS)
    {
        const NFGUID& xClient = NFINetModule::PBToNF(xData.event_client());
        const NFGUID& xPlayer = NFINetModule::PBToNF(xData.event_object());

        m_pProxyServerNet_ServerModule->EnterGameSuccessEvent(xClient, xPlayer);
    }
}

void NFCProxyServerToGameModule::LogServerInfo( const std::string& strServerInfo )
{
	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), strServerInfo, "");
}
