// -------------------------------------------------------------------------
//    @FileName      :    NFCWorldGuildModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCWorldGuildModule
//
// -------------------------------------------------------------------------

#include "NFWorldGuildPlugin.h"
#include "NFCWorldGuildModule.h"
#include "NFComm/Config/NFConfig.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCWorldGuildModule::Init()
{
	m_pWorldNet_ServerModule = dynamic_cast<NFIWorldNet_ServerModule*>(pPluginManager->FindModule("NFCWorldNet_ServerModule"));
	m_pLogModule = dynamic_cast<NFILogModule*>(pPluginManager->FindModule("NFCLogModule"));

	assert(NULL != m_pWorldNet_ServerModule);
	assert(NULL != m_pLogModule);

	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_CREATE_GUILD, this, &NFCWorldGuildModule::OnCreateGuildProcess)){ return false; }
	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_JOIN_GUILD, this, &NFCWorldGuildModule::OnJoinGuildProcess)){ return false; }
	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_LEAVE_GUILD, this, &NFCWorldGuildModule::OnLeaveGuildProcess)){ return false; }
	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_OPR_GUILD, this, &NFCWorldGuildModule::OnOprGuildMemberProcess)){ return false; }
	if (!m_pWorldNet_ServerModule->AddReciveCallBack(NFMsg::EGMI_REQ_SEARCH_GUILD, this, &NFCWorldGuildModule::OnSearchGuildProcess)){ return false; }

	return true;
}

bool NFCWorldGuildModule::Shut()
{
    return true;
}

bool NFCWorldGuildModule::Execute()
{
    return true;
}

bool NFCWorldGuildModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>("NFCKernelModule");
    m_pUUIDModule = pPluginManager->FindModule<NFIUUIDModule>("NFCUUIDModule");
    m_pWorldGuildDataModule = pPluginManager->FindModule<NFIWorldGuildDataModule>("NFCWorldGuildDataModule");

    assert(NULL != m_pKernelModule);
    assert(NULL != m_pUUIDModule);
    assert(NULL != m_pWorldGuildDataModule);

    return true;
}

const NFGUID& NFCWorldGuildModule::CreateGuild( const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP)
{
    if (strName.empty())
    {
        return NULL_OBJECT;
    }

    bool bExit = false;
    if (!m_pWorldGuildDataModule->ExitGuild(self, strName, bExit))
    {
        return NULL_OBJECT;
    }

    if (bExit)
    {
        return NULL_OBJECT;
    }

    return m_pWorldGuildDataModule->CreateGuild(self, strName, strRoleName, nLevel, nJob, nDonation, nVIP);
}

bool NFCWorldGuildModule::JoinGuild( const NFGUID& self, const NFGUID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }

    const NFINT64 nCount = m_pKernelModule->GetPropertyInt(xGuildID, "GuildMemeberCount");
    if (nCount >= m_pKernelModule->GetPropertyInt(xGuildID, "GuildMemeberMaxCount"))
    {
        //limit
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
    int nPower = NFMsg::GUILD_POWER_TYPE_NORMAL;

    const NFGUID& xPresident = pGuildObject->GetPropertyObject("PresidentID");
    if (xPresident == self)
    {
        nPower = NFMsg::GUILD_POWER_TYPE_PRESIDENT;
    }
    
    int nTitle = 0;
    int nOnlineGameID = 0;

    m_pWorldGuildDataModule->GetPlayerGameID(self, nOnlineGameID);
    m_pWorldGuildDataModule->GetPlayerInfo(self, strName, nLevel, nJob, nDonation, nVIP);

    varData << self << strName << (NFINT64)nLevel << (NFINT64)nJob << (NFINT64)nDonation << (NFINT64)nReceive << (NFINT64)nVIP << (NFINT64)nOnLine << (NFINT64)nPower << nTitle << nOnlineGameID ;

    if (pMemberRecord->AddRow(-1, varData) < 0)
    {
        return false;
    }
    
    m_pKernelModule->SetPropertyInt(xGuildID, "GuildMemeberCount", nCount+1);

	return true;
}

bool NFCWorldGuildModule::LeaveGuild( const NFGUID& self, const NFGUID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    
    return pMemberRecord->Remove(nRow);
}

bool NFCWorldGuildModule::UpGuildMmember( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    CheckPower(self, xGuildID, NFMsg::ReqAckOprGuildMember::EGAT_UP);

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);

    const int nPower = pMemberRecord->GetInt(nRow, NFrame::Guild::GuildMemberList_Power);

    if (nPower >= NFMsg::GUILD_POWER_TYPE_PRESIDENT)
    {
        return false;
    }

    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_Power, (nPower + 1)) ;
	return true;
}

bool NFCWorldGuildModule::DownGuildMmember( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    CheckPower(self, xGuildID, NFMsg::ReqAckOprGuildMember::EGAT_DOWN);

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);

    const int nPower = pMemberRecord->GetInt(nRow, NFrame::Guild::GuildMemberList_Power);
    if (nPower == 0)
    {
        return false;
    }

    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_Power, (nPower - 1));
    return true;
}

bool NFCWorldGuildModule::KickGuildMmember( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    if (self == xMmember)
    {
        return false;
    }

    CheckPower(self, xGuildID, NFMsg::ReqAckOprGuildMember::EGAT_KICK);

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->Remove(nRow);

    return true;
}

bool NFCWorldGuildModule::GetGuildBaseInfo( const NFGUID& self, const NFGUID& xGuildID )
{
	return true;
}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFGUID& self, const NFGUID& xGuildID )
{

	return true;
}

bool NFCWorldGuildModule::GetGuildMemberInfo( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{

	return false;
}

bool NFCWorldGuildModule::CheckPower( const NFGUID& self, const NFGUID& xGuildID, int nPowerType )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuildID);
    if (pGuildObject.get())
    {
        NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, "GuildMemberList");
        if (pMemberRecord.get())
        {
            NFCDataList varList;
            pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
            if (varList.GetCount() == 1)
            {
                const int nRow = varList.Int(0);
                const int nPower = pMemberRecord->GetInt(nRow, NFrame::Guild::GuildMemberList_Power); 
                if (nPower >= NFMsg::GUILD_POWER_TYPE_PRESIDENT)
                {
                    return true;
                }
            }
        }
    }

    return false;
}

bool NFCWorldGuildModule::GetOnlineMember( const NFGUID& self, const NFGUID& xGuild, NFCDataList& varMemberList, NFCDataList& varGameList)
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuild);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuild, "GuildMemberList");
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

        const NFINT64 nOnline = pMemberRecord->GetInt(i, NFrame::Guild::GuildMemberList_Online);
        const NFINT64 nGameID = pMemberRecord->GetInt(i, NFrame::Guild::GuildMemberList_GameID);
        const NFGUID& xID = pMemberRecord->GetObject(i, NFrame::Guild::GuildMemberList_GUID);
        if (nOnline > 0 && !xID.IsNull())
        {

            varMemberList.Add(xID);
            varGameList.Add(nGameID);
        }
    }

    return true;
}

bool NFCWorldGuildModule::MemberOnline( const NFGUID& self, const NFGUID& xGuild , const int& nGameID)
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuild);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuild, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() <=  0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_GameID, nGameID);
    
    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_Online, 1);

    return true;
}

bool NFCWorldGuildModule::MemberOffeline( const NFGUID& self, const NFGUID& xGuild )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pWorldGuildDataModule->GetGuild(xGuild);
    if (!pGuildObject.get())
    {
        return false;
    }


    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuild, "GuildMemberList");
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFCDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() <= 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_Online, 0);
    pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_GameID, 0);
    return true;
}


void NFCWorldGuildModule::OnCreateGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckCreateGuild);

	std::string strRoleName ;
	int nLevel = 0;
	int nJob = 0;
	int nDonation= 0;
	int nVIP= 0;

	m_pWorldGuildDataModule->GetPlayerInfo(nPlayerID, strRoleName, nLevel, nJob, nDonation, nVIP);
	NFGUID xGuild = CreateGuild(nPlayerID, xMsg.guild_name(), strRoleName, nLevel, nJob, nDonation, nVIP);

	if (!xGuild.IsNull())
	{
		NFMsg::ReqAckCreateGuild xAck;
		*xAck.mutable_guild_id() = NFINetModule::NFToPB(xGuild);
		xAck.set_guild_name(xMsg.guild_name());

		m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_CREATE_GUILD, xAck, nSockIndex, nPlayerID);
	}
	else
	{
		NFMsg::ReqAckCreateGuild xAck;
		*xAck.mutable_guild_id() = NFINetModule::NFToPB(xGuild);
		xAck.set_guild_name("");

		m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_CREATE_GUILD, xAck, nSockIndex, nPlayerID);
	}
}

void NFCWorldGuildModule::OnJoinGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqAckJoinGuild)

		if (JoinGuild(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id())))
		{
			NFMsg::ReqAckJoinGuild xAck;
			*xAck.mutable_guild_id() = xMsg.guild_id();

			NFGUID xGuild = NFINetModule::PBToNF(xMsg.guild_id());
			const std::string& strName = m_pKernelModule->GetPropertyString(xGuild, "Name");
			xAck.set_guild_name(strName);

			m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_JOIN_GUILD, xAck, nSockIndex, nPlayerID);

			int nGameID = 0;
			if(m_pWorldGuildDataModule->GetPlayerGameID(nPlayerID, nGameID))
			{
				MemberOnline(nPlayerID, xGuild, nGameID);
			}
		}
		else
		{
			NFMsg::ReqAckJoinGuild xAck;
			*xAck.mutable_guild_id() = NFINetModule::NFToPB(NFGUID());
			xAck.set_guild_name("");

			m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_JOIN_GUILD, xAck, nSockIndex, nPlayerID);
		}
}

void NFCWorldGuildModule::OnLeaveGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqAckLeaveGuild)

		if (LeaveGuild(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id())))
		{

			NFMsg::ReqAckLeaveGuild xAck;
			*xAck.mutable_guild_id() = xMsg.guild_id();
			xAck.set_guild_name(xMsg.guild_name());

			m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_LEAVE_GUILD, xAck, nSockIndex, nPlayerID);
		}
		else
		{
			NFMsg::ReqAckLeaveGuild xAck;
			*xAck.mutable_guild_id() = NFINetModule::NFToPB(NFGUID());
			xAck.set_guild_name("");

			m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_LEAVE_GUILD, xAck, nSockIndex, nPlayerID);
		}
}

void NFCWorldGuildModule::OnOprGuildMemberProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqAckOprGuildMember)

		NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType eOprType = xMsg.type();
	switch (eOprType)
	{
	case NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType::ReqAckOprGuildMember_EGGuildMemberOprType_EGAT_UP:
		UpGuildMmember(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id()), NFINetModule::PBToNF(xMsg.member_id()));
		break;
	case NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType::ReqAckOprGuildMember_EGGuildMemberOprType_EGAT_DOWN:
		DownGuildMmember(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id()), NFINetModule::PBToNF(xMsg.member_id()));
		break;
	case NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType::ReqAckOprGuildMember_EGGuildMemberOprType_EGAT_KICK:
		KickGuildMmember(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id()), NFINetModule::PBToNF(xMsg.member_id()));
		break;
	default:
		break;
	}

}

void NFCWorldGuildModule::OnSearchGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqSearchGuild);

	std::vector<NFIWorldGuildDataModule::SearchGuildObject> xList;    
	m_pWorldGuildDataModule->SearchGuild(nPlayerID, xMsg.guild_name(), xList);

	NFMsg::AckSearchGuild xAckMsg;
	for (int i = 0; i < xList.size(); ++i)
	{
		NFMsg::AckSearchGuild::SearchGuildObject* pData = xAckMsg.add_guild_list();
		if (pData)
		{
			const NFIWorldGuildDataModule::SearchGuildObject& xGuild = xList[i];
			*pData->mutable_guild_id() = NFINetModule::NFToPB(xGuild.mxGuildID);
			pData->set_guild_name(xGuild.mstrGuildName);
			pData->set_guild_icon(xGuild.mnGuildIcon);

			pData->set_guild_member_count(xGuild.mnGuildMemberCount);
			pData->set_guild_member_max_count(xGuild.mnGuildMemberMaxCount);
			pData->set_guild_honor(xGuild.mnGuildHonor);
			pData->set_guild_rank(xGuild.mnGuildRank);
		}
	}

	m_pWorldNet_ServerModule->SendMsgPB(NFMsg::EGMI_ACK_SEARCH_GUILD, xAckMsg, nSockIndex, nPlayerID);
} 
