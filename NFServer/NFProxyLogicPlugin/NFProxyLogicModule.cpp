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
#include "NFProxyLogicPlugin.h"
#include "NFProxyLogicModule.h"

bool NFProxyLogicModule::Init()
{



    return true;
}

bool NFProxyLogicModule::Shut()
{
    return true;
}

bool NFProxyLogicModule::Execute()
{


    return true;
}


bool NFProxyLogicModule::AfterInit()
{
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNetModule = pPluginManager->FindModule<NFINetModule>();
	m_pNetClientModule = pPluginManager->FindModule<NFINetClientModule>();


	m_pNetModule->AddReceiveCallBack(NFMsg::REQ_LAG_TEST, this, &NFProxyLogicModule::OnLagTestProcess);

    return true;
}

void NFProxyLogicModule::OnLagTestProcess(const NFSOCK nSockIndex, const int nMsgID, const char * msg, const uint32_t nLen)
{
	std::string msgDatag(msg, nLen);
	m_pNetModule->SendMsgWithOutHead(NFMsg::EGameMsgID::ACK_GATE_LAG_TEST, msgDatag, nSockIndex);

	//TODO improve performance
	NetObject* pNetObject = m_pNetModule->GetNet()->GetNetObject(nSockIndex);
	if (pNetObject)
	{
		const int gameID = pNetObject->GetGameID();
		m_pNetClientModule->SendByServerIDWithOutHead(gameID, nMsgID, msgDatag);
	}
}
