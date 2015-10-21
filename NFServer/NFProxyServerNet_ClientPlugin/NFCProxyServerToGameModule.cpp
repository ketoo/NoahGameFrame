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

bool NFCProxyServerToGameModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	return NFIClusterClientModule::Execute(fLasFrametime, fStartedTime);
}


bool NFCProxyServerToGameModule::AfterInit()
{

    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pProxyLogicModule = dynamic_cast<NFIProxyLogicModule*>(pPluginManager->FindModule("NFCProxyLogicModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pProxyServerNet_ServerModule = dynamic_cast<NFIProxyServerNet_ServerModule*>(pPluginManager->FindModule("NFCProxyServerNet_ServerModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
	m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));
	
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

				ServerData xServerData;

				xServerData.nGameID = nServerID;
				xServerData.eServerType = (NF_SERVER_TYPE)nServerType;
				xServerData.strIP = strIP;
				xServerData.nPort = nPort;
				xServerData.strName = strName;
				xServerData.eState = NFMsg::EServerState::EST_MAINTEN;

				NFIClusterClientModule::AddServer(xServerData);
			}
		}
	}

    return true;
}

int NFCProxyServerToGameModule::OnReciveGSPack( const NFIPacket& msg )
{
	switch (msg.GetMsgHead()->GetMsgID())
	{
	case NFMsg::EGMI_ACK_ENTER_GAME:
	    OnAckEnterGame(msg);
	    break;
	default:
	    m_pProxyServerNet_ServerModule->Transpond(msg);
	    break;
	}

	return 0;
}

int NFCProxyServerToGameModule::OnSocketGSEvent( const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet )
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
	    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
	    Register(pNet);

	}
	return 0;
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
				pData->set_server_state(NFMsg::EST_NORMAL);
				pData->set_server_type(nServerType);

				NF_SHARE_PTR<ServerData> pServerData = GetServerNetInfo(pNet);
				if (pServerData)
				{
					int nTargetID = pServerData->nGameID;
					SendToServerByPB(nTargetID, NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED, xMsg);

					m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Register");
				}
			}
		}
	}
}

void NFCProxyServerToGameModule::OnAckEnterGame(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;
    NFMsg::AckEventResult xData;
    if (!NFINetModule::RecivePB(msg, xData, nPlayerID))
    {
        return;
    }

    if (xData.event_code() == NFMsg::EGEC_ENTER_GAME_SUCCESS)
    {
        const NFIDENTID& xClient = NFINetModule::PBToNF(xData.event_client());
        const NFIDENTID& xPlayer = NFINetModule::PBToNF(xData.event_object());

        m_pProxyServerNet_ServerModule->EnterGameSuccessEvent(xClient, xPlayer);
    }
}

void NFCProxyServerToGameModule::LogServerInfo( const std::string& strServerInfo )
{
	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(), strServerInfo, "");
}
