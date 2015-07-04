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
	m_pClusterSQLModule = dynamic_cast<NFIClusterModule*>(pPluginManager->FindModule("NFCMysqlClusterModule"));
    m_pWorldGuildDataModule = dynamic_cast<NFIWorldGuildDataModule*>(pPluginManager->FindModule("NFCWorldGuildDataModule"));

    assert(NULL != m_pEventProcessModule);
    assert(NULL != m_pKernelModule);
    assert(NULL != m_pWorldLogicModule);
    assert(NULL != m_pLogModule);
    assert(NULL != m_pElementInfoModule);
	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pWorldGuildModule);
    assert(NULL != m_pClusterSQLModule);
    assert(NULL != m_pWorldGuildDataModule);

    m_pKernelModule->ResgisterCommonPropertyEvent( this, &NFCWorldNet_ServerModule::OnPropertyCommonEvent );
    m_pKernelModule->ResgisterCommonRecordEvent( this, &NFCWorldNet_ServerModule::OnRecordCommonEvent );

    m_pEventProcessModule->AddEventCallBack(NFIDENTID(), NFED_ON_CLIENT_SELECT_SERVER, this, &NFCWorldNet_ServerModule::OnSelectServerEvent);
    m_pEventProcessModule->AddEventCallBack(NFIDENTID(), NFED_ON_SHOW_STRING, this, &NFCWorldNet_ServerModule::OnShowStringEvent);
    m_pEventProcessModule->AddEventCallBack(NFIDENTID(), NFED_ON_SHOW_RECORD, this, &NFCWorldNet_ServerModule::OnShowRecordEvent);
    m_pEventProcessModule->AddEventCallBack(NFIDENTID(), NFED_ON_SHOW_PROPERTY, this, &NFCWorldNet_ServerModule::OnShowPropertyEvent);

	NF_SHARE_PTR<NFILogicClass> xLogicClass = m_pLogicClassModule->GetElement("Server");
	if (xLogicClass.get())
	{
		NFList<std::string>& xNameList = xLogicClass->GetConfigNameList();
		std::string strConfigName; 
		for (bool bRet = xNameList.First(strConfigName); bRet; bRet = xNameList.Next(strConfigName))
		{
			const int nServerType = m_pElementInfoModule->GetPropertyInt(strConfigName, "Type");
            const int nServerID = m_pElementInfoModule->GetPropertyInt(strConfigName, "ServerID");
            if (nServerType == NF_SERVER_TYPES::NF_ST_WORLD && pPluginManager->AppID() == nServerID)
			{
				const int nPort = m_pElementInfoModule->GetPropertyInt(strConfigName, "Port");
				const int nMaxConnect = m_pElementInfoModule->GetPropertyInt(strConfigName, "MaxOnline");
				const int nCpus = m_pElementInfoModule->GetPropertyInt(strConfigName, "CpuCount");
				const std::string& strName = m_pElementInfoModule->GetPropertyString(strConfigName, "Name");
				const std::string& strIP = m_pElementInfoModule->GetPropertyString(strConfigName, "IP");

				Initialization(NFIMsgHead::NF_Head::NF_HEAD_LENGTH, this, &NFCWorldNet_ServerModule::OnRecivePack, &NFCWorldNet_ServerModule::OnSocketEvent, nMaxConnect, nPort, nCpus);
			}
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
        case NFMsg::EGameMsgID::EGMI_REQ_SEARCH_GUILD:
            OnSearchGuildProcess(msg);
            break;
			//////////////////////////////////////////////////////////////////////////

        case NFMsg::EGameMsgID::EGMI_ACK_ONLINE_NOTIFY:
            OnOnline(msg);
            break;
        case NFMsg::EGameMsgID::EGMI_ACK_OFFLINE_NOTIFY:
            OnOffline(msg);
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

void NFCWorldNet_ServerModule::OnCrateGuildProcess( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS_NO_OBJECT(msg, NFMsg::ReqAckCreateGuild);

    std::string strRoleName ;
    int nLevel = 0;
    int nJob = 0;
    int nDonation= 0;
    int nVIP= 0;

    m_pWorldGuildDataModule->GetPlayerInfo(nPlayerID, strRoleName, nLevel, nJob, nDonation, nVIP);
	NFIDENTID xGuild = m_pWorldGuildModule->CreateGuild(nPlayerID, xMsg.guild_name(), strRoleName, nLevel, nJob, nDonation, nVIP);

    if (!xGuild.IsNull())
    {
        NFMsg::ReqAckCreateGuild xAck;
        *xAck.mutable_guild_id() = NFToPB(xGuild);
        xAck.set_guild_name(xMsg.guild_name());

        SendMsgPB(NFMsg::EGMI_ACK_CREATE_GUILD, xAck, msg.GetFd(), nPlayerID);

        ShowString(nPlayerID, NFMsg::EGEC_CREATE_GUILD_SUCCESS);
    }
    else
    {
        ShowString(nPlayerID, NFMsg::EGEC_UNKOWN_ERROR);
    }
}

void NFCWorldNet_ServerModule::OnJoinGuildProcess( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS_NO_OBJECT(msg, NFMsg::ReqAckJoinGuild)

	if (m_pWorldGuildModule->JoinGuild(nPlayerID, PBToNF(xMsg.guild_id())))
	{
        NFMsg::ReqAckJoinGuild xAck;
        *xAck.mutable_guild_id() = xMsg.guild_id();

        const std::string& strName = m_pKernelModule->GetPropertyString(PBToNF(xMsg.guild_id()), "Name");
        xAck.set_guild_name(strName);

        SendMsgPB(NFMsg::EGMI_ACK_JOIN_GUILD, xAck, msg.GetFd(), nPlayerID);

        SendPropertyToPlayer(PBToNF(xMsg.guild_id()), nPlayerID);
	}
    else
    {
        NFMsg::ReqAckJoinGuild xAck;
        *xAck.mutable_guild_id() = NFToPB(NFIDENTID());
        xAck.set_guild_name("");

        SendMsgPB(NFMsg::EGMI_ACK_JOIN_GUILD, xAck, msg.GetFd(), nPlayerID);
    }
}

void NFCWorldNet_ServerModule::OnLeaveGuildProcess( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS_NO_OBJECT(msg, NFMsg::ReqAckLeaveGuild)

	if (m_pWorldGuildModule->LeaveGuild(nPlayerID, PBToNF(xMsg.guild_id())))
	{

        NFMsg::ReqAckLeaveGuild xAck;
        *xAck.mutable_guild_id() = xMsg.guild_id();

        SendMsgPB(NFMsg::EGMI_ACK_LEAVE_GUILD, xAck, msg.GetFd(), nPlayerID);

        ShowString(nPlayerID, NFMsg::EGEC_LEAVE_GUILD_SUCCESS);
	}
    else
    {
        ShowString(nPlayerID, NFMsg::EGEC_UNKOWN_ERROR);
    }
}

void NFCWorldNet_ServerModule::OnOprGuildMemberProcess( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS_NO_OBJECT(msg, NFMsg::ReqAckOprGuildMember)

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

}


void NFCWorldNet_ServerModule::OnOnline( const NFIPacket& msg )
{
	CLIENT_MSG_PROCESS_NO_OBJECT(msg, NFMsg::RoleOnlineNotify);

     NFIDENTID xGuild;
    xGuild = PBToNF(xMsg.guild());

    NF_SHARE_PTR<NFIObject> pSelfObejct = m_pWorldGuildDataModule->GetGuild(xGuild);
    if (pSelfObejct)
    {
        NFCDataList varSelf;
        varSelf << nPlayerID;

        NFCDataList varObject;
        varObject << xGuild;
        OnObjectListEnter(varSelf, varObject);
    }

    m_pWorldGuildModule->GetGuildBaseInfo(nPlayerID, xGuild);
    m_pWorldGuildModule->GetGuildMemberInfo(nPlayerID, xGuild);

    int nGameID = 0;
    GetGameID(nPlayerID, nGameID);
    m_pWorldGuildModule->MemberOnline(nPlayerID, xGuild, nGameID);
}

void NFCWorldNet_ServerModule::OnOffline( const NFIPacket& msg )
{
    CLIENT_MSG_PROCESS_NO_OBJECT(msg, NFMsg::RoleOfflineNotify);

    NFIDENTID xGuild;
    xGuild = PBToNF(xMsg.guild());

    m_pWorldGuildModule->MemberOffeline(nPlayerID, xGuild);
}

int NFCWorldNet_ServerModule::OnShowStringEvent( const NFIDENTID& object, const int nEventID, const NFIDataList& var )
{
    if (var.GetCount() <= 2 || var.TypeEx(TDATA_INT, TDATA_INT))
    {
        return 1;
    }

    const NFIDENTID nPlayer = var.Object(0);
    const NFINT64 nStringResult = var.Int(1);

    int nGameID = 0;
    if (!GetGameID(nPlayer, nGameID))
    {
        return 1;
    }

    NFMsg::AckEventResult xAck;
    xAck.set_event_code((NFMsg::EGameEventCode)nStringResult);

    SendMsgToGame(nGameID, NFMsg::EGMI_EVENT_RESULT, xAck, nPlayer);
    return 0;
}

bool NFCWorldNet_ServerModule::SendMsgToGame( const int nGameID, const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFIDENTID nPlayer)
{
    NF_SHARE_PTR<ServerData> pData = mGameMap.GetElement(nGameID);
    if (pData.get())
    {
        const int nFD = pData->nFD;
        SendMsgPB(eMsgID, xData, nFD, nPlayer);
    }

    return true;
}

bool NFCWorldNet_ServerModule::GetGameID( const NFIDENTID& self, int& nGameID )
{
    std::vector<std::string> xVecFeild;
    std::vector<std::string> xVecValue;

    xVecFeild.push_back("GameID");
    if (!m_pClusterSQLModule->Query(self.ToString(), xVecFeild, xVecValue))
    {
        return false;
    }

    const std::string& strGameID = xVecValue[0];
    if (!NF_StrTo(strGameID, nGameID))
    {
        return false;
    }
    
    return true;
}

void NFCWorldNet_ServerModule::SendPropertyToPlayer( const NFIDENTID& self, const NFIDENTID& xPlayer )
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (!pObject.get())
    {
        return;
    }

    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
    if (!pPropertyManager.get())
    {
        return;
    }

    std::string strPropertyName;
    
    NFMsg::ObjectPropertyInt xPropertyInt;
    NFMsg::Ident* pIdent = xPropertyInt.mutable_player_id();
    *pIdent = NFToPB(self);

    NFMsg::ObjectPropertyFloat xPropertyFloat;
    pIdent = xPropertyFloat.mutable_player_id();
    *pIdent = NFToPB(self);

    NFMsg::ObjectPropertyFloat xPropertyDouble;
    pIdent = xPropertyDouble.mutable_player_id();
    *pIdent = NFToPB(self);

    NFMsg::ObjectPropertyString xPropertyString;
    pIdent = xPropertyString.mutable_player_id();
    *pIdent = NFToPB(self);

    NFMsg::ObjectPropertyObject xPropertyObject;
    pIdent = xPropertyObject.mutable_player_id();
    *pIdent = NFToPB(self);

    for (NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->First(strPropertyName); pProperty.get() != NULL; pProperty = pPropertyManager->Next(strPropertyName))
    {
        const int nDataType = pProperty->GetType();

        switch (nDataType)
        {
        case TDATA_INT:
            {
                NFMsg::PropertyInt* pDataInt = xPropertyInt.add_property_list();
                pDataInt->set_property_name( strPropertyName );
                pDataInt->set_data( pProperty->GetInt() );
            }
            break;

        case TDATA_FLOAT:
            {
                NFMsg::PropertyFloat* pDataFloat = xPropertyFloat.add_property_list();
                pDataFloat->set_property_name( strPropertyName );
                pDataFloat->set_data( pProperty->GetFloat() );
            }
            break;
        case TDATA_DOUBLE:
            {
                NFMsg::PropertyFloat* pDataFloat = xPropertyDouble.add_property_list();
                pDataFloat->set_property_name( strPropertyName );
                pDataFloat->set_data( pProperty->GetDouble( ) );
            }
            break;
        case TDATA_STRING:
            {
                NFMsg::PropertyString* pDataString = xPropertyString.add_property_list();
                pDataString->set_property_name( strPropertyName );
                pDataString->set_data( pProperty->GetString( ) );
            }
            break;
        case TDATA_OBJECT:
            {
                NFMsg::PropertyObject* pDataObject = xPropertyObject.add_property_list();
                pDataObject->set_property_name( strPropertyName );
                *pDataObject->mutable_data() = NFToPB(pProperty->GetObject( ));
            }
            break;    
        default:
            break;
        }
    }

    int nGameID = 0;
    GetGameID(xPlayer, nGameID);
    if (xPropertyInt.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_INT, xPropertyInt, xPlayer);
    }

    if (xPropertyFloat.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_FLOAT, xPropertyFloat, xPlayer);
    }

    if (xPropertyDouble.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_DOUBLE, xPropertyDouble, xPlayer);
    }

    if (xPropertyString.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_STRING, xPropertyString, xPlayer);
    }

    if (xPropertyObject.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_OBJECT, xPropertyObject, xPlayer);
    }
}

void NFCWorldNet_ServerModule::SendRecordToPlayer( const NFIDENTID& self, const NFIDENTID& xPlayer, const std::string& strRecordName, const int nRow /*= -1*/)
{
    NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
    if (!pObject.get())
    {
        return;
    }

    NF_SHARE_PTR<NFIRecord> pRecord = m_pKernelModule->FindRecord(self, strRecordName);
    if (!pRecord.get())
    {
        return;
    }

    NFMsg::ObjectRecordInt xRecordChangedInt;
    *xRecordChangedInt.mutable_player_id() = NFToPB(self);
    xRecordChangedInt.set_record_name( strRecordName );

    NFMsg::ObjectRecordFloat xRecordChangedFloat;
    *xRecordChangedFloat.mutable_player_id() = NFToPB(self);
    xRecordChangedFloat.set_record_name( strRecordName );

    NFMsg::ObjectRecordFloat xRecordChangedDouble;
    *xRecordChangedDouble.mutable_player_id() = NFToPB(self);
    xRecordChangedDouble.set_record_name( strRecordName );

    NFMsg::ObjectRecordString xRecordChangedString;
    *xRecordChangedString.mutable_player_id() = NFToPB(self);
    xRecordChangedString.set_record_name( strRecordName );

    NFMsg::ObjectRecordObject xRecorChangedObject;
    *xRecorChangedObject.mutable_player_id() = NFToPB(self);
    xRecorChangedObject.set_record_name( strRecordName );

    for (int iRow = 0; iRow < pRecord->GetRows(); iRow++)
    {
        if (!pRecord->IsUsed(iRow))
        {
            continue;
        }

        if ( nRow > 0 && iRow != nRow)
        {
            continue;
        }

        NFCDataList varDataType = pRecord->GetInitData();
        for (int jCol = 0; jCol < pRecord->GetCols(); jCol ++)
        {
            switch (varDataType.Type(jCol))
            {
            case TDATA_INT:
                {
                    NFMsg::RecordInt* recordProperty = xRecordChangedInt.add_property_list();
                    recordProperty->set_row( iRow );
                    recordProperty->set_col( jCol );
                    const int nData = pRecord->GetInt( iRow, jCol);
                    recordProperty->set_data( nData );
                }
                break;
            case TDATA_FLOAT:
                {
                    NFMsg::RecordFloat* recordProperty = xRecordChangedFloat.add_property_list();
                    recordProperty->set_row( iRow );
                    recordProperty->set_col( jCol );
                    const float fData = pRecord->GetFloat( iRow, jCol);
                    recordProperty->set_data( fData );
                }
                break;
            case TDATA_DOUBLE:
                {
                    NFMsg::RecordFloat* recordProperty = xRecordChangedDouble.add_property_list();
                    recordProperty->set_row( iRow );
                    recordProperty->set_col( jCol );
                    const float fData = pRecord->GetDouble( iRow, jCol);
                    recordProperty->set_data( fData );
                }
                break;
            case TDATA_STRING:
                {
                    NFMsg::RecordString* recordProperty = xRecordChangedString.add_property_list();
                    recordProperty->set_row( iRow );
                    recordProperty->set_col( jCol );
                    const std::string& strData = pRecord->GetString( iRow, jCol);
                    recordProperty->set_data( strData );
                }
                break;
            case TDATA_OBJECT:
                {
                    NFMsg::RecordObject* recordProperty = xRecorChangedObject.add_property_list();
                    recordProperty->set_row( iRow );
                    recordProperty->set_col( jCol );
                    const NFIDENTID& xData = pRecord->GetObject( iRow, jCol);
                    *recordProperty->mutable_data() = NFToPB(xData);
                }
                break; 
            default:
                break;
            }
        }
    }

    int nGameID = 0;
    GetGameID(xPlayer, nGameID);
    if (xRecordChangedInt.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_INT, xRecordChangedInt, xPlayer);
    }

    if (xRecordChangedFloat.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_FLOAT, xRecordChangedFloat, xPlayer);
    }

    if (xRecordChangedDouble.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_DOUBLE, xRecordChangedDouble, xPlayer);
    }

    if (xRecordChangedString.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_STRING, xRecordChangedString, xPlayer);
    }

    if (xRecorChangedObject.property_list_size() > 0 )
    {
        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_OBJECT, xRecorChangedObject, xPlayer);
    }
}

int NFCWorldNet_ServerModule::OnShowRecordEvent( const NFIDENTID& object, const int nEventID, const NFIDataList& var )
{
    if (var.GetCount() != 4 || !var.TypeEx(TDATA_OBJECT, TDATA_OBJECT, TDATA_STRING, TDATA_INT, TDATA_UNKNOWN))
    {
        return 1;
    }

    const NFIDENTID& self = var.Object(0);
    const NFIDENTID& xPlayer = var.Object(1);
    const std::string& strRecordName = var.String(2);
    const int nRow = var.Int(3);

    SendRecordToPlayer(self, xPlayer, strRecordName, nRow);

    return 0;
}

int NFCWorldNet_ServerModule::OnShowPropertyEvent( const NFIDENTID& object, const int nEventID, const NFIDataList& var )
{
    if (var.GetCount() != 2 || !var.TypeEx(TDATA_OBJECT, TDATA_OBJECT, TDATA_UNKNOWN))
    {
        return 1;
    }

    const NFIDENTID& self = var.Object(0);
    const NFIDENTID& xPlayer = var.Object(1);

    SendPropertyToPlayer(self, xPlayer);

    return 0;
}

int NFCWorldNet_ServerModule::OnPropertyCommonEvent( const NFIDENTID& self, const std::string& strPropertyName, const NFIDataList& oldVar, const NFIDataList& newVar)
{
    if ( oldVar.GetCount() <= 0 )
    {
        return 0;
    }

    NFCDataList valueBroadCaseList;
    NFCDataList valueBroadCaseGameList;
    int nCount = 0;//argVar.GetCount() ;
    if ( nCount <= 0 )
    {
        if ( "Guild" == m_pKernelModule->GetPropertyString( self, "ClassName" ))
        {
            m_pWorldGuildModule->GetOnlineMember(NFIDENTID(), self, valueBroadCaseList, valueBroadCaseGameList);
        }
    }
    else
    {
        //传入的参数是要广播的对象列表
        //valueBroadCaseList = argVar;
    }

    if ( valueBroadCaseList.GetCount() <= 0 || valueBroadCaseList.GetCount() != valueBroadCaseGameList.GetCount())
    {
        return 0;
    }

    switch ( oldVar.Type( 0 ) )
    {
    case TDATA_INT:
        {
            NFMsg::ObjectPropertyInt xPropertyInt;
            NFMsg::Ident* pIdent = xPropertyInt.mutable_player_id();
            *pIdent = NFToPB(self);

            NFMsg::PropertyInt* pDataInt = xPropertyInt.add_property_list();
            pDataInt->set_property_name( strPropertyName );
            pDataInt->set_data( newVar.Int( 0 ) );

            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                const NFIDENTID& identOld = valueBroadCaseList.Object( i );
                const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_INT, xPropertyInt, identOld);
            }
        }
        break;

    case TDATA_FLOAT:
        {
            NFMsg::ObjectPropertyFloat xPropertyFloat;
            NFMsg::Ident* pIdent = xPropertyFloat.mutable_player_id();
            *pIdent = NFToPB(self);

            NFMsg::PropertyFloat* pDataFloat = xPropertyFloat.add_property_list();
            pDataFloat->set_property_name( strPropertyName );
            pDataFloat->set_data( newVar.Float( 0 ) );

            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOld = valueBroadCaseList.Object( i );
                const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_FLOAT, xPropertyFloat, identOld);
            }
        }
        break;

    case TDATA_DOUBLE:
        {
            NFMsg::ObjectPropertyFloat xPropertyDouble;
            NFMsg::Ident* pIdent = xPropertyDouble.mutable_player_id();
            *pIdent = NFToPB(self);

            NFMsg::PropertyFloat* pDataFloat = xPropertyDouble.add_property_list();
            pDataFloat->set_property_name( strPropertyName );
            pDataFloat->set_data( newVar.Double( 0 ) );

            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOld = valueBroadCaseList.Object( i );
                const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_DOUBLE, xPropertyDouble, identOld);
            }
        }
        break;

    case TDATA_STRING:
        {
            NFMsg::ObjectPropertyString xPropertyString;
            NFMsg::Ident* pIdent = xPropertyString.mutable_player_id();
            *pIdent = NFToPB(self);

            NFMsg::PropertyString* pDataString = xPropertyString.add_property_list();
            pDataString->set_property_name( strPropertyName );
            pDataString->set_data( newVar.String( 0 ) );

            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOld = valueBroadCaseList.Object( i );
                const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_STRING, xPropertyString, identOld);
            }
        }
        break;

    case TDATA_OBJECT:
        {
            NFMsg::ObjectPropertyObject xPropertyObject;
            NFMsg::Ident* pIdent = xPropertyObject.mutable_player_id();
            *pIdent = NFToPB(self);

            NFMsg::PropertyObject* pDataObject = xPropertyObject.add_property_list();
            pDataObject->set_property_name( strPropertyName );
            *pDataObject->mutable_data() = NFToPB(newVar.Object( 0 ));

            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOld = valueBroadCaseList.Object( i );
                const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_PROPERTY_OBJECT, xPropertyObject, identOld);
            }
        }
        break;

    default:
        break;
    }


    return 0;
}

int NFCWorldNet_ServerModule::OnRecordCommonEvent( const NFIDENTID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList& oldVar, const NFIDataList& newVar )
{
    const std::string& strRecordName = xEventData.strRecordName; 
	int nOpType = xEventData.nOpType;
	int nRow = xEventData.nRow;
	int nCol = xEventData.nCol;

    int nObjectContainerID = m_pKernelModule->GetPropertyInt( self, "SceneID" );
    int nObjectGroupID = m_pKernelModule->GetPropertyInt( self, "GroupID" );

    if ( nObjectGroupID < 0 )
    {
        //容器
        return 0;
    }

    if ( "Player" == m_pKernelModule->GetPropertyString( self, "ClassName" ) )
    {
        if (m_pKernelModule->GetPropertyInt(self, "LoadPropertyFinish") <= 0)
        {
            return 0;
        }
    }

    NFCDataList valueBroadCaseList;
    NFCDataList valueBroadCaseGameList;
    if ( "Guild" == m_pKernelModule->GetPropertyString( self, "ClassName" ))
    {
        m_pWorldGuildModule->GetOnlineMember(NFIDENTID(), self, valueBroadCaseList, valueBroadCaseGameList);
    }

    if (valueBroadCaseList.GetCount() <= 0 || valueBroadCaseList.GetCount() != valueBroadCaseGameList.GetCount())
    {
        return 0;
    }

    switch ( nOpType )
    {
    case NFIRecord::RecordOptype::Add:
        {
            NFMsg::ObjectRecordAddRow xAddRecordRow;
            NFMsg::Ident* pIdent = xAddRecordRow.mutable_player_id();
            *pIdent = NFToPB(self);

            xAddRecordRow.set_record_name( strRecordName );

            NFMsg::RecordAddRowStruct* pAddRowData = xAddRecordRow.add_row_data();
            pAddRowData->set_row(nRow);

            //add row 需要完整的row
            for ( int i = 0; i < newVar.GetCount(); i++ )
            {
                switch ( newVar.Type( i ) )
                {
                case TDATA_INT:
                    {
                        //添加的时候数据要全s
                        int nValue = newVar.Int( i );
                        //if ( 0 != nValue )
                        {
                            NFMsg::RecordInt* pAddData = pAddRowData->add_record_int_list();
                            pAddData->set_col( i );
                            pAddData->set_row( nRow );
                            pAddData->set_data( nValue );
                        }
                    }
                    break;
                case TDATA_FLOAT:
                    {
                        float fValue = newVar.Float( i );
                        //if ( fValue > 0.001f  || fValue < -0.001f )
                        {
                            NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
                            pAddData->set_col( i );
                            pAddData->set_row( nRow );
                            pAddData->set_data( fValue );
                        }
                    }
                    break;
                case TDATA_DOUBLE:
                    {
                        float fValue = newVar.Double( i );
                        //if ( fValue > 0.001f  || fValue < -0.001f )
                        {
                            NFMsg::RecordFloat* pAddData = pAddRowData->add_record_float_list();
                            pAddData->set_col( i );
                            pAddData->set_row( nRow );
                            pAddData->set_data( fValue );
                        }
                    }
                    break;
                case TDATA_STRING:
                    {
                        const std::string& str = newVar.String( i );
                        //if (!str.empty())
                        {
                            NFMsg::RecordString* pAddData = pAddRowData->add_record_string_list();
                            pAddData->set_col( i );
                            pAddData->set_row( nRow );
                            pAddData->set_data( str );
                        }
                    }
                    break;
                case TDATA_OBJECT:
                    {
                        NFIDENTID identValue = newVar.Object( i );
                        //if (!identValue.IsNull())
                        {
                            NFMsg::RecordObject* pAddData = pAddRowData->add_record_object_list();
                            pAddData->set_col( i );
                            pAddData->set_row( nRow );

                            *pAddData->mutable_data() = NFToPB(identValue);
                        }
                    }
                    break;
                default:
                    break;
                }
            }


            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOther = valueBroadCaseList.Object( i );
                const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_ADD_ROW, xAddRecordRow, identOther);
            }
        }
        break;
    case NFIRecord::RecordOptype::Del:
        {
            NFMsg::ObjectRecordRemove xReoveRecordRow;

            NFMsg::Ident* pIdent = xReoveRecordRow.mutable_player_id();
            *pIdent = NFToPB(self);

            xReoveRecordRow.set_record_name( strRecordName );
            xReoveRecordRow.add_remove_row( nRow );

            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOther = valueBroadCaseList.Object( i );
                const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_REMOVE_ROW, xReoveRecordRow, identOther);
            }
        }
        break;
    case NFIRecord::RecordOptype::Swap:
        {
            //其实是2个row交换
            NFMsg::ObjectRecordSwap xSwapRecord;
            *xSwapRecord.mutable_player_id() = NFToPB(self);

            xSwapRecord.set_origin_record_name( strRecordName );
            xSwapRecord.set_target_record_name( strRecordName ); // 暂时没用
            xSwapRecord.set_row_origin( nRow );
            xSwapRecord.set_row_target( nCol );

            for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
            {
                NFIDENTID identOther = valueBroadCaseList.Object( i );
                const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                SendMsgToGame(nGameID, NFMsg::EGMI_ACK_SWAP_ROW, xSwapRecord, identOther);
            }
        }
        break;
    case NFIRecord::RecordOptype::UpData:
        {
            switch ( oldVar.Type( 0 ) )
            {
            case TDATA_INT:
                {
                    NFMsg::ObjectRecordInt xRecordChanged;
                    *xRecordChanged.mutable_player_id() = NFToPB(self);

                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordInt* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    int nData = newVar.Int( 0 );
                    recordProperty->set_data( nData );

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_INT, xRecordChanged, identOther);
                    }
                }
                break;

            case TDATA_FLOAT:
                {
                    NFMsg::ObjectRecordFloat xRecordChanged;
                    *xRecordChanged.mutable_player_id() = NFToPB(self);

                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    recordProperty->set_data( newVar.Float( 0 ) );

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_FLOAT, xRecordChanged, identOther);
                    }
                }
            case TDATA_DOUBLE:
                {
                    NFMsg::ObjectRecordFloat xRecordChanged;
                    *xRecordChanged.mutable_player_id() = NFToPB(self);

                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordFloat* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    recordProperty->set_data( newVar.Double( 0 ) );

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_DOUBLE, xRecordChanged, identOther);
                    }
                }
                break;
            case TDATA_STRING:
                {
                    NFMsg::ObjectRecordString xRecordChanged;
                    *xRecordChanged.mutable_player_id() = NFToPB(self);

                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordString* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    recordProperty->set_data( newVar.String( 0 ) );

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_STRING, xRecordChanged, identOther);
                    }
                }
                break;
            case TDATA_OBJECT:
                {
                    NFMsg::ObjectRecordObject xRecordChanged;
                    *xRecordChanged.mutable_player_id() = NFToPB(self);

                    xRecordChanged.set_record_name( strRecordName );
                    NFMsg::RecordObject* recordProperty = xRecordChanged.add_property_list();
                    recordProperty->set_row( nRow );
                    recordProperty->set_col( nCol );
                    *recordProperty->mutable_data() = NFToPB(newVar.Object( 0 ));

                    for ( int i = 0; i < valueBroadCaseList.GetCount(); i++ )
                    {
                        NFIDENTID identOther = valueBroadCaseList.Object( i );
                        const NFINT64  nGameID = valueBroadCaseGameList.Int( i );

                        SendMsgToGame(nGameID, NFMsg::EGMI_ACK_RECORD_OBJECT, xRecordChanged, identOther);
                    }
                }
                break;

            default:
                return 0;
                break;
            }
        }
        break;
    case NFIRecord::RecordOptype::Create:
        return 0;
        break;
    case NFIRecord::RecordOptype::Cleared:
        {
        }
        break;
    default:
        break;
    }

    return 0;
}

bool NFCWorldNet_ServerModule::SendMsgToPlayer( const NFMsg::EGameMsgID eMsgID, google::protobuf::Message& xData, const NFIDENTID nPlayer)
{
    int nGameID = 0;
    GetGameID(nPlayer, nGameID);

    SendMsgToGame(nGameID, eMsgID, xData, nPlayer);

    return true;
}

void NFCWorldNet_ServerModule::OnSearchGuildProcess( const NFIPacket& msg )
{
    CLIENT_MSG_PROCESS_NO_OBJECT(msg, NFMsg::ReqSearchGuild);

    std::vector<NFIWorldGuildDataModule::SearchGuildObject> xList;    
    m_pWorldGuildDataModule->SearchGuild(nPlayerID, xMsg.guild_name(), xList);

    NFMsg::AckSearchGuild xAckMsg;
    for (int i = 0; i < xList.size(); ++i)
    {
        NFMsg::AckSearchGuild::SearchGuildObject* pData = xAckMsg.add_guild_list();
        if (pData)
        {
            const NFIWorldGuildDataModule::SearchGuildObject& xGuild = xList[i];
            *pData->mutable_guild_id() = NFToPB(xGuild.mxGuildID);
            pData->set_guild_name(xGuild.mstrGuildName);
            pData->set_guild_icon(xGuild.mnGuildIcon);

            pData->set_guild_member_count(xGuild.mnGuildMemberCount);
            pData->set_guild_member_max_count(xGuild.mnGuildMemberMaxCount);
            pData->set_guild_honor(xGuild.mnGuildHonor);
            pData->set_guild_rank(xGuild.mnGuildRank);
        }
    }

    SendMsgToPlayer(NFMsg::EGMI_ACK_SEARCH_GUILD, xAckMsg, nPlayerID);
} 

void NFCWorldNet_ServerModule::ShowString( const NFIDENTID& self, const int nResultID )
{
//     NFMsg::AckEventResult xAck;
//     xAck.set_event_code((NFMsg::EGameEventCode)nResultID);
// 
//     int nGameID = 0;
//     GetGameID(self, nGameID);
// 
//     SendMsgToGame(nGameID, NFMsg::EGMI_EVENT_RESULT, xAck, self);
}

int NFCWorldNet_ServerModule::OnObjectListEnter( const NFIDataList& self, const NFIDataList& argVar )
{
    if ( self.GetCount() <= 0 || argVar.GetCount() <= 0)
    {
        return 0;
    }

    NFMsg::AckPlayerEntryList xPlayerEntryInfoList;
    for ( int i = 0; i < argVar.GetCount(); i++ )
    {
        NFIDENTID identOld = argVar.Object( i );
        //排除空对象
        if (identOld.IsNull())
        {
            continue;
        }

        NFMsg::PlayerEntryInfo* pEntryInfo = xPlayerEntryInfoList.add_object_list();
        *(pEntryInfo->mutable_object_guid()) = NFToPB(identOld);

        pEntryInfo->set_x( m_pKernelModule->GetPropertyFloat( identOld, "X" ) );
        pEntryInfo->set_y( m_pKernelModule->GetPropertyFloat( identOld, "Y" ) );
        pEntryInfo->set_z( m_pKernelModule->GetPropertyFloat( identOld, "Z" ) );
        pEntryInfo->set_career_type( m_pKernelModule->GetPropertyInt( identOld, "Job" ) );
        pEntryInfo->set_player_state( m_pKernelModule->GetPropertyInt( identOld, "State" ) );
        pEntryInfo->set_config_id( m_pKernelModule->GetPropertyString( identOld, "ConfigID" ) );
        pEntryInfo->set_scene_id( m_pKernelModule->GetPropertyInt( identOld, "SceneID" ) );
        pEntryInfo->set_class_id( m_pKernelModule->GetPropertyString( identOld, "ClassName" ) );

    }

    if (xPlayerEntryInfoList.object_list_size() <= 0)
    {
        return 0;
    }

    for (int i = 0; i < self.GetCount(); i++)
    {
        NFIDENTID ident = self.Object(i);
        if (ident.IsNull())
        {
            continue;
        }

        //可能在不同的网关呢,得到后者所在的网关FD
        SendMsgToPlayer(NFMsg::EGMI_ACK_OBJECT_ENTRY, xPlayerEntryInfoList, ident);
    }

    return 1;
}


int NFCWorldNet_ServerModule::OnObjectListLeave( const NFIDataList& self, const NFIDataList& argVar )
{
    if ( self.GetCount() <= 0 || argVar.GetCount() <= 0)
    {
        return 0;
    }

    NFMsg::AckPlayerLeaveList xPlayerLeaveInfoList;
    for ( int i = 0; i < argVar.GetCount(); i++ )
    {
        NFIDENTID identOld = argVar.Object( i );
        //排除空对象
        if (identOld.IsNull())
        {
            continue;
        }

        NFMsg::Ident* pIdent = xPlayerLeaveInfoList.add_object_list();
        *pIdent = NFToPB(argVar.Object(i));
    }

    for (int i = 0; i < self.GetCount(); i++)
    {
        NFIDENTID ident = self.Object(i);
        if (ident.IsNull())
        {
            continue;
        }
        //可能在不同的网关呢,得到后者所在的网关FD
        SendMsgToPlayer(NFMsg::EGMI_ACK_OBJECT_ENTRY, xPlayerLeaveInfoList, ident);
    }

    return 1;
}