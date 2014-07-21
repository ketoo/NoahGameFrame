// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldNet_ServerModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-01-02
//    @Module           :    NFCWorldNet_ServerModule
//    @Desc             :
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCWorldNet_ServerModule.h"
#include "NFWorldNet_ServerPlugin.h"

bool NFCWorldNet_ServerModule::Init()
{
    mstrConfigIdent = "WorldServer";

    return true;
}

bool NFCWorldNet_ServerModule::AfterInit()
{
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pWorldLogicModule = dynamic_cast<NFIWorldLogicModule*>(pPluginManager->FindModule("NFCWorldLogicModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLogicClassModule = dynamic_cast<NFILogicClassModule*>(pPluginManager->FindModule("NFCLogicClassModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pWorldLogicModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogicClassModule);

    m_pEventProcessModule->AddEventCallBack(0, NFED_ON_CLIENT_SELECT_SERVER, this, &NFCWorldNet_ServerModule::OnSelectServerEvent);

    const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
    const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
    const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
    const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
    const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
    const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

    m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCWorldNet_ServerModule::OnRecivePack, &NFCWorldNet_ServerModule::OnSocketEvent);
    int nRet = m_pNet->Initialization(nMaxConnect, nPort, nCpus);
    if (nRet <= 0)
    {
        assert(0);
    }
    return true;
}

bool NFCWorldNet_ServerModule::Shut()
{
    m_pNet->Final();

    return true;
}

bool NFCWorldNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    return m_pNet->Execute(fLasFrametime, fStartedTime);
}

int NFCWorldNet_ServerModule::OnGameServerRegisteredProcess(const NFIPacket& msg)
{
    int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        ServerData* pServerData =  mGameMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mGameMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

         m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "GameServerRegistered");
    }

    SynGameToProxy();

    return 0;
}

int NFCWorldNet_ServerModule::OnGameServerUnRegisteredProcess(const NFIPacket& msg)
{
    int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        ServerData* pServerData =  mGameMap.RemoveElement(pData->server_id());
        if (pServerData)
        {
            delete pServerData;
            pServerData = NULL;
        }

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "GameServerRegistered");
    }
    return 0;
}

int NFCWorldNet_ServerModule::OnRefreshGameServerInfoProcess(const NFIPacket& msg)
{
    int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        ServerData* pServerData =  mGameMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mGameMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "GameServerRegistered");
    }

    SynGameToProxy();

    return 0;
}

int NFCWorldNet_ServerModule::OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIValueList& var)
{
    if (4 != var.GetCount()
        || !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return 0;
    }

    const int nWorldID = var.IntVal(0);
    const int nSenderAddress = var.IntVal(1);
    const int nLoginID = var.IntVal(2);
    const std::string& strAccount = var.StringVal(3);

    //////////////////////////////////////////////////////////////////////////
    if (InThisWorld(strAccount))
    {
        return 0;
    }

    ServerData* pServerData = mProxyMap.First();
    if (pServerData)
    {
        NFMsg::AckConnectWorldResult xData;

        xData.set_world_id(nWorldID);
        xData.set_sender_ip(nSenderAddress);
        xData.set_login_id(nLoginID);
        xData.set_account(strAccount);

        xData.set_world_ip(pServerData->pData->server_ip());
        xData.set_world_port(pServerData->pData->server_port());
        xData.set_world_key(strAccount);

        SendMsgPB(NFMsg::EGMI_ACK_CONNECT_WORLD, xData, pServerData->nFD);

        //结果
        NFCValueList varResult;
        varResult << nWorldID << nSenderAddress << nLoginID << strAccount << pServerData->pData->server_ip() << pServerData->pData->server_port() << strAccount;
        m_pEventProcessModule->DoEvent(0, NFED_ON_CLIENT_SELECT_SERVER_RESULTS, varResult);
    }
    
    return 0;
}

int NFCWorldNet_ServerModule::OnProxyServerRegisteredProcess(const NFIPacket& msg)
{
    int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        ServerData* pServerData =  mProxyMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mProxyMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "Proxy Registered");

        SynGameToProxy(msg.GetFd());
    }

    return 0;
}

int NFCWorldNet_ServerModule::OnProxyServerUnRegisteredProcess(const NFIPacket& msg)
{
    int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        ServerData* pServerData =  mGameMap.RemoveElement(pData->server_id());
        if (pServerData)
        {
            delete pServerData;
            pServerData = NULL;
        }

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "Proxy UnRegistered");
    }

    return 0;
}

int NFCWorldNet_ServerModule::OnRefreshProxyServerInfoProcess(const NFIPacket& msg)
{
    int64_t nPlayerID = 0;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        ServerData* pServerData =  mProxyMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new ServerData();
            mGameMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "Proxy Registered");

        SynGameToProxy(msg.GetFd());
    }

    return 0;
}

int NFCWorldNet_ServerModule::OnLeaveGameProcess(const NFIPacket& msg)
{

    return 0;
}

int NFCWorldNet_ServerModule::OnRecivePack( const NFIPacket& msg )
{
    switch (msg.GetMsgHead()->GetMsgID())
    {
        case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REFRESH:
            OnRefreshProxyServerInfoProcess(msg);
            break;

        case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED:
            OnProxyServerRegisteredProcess(msg);
            break;

        case NFMsg::EGameMsgID::EGMI_PTWG_PROXY_UNREGISTERED:
            OnProxyServerUnRegisteredProcess(msg);
            break;

        case NFMsg::EGameMsgID::EGMI_GTW_GAME_REGISTERED:
            OnGameServerRegisteredProcess(msg);
            break;

        case NFMsg::EGameMsgID::EGMI_GTW_GAME_UNREGISTERED:
            OnGameServerUnRegisteredProcess(msg);
            break;

        case NFMsg::EGameMsgID::EGMI_GTW_GAME_REFRESH:
            OnRefreshGameServerInfoProcess(msg);
            break;
        default:
            break;
    }
    return 0;
}

int NFCWorldNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }


    return 0;

}

void NFCWorldNet_ServerModule::SynGameToProxy()
{
    NFMsg::ServerInfoReportList xData;

    ServerData* pServerData =  mProxyMap.First();
    while (pServerData)
    {
        SynGameToProxy(pServerData->nFD);

        pServerData = mProxyMap.Next();
    }
}

void NFCWorldNet_ServerModule::SynGameToProxy( const int nFD )
{
    NFMsg::ServerInfoReportList xData;

    ServerData* pServerData =  mGameMap.First();
    while (pServerData)
    {
        NFMsg::ServerInfoReport* pData = xData.add_server_list();
        *pData = *(pServerData->pData);

        pServerData = mGameMap.Next();
    }
    
    SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_NET_INFO, xData, nFD);
}

void NFCWorldNet_ServerModule::OnClientDisconnect( const int nAddress )
{
    //不管是game还是proxy都要找出来,替他反注册
    ServerData* pServerData =  mGameMap.First();
    while (pServerData)
    {
        if (nAddress == pServerData->nFD)
        {
            pServerData->pData->set_server_state(NFMsg::EST_CRASH);
            pServerData->nFD = 0;

            SynGameToProxy();
            return;
        }

        pServerData = mGameMap.Next();
    }

    //////////////////////////////////////////////////////////////////////////

    int nServerID = 0;
    pServerData =  mProxyMap.First();
    while (pServerData)
    {
        if (nAddress == pServerData->nFD)
        {
            nServerID = pServerData->pData->server_id();
            break;
        }

        pServerData = mProxyMap.Next();
    }

    if (nServerID != 0)
    {
        ServerData* pRmServerData = mProxyMap.RemoveElement(nServerID);
        if (pRmServerData)
        {
            delete pRmServerData;
            pRmServerData = NULL;
        }
    }
}

void NFCWorldNet_ServerModule::OnClientConnected( const int nAddress )
{


}

bool NFCWorldNet_ServerModule::InThisWorld( const std::string& strAccount )
{
    return false;
}

