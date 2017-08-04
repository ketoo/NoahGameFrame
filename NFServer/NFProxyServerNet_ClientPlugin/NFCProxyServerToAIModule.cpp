// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyServerToAIModule.cpp
//    @Author           :    Bluesky
//    @Date             :    2017-6-23
//    @Module           :    NFCProxyServerToAIModule
//
// -------------------------------------------------------------------------

#include "NFCProxyServerToAIModule.h"
#include "NFProxyServerNet_ClientPlugin.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCProxyServerToAIModule::Init()
{
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pProxyServerNet_ServerModule = pPluginManager->FindModule<NFIProxyServerNet_ServerModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();

    return true;
}

bool NFCProxyServerToAIModule::Shut()
{
    //Final();
    //Clear();
    return true;
}

bool NFCProxyServerToAIModule::Execute()
{
	return true;
}

bool NFCProxyServerToAIModule::AfterInit()
{
	m_pNetClientModule->AddReceiveCallBack(NF_SERVER_TYPES::NF_ST_AI, this, &NFCProxyServerToAIModule::Transpond);
	m_pNetClientModule->AddEventCallBack(NF_SERVER_TYPES::NF_ST_AI, this, &NFCProxyServerToAIModule::OnSocketGSEvent);
	m_pNetClientModule->ExpandBufferSize();

    NF_SHARE_PTR<NFIClass> xLogicClass = m_pClassModule->GetElement(NFrame::Server::ThisName());
    if (xLogicClass)
    {
		const std::vector<std::string>& strIdList = xLogicClass->GetIDList();
		for (int i = 0; i < strIdList.size(); ++i)
		{
			const std::string& strId = strIdList[i];

            const int nServerType = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Type());
            const int nServerID = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::ServerID());
            if (nServerType == NF_SERVER_TYPES::NF_ST_AI)
            {
                const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
                const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
                const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
                const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
                const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

                ConnectData xServerData;

                xServerData.nGameID = nServerID;
                xServerData.eServerType = (NF_SERVER_TYPES)nServerType;
                xServerData.strIP = strIP;
                xServerData.nPort = nPort;
                xServerData.strName = strName;

				m_pNetClientModule->AddServer(xServerData);
            }
        }
    }

    return true;
}

void NFCProxyServerToAIModule::OnSocketGSEvent(const NFSOCK nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
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

void NFCProxyServerToAIModule::Register(NFINet* pNet)
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
            if (nServerType == NF_SERVER_TYPES::NF_ST_PROXY && pPluginManager->GetAppID() == nServerID)
            {
                const int nPort = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::Port());
                const int nMaxConnect = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::MaxOnline());
                const int nCpus = m_pElementModule->GetPropertyInt32(strId, NFrame::Server::CpuCount());
                const std::string& strName = m_pElementModule->GetPropertyString(strId, NFrame::Server::Name());
                const std::string& strIP = m_pElementModule->GetPropertyString(strId, NFrame::Server::IP());

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

                NF_SHARE_PTR<ConnectData> pServerData = m_pNetClientModule->GetServerNetInfo(pNet);
                if (pServerData)
                {
                    int nTargetID = pServerData->nGameID;
                    m_pNetClientModule->SendToServerByPB(nTargetID, NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED, xMsg);

                    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "Register");
                }
            }
        }
    }
}

void NFCProxyServerToAIModule::OnAckEnterGame(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::AckEventResult xData;
    if (!NFINetModule::ReceivePB( nMsgID, msg, nLen, xData, nPlayerID))
    {
        return;
    }
 
	const NFGUID& xClient = NFINetModule::PBToNF(xData.event_client());
	const NFGUID& xPlayer = NFINetModule::PBToNF(xData.event_object());

	m_pProxyServerNet_ServerModule->EnterGameSuccessEvent(xClient, xPlayer);
	m_pProxyServerNet_ServerModule->Transpond(nSockIndex, nMsgID, msg, nLen);
}

void NFCProxyServerToAIModule::LogServerInfo(const std::string& strServerInfo)
{
    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), strServerInfo, "");
}

void NFCProxyServerToAIModule::Transpond(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	m_pProxyServerNet_ServerModule->Transpond(nSockIndex, nMsgID, msg, nLen);
}
