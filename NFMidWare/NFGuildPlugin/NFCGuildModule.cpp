// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildModule.cpp
//    @Author           :    chuanbo.guo
//    @Date             :    2015-05-24 08:51
//    @Module           :   NFCGuildModule
//
// -------------------------------------------------------------------------

#include "NFCGuildModule.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCGuildModule::Init()
{
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pGuildDataModule = pPluginManager->FindModule<NFIGuildDataModule>();

	return true;
}

bool NFCGuildModule::Shut()
{
    return true;
}

bool NFCGuildModule::Execute()
{
    return true;
}

bool NFCGuildModule::AfterInit()
{
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_GUILD, this, &NFCGuildModule::OnCreateGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_GUILD, this, &NFCGuildModule::OnJoinGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_GUILD, this, &NFCGuildModule::OnLeaveGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_OPR_GUILD, this, &NFCGuildModule::OnOprGuildMemberProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_GUILD, this, &NFCGuildModule::OnSearchGuildProcess)) { return false; }

    return true;
}

const NFGUID& NFCGuildModule::CreateGuild( const NFGUID& self, const std::string& strName, const std::string& strRoleName, const int nLevel, const int nJob , const int nDonation , const int nVIP)
{
    if (strName.empty())
    {
        return NULL_OBJECT;
    }

    bool bExit = false;
    if (!m_pGuildDataModule->ExitGuild(self, strName, bExit))
    {
        return NULL_OBJECT;
    }

    if (bExit)
    {
        return NULL_OBJECT;
    }

    return m_pGuildDataModule->CreateGuild(self, strName, strRoleName, nLevel, nJob, nDonation, nVIP);
}

bool NFCGuildModule::JoinGuild( const NFGUID& self, const NFGUID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::R_GuildMemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() > 0)
    {
        return false;
    }

    const NFINT64 nCount = m_pKernelModule->GetPropertyInt(xGuildID, NFrame::Guild::GuildMemeberCount());
    if (nCount >= m_pKernelModule->GetPropertyInt(xGuildID, NFrame::Guild::GuildMemeberMaxCount()))
    {
        //limit
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
    int nPower = NFMsg::GUILD_POWER_TYPE_NORMAL;

    const NFGUID& xPresident = pGuildObject->GetPropertyObject(NFrame::Guild::PresidentID());
    if (xPresident == self)
    {
        nPower = NFMsg::GUILD_POWER_TYPE_PRESIDENT;
    }
    
    int nTitle = 0;
    int nOnlineGameID = 0;

    m_pGuildDataModule->GetPlayerGameID(self, nOnlineGameID);
    m_pGuildDataModule->GetPlayerInfo(self, strName, nLevel, nJob, nDonation, nVIP);

    varData << self << strName << (NFINT64)nLevel << (NFINT64)nJob << (NFINT64)nDonation << (NFINT64)nReceive << (NFINT64)nVIP << (NFINT64)nOnLine << (NFINT64)nPower << nTitle << nOnlineGameID ;

    if (pMemberRecord->AddRow(-1, varData) < 0)
    {
        return false;
    }
    
    m_pKernelModule->SetPropertyInt(xGuildID, NFrame::Guild::GuildMemeberCount(), nCount+1);

	return true;
}

bool NFCGuildModule::LeaveGuild( const NFGUID& self, const NFGUID& xGuildID )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::R_GuildMemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    
    return pMemberRecord->Remove(nRow);
}

bool NFCGuildModule::UpGuildMmember( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::R_GuildMemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    CheckPower(self, xGuildID, NFMsg::ReqAckOprGuildMember::EGAT_UP);

    NFDataList varList;
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

bool NFCGuildModule::DownGuildMmember( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::R_GuildMemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    CheckPower(self, xGuildID, NFMsg::ReqAckOprGuildMember::EGAT_DOWN);

    NFDataList varList;
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

bool NFCGuildModule::KickGuildMmember( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pGuildDataModule->GetGuild(xGuildID);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::R_GuildMemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    if (self == xMmember)
    {
        return false;
    }

    CheckPower(self, xGuildID, NFMsg::ReqAckOprGuildMember::EGAT_KICK);

    NFDataList varList;
    pMemberRecord->FindObject(NFrame::Guild::GuildMemberList_GUID, self, varList);
    if (varList.GetCount() == 0)
    {
        return false;
    }

    const int nRow = varList.Int(0);
    pMemberRecord->Remove(nRow);

    return true;
}

bool NFCGuildModule::GetGuildBaseInfo( const NFGUID& self, const NFGUID& xGuildID )
{
	return true;
}

bool NFCGuildModule::GetGuildMemberInfo( const NFGUID& self, const NFGUID& xGuildID )
{

	return true;
}

bool NFCGuildModule::GetGuildMemberInfo( const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMmember )
{

	return false;
}

bool NFCGuildModule::CheckPower( const NFGUID& self, const NFGUID& xGuildID, int nPowerType )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pGuildDataModule->GetGuild(xGuildID);
    if (pGuildObject.get())
    {
        NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuildID, NFrame::Guild::R_GuildMemberList());
        if (pMemberRecord.get())
        {
            NFDataList varList;
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

bool NFCGuildModule::GetOnlineMember( const NFGUID& self, const NFGUID& xGuild, NFDataList& varMemberList, NFDataList& varGameList)
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pGuildDataModule->GetGuild(xGuild);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuild, NFrame::Guild::R_GuildMemberList());
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

bool NFCGuildModule::MemberOnline( const NFGUID& self, const NFGUID& xGuild , const int& nGameID)
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pGuildDataModule->GetGuild(xGuild);
    if (!pGuildObject.get())
    {
        return false;
    }

    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuild, NFrame::Guild::R_GuildMemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFDataList varList;
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

bool NFCGuildModule::MemberOffline( const NFGUID& self, const NFGUID& xGuild )
{
    NF_SHARE_PTR<NFIObject> pGuildObject = m_pGuildDataModule->GetGuild(xGuild);
    if (!pGuildObject.get())
    {
        return false;
    }


    NF_SHARE_PTR<NFIRecord> pMemberRecord = m_pKernelModule->FindRecord(xGuild, NFrame::Guild::R_GuildMemberList());
    if (!pMemberRecord.get())
    {
        return false;
    }

    NFDataList varList;
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


void NFCGuildModule::OnCreateGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen, NFMsg::ReqAckCreateGuild);

	std::string strRoleName ;
	int nLevel = 0;
	int nJob = 0;
	int nDonation= 0;
	int nVIP= 0;

	m_pGuildDataModule->GetPlayerInfo(nPlayerID, strRoleName, nLevel, nJob, nDonation, nVIP);
	NFGUID xGuild = CreateGuild(nPlayerID, xMsg.guild_name(), strRoleName, nLevel, nJob, nDonation, nVIP);

	if (!xGuild.IsNull())
	{
		NFMsg::ReqAckCreateGuild xAck;
		*xAck.mutable_guild_id() = NFINetModule::NFToPB(xGuild);
		xAck.set_guild_name(xMsg.guild_name());

		m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_CREATE_GUILD, xAck, nSockIndex, nPlayerID);
	}
	else
	{
		NFMsg::ReqAckCreateGuild xAck;
		*xAck.mutable_guild_id() = NFINetModule::NFToPB(xGuild);
		xAck.set_guild_name("");

		m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_CREATE_GUILD, xAck, nSockIndex, nPlayerID);
	}
}

void NFCGuildModule::OnJoinGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqAckJoinGuild)

		if (JoinGuild(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id())))
		{
			NFMsg::ReqAckJoinGuild xAck;
			*xAck.mutable_guild_id() = xMsg.guild_id();

			NFGUID xGuild = NFINetModule::PBToNF(xMsg.guild_id());
			const std::string& strName = m_pKernelModule->GetPropertyString(xGuild, NFrame::Guild::Name());
			xAck.set_guild_name(strName);

			m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_JOIN_GUILD, xAck, nSockIndex, nPlayerID);

			int nGameID = 0;
			if(m_pGuildDataModule->GetPlayerGameID(nPlayerID, nGameID))
			{
				MemberOnline(nPlayerID, xGuild, nGameID);
			}
		}
		else
		{
			NFMsg::ReqAckJoinGuild xAck;
			*xAck.mutable_guild_id() = NFINetModule::NFToPB(NFGUID());
			xAck.set_guild_name("");

			m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_JOIN_GUILD, xAck, nSockIndex, nPlayerID);
		}
}

void NFCGuildModule::OnLeaveGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqAckLeaveGuild)

		if (LeaveGuild(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id())))
		{

			NFMsg::ReqAckLeaveGuild xAck;
			*xAck.mutable_guild_id() = xMsg.guild_id();
			xAck.set_guild_name(xMsg.guild_name());

			m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_LEAVE_GUILD, xAck, nSockIndex, nPlayerID);
		}
		else
		{
			NFMsg::ReqAckLeaveGuild xAck;
			*xAck.mutable_guild_id() = NFINetModule::NFToPB(NFGUID());
			xAck.set_guild_name("");

			m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_LEAVE_GUILD, xAck, nSockIndex, nPlayerID);
		}
}

void NFCGuildModule::OnOprGuildMemberProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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

void NFCGuildModule::OnSearchGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nSockIndex, nMsgID, msg, nLen,NFMsg::ReqSearchGuild);

	std::vector<NFIGuildDataModule::SearchGuildObject> xList;    
	m_pGuildDataModule->SearchGuild(nPlayerID, xMsg.guild_name(), xList);

	NFMsg::AckSearchGuild xAckMsg;
	for (int i = 0; i < xList.size(); ++i)
	{
		NFMsg::AckSearchGuild::SearchGuildObject* pData = xAckMsg.add_guild_list();
		if (pData)
		{
			const NFIGuildDataModule::SearchGuildObject& xGuild = xList[i];
			*pData->mutable_guild_id() = NFINetModule::NFToPB(xGuild.mxGuildID);
			pData->set_guild_name(xGuild.mstrGuildName);
			pData->set_guild_icon(xGuild.mnGuildIcon);

			pData->set_guild_member_count(xGuild.mnGuildMemberCount);
			pData->set_guild_member_max_count(xGuild.mnGuildMemberMaxCount);
			pData->set_guild_honor(xGuild.mnGuildHonor);
			pData->set_guild_rank(xGuild.mnGuildRank);
		}
	}

	m_pNetModule->SendMsgPB(NFMsg::EGMI_ACK_SEARCH_GUILD, xAckMsg, nSockIndex, nPlayerID);
} 