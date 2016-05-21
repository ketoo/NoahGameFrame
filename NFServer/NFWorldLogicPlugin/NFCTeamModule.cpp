// -------------------------------------------------------------------------
//    @FileName      :    NFCTeamModule.cpp
//    @Author           :    chuanbo.guo
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCTeamModule
//
// -------------------------------------------------------------------------

#include "NFCTeamModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFCTeamModule::Init()
{
    m_pWorldNet_ServerModule = dynamic_cast<NFIWorldNet_ServerModule*>(pPluginManager->FindModule("NFCWorldNet_ServerModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));
	m_pTeamDataModule = dynamic_cast<NFITeamDataModule*>(pPluginManager->FindModule("NFCTeamDataModule"));
	
	assert(NULL != m_pWorldNet_ServerModule);
	assert(NULL != m_pLogModule);
	assert(NULL != m_pTeamDataModule);

	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CREATE_TEAM, this, &NFCTeamModule::OnCreateTeamProcess)){ return false; }
	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_JOIN_TEAM, this, &NFCTeamModule::OnJoinTeamProcess)){ return false; }
	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_LEAVE_TEAM, this, &NFCTeamModule::OnLeaveTeamProcess)){ return false; }

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
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pUUIDModule);

    return true;
}

const NFGUID& NFCTeamModule::CreateTeam( const NFGUID& self, const NFGUID& xDefaultTeamID, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP)
{
    if (strName.empty())
    {
        return NULL_OBJECT;
    }

    return m_pTeamDataModule->CreateTeam(self, xDefaultTeamID, strName, strRoleName, nLevel, nJob, nDonation, nVIP);
}

bool NFCTeamModule::JoinTeam( const NFGUID& self, const NFGUID& xTeamID )
{
    NF_SHARE_PTR<NFIObject> pTeamObject = m_pTeamDataModule->GetTeam(xTeamID);
    if (!pTeamObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xTeamID, NFrame::Team::R_MemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList_GUID, self, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }

    NFCDataList varData;

    std::string strName ;
    int nLevel = 0;
    int nJob = 0;
    int nDonation = 0;
    int nReceive = 0;
    int nVIP = 0;
    int nOnLine = 0;
    
    int nTitle = 0;
    int nOnlineGameID = 0;

    m_pTeamDataModule->GetPlayerGameID(self, nOnlineGameID);
    m_pTeamDataModule->GetPlayerInfo(self, strName, nLevel, nJob, nDonation, nVIP);

    varData << self << strName << (NFINT64)nLevel << (NFINT64)nJob << (NFINT64)nDonation << (NFINT64)nReceive << (NFINT64)nVIP << (NFINT64)nOnLine << nTitle << nOnlineGameID ;

    if (pMemberRecord->AddRow(-1, varData) < 0)
    {
        return false;
    }

	return true;
}

bool NFCTeamModule::LeaveTeam( const NFGUID& self, const NFGUID& xTeamID )
{
    NF_SHARE_PTR<NFIObject> pTeamObject = m_pTeamDataModule->GetTeam(xTeamID);
    if (!pTeamObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xTeamID, NFrame::Team::R_MemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    
    return pMemberRecord->Remove(nRow);
}

bool NFCTeamModule::KickTeamMmember( const NFGUID& self, const NFGUID& xTeamID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pTeamObject = m_pTeamDataModule->GetTeam(xTeamID);
    if (!pTeamObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xTeamID, NFrame::Team::R_MemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    if (self == xMmember)
    {
        return false;
    }

	const NFGUID xID = pTeamObject->GetPropertyObject(NFrame::Team::Captain());
	if (self != xID)
	{
		return false;
	}

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->Remove(nRow);

    return true;
}

bool NFCTeamModule::GetOnlineMember( const NFGUID& self, const NFGUID& xTeam, NFCDataList& varMemberList, NFCDataList& varGameList)
{
    NF_SHARE_PTR<NFIObject> pTeamObject = m_pTeamDataModule->GetTeam(xTeam);
    if (!pTeamObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xTeam, NFrame::Team::R_MemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    for (int i = 0; i< pMemberRecord->GetRows(); i++)
    {
        if (!pMemberRecord->IsUsed(i))
        {
            continue;
        }

        const NFINT64 nOnline = pMemberRecord->GetInt(i, NFrame::Team::MemberList_Online);
        const NFINT64 nGameID = pMemberRecord->GetInt(i, NFrame::Team::MemberList_GameID);
        const NFGUID& xID = pMemberRecord->GetObject(i, NFrame::Team::MemberList_GUID);
        if (nOnline > 0 && !xID.IsNull())
        {

            varMemberList.Add(xID);
            varGameList.Add(nGameID);
        }
    }

    return true;
}

bool NFCTeamModule::MemberOnline( const NFGUID& self, const NFGUID& xTeam , const int& nGameID)
{
    NF_SHARE_PTR<NFIObject> pTeamObject = m_pTeamDataModule->GetTeam(xTeam);
    if (!pTeamObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xTeam, "TeamMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList_GUID, self, varList);
    if (varList.GetCount() <=  0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->SetInt(nRow, NFrame::Team::MemberList_GameID, nGameID);
    
    pMemberRecord->SetInt(nRow, NFrame::Team::MemberList_Online, 1);

    return true;
}

bool NFCTeamModule::MemberOffeline( const NFGUID& self, const NFGUID& xTeam )
{
    NF_SHARE_PTR<NFIObject> pTeamObject = m_pTeamDataModule->GetTeam(xTeam);
    if (!pTeamObject.get())
    {
        return false;
    }


    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xTeam, "TeamMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Team::MemberList_GUID, self, varList);
    if (varList.GetCount() <= 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->SetInt(nRow, NFrame::Team::MemberList_Online, 0);
    pMemberRecord->SetInt(nRow, NFrame::Team::MemberList_GameID, 0);
    return true;
}


void NFCTeamModule::OnCreateTeamProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckCreateTeam);

	std::string strRoleName ;
	int nLevel = 0;
	int nJob = 0;
	int nDonation= 0;
	int nVIP= 0;

	m_pTeamDataModule->GetPlayerInfo(nPlayerID, strRoleName, nLevel, nJob, nDonation, nVIP);
	NFGUID xTeam = CreateTeam(nPlayerID, NFINetModule::PBToNF(xMsg.team_id()), strRoleName, strRoleName, nLevel, nJob, nDonation, nVIP);

	if (!xTeam.IsNull())
	{
		NFMsg::ReqAckCreateTeam xAck;
		*xAck.mutable_team_id() = NFINetModule::NFToPB(xTeam);

		m_pGameServerNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_CREATE_TEAM, xAck, nSockIndex, nPlayerID);
	}
	else
	{
		NFMsg::ReqAckCreateTeam xAck;
		*xAck.mutable_team_id() = NFINetModule::NFToPB(xTeam);

		m_pGameServerNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_CREATE_TEAM, xAck, nSockIndex, nPlayerID);
	}
}

void NFCTeamModule::OnJoinTeamProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqAckJoinTeam)

		if (JoinTeam(nPlayerID, NFINetModule::PBToNF(xMsg.team_id())))
		{
			NFMsg::ReqAckJoinTeam xAck;
			*xAck.mutable_team_id() = xMsg.team_id();

			NFGUID xTeam = NFINetModule::PBToNF(xMsg.team_id());
			const std::string& strName = m_pKernelModule->GetPropertyString(xTeam, "Name");

			m_pGameServerNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_JOIN_TEAM, xAck, nSockIndex, nPlayerID);

			int nGameID = 0;
			if(m_pTeamDataModule->GetPlayerGameID(nPlayerID, nGameID))
			{
				MemberOnline(nPlayerID, xTeam, nGameID);
			}
		}
		else
		{
			NFMsg::ReqAckJoinTeam xAck;
			*xAck.mutable_team_id() = NFINetModule::NFToPB(NFGUID());

			m_pGameServerNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_JOIN_TEAM, xAck, nSockIndex, nPlayerID);
		}
}

void NFCTeamModule::OnLeaveTeamProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqAckLeaveTeam)

		if (LeaveTeam(nPlayerID, NFINetModule::PBToNF(xMsg.team_id())))
		{

			NFMsg::ReqAckLeaveTeam xAck;
			*xAck.mutable_team_id() = xMsg.team_id();

			m_pGameServerNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_LEAVE_TEAM, xAck, nSockIndex, nPlayerID);
		}
		else
		{
			NFMsg::ReqAckLeaveTeam xAck;
			*xAck.mutable_team_id() = NFINetModule::NFToPB(NFGUID());

			m_pGameServerNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_LEAVE_TEAM, xAck, nSockIndex, nPlayerID);
		}
}

void NFCTeamModule::OnOprTeamMemberProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqAckOprTeamMember)

	NFMsg::ReqAckOprTeamMember::EGTeamMemberOprType eOprType = xMsg.type();
	switch (eOprType)
	{
	case NFMsg::ReqAckOprTeamMember::EGAT_KICKOUT:
		KickTeamMmember(nPlayerID, NFINetModule::PBToNF(xMsg.team_id()), NFINetModule::PBToNF(xMsg.member_id()));
		break;
	default:
		break;
	}

}
