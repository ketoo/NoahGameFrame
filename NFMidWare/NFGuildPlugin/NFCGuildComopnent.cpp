// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildComponent.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-07-21
//    @Module           :    NFCGuildComponent
//
// -------------------------------------------------------------------------
#include "NFCGuildComponent.h"
#ifdef _MSC_VER
#pragma warning(disable: 4244 4267)
#endif
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#ifdef _MSC_VER
#pragma warning(default: 4244 4267)
#endif
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCGuildComponent::CheckPower(const NFGUID& self, const NFGUID& xGuildID, int nPowerType)
{
	/*
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
	*/
	return false;
}

int NFCGuildComponent::OnCreateGuildProcess(const NFGUID & self, const int from, const int event, std::string & arg)
{
	CLIENT_MSG_PROCESS_NO_LOG(event, arg.c_str(), (uint32_t)arg.length(), NFMsg::ReqAckCreateGuild)

	std::string strRoleName;
	int nLevel = 0;
	int nJob = 0;
	int nDonation= 0;
	int nVIP= 0;
	/*
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
	*/
	return 0;
}

int NFCGuildComponent::OnJoinGuildProcess(const NFGUID & self, const int from, const int event, std::string & arg)
{
	return 0;
}

int NFCGuildComponent::OnLeaveGuildProcess(const NFGUID & self, const int from, const int event, std::string & arg)
{
	return 0;
}

int NFCGuildComponent::OnOprGuildMemberProcess(const NFGUID & self, const int from, const int event, std::string & arg)
{
	/*
	NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType eOprType = xMsg.type();
	switch (eOprType)
	{
	case NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType::ReqAckOprGuildMember_EGGuildMemberOprType_EGAT_UP:
	PromotionMember(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id()), NFINetModule::PBToNF(xMsg.member_id()));
	break;
	case NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType::ReqAckOprGuildMember_EGGuildMemberOprType_EGAT_DOWN:
	DemotionMember(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id()), NFINetModule::PBToNF(xMsg.member_id()));
	break;
	case NFMsg::ReqAckOprGuildMember::EGGuildMemberOprType::ReqAckOprGuildMember_EGGuildMemberOprType_EGAT_KICK:
	KickMmember(nPlayerID, NFINetModule::PBToNF(xMsg.guild_id()), NFINetModule::PBToNF(xMsg.member_id()));
	break;
	default:
	break;
	}
	*/
	return 0;
}

int NFCGuildComponent::OnSearchGuildProcess(const NFGUID & self, const int from, const int event, std::string & arg)
{
	/*
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
	*/
	return 0;
}

bool NFCGuildComponent::Init()
{
	AddMsgObserver(NFMsg::EGMI_REQ_CREATE_GUILD, this, &NFCGuildComponent::OnCreateGuildProcess);
	AddMsgObserver(NFMsg::EGMI_REQ_JOIN_GUILD, this, &NFCGuildComponent::OnJoinGuildProcess);
	AddMsgObserver(NFMsg::EGMI_REQ_LEAVE_GUILD, this, &NFCGuildComponent::OnLeaveGuildProcess);
	AddMsgObserver(NFMsg::EGMI_REQ_OPR_GUILD, this, &NFCGuildComponent::OnOprGuildMemberProcess);
	AddMsgObserver(NFMsg::EGMI_REQ_SEARCH_GUILD, this, &NFCGuildComponent::OnSearchGuildProcess);

	return false;
}

bool NFCGuildComponent::AfterInit()
{
	//init 100 guilds
	return false;
}

int NFCGuildComponent::OnASyncEvent(const NFGUID & self, const int from, const int event, std::string & arg)
{
	return 0;
}

const NFGUID & NFCGuildComponent::CreateGuild(const NFGUID & self, const std::string & strName, const std::string & strRoleName, const int nLevel, const int nJob, const int nDonation, const int nVIP)
{
	return NULL_OBJECT;
}

bool NFCGuildComponent::JoinGuild(const NFGUID & self, const NFGUID & xGuildID)
{
	/*
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
	*/
	return true;
}

bool NFCGuildComponent::LeaveGuild(const NFGUID & self, const NFGUID & xGuildID)
{
	/*
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
	*/

	return false;
}


bool NFCGuildComponent::DemotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember)
{
	/*
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
	*/
	return true;
}



bool NFCGuildComponent::PromotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember)
{
	/*
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

	pMemberRecord->SetInt(nRow, NFrame::Guild::GuildMemberList_Power, (nPower + 1));
	*/
	return true;
}


bool NFCGuildComponent::KickMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember)
{
	/*
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

	if (self == xMember)
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
	*/
	return true;
}


bool NFCGuildComponent::GetOnlineMember(const NFGUID& self, const NFGUID& xGuild, NFDataList& varMemberList, NFDataList& varGameList)
{
	/*
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
	*/
	return true;
}

bool NFCGuildComponent::MemberOnline(const NFGUID& self, const NFGUID& xGuild, const int& nGameID)
{
	/*
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
	*/
	return true;
}

bool NFCGuildComponent::MemberOffline(const NFGUID& self, const NFGUID& xGuild)
{
	/*
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
	*/
	return true;
}


bool NFCGuildComponent::GetGuildBaseInfo(const NFGUID& self, const NFGUID& xGuildID)
{
	return true;
}

bool NFCGuildComponent::GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID)
{

	return true;
}

bool NFCGuildComponent::GetGuildMemberInfo(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember)
{

	return false;
}

void NFCGuildComponent::OnGuildOnlineProcess(const NFGUID & xGuildID)
{
}

void NFCGuildComponent::OnGuildOfflineProcess(const NFGUID & xGuildID)
{
}
