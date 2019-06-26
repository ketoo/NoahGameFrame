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

#include "NFChatModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFChatModule::Init()
{
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pGameServerToWorldModule = pPluginManager->FindModule<NFIGameServerToWorldModule>();

	return true;
}

bool NFChatModule::AfterInit()
{

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_CHAT, this, &NFChatModule::OnClienChatProcess);

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

void NFChatModule::OnClienChatProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAckPlayerChat);

	switch (xMsg.chat_type())
	{
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_WORLD:
	{
		m_pNetModule->SendMsgPBToAllClient(NFMsg::EGMI_ACK_CHAT, xMsg);
	}
	break;
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_CLAN:
	{
		NFGUID xTargetID = NFINetModule::PBToNF(xMsg.target_id());
		NFGUID xClanID = m_pKernelModule->GetPropertyObject(nPlayerID, NFrame::Player::Clan_ID());
		if (!xClanID.IsNull() && xClanID == xTargetID)
		{
			//send to world server
			m_pGameServerToWorldModule->TransmitToWorld(xClanID.nData64, nMsgID, xMsg);
		}
	}
	break;
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_PRIVATE:
	case NFMsg::ReqAckPlayerChat_EGameChatType::ReqAckPlayerChat_EGameChatType_EGCT_TEAM:
	{
		if (xMsg.has_target_id())
		{
			NFGUID xTargetID = NFINetModule::PBToNF(xMsg.target_id());
			if (!xTargetID.IsNull())
			{
				if (m_pKernelModule->GetObject(xTargetID))
				{

				}
				else
				{
					//send to world server
					m_pGameServerToWorldModule->TransmitToWorld(xTargetID.nData64, nMsgID, xMsg);
				}
			}
		}
	}
	break;
	default:
		break;;
	}
}