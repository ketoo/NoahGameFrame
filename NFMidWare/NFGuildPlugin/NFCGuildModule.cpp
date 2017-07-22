// -------------------------------------------------------------------------
//    @FileName			:    NFCGuildModle.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2017-07-21
//    @Module           :    NFCGuildComponent
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
	m_pActorModule = pPluginManager->FindModule<NFIActorModule>();

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
	StartActorPool(20);

	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CREATE_GUILD, this, &NFCGuildModule::OnCreateGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_JOIN_GUILD, this, &NFCGuildModule::OnJoinGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_LEAVE_GUILD, this, &NFCGuildModule::OnLeaveGuildProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_OPR_GUILD, this, &NFCGuildModule::OnOprGuildMemberProcess)) { return false; }
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_SEARCH_GUILD, this, &NFCGuildModule::OnSearchGuildProcess)) { return false; }

    return true;
}

bool NFCGuildModule::StartActorPool(const int nCount)
{
	for (int i = 0; i < nCount; i++)
	{
		int nActorID = m_pActorModule->RequireActor<NFCGuildComponent>(this, &NFCGuildModule::ComponentAsyEnd);
		if (nActorID > 0)
		{
			mActorList.AddElement(i, NF_SHARE_PTR<int>(NF_NEW int(nActorID)));
		}
	}


	return false;
}

bool NFCGuildModule::CloseActorPool()
{
	return false;
}

int NFCGuildModule::ComponentAsyEnd(const NFGUID & self, const int nFormActor, const int nSubMsgID, const std::string & strData)
{
	return 0;
}

void NFCGuildModule::OnCreateGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckCreateGuild);
	NF_SHARE_PTR<int> xActorID = mActorList.GetElementBySuit(xMsg.guild_id().index());
	m_pActorModule->SendMsgToActor(*xActorID, NFGUID(0, nSockIndex), nMsgID, std::string(msg, nLen));
}

void NFCGuildModule::OnJoinGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	/*
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen,NFMsg::ReqAckJoinGuild)

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
		*/
}

void NFCGuildModule::OnLeaveGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	/*
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen,NFMsg::ReqAckLeaveGuild)

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
		*/
}

void NFCGuildModule::OnOprGuildMemberProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen,NFMsg::ReqAckOprGuildMember)
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
}

void NFCGuildModule::OnSearchGuildProcess(const int nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen,NFMsg::ReqSearchGuild);
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
} 