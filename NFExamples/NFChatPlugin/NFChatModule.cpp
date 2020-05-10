/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

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

#include "NFChatModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFChatModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

	return true;
}

bool NFChatModule::AfterInit()
{
	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_CHAT, this, &NFChatModule::OnClientChatProcess);

	return true;
}

bool NFChatModule::Shut()
{

	return true;
}

bool NFChatModule::Execute()
{
	return true;
}

void NFChatModule::OnClientChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckPlayerChat);

	switch (xMsg.chat_channel())
	{
	case NFMsg::ReqAckPlayerChat::EGCC_GLOBAL:
	{
		//this code means the game server will sends a message to all players who playing game
		m_pNetModule->SendMsgPBToAllClient(NFMsg::ACK_CHAT, xMsg);
	}
	break;
	case NFMsg::ReqAckPlayerChat::EGCC_ROOM:
	{
		const int sceneID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::SceneID());
		const int groupID = m_pKernelModule->GetPropertyInt(nPlayerID, NFrame::Player::GroupID());

		//this code means the game server will sends a message to all players who in the same room
		m_pGameServerNet_ServerModule->SendGroupMsgPBToGate(NFMsg::ACK_CHAT, xMsg, sceneID, groupID);
	}
	break;
	default:
	{
		//this code means the game server will sends a message yourself(nPlayerID)
		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::ACK_CHAT, xMsg, nPlayerID);
	}
	break;;
	}
}