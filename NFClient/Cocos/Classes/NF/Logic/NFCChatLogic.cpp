// -------------------------------------------------------------------------
//    @FileName			:    NFCChatLogic.cpp
//    @Author           :    Johance
//    @Date             :    2016-12-28
//    @Module           :    NFCChatLogic
//
// -------------------------------------------------------------------------

#include "stdafx.h"
#include "NFCChatLogic.h"
#include "NFComm/NFMessageDefine/NFMsgDefine.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFCNetLogic.h"
#include "NFCPlayerLogic.h"

bool NFCChatLogic::Init()
{
    return true;
}

bool NFCChatLogic::Shut()
{
    return true;
}

bool NFCChatLogic::ReadyExecute()
{
    return true;
}

bool NFCChatLogic::Execute()
{


    return true;
}

bool NFCChatLogic::AfterInit()
{
	NFCLogicBase::AfterInit();
	g_pNetLogic->AddReceiveCallBack(NFMsg::EGMI_ACK_CHAT, this, &NFCChatLogic::OnChatProcess);
    return true;
}
//--------------------------------------------发消息-------------------------------------------------------------
void NFCChatLogic::RequireChatWorld(std::string strContent)
{
	NFMsg::ReqAckPlayerChat xMsg;
	xMsg.set_chat_name(g_pPlayerLogic->GetRoleInfo().noob_name());
	*xMsg.mutable_chat_id() = NFINetModule::NFToPB(g_pPlayerLogic->GetRoleGuid());
	xMsg.set_chat_info(strContent);
	xMsg.set_chat_type(NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_WORLD);
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_CHAT, xMsg);
}

void NFCChatLogic::RequireChatGuild(std::string strContent)
{
	NFMsg::ReqAckPlayerChat xMsg;
	xMsg.set_chat_name(g_pPlayerLogic->GetRoleInfo().noob_name());
	*xMsg.mutable_chat_id() = NFINetModule::NFToPB(g_pPlayerLogic->GetRoleGuid());
	xMsg.set_chat_info(strContent);
	xMsg.set_chat_type(NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_GUILD);
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_CHAT, xMsg);
}

void NFCChatLogic::RequireChatPrivate(const NFGUID &target, std::string strContent)
{
	NFMsg::ReqAckPlayerChat xMsg;
	xMsg.set_chat_name(g_pPlayerLogic->GetRoleInfo().noob_name());
	*xMsg.mutable_chat_id() = NFINetModule::NFToPB(g_pPlayerLogic->GetRoleGuid());
	*xMsg.mutable_target_id() = NFINetModule::NFToPB(target);
	xMsg.set_chat_info(strContent);
	xMsg.set_chat_type(NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_PRIVATE);
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_CHAT, xMsg);
}

void NFCChatLogic::RequireChatTeam(std::string strContent)
{
	NFMsg::ReqAckPlayerChat xMsg;
	xMsg.set_chat_name(g_pPlayerLogic->GetRoleInfo().noob_name());
	*xMsg.mutable_chat_id() = NFINetModule::NFToPB(g_pPlayerLogic->GetRoleGuid());
	xMsg.set_chat_info(strContent);
	xMsg.set_chat_type(NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_TEAM);
	g_pNetLogic->SendToServerByPB(NFMsg::EGameMsgID::EGMI_REQ_CHAT, xMsg);
}

//--------------------------------------------收消息-------------------------------------------------------------
void NFCChatLogic::OnChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqAckPlayerChat xMsg;
	if (!NFINetModule::ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	NFDataList var;
	var.AddObject(NFINetModule::PBToNF(xMsg.chat_id()));
	var.AddInt(xMsg.chat_type());
	var.AddString(xMsg.chat_name());
	var.AddString(xMsg.chat_info());
	var.AddInt(xMsg.container_data_size());
	for (size_t i = 0; i < xMsg.container_data_size(); i++)
	{
		var.AddInt(xMsg.container_data(i).containertype());
		var.AddString(xMsg.container_data(i).data_info());
	}

	switch (xMsg.chat_type())
	{
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_WORLD:
	{
		DoEvent(E_ChatEvent_ChatWorld, var);
	}
	break;
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_GUILD:
	{
		DoEvent(E_ChatEvent_ChatGuild, var);
	}
	break;
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_PRIVATE:
	{
		DoEvent(E_ChatEvent_ChatPrivate, var);
	}
	break;
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_TEAM:
	{
		DoEvent(E_ChatEvent_ChatTeam, var);
	}
	break;
	default:
		break;;
	}
}
