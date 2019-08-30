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
	m_pWorldNet_ServerModule = pPluginManager->FindModule<NFIWorldNet_ServerModule>();


    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_FRIEND_LIST, this, &NFFriendModule::OnReqFriendListProcess)) { return false; }

    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_SEND_INVITE, this, &NFFriendModule::OnReqSendInviteProcess)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_ACCEPT_INVITE, this, &NFFriendModule::OnReqAcceptInviteProcess)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_REJECT_INVITE, this, &NFFriendModule::OnReqRejectInviteProcess)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_IGNORE_INVITE, this, &NFFriendModule::OnReqIgnoreInviteProcess)) { return false; }

    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_BLOCK_PLAYER, this, &NFFriendModule::OnReqBlockPlayerProcess)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_UNBLOCK_PLAYER, this, &NFFriendModule::OnReqUnBlockPlayerProcess)) { return false; }

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

void NFFriendModule::OnReqFriendListProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckFriendList)
    
    std::cout << nPlayerID.ToString() << std::endl;

    std::vector<NFIFriendRedisModule::FriendData> friendList;
    m_pFriendRedisModule->GetFriendsList(nPlayerID, friendList);

    std::vector<NFIFriendRedisModule::FriendData> inviteList;
    m_pFriendRedisModule->GetInvitationList(nPlayerID, inviteList);

    for (int i = 0; i < friendList.size(); ++i)
    {
        NFMsg::FriendData* pData = xMsg.add_friendlist();
        if (pData)
        {
            pData->set_name(friendList[i].name);
            *pData->mutable_id() = NFINetModule::NFToPB(friendList[i].id);
        }
    }

    for (int i = 0; i < inviteList.size(); ++i)
    {
        NFMsg::FriendData* pData = xMsg.add_invitelist();
        if (pData)
        {
            pData->set_name(inviteList[i].name);
            *pData->mutable_id() = NFINetModule::NFToPB(inviteList[i].id);
        }
    }

    m_pNetModule->SendMsgPB(NFMsg::EGMI_REQ_ACK_FRIEND_LIST, xMsg, nSockIndex, nPlayerID);
}

void NFFriendModule::OnReqDeleteFriendProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckDeleteFriend)

    NFGUID stranger = NFINetModule::PBToNF(xMsg.id());

    m_pFriendRedisModule->DeleteFriend(nPlayerID, stranger);
}

void NFFriendModule::OnReqSendInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckSendInvite)

    NFGUID stranger = NFINetModule::PBToNF(xMsg.id());
    const std::string& name = xMsg.name();
    m_pFriendRedisModule->SendInvite(nPlayerID, name, stranger);

    NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pStrangerData = m_pWorldNet_ServerModule->GetPlayerData(stranger);
    NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(nPlayerID);
    if (pPlayerData && pStrangerData)
    {
        //send message to the game server
        NFMsg::ReqAckFriendList xReqAckFriendList;
        NFMsg::FriendData* pData = xReqAckFriendList.add_invitelist();
        if (pData)
        {
            pData->set_name(pPlayerData->name);
            *(pData->mutable_id()) = NFINetModule::NFToPB(nPlayerID);
            m_pWorldNet_ServerModule->SendMsgToGame(stranger, NFMsg::EGMI_ACK_INVITE_ADD, xReqAckFriendList);
        }
    }
}

void NFFriendModule::OnReqAcceptInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckAcceptInvite)

    NFGUID stranger = NFINetModule::PBToNF(xMsg.id());
    if (m_pFriendRedisModule->AcceptInvite(nPlayerID, "", stranger))
    {
        NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pStrangerData = m_pWorldNet_ServerModule->GetPlayerData(stranger);
        NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(nPlayerID);
        if (pPlayerData)
        {
            //send message to the game server
            NFMsg::ReqAckFriendList xReqAckFriendList;
            NFMsg::FriendData* pData = xReqAckFriendList.add_friendlist();
            if (pData)
            {
                if (pStrangerData)
                {
                    pData->set_name(pStrangerData->name);
                }
                *(pData->mutable_id()) = NFINetModule::NFToPB(stranger);
                m_pWorldNet_ServerModule->SendMsgToGame(stranger, NFMsg::EGMI_ACK_FRIEND_ADD, xReqAckFriendList);
            }
        }

        //send message to the game server
        if (pStrangerData)
        {
            //send message to the game server       
            NFMsg::ReqAckFriendList xReqAckFriendList;
            NFMsg::FriendData* pData = xReqAckFriendList.add_friendlist();
            if (pData)
            {
                if (pPlayerData)
                {
                    pData->set_name(pPlayerData->name);
                }
                *(pData->mutable_id()) = NFINetModule::NFToPB(nPlayerID);
                m_pWorldNet_ServerModule->SendMsgToGame(stranger, NFMsg::EGMI_ACK_FRIEND_ADD, xReqAckFriendList);
            }
        }


    }
}

void NFFriendModule::OnReqRejectInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
   CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckRejectInvite)

    NFGUID stranger = NFINetModule::PBToNF(xMsg.id());

    NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(nPlayerID);
    if (pPlayerData)
    {
        m_pFriendRedisModule->RejectInvite(nPlayerID, stranger);
    }
}

void NFFriendModule::OnReqIgnoreInviteProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
   CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckIgnoreInvite)

    NFGUID stranger = NFINetModule::PBToNF(xMsg.id());

    NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(nPlayerID);
    if (pPlayerData)
    {
        m_pFriendRedisModule->IgnoreInvite(nPlayerID, stranger);
    }
}

void NFFriendModule::OnReqBlockPlayerProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
   CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckBlockPlayer)

    NFGUID stranger = NFINetModule::PBToNF(xMsg.id());

    NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(nPlayerID);
    if (pPlayerData)
    {
        m_pFriendRedisModule->BlockPlayer(nPlayerID, stranger);
    }
}

void NFFriendModule::OnReqUnBlockPlayerProcess(const NFSOCK nSockIndex, const int nMsgID, const char *msg, const uint32_t nLen)
{
   CLIENT_MSG_PROCESS_NO_OBJECT(nMsgID, msg, nLen, NFMsg::ReqAckBlockPlayer)

    NFGUID stranger = NFINetModule::PBToNF(xMsg.id());

    NF_SHARE_PTR<NFIWorldNet_ServerModule::PlayerData> pPlayerData = m_pWorldNet_ServerModule->GetPlayerData(nPlayerID);
    if (pPlayerData)
    {
        m_pFriendRedisModule->UnBlockPlayer(nPlayerID, stranger);
    }
}