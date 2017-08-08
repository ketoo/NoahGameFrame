// -------------------------------------------------------------------------
//    @FileName      :    NFCTeamModule.cpp
//    @Author           :    chuanbo.guo
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCTeamModule
//
// -------------------------------------------------------------------------

#include "NFCTeamModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCTeamModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	
	return true;
}

bool NFCTeamModule::Shut()
{
    return true;
}

bool NFCTeamModule::Execute()
{
    return true;
}

bool NFCTeamModule::AfterInit()
{

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_TEAM, this, &NFCTeamModule::OnCreateTeamProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_TEAM, this, &NFCTeamModule::OnJoinTeamProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_TEAM, this, &NFCTeamModule::OnLeaveTeamProcess)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_TEAM_ENTER_ECTYPE, this, &NFCTeamModule::OnTeamEnterEctypeProcess)) { return false; }

    return true;
}

NFGUID NFCTeamModule::CreateTeam( const NFGUID& self, const NFGUID& xDefaultTeamID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP)
{
    if (strName.empty())
    {
        return NULL_OBJECT;
    }

    NFGUID xTeam = xDefaultTeamID;
    if (xTeam.IsNull())
    {
        xTeam = m_pKernelModule->CreateGUID();
    }

    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pCommonRedisModule->NewPropertyManager(NFrame::Team::ThisName());
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->NewRecordManager(NFrame::Team::ThisName());

    if (!pPropertyManager)
    {
        return NFGUID();
    }

    if (!pRecordManager)
    {
        return NFGUID();
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = pRecordManager->GetElement(NFrame::Team::MemberList::ThisName());
    if (!pMemberRecord.get())
    {
        return NFGUID();
    }

    NFDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList::GUID, self, varList);
    if (varList.GetCount() > 0)
    {
        return NFGUID();
    }

    NFDataList varData;

    int nReceive = 0;
    int nOnLine = 0;

    int nTitle = 0;
    int nOnlineGameID = 0;

    GetPlayerGameID(self, nOnlineGameID);

    varData << self << strName << (NFINT64)nLevel << (NFINT64)nJob << (NFINT64)nDonation << (NFINT64)nReceive << (NFINT64)nVIP << (NFINT64)nOnLine << nTitle << nOnlineGameID;

    if (pMemberRecord->AddRow(-1, varData) < 0)
    {
        return NFGUID();
    }

    if (!pPropertyManager->SetPropertyObject(NFrame::Team::Captain(), self))
    {
        return NFGUID();
    }

    if (!m_pCommonRedisModule->SaveCachePropertyInfo(xTeam, pPropertyManager))
    {
        return NFGUID();
    }

    if (!m_pCommonRedisModule->SaveCacheRecordInfo(xTeam, pRecordManager))
    {
        return NFGUID();
    }

    return xTeam;
}

bool NFCTeamModule::JoinTeam( const NFGUID& self, const NFGUID& xTeamID )
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->GetCacheRecordInfo(xTeamID, NFrame::Team::ThisName());
    if (!pRecordManager)
    {
        return false;
    }
    NF_SHARE_PTR<NFIRecord> pMemberRecord = pRecordManager->GetElement(NFrame::Team::MemberList::ThisName());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList::GUID, self, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }

    NFDataList varData;

    std::string strName ;
    int nLevel = 0;
    int nJob = 0;
    int nDonation = 0;
    int nReceive = 0;
    int nVIP = 0;
    int nOnLine = 0;
    
    int nTitle = 0;
    int nOnlineGameID = 0;

    GetPlayerGameID(self, nOnlineGameID);

    varData << self << strName << (NFINT64)nLevel << (NFINT64)nJob << (NFINT64)nDonation << (NFINT64)nReceive << (NFINT64)nVIP << (NFINT64)nOnLine << nTitle << nOnlineGameID ;

    if (pMemberRecord->AddRow(-1, varData) < 0)
    {
        return false;
    }

    return m_pCommonRedisModule->SaveCacheRecordInfo(xTeamID, pRecordManager);
}

bool NFCTeamModule::LeaveTeam( const NFGUID& self, const NFGUID& xTeamID )
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->GetCacheRecordInfo(xTeamID, NFrame::Team::ThisName());
    if (!pRecordManager)
    {
        return false;
    }
    NF_SHARE_PTR<NFIRecord> pMemberRecord = pRecordManager->GetElement(NFrame::Team::MemberList::ThisName());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList::GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int32(0);
    
    if (!pMemberRecord->Remove(nRow))
    {
        return false;
    }

    return m_pCommonRedisModule->SaveCacheRecordInfo(xTeamID, pRecordManager);
}

bool NFCTeamModule::KickTeamMmember( const NFGUID& self, const NFGUID& xTeamID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->GetCacheRecordInfo(xTeamID, NFrame::Team::ThisName());
    if (!pRecordManager)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = pRecordManager->GetElement(NFrame::Team::MemberList::ThisName());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pCommonRedisModule->GetCachePropertyInfo(xTeamID, NFrame::Team::ThisName());
    if (!pPropertyManager)
    {
        return false;
    }

    if (self == xMmember)
    {
        return false;
    }

	const NFGUID xID = pPropertyManager->GetPropertyObject(NFrame::Team::Captain());
	if (self != xID)
	{
		return false;
	}

    NFDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList::GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int32(0);
    if (!pMemberRecord->Remove(nRow))
    {
        return false;
    }
    
    return m_pCommonRedisModule->SaveCacheRecordInfo(xTeamID, pRecordManager);
}


bool NFCTeamModule::GetPlayerGameID(const NFGUID& self, int& nGameID)
{
    std::vector<std::string> xVecFeild;
    std::vector<std::string> xVecValue;

    xVecFeild.push_back("GameID");

	return true;
}


bool NFCTeamModule::GetMemberList(const NFGUID& self, const NFGUID& xTeam, std::vector<NFGUID>& xmemberList)
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->GetCacheRecordInfo(xTeam, NFrame::Team::ThisName());
    if (!pRecordManager)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = pRecordManager->GetElement(NFrame::Team::MemberList::ThisName());
    if (!pMemberRecord.get())
    {
        return false;
    }

    for (int i = 0; i < pMemberRecord->GetRows(); i++)
    {
        if (!pMemberRecord->IsUsed(i))
        {
            continue;
        }

        const NFINT64 nOnline = pMemberRecord->GetInt(i, NFrame::Team::MemberList::Online);
        const NFINT64 nGameID = pMemberRecord->GetInt(i, NFrame::Team::MemberList::GameID);
        const NFGUID& xID = pMemberRecord->GetObject(i, NFrame::Team::MemberList::GUID);
        if (!xID.IsNull())
        {

            xmemberList.push_back(xID);
        }
    }

    return true;
}

bool NFCTeamModule::MemberOnline( const NFGUID& self, const NFGUID& xTeam , const int& nGameID)
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->GetCacheRecordInfo(xTeam, NFrame::Team::ThisName());
    if (!pRecordManager)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = pRecordManager->GetElement(NFrame::Team::MemberList::ThisName());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList::GUID, self, varList);
    if (varList.GetCount() <=  0)
    {
        return false;
    }

    const int nRow = varList.Int32(0);
    pMemberRecord->SetInt(nRow, NFrame::Team::MemberList::GameID, nGameID);
    
    pMemberRecord->SetInt(nRow, NFrame::Team::MemberList::Online, 1);

    return m_pCommonRedisModule->SaveCacheRecordInfo(xTeam, pRecordManager);
}

bool NFCTeamModule::MemberOffline( const NFGUID& self, const NFGUID& xTeam )
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->GetCacheRecordInfo(xTeam, NFrame::Team::ThisName());
    if (!pRecordManager)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = pRecordManager->GetElement(NFrame::Team::MemberList::ThisName());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList::GUID, self, varList);
    if (varList.GetCount() <= 0)
    {
        return false;
    }

    const int nRow = varList.Int32(0);
    pMemberRecord->SetInt(nRow, NFrame::Team::MemberList::Online, 0);
    pMemberRecord->SetInt(nRow, NFrame::Team::MemberList::GameID, 0);
    return m_pCommonRedisModule->SaveCacheRecordInfo(xTeam, pRecordManager);
}

bool NFCTeamModule::BroadcastMsgToTeam(const NFGUID& self, const NFGUID& xTeam, const uint16_t nMsgID, google::protobuf::Message& xData)
{
    std::vector<std::string> xPlayerList;
    std::vector<int> xGameIDList;

    std::vector<NFGUID> xPlayerIDList;
    if (!GetMemberList(self, xTeam, xPlayerIDList))
    {
        return false;
    }

    for (int i = 0; i < xPlayerIDList.size(); i++)
    {
        xPlayerList.push_back(xPlayerIDList[i].ToString());
    }

    for (int i = 0; i < xGameIDList.size() && i < xPlayerList.size(); i++)
    {
        int nGameID = xGameIDList[i];
        NFGUID xPlayer;
        xPlayer.FromString(xPlayerList[i]);
        m_pWorldNet_ServerModule->SendMsgToGame(nGameID, (NFMsg::EGameMsgID)nMsgID, xData, xPlayer);
    }

    return true;
}

void NFCTeamModule::OnCreateTeamProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckCreateTeam);

	std::string strRoleName ;
	int nLevel = 0;
	int nJob = 0;
	int nDonation= 0;
	int nVIP= 0;

	NFGUID xTeam = CreateTeam(nPlayerID, NFINetModule::PBToNF(xMsg.team_id()), strRoleName, strRoleName, nLevel, nJob, nDonation, nVIP);

	if (!xTeam.IsNull())
	{
		NFMsg::ReqAckCreateTeam xAck;
		*xAck.mutable_team_id() = NFINetModule::NFToPB(xTeam);

        NFMsg::TeamInfo xTeamInfo;
        if (!GetTeamInfo(nPlayerID, xTeam, xTeamInfo))
        {
            xAck.mutable_xteaminfo()->CopyFrom(xTeamInfo);
            m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_CREATE_TEAM, xAck, nSockIndex, nPlayerID);
            return;
        }
    }

    NFMsg::ReqAckCreateTeam xAck;
    *xAck.mutable_team_id() = NFINetModule::NFToPB(xTeam);

    m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_CREATE_TEAM, xAck, nSockIndex, nPlayerID);
}

void NFCTeamModule::OnJoinTeamProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckJoinTeam);

    if (JoinTeam(nPlayerID, NFINetModule::PBToNF(xMsg.team_id())))
    {
        NFMsg::ReqAckJoinTeam xAck;
        *xAck.mutable_team_id() = xMsg.team_id();

        NFGUID xTeam = NFINetModule::PBToNF(xMsg.team_id());

        NFMsg::TeamInfo xTeamInfo;
        if (!GetTeamInfo(nPlayerID, xTeam, xTeamInfo))
        {
            xAck.mutable_xteaminfo()->CopyFrom(xTeamInfo);
        }

        BroadcastMsgToTeam(nPlayerID, xTeam, NFMsg::EGMI_ACK_JOIN_TEAM, xAck);

    }
    else
    {
        NFMsg::ReqAckJoinTeam xAck;
        *xAck.mutable_team_id() = NFINetModule::NFToPB(NFGUID());

        m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_JOIN_TEAM, xAck, nSockIndex, nPlayerID);
    }
}

void NFCTeamModule::OnLeaveTeamProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckLeaveTeam);

    if (LeaveTeam(nPlayerID, NFINetModule::PBToNF(xMsg.team_id())))
    {

        NFMsg::ReqAckLeaveTeam xAck;
        *xAck.mutable_team_id() = xMsg.team_id();

        NFGUID xTeam = NFINetModule::PBToNF(xMsg.team_id());

        NFMsg::TeamInfo xTeamInfo;
        if (!GetTeamInfo(nPlayerID, xTeam, xTeamInfo))
        {
            xAck.mutable_xteaminfo()->CopyFrom(xTeamInfo);
        }

        m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_LEAVE_TEAM, xAck, nSockIndex, nPlayerID);

        BroadcastMsgToTeam(nPlayerID, xTeam, NFMsg::EGMI_ACK_LEAVE_TEAM, xAck);
    }
    else
    {
        NFMsg::ReqAckLeaveTeam xAck;
        *xAck.mutable_team_id() = NFINetModule::NFToPB(NFGUID());

        m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_LEAVE_TEAM, xAck, nSockIndex, nPlayerID);
    }
}

void NFCTeamModule::OnOprTeamMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckOprTeamMember);

    NFMsg::ReqAckOprTeamMember::EGTeamMemberOprType eOprType = xMsg.type();
    switch (eOprType)
    {
    case NFMsg::ReqAckOprTeamMember::EGAT_KICKOUT:
        KickTeamMmember(nPlayerID, NFINetModule::PBToNF(xMsg.team_id()), NFINetModule::PBToNF(xMsg.member_id()));
        break;
    default:
        break;
    }

    NFGUID xTeam = NFINetModule::PBToNF(xMsg.team_id());
    NFMsg::TeamInfo xTeamInfo;
    if (!GetTeamInfo(nPlayerID, xTeam, xTeamInfo))
    {
        xMsg.mutable_xteaminfo()->CopyFrom(xTeamInfo);
    }

    BroadcastMsgToTeam(nPlayerID, xTeam, NFMsg::EGMI_ACK_OPRMEMBER_TEAM, xMsg);
}

void NFCTeamModule::OnTeamEnterEctypeProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqTeamEnterEctype);

    NFGUID xTeam = NFINetModule::PBToNF(xMsg.team_id());
    NFGUID xSelfID = NFINetModule::PBToNF(xMsg.self_id());
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pCommonRedisModule->GetCachePropertyInfo(xTeam, NFrame::Team::ThisName());
    if (pPropertyManager)
    {
        NFGUID xCaptain = pPropertyManager->GetPropertyObject(NFrame::Team::Captain());
        if (xCaptain == xSelfID)
        {
            BroadcastMsgToTeam(nPlayerID, xTeam, NFMsg::EGMI_REQ_TEAM_ENTER_ECTYPE, xMsg);
            return;
        }
    }

	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_TEAM_ENTER_ECTYPE, xMsg, nSockIndex, nPlayerID);
}

bool NFCTeamModule::GetTeamInfo(const NFGUID& self, const NFGUID& xTeam, NFMsg::TeamInfo& xTeamInfo)
{
    if (xTeam.IsNull())
    {
        return false;
    }

    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = m_pCommonRedisModule->GetCachePropertyInfo(xTeam, NFrame::Team::ThisName());
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = m_pCommonRedisModule->GetCacheRecordInfo(xTeam, NFrame::Team::ThisName());

    if (!pPropertyManager)
    {
        return false;
    }

    if (!pRecordManager)
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = pRecordManager->GetElement(NFrame::Team::MemberList::ThisName());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFGUID xCaptain = pPropertyManager->GetPropertyObject(NFrame::Team::Captain());
    if (!xCaptain.IsNull())
    {
        return false;
    }

    *xTeamInfo.mutable_team_id() = NFINetModule::NFToPB(xTeam);
    *xTeamInfo.mutable_captain_id() = NFINetModule::NFToPB(xCaptain);

    for (int i = 0; i < pMemberRecord->GetRows(); i++)
    {
        if (!pMemberRecord->IsUsed(i))
        {
            continue;
        }

        NFMsg::TeammemberInfo* pMemberinfo = xTeamInfo.add_teammemberinfo();
        if (!pMemberinfo)
        {
            continue;
        }


        std::string strName = pMemberRecord->GetString(i, NFrame::Team::MemberList::Name);
        const int nLevel = pMemberRecord->GetInt32(i, NFrame::Team::MemberList::Level);
        const int nJob = pMemberRecord->GetInt32(i, NFrame::Team::MemberList::Job);
        const NFGUID xPlayerID = pMemberRecord->GetObject(i, NFrame::Team::MemberList::GUID);

        pMemberinfo->set_name(strName);
        pMemberinfo->set_nlevel(nLevel);
        pMemberinfo->set_job(nJob);
        pMemberinfo->set_headicon("");
        *pMemberinfo->mutable_player_id() = NFINetModule::NFToPB(xPlayerID);
    }

    return true;
}
