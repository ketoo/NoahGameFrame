// -------------------------------------------------------------------------
//    @FileName         :    NFCGSTeamModule.cpp
//    @Author               :    chuanbo.Guo
//    @Date                 :    2013-06-11
//    @Module               :    NFCGSTeamModule
//    @Desc                 :
// -------------------------------------------------------------------------

#include "NFCGSTeamModule.h"

bool NFCGSTeamModule::Init()
{
	m_pGameServerToWorldModule = pPluginManager->FindModule<NFIGameServerToWorldModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

    if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_TEAM, this, &NFCGSTeamModule::OnReqCreateTeamFromClient)) { return false; }
    if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_TEAM, this, &NFCGSTeamModule::OnReqJoinTeamFromClient)) { return false; }
    if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_TEAM, this, &NFCGSTeamModule::OnReqLeaveTeamFromClient)) { return false; }
    if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_INVITE_TEAM, this, &NFCGSTeamModule::OnReqInviteTeamFromClient)) { return false; }
    if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_OPRMEMBER_TEAM, this, &NFCGSTeamModule::OnReqOprmemberTeamFromClient)) { return false; }
    if (!m_pGameServerNet_ServerModule->GetNetModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_TEAM_ENTER_ECTYPE, this, &NFCGSTeamModule::OnReqTeamEnterEctypeFromClient)) { return false; }

    if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_ACK_CREATE_TEAM, this, &NFCGSTeamModule::OnAckCreateTeamFromWorldServer)) { return false; }
    if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_ACK_JOIN_TEAM, this, &NFCGSTeamModule::OnAckJoinTeamFromWorldServer)) { return false; }
    if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_ACK_LEAVE_TEAM, this, &NFCGSTeamModule::OnAckLeaveTeamFromWorldServer)) { return false; }
    if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_ACK_OPRMEMBER_TEAM, this, &NFCGSTeamModule::OnAckOprMemberTeamFromWorldServer)) { return false; }
    if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_REQ_TEAM_ENTER_ECTYPE, this, &NFCGSTeamModule::OnReqTeamEnterEctypeFromWorldServer)) { return false; }
    if (!m_pGameServerToWorldModule->GetClusterClientModule()->AddReceiveCallBack(NFMsg::EGMI_ACK_TEAM_ENTER_ECTYPE, this, &NFCGSTeamModule::OnAckTeamEnterEcypeFromWorldServer)) { return false; }

	return true;
}

bool NFCGSTeamModule::Shut()
{
    return true;
}

bool NFCGSTeamModule::Execute()
{
    //Î»ÖÃÄØ
    return true;
}

bool NFCGSTeamModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
    m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
    m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
    m_pLevelModule = pPluginManager->FindModule<NFILevelModule>();
    m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
    m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>();
    m_pGSSwitchServerModule = pPluginManager->FindModule<NFIGSSwichServerModule>();

    return true;
}

void NFCGSTeamModule::OnReqCreateTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckCreateTeam);
    
    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
    if (xTeamID.IsNull())
    {
        xTeamID = m_pUUIDModule->CreateGUID();

        *xMsg.mutable_team_id() = NFINetModule::NFToPB(xTeamID);
    }

    m_pGameServerToWorldModule->GetClusterClientModule()->SendSuitByPB(xTeamID.GetData(), nMsgID, xMsg);
}

void NFCGSTeamModule::OnReqJoinTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckJoinTeam);

    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
    if (xTeamID.IsNull())
    {
        return;
    }

    m_pGameServerToWorldModule->GetClusterClientModule()->SendSuitByPB(xTeamID.GetData(), nMsgID, xMsg);
}

void NFCGSTeamModule::OnReqLeaveTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckLeaveTeam);

    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
    if (xTeamID.IsNull())
    {
        return;
    }

    m_pGameServerToWorldModule->GetClusterClientModule()->SendSuitByPB(xTeamID.GetData(), nMsgID, xMsg);
}

void NFCGSTeamModule::OnReqInviteTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckInviteTeam);

    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
    if (xTeamID.IsNull())
    {
        return;
    }

    m_pGameServerToWorldModule->GetClusterClientModule()->SendSuitByPB(xTeamID.GetData(), nMsgID, xMsg);
}

void NFCGSTeamModule::OnReqOprmemberTeamFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckOprTeamMember);

    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
    if (xTeamID.IsNull())
    {
        return;
    }

    m_pGameServerToWorldModule->GetClusterClientModule()->SendSuitByPB(xTeamID.GetData(), nMsgID, xMsg);
}

void NFCGSTeamModule::OnReqTeamEnterEctypeFromClient(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqTeamEnterEctype);

    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
    if (xTeamID.IsNull())
    {
        return;
    }

    m_pGameServerToWorldModule->GetClusterClientModule()->SendSuitByPB(xTeamID.GetData(), nMsgID, xMsg);
}

void NFCGSTeamModule::OnAckTeamEnterEcypeFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::AckTeamEnterEctype);
    m_pGameServerNet_ServerModule->SendMsgPBToGate(nMsgID, xMsg, nPlayerID);
}

void NFCGSTeamModule::OnAckCreateTeamFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckCreateTeam);

    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
    NF_SHARE_PTR<NFIObject> pSelf = m_pKernelModule->GetObject(nPlayerID);
    if (pSelf && xMsg.has_xteaminfo() && !xTeamID.IsNull())
    {
        const NFMsg::TeamInfo& xInfo = xMsg.xteaminfo();
        for (int i = 0; i < xInfo.teammemberinfo_size(); i++)
        {
            NFGUID xMemberID = NFINetModule::PBToNF(xInfo.teammemberinfo(i).player_id());
            if (nPlayerID == xMemberID)
            {
                pSelf->SetPropertyObject(NFrame::Player::TeamID(), xTeamID);
                break;
            }
        }
    }

    m_pGameServerNet_ServerModule->SendMsgPBToGate(nMsgID, xMsg, nPlayerID);
}

void NFCGSTeamModule::OnAckJoinTeamFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckJoinTeam);

    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
    NF_SHARE_PTR<NFIObject> pSelf = m_pKernelModule->GetObject(nPlayerID);
    if (pSelf && xMsg.has_xteaminfo() && !xTeamID.IsNull())
    {
        const NFMsg::TeamInfo& xInfo = xMsg.xteaminfo();
        for (int i = 0; i < xInfo.teammemberinfo_size(); i++)
        {
            NFGUID xMemberID = NFINetModule::PBToNF(xInfo.teammemberinfo(i).player_id());
            if (nPlayerID == xMemberID)
            {
                pSelf->SetPropertyObject(NFrame::Player::TeamID(), xTeamID);
                break;
            }
        }
    }

    m_pGameServerNet_ServerModule->SendMsgPBToGate(nMsgID, xMsg, nPlayerID);
}

void NFCGSTeamModule::OnAckLeaveTeamFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckLeaveTeam);

    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
    NF_SHARE_PTR<NFIObject> pSelf = m_pKernelModule->GetObject(nPlayerID);
    if (pSelf && xMsg.has_xteaminfo() && !xTeamID.IsNull())
    {
        const NFMsg::TeamInfo& xInfo = xMsg.xteaminfo();
        bool bFindSelf = false;
        for (int i = 0; i < xInfo.teammemberinfo_size(); i++)
        {
            NFGUID xMemberID = NFINetModule::PBToNF(xInfo.teammemberinfo(i).player_id());
            if (nPlayerID == xMemberID)
            {
                bFindSelf = true;
            }
        }

        if (!bFindSelf)
        {
            pSelf->SetPropertyObject(NFrame::Player::TeamID(), NFGUID());
        }
    }

    m_pGameServerNet_ServerModule->SendMsgPBToGate(nMsgID, xMsg, nPlayerID);
}

void NFCGSTeamModule::OnAckOprMemberTeamFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckOprTeamMember);
    m_pGameServerNet_ServerModule->SendMsgPBToGate(nMsgID, xMsg, nPlayerID);
}

void NFCGSTeamModule::OnReqTeamEnterEctypeFromWorldServer(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqTeamEnterEctype);
    if ( nPlayerID != NFINetModule::PBToNF(xMsg.self_id()))
    {
        return;
    }

    NFGUID xTeamID = NFINetModule::PBToNF(xMsg.team_id());
  
    int nServerId = -1;
    int nSceneID = -1;
    int nGroup = -1;

    if (xMsg.has_nectypeid())
    {
        nSceneID = xMsg.nectypeid();
    }

    if (xMsg.has_ngroupid())
    {
        nGroup = xMsg.ngroupid();
    }

    if (xMsg.has_nserverid())
    {
        nServerId = xMsg.nserverid();
    }

    m_pGSSwitchServerModule->ChangeServer(nPlayerID, nSceneID, nSceneID, nGroup);
}
