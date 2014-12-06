// -------------------------------------------------------------------------
//    @FileName      :    NFCProxyServerNet_ClientModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-05-06
//    @Module           :    NFCProxyServerNet_ClientModule
//
// -------------------------------------------------------------------------

//#include "stdafx.h"
#include "NFCProxyServerNet_ClientModule.h"
#include "NFProxyServerNet_ClientPlugin.h"
#include "NFComm/NFCore/NFIHeartBeatManager.h"
#include "NFComm/NFCore/NFCHeartBeatManager.h"

bool NFCProxyServerNet_ClientModule::Init()
{
    mstrConfigIdent = "ProxyServer";
    //mnWantToConnectContainer = -2;
    //mnGameContainerID = -3;
    //是连接world的
    
    return true;
}

bool NFCProxyServerNet_ClientModule::Shut()
{
    //Final();
    //Clear();
    return true;
}

bool NFCProxyServerNet_ClientModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    NFINetModule* pProxy = First();
    while (pProxy)
    {
        pProxy->Execute(fLasFrametime, fStartedTime);

        pProxy = Next();
    }

    KeepAlive(fLasFrametime);

    return m_pNet->Execute(fLasFrametime, fStartedTime);
}

void NFCProxyServerNet_ClientModule::KeepAlive(const float fLasFrametime)
{
    if (mfLastHBTime < 10.0f)
    {
        mfLastHBTime += fLasFrametime;
        return;
    }

    mfLastHBTime = 0.0f;

    NFMsg::ServerHeartBeat xMsg;
    xMsg.set_count(0);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_HEART_BEAT, xMsg, mnSocketFD);
}

int NFCProxyServerNet_ClientModule::OnRecivePack( const NFIPacket& msg )
{
    //这里是worldserver发来的消息
    switch (msg.GetMsgHead()->GetMsgID())
    {
    case NFMsg::EGameMsgID::EGMI_ACK_CONNECT_WORLD:
        OnSelectServerResultProcess(msg);
        break;

    case NFMsg::EGameMsgID::EGMI_STS_NET_INFO:
        OnGameInfoProcess(msg);
        break;

    default:
        break;
    }

    return 0;
}

int NFCProxyServerNet_ClientModule::OnGameInfoProcess( const NFIPacket& msg )
{
    NFIDENTID nPlayerID;	
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    int nSize = xMsg.server_list_size();
    for (int i = 0; i < nSize; ++i)
    {
        const NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);

        GameData* pServerData = mGameDataMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = new GameData();
            pServerData->nGameID = pData->server_id();
            pServerData->strIP = pData->server_ip();
            pServerData->nPort = pData->server_port();
            pServerData->strName = pData->server_name();
            pServerData->eState = pData->server_state();

            mGameDataMap.AddElement(pData->server_id(), pServerData);
        }
        else
        {
            pServerData->strIP = pData->server_ip();
            pServerData->nPort = pData->server_port();
            pServerData->strName = pData->server_name();
            pServerData->eState = pData->server_state();

        }
    }
    //////////////////////////////////////////////////////////////////////////
    GameData* pGameData = mGameDataMap.First();
    while (pGameData)
    {
        NFINetModule* pNetObject = GetElement(pGameData->nGameID);
        if (!pNetObject)
        {
            pNetObject = new NFCProxyConnectObject(pGameData->nGameID, pGameData->strIP, pGameData->nPort, pPluginManager);
            AddElement(pGameData->nGameID, pNetObject);

            m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pGameData->nGameID), pGameData->strIP, pGameData->nPort, "Initialization to connect GameServer");
        }

        pGameData = mGameDataMap.Next();
    }

    return 0;
}

int NFCProxyServerNet_ClientModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        Register();
    }

    return 0;
}

void NFCProxyServerNet_ClientModule::OnClientDisconnect( const int nAddress )
{

}

void NFCProxyServerNet_ClientModule::OnClientConnected( const int nAddress )
{
    Register();
}


void NFCProxyServerNet_ClientModule::Register()
{

    int nProxyID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
    const std::string& strIP = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "IP");
    int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

    NFMsg::ServerInfoReportList xMsg;
    NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

    pData->set_server_id(nProxyID);
    pData->set_server_name("代理服务器1");
    pData->set_server_cur_count(0);
    pData->set_server_ip(strIP);
    pData->set_server_port(nPort);
    pData->set_server_max_online(100000);
    pData->set_server_state(NFMsg::EST_NARMAL);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED, xMsg, mnSocketFD);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Register");
}

void NFCProxyServerNet_ClientModule::UnRegister()
{
    int nProxyID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
    const std::string& strIP = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "IP");
    int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Cpu");
    int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");
    int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");


    NFMsg::ServerInfoReportList xMsg;
    NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

    pData->set_server_id(nProxyID);
    pData->set_server_name("代理服务器1");
    pData->set_server_cur_count(0);
    pData->set_server_ip("");
    pData->set_server_port(0);
    pData->set_server_max_online(100000);
    pData->set_server_state(NFMsg::EST_MAINTEN);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_PTWG_PROXY_UNREGISTERED, xMsg, mnSocketFD);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "UnRegister");
}

bool NFCProxyServerNet_ClientModule::AfterInit()
{

    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pProxyLogicModule = dynamic_cast<NFIProxyLogicModule*>(pPluginManager->FindModule("NFCProxyLogicModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pProxyServerNet_ServerModule = dynamic_cast<NFIProxyServerNet_ServerModule*>(pPluginManager->FindModule("NFCProxyServerNet_ServerModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pProxyLogicModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pProxyServerNet_ServerModule);
    assert(NULL != m_pElementInfoModule);
    assert(NULL != m_pLogModule);

    const int nServerID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
    const std::string& strServerIP = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "ServerIP");
    const std::string& strName = m_pElementInfoModule->GetPropertyString(mstrConfigIdent, "Name");
    const int nServerPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerPort");
    const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
    const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");
    const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");

    m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCProxyServerNet_ClientModule::OnRecivePack, &NFCProxyServerNet_ClientModule::OnSocketEvent);
    mnSocketFD = m_pNet->Initialization(strServerIP.c_str(), nServerPort);
    if (mnSocketFD < 0)
    {
        assert(0);
    }
    return true;
}

int NFCProxyServerNet_ClientModule::Transpond(int nGameServerID, const NFIPacket& msg)
{
    NFINetModule* pProxy = GetElement(nGameServerID);
    if (pProxy)
    {
        pProxy->GetNet()->SendMsg(msg, pProxy->GetFD());
    }

    return 0;
}

int NFCProxyServerNet_ClientModule::OnSelectServerResultProcess(const NFIPacket& msg)
{
    //保持记录,直到下线,或者1分钟不上线即可删除
    NFIDENTID nPlayerID;
    NFMsg::AckConnectWorldResult xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    std::shared_ptr<ConnectData> pConnectData = mWantToConnectMap.GetElement(xMsg.account());
    if (NULL != pConnectData.get())
    {
        pConnectData->strConnectKey = xMsg.world_key();
        return 0;
    }

    pConnectData = std::shared_ptr<ConnectData>(NF_NEW ConnectData());
    pConnectData->strAccount = xMsg.account();
    pConnectData->strConnectKey = xMsg.world_key();
    mWantToConnectMap.AddElement(pConnectData->strAccount, pConnectData);

    return 0;
}

NFIProxyServerNet_ClientModule::GameData* NFCProxyServerNet_ClientModule::GetGameData(int nGameID)
{
    return mGameDataMap.GetElement(nGameID);
}

bool NFCProxyServerNet_ClientModule::VerifyConnectData( const std::string& strAccount, const std::string& strKey )
{
    mWantToConnectMap.RemoveElement(strAccount);

    return true;
}

//////////////////////////////////////////////////////////////////////////

bool NFCProxyConnectObject::Execute(float fFrameTime, float fTotalTime)
{
    KeepAlive(fFrameTime);
    return m_pNet->Execute(fFrameTime, fTotalTime);
}

void NFCProxyConnectObject::KeepAlive(float fLasFrametime)
{
    if (mfLastHBTime < 10.0f)
    {
        mfLastHBTime += fLasFrametime;
        return;
    }

    mfLastHBTime = 0.0f;

    NFMsg::ServerHeartBeat xMsg;
    xMsg.set_count(0);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_STS_HEART_BEAT, xMsg, mnSocketFD);
}

NFCProxyConnectObject::NFCProxyConnectObject(int nGameServerID, const std::string& strIP, const int nPort,  NFIPluginManager* p)
{
    mfLastHBTime = 0.0f;

    mstrConfigIdent = "ProxyServer";
    mnGameServerID = nGameServerID;
    pPluginManager = p;

    m_pProxyServerNet_ServerModule = dynamic_cast<NFIProxyServerNet_ServerModule*>(pPluginManager->FindModule("NFCProxyServerNet_ServerModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pProxyLogicModule = dynamic_cast<NFIProxyLogicModule*>(pPluginManager->FindModule("NFCProxyLogicModule"));
    m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
    m_pElementInfoModule = dynamic_cast<NFIElementInfoModule*>(pPluginManager->FindModule("NFCElementInfoModule"));
    
    if (1 == mnGameServerID)
    {
        //只有1号gs才会受理可延时消息
        //
    }

    m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCProxyConnectObject::OnRecivePack, &NFCProxyConnectObject::OnSocketEvent);
    mnSocketFD = m_pNet->Initialization(strIP.c_str(), nPort);
    if (mnSocketFD < 0)
    {
        assert(0);
    }
}

void NFCProxyConnectObject::OnClientDisconnect( const int nAddress )
{
}

void NFCProxyConnectObject::OnClientConnected( const int nAddress )
{

}

int NFCProxyConnectObject::OnRecivePack( const NFIPacket& msg )
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

int NFCProxyConnectObject::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
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
        Register();
    }

    return 0;
}

void NFCProxyConnectObject::Register()
{
    const int nID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
    const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
    const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");
    const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");

    NFMsg::ServerInfoReportList xMsg;
    NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

    pData->set_server_id(nID);
    pData->set_server_name("代理1");
    pData->set_server_cur_count(0);
    pData->set_server_ip("");
    pData->set_server_port(nPort);
    pData->set_server_max_online(100000);
    pData->set_server_state(NFMsg::EST_NARMAL);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_PTWG_PROXY_REGISTERED, xMsg, mnSocketFD);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Register");
}

void NFCProxyConnectObject::UnRegister()
{
    const int nID = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "ServerID");
    const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "MaxConnect");
    const int nPort = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "Port");
    const int nCpus = m_pElementInfoModule->GetPropertyInt(mstrConfigIdent, "CpuCount");


    NFMsg::ServerInfoReportList xMsg;
    NFMsg::ServerInfoReport* pData = xMsg.add_server_list();

    pData->set_server_id(nID);
    pData->set_server_name("代理1");
    pData->set_server_cur_count(0);
    pData->set_server_ip("");
    pData->set_server_port(0);
    pData->set_server_max_online(100000);
    pData->set_server_state(NFMsg::EST_NARMAL);

    SendMsgPB(NFMsg::EGameMsgID::EGMI_PTWG_PROXY_UNREGISTERED, xMsg, mnSocketFD);

    Execute(0.0f, 0.0f);

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "UnRegister");

}

void NFCProxyConnectObject::OnAckEnterGame(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;
    NFMsg::AckEventResult xData;
    if (!RecivePB(msg, xData, nPlayerID))
    {
        return;
    }

    if (xData.event_code() == NFMsg::EGEC_ENTER_GAME_SUCCESS)
    {
        NFINT64 nFD = xData.event_arg();
        NetObject* pNetObject = m_pProxyServerNet_ServerModule->GetNet()->GetNetObject(nFD);
        if (pNetObject)
        {
            pNetObject->SetUserID(PBToNF(xData.event_object()));
        }
    }
}