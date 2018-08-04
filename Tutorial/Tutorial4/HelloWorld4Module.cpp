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

#include "HelloWorld4Module.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFCHelloWorld4Module::Init()
{
	m_pActorModule = pPluginManager->FindModule<NFIActorModule>();

	return true;
}

int NFCHelloWorld4Module::RequestAsyEnd(const int nFormActor, const int nSubMsgID, const std::string& strData)
{
	std::cout << "Main thread: " << std::this_thread::get_id() << " Actor: " << nFormActor << " MsgID: " << nSubMsgID << " Data:" << strData << std::endl;

	int nActorID2 = m_pActorModule->RequireActor();
	return 0;
}

bool NFCHelloWorld4Module::AfterInit()
{
	
	std::cout << "Hello, world4, AfterInit" << std::endl;


	int nActorID = m_pActorModule->RequireActor();
	m_pActorModule->AddComponent<NFHttpComponent>(nActorID);
	m_pActorModule->AddDefaultEndFunc(nActorID, this, &NFCHelloWorld4Module::RequestAsyEnd);

	for (int i = 0; i < 10; ++i)
	{
		m_pActorModule->SendMsgToActor(nActorID, i, "Test actor!");
	}

	std::cout << "Hello, world4, AfterInit end" << std::endl;
	return true;
}

bool NFCHelloWorld4Module::Execute()
{
	
	//std::cout << "Hello, world4, Execute" << std::endl;

	return true;
}

bool NFCHelloWorld4Module::BeforeShut()
{
	
	std::cout << "Hello, world4, BeforeShut" << std::endl;

	return true;
}

bool NFCHelloWorld4Module::Shut()
{
	
	std::cout << "Hello, world4, Shut" << std::endl;

	return true;
}
