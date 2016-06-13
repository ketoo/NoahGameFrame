// -------------------------------------------------------------------------
//    @FileName      :    NFCLoginNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCLoginNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

#include "NFCMasterNet_ServerModule.h"
#include "NFMasterNet_ServerPlugin.h"

bool NFCMasterNet_ServerModule::Init()
{
	m_pNetModule = NF_NEW NFINetModule(pPluginManager);
    return true;
}

bool NFCMasterNet_ServerModule::Shut()
{
    return true;
}

void NFCMasterNet_ServerModule::OnWorldRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.GetElement(xData.server_id());
        if (!pServerData.get())
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

void NFCMasterNet_ServerModule::OnWorldUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
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

void NFCMasterNet_ServerModule::OnRefreshWorldInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.GetElement(xData.server_id());
        if (!pServerData.get())
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

void NFCMasterNet_ServerModule::OnLoginRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mLoginMap.GetElement(xData.server_id());
        if (!pServerData.get())
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

void NFCMasterNet_ServerModule::OnLoginUnRegisteredProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
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

void NFCMasterNet_ServerModule::OnRefreshLoginInfoProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        const NFMsg::ServerInfoReport& xData = xMsg.server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mLoginMap.GetElement(xData.server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mLoginMap.AddElement(xData.server_id(), pServerData);
        }

        pServerData->nFD = nSockIndex;
        *(pServerData->pData) = xData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(0, xData.server_id()), xData.server_name(), "RefreshLoginInfo");

    }
}

void NFCMasterNet_ServerModule::OnSelectWorldProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::ReqConnectWorld xMsg;
    if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
        return;
    }

    NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.GetElement(xMsg.world_id());
    if (!pServerData.get())
    {
        return;
    }

    //转发送到世界服务器
	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_REQ_CONNECT_WORLD, xMsg, pServerData->nFD);
}

bool NFCMasterNet_ServerModule::Execute()
{
    LogGameServer();

	m_pNetModule->Execute();
    return true;
}

void NFCMasterNet_ServerModule::OnSelectServerResultProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    NFGUID nPlayerID;
    NFMsg::AckConnectWorldResult xMsg;
    if (!m_pNetModule->ReceivePB(nSockIndex, nMsgID, msg, nLen, xMsg, nPlayerID))
    {
		return;
    }

    NF_SHARE_PTR<ServerData> pServerData =  mLoginMap.GetElement(xMsg.login_id());
    if (!pServerData.get())
    {
        return;
    }

    //转发送到登录服务器
	m_pNetModule->SendMsgPB(NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD, xMsg, pServerData->nFD);
}

bool NFCMasterNet_ServerModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_STS_HEART_BEAT, this, &NFCMasterNet_ServerModule::OnHeartBeat);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_MTL_WORLD_REGISTERED, this, &NFCMasterNet_ServerModule::OnWorldRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_MTL_WORLD_UNREGISTERED, this, &NFCMasterNet_ServerModule::OnWorldUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_MTL_WORLD_REFRESH, this, &NFCMasterNet_ServerModule::OnRefreshWorldInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_LTM_LOGIN_REGISTERED, this, &NFCMasterNet_ServerModule::OnLoginRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_LTM_LOGIN_UNREGISTERED, this, &NFCMasterNet_ServerModule::OnLoginUnRegisteredProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_LTM_LOGIN_REFRESH, this, &NFCMasterNet_ServerModule::OnRefreshLoginInfoProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CONNECT_WORLD, this, &NFCMasterNet_ServerModule::OnSelectWorldProcess);
	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_ACK_CONNECT_WORLD, this, &NFCMasterNet_ServerModule::OnSelectServerResultProcess);
	m_pNetModule->AddReceiveCallBack(this, &NFCMasterNet_ServerModule::InvalidMessage);

	m_pNetModule->AddEventCallBack(this, &NFCMasterNet_ServerModule::OnSocketEvent);

    NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
    if (xLogicClass.get())
    {
        NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
        std::string strConfigName;
        for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
        {
            const int nServerType = m_pElementModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_MASTER && pPluginManager->AppID() == nServerID)
            {
                const int nPort = m_pElementModule->GetPropertyInt(strConfigName, "Port");
                const int nMaxConnect = m_pElementModule->GetPropertyInt(strConfigName, "MaxOnline");
                const int nCpus = m_pElementModule->GetPropertyInt(strConfigName, "CpuCount");
                const std::string& strName = m_pElementModule->GetPropertyString(strConfigName, "Name");
                const std::string& strIP = m_pElementModule->GetPropertyString(strConfigName, "IP");

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

void NFCMasterNet_ServerModule::OnSocketEvent(const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet)
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

void NFCMasterNet_ServerModule::OnClientDisconnect(const int nAddress)
{
    //不管是login还是world都要找出来,替他反注册
    NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.First();
    while (pServerData.get())
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
    pServerData =  mLoginMap.First();
    while (pServerData.get())
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

void NFCMasterNet_ServerModule::OnClientConnected(const int nAddress)
{
    //连接上来啥都不做
}

void NFCMasterNet_ServerModule::SynWorldToLogin()
{
    NFMsg::ServerInfoReportList xData;

    NF_SHARE_PTR<ServerData> pServerData =  mWorldMap.First();
    while (pServerData.get())
    {
        NFMsg::ServerInfoReport* pData = xData.add_server_list();
        *pData = *(pServerData->pData);

        pServerData = mWorldMap.Next();
    }

    //广播给所有loginserver
    pServerData =  mLoginMap.First();
    while (pServerData.get())
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
        stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;
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
        stream << "Type: " << pGameData->pData->server_type() << " ID: " << pGameData->pData->server_id() << " State: " <<  NFMsg::EServerState_Name(pGameData->pData->server_state()) << " IP: " << pGameData->pData->server_ip() << " FD: " << pGameData->nFD;
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), stream);

        pGameData = mLoginMap.Next();
    }

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFGUID(), "End Log LoginServer Info", "");

}

void NFCMasterNet_ServerModule::OnHeartBeat(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
}

void NFCMasterNet_ServerModule::InvalidMessage(const int nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	printf("NFNet || 非法消息:unMsgID=%d\n", nMsgID);
}
