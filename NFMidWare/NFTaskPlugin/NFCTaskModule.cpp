/*
            This file is part of: 
                NoahFrame
            http://noahframe.com

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is opensorece software and you can redistribute it and/or modify
   it under the terms of the License.

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

#include "NFCTaskModule.h"

bool NFCTaskModule::Init()
{
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

bool NFCTaskModule::Shut()
{
    return true;
}

bool NFCTaskModule::Execute()
{
    return true;
}

bool NFCTaskModule::AfterInit()
{

	
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCTaskModule::OnClassObjectEvent );
	
	CheckConfig();

	//////////////////////////////////////////////////////////////////////////
	// add msg handler
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_ACCEPT_TASK, this, &NFCTaskModule::OnClientAcceptTask)) { return false; }
    if (!m_pNetModule->AddReceiveCallBack(NFMsg::EGMI_REQ_COMPELETE_TASK, this, &NFCTaskModule::OnClientPushTask)) { return false; }

    return true;
}

int NFCTaskModule::OnClassObjectEvent( const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var )
{
    if ( CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent )
    {

    }
    else if ( CLASS_OBJECT_EVENT::COE_CREATE_NODATA == eClassEvent )
    {
    }

    return 0;
}

void NFCTaskModule::OnClientAcceptTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqAcceptTask)
}

void NFCTaskModule::OnClientPushTask(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    CLIENT_MSG_PROCESS( nMsgID, msg, nLen, NFMsg::ReqCompeleteTask)
}