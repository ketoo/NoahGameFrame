// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyServerNet_ClientModule.cpp
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
    return NFIClusterClientModule::Execute();
}

void NFCProxyServerToWorldModule::OnReciveWSPack(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //������worldserver��������Ϣ
    switch (nMsgID)
    {
        case NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD:
            OnSelectServerResultProcess(nSockIndex, nMsgID, msg, nLen);
            break;

        case NFMsg::EGameMsgID::EGMI_STS_NET_INFO:
            OnServerInfoProcess(nSockIndex, nMsgID, msg, nLen);
            break;

        default:
            m_pProxyServerNet_ServerModule->Transpond(nSockIndex, nMsgID, msg, nLen);
            break;
    }
}

void NFCProxyServerToWorldModule::OnServerInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    int nSize = xMsg.server_list_size();
    for (int i = 0; i < nSize; ++i)
    {
        const NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        //type
        ConnectData xServerData;

        xServerData.nGameID = pData->server_id();
        xServerData.strIP = pData->server_ip();
        xServerData.nPort = pData->server_port();
        xServerData.strName = pData->server_name();
        //xServerData.eState = pData->server_state();
        xServerData.eServerType = (NF_SERVER_TYPES)pData->server_type();

        switch (xServerData.eServerType)
        {
            case NF_SERVER_TYPES::NF_ST_GAME:
            {
                m_pToGameServerClusterClient->AddServer(xServerData);
            }
            break;
            case NF_SERVER_TYPES::NF_ST_WORLD:
            {
                NFIClusterClientModule::AddServer(xServerData);
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

bool NFCProxyServerToWorldModule::AfterInit()
{
    m_pProxyLogicModule = dynamic_cast<NFIProxyLogicModule*>(pPluginManager->FindModule("NFCProxyLogicModule"));
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pProxyServerNet_ServerModule = dynamic_cast<NFIProxyServerNet_ServerModule*>(pPluginManager->FindModule("NFCProxyServerNet_ServerModule"));
    m_pElementInfoModule = pPluginManager->FindModule<NFIElementInfoModule>("NFCElementInfoModule");
    m_pLogModule = pPluginManager->FindModule<NFILogModule>("NFCLogModule");
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
    m_pToGameServerClusterClient = dynamic_cast<NFIClusterClientModule*>(pPluginManager->FindModule("NFCProxyServerToGameModule"));

    assert(NULL != m_pProxyLogicModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pProxyServerNet_ServerModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pLogicClassModule);
    assert(NULL != m_pToGameServerClusterClient);

    NFIClusterClientModule::Bind(this, &NFCProxyServerToWorldModule::OnReciveWSPack, &NFCProxyServerToWorldModule::OnSocketWSEvent);

    NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
    if (xLogicClass.get())
    {
        NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
        std::string strConfigName;
        for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
        {
            const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_WORLD)
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


void NFCProxyServerToWorldModule::OnSelectServerResultProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    //���ּ�¼,ֱ������,����1���Ӳ����߼���ɾ��
    NFGUID nPlayerID;
    NFMsg::AckConnectWorldResult xMsg;
    if (!NFINetModule::RecivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
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
