/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/


#include "NFFriendModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFFriendModule::Init()
{
	return true;
}

bool NFFriendModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pFriendRedisModule = pPluginManager->FindModule<NFIFriendRedisModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();


    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_FRIEND_LIST, this, &NFFriendModule::OnReqFriendList)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_INVITE_LIST, this, &NFFriendModule::OnReqInviteList)) { return false; }

    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_SEND_INVITE, this, &NFFriendModule::OnReqSendInvite)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_ACCEPT_INVITE, this, &NFFriendModule::OnReqAcceptInvite)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_REJECT_INVITE, this, &NFFriendModule::OnReqRejectInvite)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_IGNORE_INVITE, this, &NFFriendModule::OnReqIgnoreInvite)) { return false; }

    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_BLOCK_PLAYER, this, &NFFriendModule::OnReqBlockPlayer)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_UNBLOCK_PLAYER, this, &NFFriendModule::OnReqUnBlockPlayer)) { return false; }

    /*
	EGMI_REQ_ACK_FRIEND_LIST = 50001;
	EGMI_REQ_ACK_INVITE_LIST = 50002;

	EGMI_REQ_ACK_SEND_INVITE = 50010;
	EGMI_REQ_ACK_ACCEPT_INVITE = 50011;
	EGMI_REQ_ACK_REJECT_INVITE = 50012;
	EGMI_REQ_ACK_IGNORE_INVITE = 50013;

	EGMI_REQ_ACK_BLOCK_PLAYER = 50020;
	EGMI_REQ_ACK_UNBLOCK_PLAYER = 50021;
*/
	return true;
}

bool NFFriendModule::Shut()
{

	return true;
}

bool NFFriendModule::Execute()
{
	return true;
}

void NFFriendModule::GetFriendsList(const NFGUID& self, NFList<NFIFriendRedisModule::FriendData>& friendList)
{
    m_pFriendRedisModule->GetFriendsList(self, friendList);
}

void NFFriendModule::GetInvitationList(const NFGUID& self, NFList<NFIFriendRedisModule::FriendData>& friendList)
{
    m_pFriendRedisModule->GetInvitationList(self, friendList);
}

void NFFriendModule::DeleteFriend(const NFGUID& self, const NFGUID& other)
{
    m_pFriendRedisModule->DeleteFriend(self, other);
}

void NFFriendModule::SendInvite(const NFGUID& self, const std::string& selfName, const NFGUID& stranger)
{
    m_pFriendRedisModule->SendInvite(self, selfName, stranger);
}

void NFFriendModule::AcceptInvite(const NFGUID& self, const std::string& selfName, const NFGUID& inviter)
{
    m_pFriendRedisModule->AcceptInvite(self, selfName, inviter);
}

void NFFriendModule::IgnoreInvite(const NFGUID& self, const NFGUID& inviter)
{
    m_pFriendRedisModule->IgnoreInvite(self, inviter);
}

void NFFriendModule::RejectInvite(const NFGUID& self, const std::string& selfName, const NFGUID& inviter)
{
    m_pFriendRedisModule->RejectInvite(self, inviter);
}

void NFFriendModule::BlockPlayer(const NFGUID& self, const NFGUID& other)
{
    m_pFriendRedisModule->BlockPlayer(self, other);
}

void NFFriendModule::UnBlockPlayer(const NFGUID& self, const NFGUID& other)
{
    m_pFriendRedisModule->UnBlockPlayer(self, other);
}

void NFFriendModule::OnReqFriendList(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckFriendList)

    std::cout << nPlayerID.ToString() << std::endl;
    //xMsg.
    //nPlayerID
}

void NFFriendModule::OnReqInviteList(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckInviteList)

}


void NFFriendModule::OnReqSendInvite(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{

}

void NFFriendModule::OnReqAcceptInvite(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{

}

void NFFriendModule::OnReqRejectInvite(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{

}

void NFFriendModule::OnReqIgnoreInvite(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{

}

void NFFriendModule::OnReqBlockPlayer(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{

}

void NFFriendModule::OnReqUnBlockPlayer(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{

}