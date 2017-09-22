// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildModle.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-07-21
//    @Module           :    NFCGuildComponent
//
// -------------------------------------------------------------------------

#include "NFCGuildModule.h"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFComm/NFMessageDefine/NFMsgShare.pb.h"
#include "NFComm/NFPluginModule/NFINetModule.h"

bool NFCGuildModule::Init()
{
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();

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
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CHAT, this, &NFCGuildModule::OnClientChatProcess)) { return false; }

    return true;
}

bool NFCGuildModule::CreateGuild(const NFGUID& self, const NFGUID& xGuildID, const std::string& strName, const std::string& strGuildName)
{
    return false;
}

bool NFCGuildModule::JoinGuild(const NFGUID& self, const NFGUID& xGuildID)
{
    return false;

}

bool NFCGuildModule::LeaveGuild(const NFGUID& self, const NFGUID& xGuildID)
{

    return false;
}

bool NFCGuildModule::PromotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember)
{

    return false;
}

bool NFCGuildModule::DemotionMember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember)
{

    return false;
}

bool NFCGuildModule::KickMmember(const NFGUID& self, const NFGUID& xGuildID, const NFGUID& xMember)
{

    return false;
}

bool NFCGuildModule::MemberOnline(const NFGUID& self, const NFGUID& xGuild)
{

    return false;
}

bool NFCGuildModule::MemberOffline(const NFGUID& self, const NFGUID& xGuild)
{
	NF_SHARE_PTR<NFIRecord> xRecord = m_pKernelModule->FindRecord(xGuild, NFrame::Guild::ThisName());
	const int nRow = xRecord->FindObject(NFrame::Guild::GuildMemberList::GUID, self);
	if (nRow >= 0)
	{
		xRecord->SetInt(nRow, NFrame::Guild::GuildMemberList::Online, 0);
		xRecord->SetInt(nRow, NFrame::Guild::GuildMemberList::GameID, 0);

		NFDateTime xTime = NFDateTime::Now();
		xRecord->SetString(nRow, NFrame::Guild::GuildMemberList::LastTime, xTime.GetAsString());
	}

    return false;
}

void NFCGuildModule::OnGuildOnlineProcess(const NFGUID& xGuildID)
{

}

void NFCGuildModule::OnGuildOfflineProcess(const NFGUID& xGuildID)
{

}

NFIGuildModule::MEMBER_TYPE NFCGuildModule::CheckPower(const NFGUID& self, const NFGUID& xGuildID)
{
    return NFIGuildModule::MEMBER_TYPE::MT_MEMBER;
}


void NFCGuildModule::OnCreateGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckCreateGuild);

    //CreateGuild(NFGUID(), xMsg.name xMsg.guild_name(), );
}

void NFCGuildModule::OnJoinGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckJoinGuild);
}

void NFCGuildModule::OnLeaveGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckLeaveGuild);
}

void NFCGuildModule::OnOprGuildMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckOprGuildMember);
}

void NFCGuildModule::OnSearchGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen,NFMsg::ReqSearchGuild);

}

void NFCGuildModule::OnClientChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckPlayerChat);

	NFGUID xTargetID = NFINetModule::PBToNF(xMsg.target_id());

	NF_SHARE_PTR<NFIObject> xGuidObject = m_pKernelModule->GetObject(xTargetID);
	if (xGuidObject)
	{
		//cache the message for the guild member that off line
		//m_pWorldNet_ServerModule->SendMsgPBToAllClient(NFMsg::EGMI_ACK_CHAT, xMsg);
	}
} 