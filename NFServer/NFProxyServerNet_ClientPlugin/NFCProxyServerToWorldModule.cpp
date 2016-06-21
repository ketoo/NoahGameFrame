// -------------------------------------------------------------------------
//    @FileName			:    NFCProxyServerNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyServerNet_ClientModule
//
// -------------------------------------------------------------------------

#include "NFCProxyServerToWorldModule.h"
#include "NFProxyServerNet_ClientPlugin.h"
#include "NFComm/NFCore/NFIHeartBeatManager.h"
#include "NFComm/NFCore/NFCHeartBeatManager.h"
#include "NFComm/NFPluginModule/NFILogicClassModule.h"

bool NFCProxyServerToWorldModule::Init()
{
	m_pNetClientModule = NF_NEW NFINetClientModule(pPluginManager);

	m_pNetClientModule->Init();

    return true;
}

bool NFCProxyServerToWorldModule::Shut()
{
    //Final();
    //Clear();
    return true;
}

bool NFCProxyServerToWorldModule::Execute()
{
	m_pNetClientModule->Execute();

	return true;
}

void NFCProxyServerToWorldModule::OnServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
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
        //xServerData.eState = pData->server_state();
        xServerData.eServerType = (NF_SERVER_TYPES)xData.server_type();

        switch (xServerData.eServerType)
        {
            case NF_SERVER_TYPES::NF_ST_GAME:
            {
                m_pProxyServerToGameModule->GetClusterModule()->AddServer(xServerData);
            }
            break;
            case NF_SERVER_TYPES::NF_ST_WORLD:
            {
				m_pNetClientModule->AddServer(xServerData);
            }
            break;
            default:
                break;
        }

    }
}

void NFCProxyServerToWorldModule::OnSocketWSEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
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

void NFCProxyServerToWorldModule::Register(NFINet* pNet)
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
            if (nServerType == NF_SERVER_TYPES::NF_ST_PROXY && pPluginManager->AppID() == nServerID)
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

                NF_SHARE_PTR<ConnectData> pServerData = GetClusterModule()->GetServerNetInfo(pNet);
                if (pServerData)
                {
                    int nTargetID = pServerData->nGameID;
					GetClusterModule()->SendToServerByPB(nTargetID, NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED, xMsg);

                    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, pData->server_id()), pData->server_name(), "Register");
                }
            }
        }
    }
}

bool NFCProxyServerToWorldModule::AfterInit()
{
    m_pProxyLogicModule = pPluginManager->FindModule<NFIProxyLogicModule>();
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pProxyServerNet_ServerModule = pPluginManager->FindModule<NFIProxyServerNet_ServerModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>();
    m_pProxyServerToGameModule = pPluginManager->FindModule<NFIProxyServerToGameModule>();

	m_pNetClientModule->AddReceiveCallBack(NFMsg::EGMI_ACK_CONNECT_WORLD, this, &NFCProxyServerToWorldModule::OnSelectServerResultProcess);
	m_pNetClientModule->AddReceiveCallBack(NFMsg::EGMI_STS_NET_INFO, this, &NFCProxyServerToWorldModule::OnServerInfoProcess);
	m_pNetClientModule->AddReceiveCallBack(this, &NFCProxyServerToWorldModule::OnOtherMessage);

	m_pNetClientModule->AddEventCallBack(this, &NFCProxyServerToWorldModule::OnSocketWSEvent);

    NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
    if (xLogicClass.get())
    {
        NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
        std::string strConfigName;
        for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
        {
            const int nServerType = m_pElementModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_WORLD)
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


void NFCProxyServerToWorldModule::OnSelectServerResultProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //保持记录,直到下线,或者1分钟不上线即可删除
    NFGUID nPlayerID;
    NFMsg::AckConnectWorldResult xMsg;
    if (!NFINetModule::ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    NF_SHARE_PTR<ClientConnectData> pConnectData = mWantToConnectMap.GetElement(xMsg.account());
    if (NULL != pConnectData.get())
    {
        pConnectData->strConnectKey = xMsg.world_key();
        return;
    }

    pConnectData = NF_SHARE_PTR<ClientConnectData>(NF_NEW ClientConnectData());
    pConnectData->strAccount = xMsg.account();
    pConnectData->strConnectKey = xMsg.world_key();
    mWantToConnectMap.AddElement(pConnectData->strAccount, pConnectData);
}

NFINetClientModule* NFCProxyServerToWorldModule::GetClusterModule()
{
	return m_pNetClientModule;
}

bool NFCProxyServerToWorldModule::VerifyConnectData(const std::string& strAccount, const std::string& strKey)
{
    NF_SHARE_PTR<ClientConnectData> pConnectData = mWantToConnectMap.GetElement(strAccount);
    if (pConnectData && strKey == pConnectData->strConnectKey)
    {
        mWantToConnectMap.RemoveElement(strAccount);

        return true;
    }

    return false;
}

void NFCProxyServerToWorldModule::LogServerInfo(const std::string& strServerInfo)
{
    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), strServerInfo, "");
}

void NFCProxyServerToWorldModule::OnOtherMessage(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	m_pProxyServerNet_ServerModule->Transpond(nSockIndex, nMsgID, msg, nLen);
}
