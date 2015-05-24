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
	m_pWorldGuildModule = dynamic_cast<NFIWorldGuildModule*>(pPluginManager->FindModule("NFCWorldGuildModule"));
	
    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pWorldLogicModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pWorldGuildModule);

    m_pEventProcessModule->AddEventCallBack(NFIDENTID(), NFED_ON_CLIENT_SELECT_SERVER, this, &NFCWorldNet_ServerModule::OnSelectServerEvent);

	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("WorldServer");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName; 
		if (xNameList.Get(0, strConfigName))
		{
			const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
			const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
			const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
			const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
			const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
			const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

			Initialization(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCWorldNet_ServerModule::OnRecivePack, &NFCWorldNet_ServerModule::OnSocketEvent, nMaxConnect, nPort, nCpus);

		}
	}

    return true;
}

bool NFCWorldNet_ServerModule::Shut()
{

    return true;
}

bool NFCWorldNet_ServerModule::Execute(const float fLasFrametime, const float fStartedTime)
{
	LogGameServer(fLasFrametime);

	return NFINetModule::Execute(fLasFrametime, fStartedTime);
}

int NFCWorldNet_ServerModule::OnGameServerRegisteredProcess(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mGameMap.GetElement(pData->server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mGameMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

         m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "GameServerRegistered");
    }

    SynGameToProxy();

    return 0;
}

int NFCWorldNet_ServerModule::OnGameServerUnRegisteredProcess(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        mGameMap.RemoveElement(pData->server_id());

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "GameServerRegistered");
    }
    return 0;
}

int NFCWorldNet_ServerModule::OnRefreshGameServerInfoProcess(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mGameMap.GetElement(pData->server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mGameMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "GameServerRegistered");
    }

    SynGameToProxy();

    return 0;
}

int NFCWorldNet_ServerModule::OnSelectServerEvent(const NFIDENTID& object, const int nEventID, const NFIDataList& var)
{
    if (4 != var.GetCount()
        || !var.TypeEx(TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_OBJECT, TDATA_TYPE::TDATA_INT, TDATA_TYPE::TDATA_STRING, TDATA_TYPE::TDATA_UNKNOWN))
    {
        return 0;
    }

    const int nWorldID = var.Int(0);
    const NFIDENTID xClientIdent = var.Object(1);
    const int nLoginID = var.Int(2);
    const std::string& strAccount = var.String(3);

    //////////////////////////////////////////////////////////////////////////
    if (InThisWorld(strAccount))
    {
        return 0;
    }

    NF_SHARE_PTR<ServerData> pServerData = mProxyMap.First();
    if (pServerData.get())
    {
        NFMsg::AckConnectWorldResult xData;

        xData.set_world_id(nWorldID);
        xData.mutable_sender()->CopyFrom(NFToPB(xClientIdent));
        xData.set_login_id(nLoginID);
        xData.set_account(strAccount);

        xData.set_world_ip(pServerData->pData->server_ip());
        xData.set_world_port(pServerData->pData->server_port());
        xData.set_world_key(strAccount);

        SendMsgPB(NFMsg::EGMI_ACK_CONNECT_WORLD, xData, pServerData->nFD);

        //结果
        NFCDataList varResult;
        varResult << nWorldID << xClientIdent << nLoginID << strAccount << pServerData->pData->server_ip() << pServerData->pData->server_port() << strAccount;
        m_pEventProcessModule->DoEvent(NFIDENTID(), NFED_ON_CLIENT_SELECT_SERVER_RESULTS, varResult);
    }
    
    return 0;
}

int NFCWorldNet_ServerModule::OnProxyServerRegisteredProcess(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.GetElement(pData->server_id());
        if (!pServerData)
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mProxyMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Proxy Registered");

        SynGameToProxy(msg.GetFd());
    }

    return 0;
}

int NFCWorldNet_ServerModule::OnProxyServerUnRegisteredProcess(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        mGameMap.RemoveElement(pData->server_id());

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Proxy UnRegistered");
    }

    return 0;
}

int NFCWorldNet_ServerModule::OnRefreshProxyServerInfoProcess(const NFIPacket& msg)
{
    NFIDENTID nPlayerID;
    NFMsg::ServerInfoReportList xMsg;
    if (!RecivePB(msg, xMsg, nPlayerID))
    {
        return 0;
    }

    for (int i = 0; i < xMsg.server_list_size(); ++i)
    {
        NFMsg::ServerInfoReport* pData = xMsg.mutable_server_list(i);
        NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.GetElement(pData->server_id());
        if (!pServerData.get())
        {
            pServerData = NF_SHARE_PTR<ServerData>(NF_NEW ServerData());
            mGameMap.AddElement(pData->server_id(), pServerData);
        }

        pServerData->nFD = msg.GetFd();
        *(pServerData->pData) = *pData;

        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, pData->server_id()), pData->server_name(), "Proxy Registered");

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
			///////////GUILD///////////////////////////////////////////////////////////////
		case NFMsg::EGameMsgID::EGMI_REQ_CREATE_GUILD:
			OnCrateGuildProcess(msg);
			break;
		case NFMsg::EGameMsgID::EGMI_REQ_JOIN_GUILD:
			OnJoinGuildProcess(msg);
			break;
		case NFMsg::EGameMsgID::EGMI_REQ_LEAVE_GUILD:
			OnLeaveGuildProcess(msg);
			break;
		case NFMsg::EGameMsgID::EGMI_REQ_OPR_GUILD:
			OnOprGuildMemberProcess(msg);
			break;
			//////////////////////////////////////////////////////////////////////////
        default:
            break;
    }
    return 0;
}

int NFCWorldNet_ServerModule::OnSocketEvent( const int nSockIndex, const NF_NET_EVENT eEvent, NFINet* pNet )
{
    if (eEvent & NF_NET_EVENT_EOF) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_EOF", "Connection closed", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    } 
    else if (eEvent & NF_NET_EVENT_ERROR) 
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_ERROR", "Got an error on the connection", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else if (eEvent & NF_NET_EVENT_TIMEOUT)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_TIMEOUT", "read timeout", __FUNCTION__, __LINE__);
        OnClientDisconnect(nSockIndex);
    }
    else  if (eEvent == NF_NET_EVENT_CONNECTED)
    {
        m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(0, nSockIndex), "NF_NET_EVENT_CONNECTED", "connectioned success", __FUNCTION__, __LINE__);
        OnClientConnected(nSockIndex);
    }


    return 0;

}

void NFCWorldNet_ServerModule::SynGameToProxy()
{
    NFMsg::ServerInfoReportList xData;

    NF_SHARE_PTR<ServerData> pServerData =  mProxyMap.First();
    while (pServerData.get())
    {
        SynGameToProxy(pServerData->nFD);

        pServerData = mProxyMap.Next();
    }
}

void NFCWorldNet_ServerModule::SynGameToProxy( const int nFD )
{
    NFMsg::ServerInfoReportList xData;

    NF_SHARE_PTR<ServerData> pServerData =  mGameMap.First();
    while (pServerData.get())
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
    NF_SHARE_PTR<ServerData> pServerData =  mGameMap.First();
    while (pServerData.get())
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

    mProxyMap.RemoveElement(nServerID);
}

void NFCWorldNet_ServerModule::OnClientConnected( const int nAddress )
{


}

bool NFCWorldNet_ServerModule::InThisWorld( const std::string& strAccount )
{
    return false;
}

void NFCWorldNet_ServerModule::LogGameServer(const float fLastTime)
{
	if (mfLastLogTime < 10.0f)
	{
		mfLastLogTime += fLastTime;
		return;
	}

	mfLastLogTime = 0.0f;

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(), "Begin Log GameServer Info", "");

	NF_SHARE_PTR<ServerData> pGameData = mGameMap.First();
	while (pGameData)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(), "GameServer:", pGameData->pData->server_state());

		pGameData = mGameMap.Next();
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(), "End Log GameServer Info", "");

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(), "Begin Log ProxyServer Info", "");

	pGameData = mProxyMap.First();
	while (pGameData)
	{
		m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(), "ProxyServer:", pGameData->pData->server_state());

		pGameData = mProxyMap.Next();
	}

	m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, NFIDENTID(), "End Log ProxyServer Info", "");




}

int NFCWorldNet_ServerModule::OnCrateGuildProcess( const NFIPacket& msg )
{
	NFMsg::ReqCreateGuild xMsg;
	CLIENT_MSG_PROCESS(msg, xMsg)

	m_pWorldGuildModule->CreateGuild(nPlayerID, xMsg.guild_name());
	
	return 0;
}

int NFCWorldNet_ServerModule::OnJoinGuildProcess( const NFIPacket& msg )
{
	NFMsg::ReqJoinGuild xMsg;
	CLIENT_MSG_PROCESS(msg, xMsg)

	m_pWorldGuildModule->JoinGuild(nPlayerID, PBToNF(xMsg.guild_id()));

	return 0;
}

int NFCWorldNet_ServerModule::OnLeaveGuildProcess( const NFIPacket& msg )
{
	NFMsg::ReqAckLeaveGuild xMsg;
	CLIENT_MSG_PROCESS(msg, xMsg)

	m_pWorldGuildModule->LeaveGuild(nPlayerID, PBToNF(xMsg.guild_id()));

	return 0;
}

int NFCWorldNet_ServerModule::OnOprGuildMemberProcess( const NFIPacket& msg )
{
	NFMsg::ReqAckOprGuildMember xMsg;
	CLIENT_MSG_PROCESS(msg, xMsg)

	NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType eOprType = xMsg.type();
	switch (eOprType)
	{
	case NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType::ReqAckOprGuildMember_EGGuildMemberOprType_EGAT_UP:
		m_pWorldGuildModule->UpGuildMmember(nPlayerID, PBToNF(xMsg.guild_id()), PBToNF(xMsg.member_id()));
		break;
	case NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType::ReqAckOprGuildMember_EGGuildMemberOprType_EGAT_DOWN:
		m_pWorldGuildModule->DownGuildMmember(nPlayerID, PBToNF(xMsg.guild_id()), PBToNF(xMsg.member_id()));
		break;
	case NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType::ReqAckOprGuildMember_EGGuildMemberOprType_EGAT_KICK:
		m_pWorldGuildModule->KickGuildMmember(nPlayerID, PBToNF(xMsg.guild_id()), PBToNF(xMsg.member_id()));
		break;
	default:
		break;
	}

	return 0;
}
