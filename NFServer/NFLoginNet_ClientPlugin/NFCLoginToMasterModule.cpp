// -------------------------------------------------------------------------
//    @FileName			:    NFCLoginNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ClientModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCLoginToMasterModule.h"
#include "NFLoginNet_ClientPlugin.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"

bool NFCLoginToMasterModule::Init()
{
	m_pNetClientModule = NF_NEW NFINetClientModule(pPluginManager);

	m_pNetClientModule->Init();

    return true;
}

bool NFCLoginToMasterModule::Shut()
{
    return true;
}

bool NFCLoginToMasterModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pLoginLogicModule = pPluginManager->FindModule<NFILoginLogicModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pLoginNet_ServerModule = pPluginManager->FindModule<NFILoginNet_ServerModule>();

	m_pNetClientModule->AddReceiveCallBack(NFMsg::EGMI_ACK_CONNECT_WORLD, this, &NFCLoginToMasterModule::OnSelectServerResultProcess);
	m_pNetClientModule->AddReceiveCallBack(NFMsg::EGMI_STS_NET_INFO, this, &NFCLoginToMasterModule::OnWorldInfoProcess);
	
	m_pNetClientModule->AddEventCallBack(this, &NFCLoginToMasterModule::OnSocketMSEvent);

    NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
    if (xLogicClass.get())
    {
        NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
        std::string strConfigName;
        for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
        {
            const int nServerType = m_pElementModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_MASTER)
            {
                const int nPort = m_pElementModule->GetPropertyInt(strConfigName, "Port");
                const int nMaxConnect = m_pElementModule->GetPropertyInt(strConfigName, "MaxOnline");
                const int nCpus = m_pElementModule->GetPropertyInt(strConfigName, "CpuCount");
                const std::string& strName = m_pElementModule->GetPropertyString(strConfigName, "Name");
                const std::string& strIP = m_pElementModule->GetPropertyString(strConfigName, "IP");

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

bool NFCLoginToMasterModule::BeforeShut()
{

    return false;
}

bool NFCLoginToMasterModule::Execute()
{
    m_pNetClientModule->Execute();

	return true;
}

void NFCLoginToMasterModule::Register(NFINet* pNet)
{
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
                const std::string& strName = m_pElementModule->GetPropertyString(strConfigName, "Name");
                const std::string& strIP = m_pElementModule->GetPropertyString(strConfigName, "IP");

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
					m_pNetClientModule->SendToServerByPB(nTargetID, NFMsg::EGameMsgID::EGMI_LTM_LOGIN_REGISTERED, xMsg);

                    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "Register");
                }
            }
        }
    }
}

void NFCLoginToMasterModule::OnSelectServerResultProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::AckConnectWorldResult xMsg;
    if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    m_pLoginNet_ServerModule->OnSelectWorldResultsProcess(xMsg.world_id(), NFINetModule::PBToNF(xMsg.sender()), xMsg.login_id(), xMsg.account(), xMsg.world_ip(), xMsg.world_port(), xMsg.world_key());
}

void NFCLoginToMasterModule::OnSocketMSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
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

void NFCLoginToMasterModule::OnWorldInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID ;
    NFMsg::ServerInfoReportList xMsg;
    if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);

        NF_SHARE_PTR<NFMsg::ServerInfoReport> pServerData = mWorldMap.GetElement(xData.server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<NFMsg::ServerInfoReport>(NF_NEW NFMsg::ServerInfoReport());
            *pServerData = xData;

            mWorldMap.AddElement(xData.server_id(), pServerData);
        }

    }

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xMsg.server_list_size()), "", "WorldInfo");
}

NFINetClientModule* NFCLoginToMasterModule::GetClusterModule()
{
	return m_pNetClientModule;
}

NFMapEx<int, NFMsg::ServerInfoReport>& NFCLoginToMasterModule::GetWorldMap()
{
    return mWorldMap;
}