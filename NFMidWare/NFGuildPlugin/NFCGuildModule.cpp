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
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();
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
	if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CHAT, this, &NFCGuildModule::OnClienChatProcess)) { return false; }
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
	//net_msg? end_func?
	return 0;
}

void NFCGuildModule::OnCreateGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckCreateGuild);

}

void NFCGuildModule::OnJoinGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckJoinGuild);
	NF_SHARE_PTR<int> xActorID = mActorList.GetElementBySuit(xMsg.guild_id().index());
	m_pActorModule->SendMsgToActor(*xActorID, NFGUID(0, nSockIndex), nMsgID, std::string(msg, nLen));
}

void NFCGuildModule::OnLeaveGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckLeaveGuild);
	NF_SHARE_PTR<int> xActorID = mActorList.GetElementBySuit(xMsg.guild_id().index());
	m_pActorModule->SendMsgToActor(*xActorID, NFGUID(0, nSockIndex), nMsgID, std::string(msg, nLen));
}

void NFCGuildModule::OnOprGuildMemberProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckOprGuildMember);
	NF_SHARE_PTR<int> xActorID = mActorList.GetElementBySuit(xMsg.guild_id().index());
	m_pActorModule->SendMsgToActor(*xActorID, NFGUID(0, nSockIndex), nMsgID, std::string(msg, nLen));
}

void NFCGuildModule::OnSearchGuildProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen,NFMsg::ReqSearchGuild);
	NF_SHARE_PTR<int> xActorID = mActorList.GetElementBySuit(nSockIndex);
	m_pActorModule->SendMsgToActor(*xActorID, NFGUID(0, nSockIndex), nMsgID, std::string(msg, nLen));
	
}

void NFCGuildModule::OnClienChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
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