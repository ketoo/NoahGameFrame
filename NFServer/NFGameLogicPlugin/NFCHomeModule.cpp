/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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

#include "NFCHomeModule.h"

bool NFCHomeModule::Init()
{

	return true;
}

bool NFCHomeModule::Shut()
{
	return true;
}

bool NFCHomeModule::Execute()
{
	return true;
}

bool NFCHomeModule::AfterInit()
{
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pPlayerRedisModule = pPluginManager->FindModule<NFIPlayerRedisModule>();
	m_pScheduleModule = pPluginManager->FindModule<NFIScheduleModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pSceneAOIModule = pPluginManager->FindModule<NFISceneAOIModule>();
	m_pSceneProcessModule = pPluginManager->FindModule<NFISceneProcessModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pRankModule = pPluginManager->FindModule<NFIRankModule>();

	m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACK_RELIVE, this, &NFCHomeModule::OnRequireReliveProcess);

	return true;
}

void NFCHomeModule::Relive(const NFGUID & self)
{
	NFMsg::ReqAckRelive xMsg;

	int nHP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::HP());
	int nMAXHP = m_pKernelModule->GetPropertyInt(self, NFrame::Player::MAXHP());
	NFMsg::EPVPType ePvpType = (NFMsg::EPVPType)m_pKernelModule->GetPropertyInt(self, NFrame::Player::PVPType());
	if (ePvpType == NFMsg::EPVPType::PVP_HOME
		&& nHP <= 0
		&& nMAXHP > 0)
	{
		m_pKernelModule->SetPropertyInt(self, NFrame::Player::HP(), nMAXHP);

		xMsg.set_diamond(1);
		m_pGameServerNet_ServerModule->SendMsgPBToGate(NFMsg::EGMI_REQ_ACK_RELIVE, xMsg, self);
	}
}

void NFCHomeModule::OnRequireReliveProcess(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
	NFGUID nPlayerID;
	NFMsg::ReqAckRelive xMsg;
	if (!m_pNetModule->ReceivePB(nMsgID, msg, nLen, xMsg, nPlayerID))
	{
		return;
	}

	Relive(nPlayerID);
}
