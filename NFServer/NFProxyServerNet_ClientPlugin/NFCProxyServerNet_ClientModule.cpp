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

    return m_pNet->Execute(fLasFrametime, fStartedTime);
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
    int64_t nPlayerID = 0;	
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
        if (NULL != pGameData)
        {
            NFINetModule* pNetObject = GetElement(pGameData->nGameID);
            if (!pNetObject)
            {
                pNetObject = new NFCProxyConnectObject(pGameData->nGameID, pPluginManager);
                AddElement(pGameData->nGameID, pNetObject);

                pNetObject->GetNet()->Initialization(pGameData->strIP.c_str(), pGameData->nPort);

                m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pGameData->nGameID, pGameData->strIP, pGameData->nPort, "Initialization to connect GameServer");
            }
        }

        pGameData = mGameDataMap.Next();
    }

    return 0;
}

int NFCProxyServerNet_ClientModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, nSockIndex, "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
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

	int nProxyID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strIP = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "IP");
	int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

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

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "Register");
}

void NFCProxyServerNet_ClientModule::UnRegister()
{
	int nProxyID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strIP = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "IP");
	int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Cpu");
	int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");
	int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");


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

    m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, pData->server_id(), pData->server_name(), "UnRegister");
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

	const int nServerID = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerID");
	const std::string& strServerIP = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "ServerIP");
	const std::string& strName = m_pElementInfoModule->QueryPropertyString(mstrConfigIdent, "Name");
	const int nServerPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "ServerPort");
	const int nMaxConnect = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "MaxConnect");
	const int nCpus = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "CpuCount");
	const int nPort = m_pElementInfoModule->QueryPropertyInt(mstrConfigIdent, "Port");

	m_pNet = new NFCNet(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCProxyServerNet_ClientModule::OnRecivePack, &NFCProxyServerNet_ClientModule::OnSocketEvent);
	int nRet = m_pNet->Initialization(strServerIP.c_str(), nServerPort);
	if (nRet < 0)
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
        pProxy->GetNet()->SendMsg(msg);
    }

    return 0;
}

int NFCProxyServerNet_ClientModule::OnSelectServerResultProcess(const NFIPacket& msg)
{
    //保持记录,直到下线,或者1分钟不上线即可删除
    int64_t nPlayerID = 0;
    NFMsg::AckConnectWorldResult xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    //// 保存起来，等待客户端连接
    //NFCValueList varObjectList;
    ////帐号相等的只能有一个
    //int nCount = m_pKernelModule->GetObjectByProperty(mnWantToConnectContainer, "Account", NFCValueList() << xMsg.account(), varObjectList);
    //if (1 == nCount)
    //{
    //    m_pKernelModule->SetPropertyString(varObjectList.ObjectVal(0), "ConnectKey", xMsg.world_key());
    //    return 0;
    //}

    //NFCValueList arg;
    //arg << "Account" << xMsg.account();
    //arg << "ConnectKey" << xMsg.world_key();
    //m_pKernelModule->CreateObject(0, mnWantToConnectContainer, 0, "WantToConnect", "", arg);

    ConnectData* pConnectData = mWantToConnectMap.GetElement(xMsg.account());
    if (NULL != pConnectData)
    {
        pConnectData->strConnectKey = xMsg.world_key();
        return 0;
    }

    ConnectData xConnectData;
    xConnectData.strAccount = xMsg.account();
    xConnectData.strConnectKey = xMsg.world_key();
    mWantToConnectMap.AddElement(xConnectData.strAccount, &xConnectData);

    return 0;
}

NFIProxyServerNet_ClientModule::ConnectData* NFCProxyServerNet_ClientModule::GetConnectData(const std::string& strAccount)
{
    return mWantToConnectMap.GetElement(strAccount);
}

NFIProxyServerNet_ClientModule::GameData* NFCProxyServerNet_ClientModule::GetGameData(int nGameID)
{
    return mGameDataMap.GetElement(nGameID);
}

//////////////////////////////////////////////////////////////////////////

bool NFCProxyConnectObject::Execute(float fFrameTime, float fTotalTime)
{
    return m_pNet->Execute(fFrameTime, fTotalTime);
}

NFCProxyConnectObject::NFCProxyConnectObject(int nGameServerID, NFIPluginManager* p)
{
    mnGameServerID = nGameServerID;
    pPluginManager = p;

    m_pProxyServerNet_ServerModule = dynamic_cast<NFIProxyServerNet_ServerModule*>(pPluginManager->FindModule("NFCProxyServerNet_ServerModule"));
    m_pEventProcessModule = dynamic_cast<NFIEventProcessModule*>(pPluginManager->FindModule("NFCEventProcessModule"));
    m_pKernelModule = dynamic_cast<NFIKernelModule*>(pPluginManager->FindModule("NFCKernelModule"));
    m_pProxyLogicModule = dynamic_cast<NFIProxyLogicModule*>(pPluginManager->FindModule("NFCProxyLogicModule"));
    if (1 == mnGameServerID)
    {
        //只有1号gs才会受理可延时消息
        //
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
    m_pProxyServerNet_ServerModule->Transpond(msg);

    return 0;
}

int NFCProxyConnectObject::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent )
{
    return 0;
}