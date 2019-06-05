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

#include "NFTaskModule.h"

bool NFTaskModule::Init()
{
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pPackModule = pPluginManager->FindModule<NFIPackModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pPropertyModule = pPluginManager->FindModule<NFIPropertyModule>();
	m_pHeroModule = pPluginManager->FindModule<NFIHeroModule>();
	m_pCommonConfigModule = pPluginManager->FindModule<NFICommonConfigModule>();
	m_pGameServerNet_ServerModule = pPluginManager->FindModule<NFIGameServerNet_ServerModule>();

    return true;
}

bool NFTaskModule::Shut()
{
    return true;
}

bool NFTaskModule::Execute()
{
    return true;
}

bool NFTaskModule::AfterInit()
{

	
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFTaskModule::OnClassObjectEvent );
	
	CheckConfig();

	//////////////////////////////////////////////////////////////////////////
	// add msg handler
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACCEPT_TASK, this, &NFTaskModule::OnClientAcceptTask)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_COMPELETE_TASK, this, &NFTaskModule::OnClientPushTask)) { return false; }

    return true;
}

int NFTaskModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {

    }
	else if (CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent)
	{
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		m_pKernelModule->AddPropertyCallBack(self, NFrame::Player::Level(), this, &NFTaskModule::OnPlayerLevelEvent);
	}

    return 0;
}

int NFTaskModule::OnPlayerLevelEvent(const NFGUID & self, const std::string & strPropertyName, const NFData & oldVar, const NFData & newVar)
{
	return 0;
}

void NFTaskModule::OnClientAcceptTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAcceptTask)
}

void NFTaskModule::OnClientPushTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqCompeleteTask)
}