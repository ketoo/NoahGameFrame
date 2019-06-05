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

#include "HelloWorld4Module.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"

bool NFHelloWorld4Module::Init()
{
	m_pActorModule = pPluginManager->FindModule<NFIActorModule>();
	m_pThreadPoolModule = pPluginManager->FindModule<NFIThreadPoolModule>();
	
	return true;
}

int NFHelloWorld4Module::RequestAsyEnd(const int nFormActor, const int nSubMsgID, const std::string& strData)
{
	std::cout << "Main thread: " << std::this_thread::get_id() << " Actor: " << nFormActor << " MsgID: " << nSubMsgID << " Data:" << strData << std::endl;

	//int nActorID2 = m_pActorModule->RequireActor();
	return 0;
}

bool NFHelloWorld4Module::AfterInit()
{

	std::cout << "Hello, world4, AfterInit, Main thread: " << std::this_thread::get_id() << std::endl;

	/*
	//example 1
	int nActorID1 = m_pActorModule->RequireActor();
	m_pActorModule->AddComponent<NFHttpComponent>(nActorID1);
	m_pActorModule->AddDefaultEndFunc(nActorID1, this, &NFHelloWorld4Module::RequestAsyEnd);


	for (int i = 0; i < 10; ++i)
	{
		m_pActorModule->SendMsgToActor(nActorID1, i, "Test actor!");
	}


	//example 2
	int nActorID2 = m_pActorModule->RequireActor();
	m_pActorModule->AddComponent<NFHttpComponent>(nActorID2);
	m_pActorModule->AddEndFunc(nActorID2, 1, [nActorID2](const int nFormActor, const int nSubMsgID, std::string& strData) -> int
	{
		std::cout << "example 2 AddEndFunc" << nActorID2 << std::endl;
		return nSubMsgID;
	});

	for (int i = 0; i < 10; ++i)
	{
		m_pActorModule->SendMsgToActor(nActorID2, i, "Test actor!");
	}

	//example 3
	int nActorID3 = m_pActorModule->RequireActor();
	m_pActorModule->AddAsyncFunc(nActorID3, 1,
		[nActorID3](const int nFormActor, const int nSubMsgID, std::string& strData) -> int
	{
		std::cout << "example 3 Async " << nActorID3 << " " << nSubMsgID << std::endl;
		return nSubMsgID;
	},
		[nActorID3](const int nFormActor, const int nSubMsgID, std::string& strData) -> int
	{
		std::cout << "example 3 end " << nActorID3 << " " << nSubMsgID  <<  std::endl;
		return nSubMsgID;
	});

	for (int i = 0; i < 10; ++i)
	{
		m_pActorModule->SendMsgToActor(nActorID3, i, "Test actor!");
	}
	*/

	//example 4
	for (int i = 0; i < 100; ++i)
	{
		m_pThreadPoolModule->DoAsyncTask("sas",
			[](const NFGUID taskID, const std::string& strData) -> std::string
		{
			std::cout << "example 4 thread id: " << std::this_thread::get_id() << " task id:" << taskID.ToString() << " task data:" << strData << std::endl;
			return "aaaaaresulttttttt";
		},
			[](const NFGUID taskID, const std::string& strData) -> void
		{
			std::cout << "example 4 thread id: " << std::this_thread::get_id() << " task id:" << taskID.ToString() << " task result:" << strData << std::endl;
		});
	}

	//example 5
	for (int i = 0; i < 100; ++i)
	{
		m_pThreadPoolModule->DoAsyncTask(i, NFGUID(0, i), "sas",
			[](const NFGUID taskID, const std::string& strData) -> std::string
		{
			std::cout << "example 5 thread id: " << std::this_thread::get_id() << " task id:" << taskID.ToString() << " task data:" << strData << std::endl;
			return "aaaaaresulttttttt";
		},
			[](const NFGUID taskID, const std::string& strData) -> void
		{
			std::cout << "example 5 thread id: " << std::this_thread::get_id() << " task id:" << taskID.ToString() << " task result:" << strData << std::endl;
		});
	}


	std::cout << "Hello, world4, AfterInit end" << std::endl;
	return true;
}

bool NFHelloWorld4Module::Execute()
{
	
	//std::cout << "Hello, world4, Execute" << std::endl;

	return true;
}

bool NFHelloWorld4Module::BeforeShut()
{
	
	std::cout << "Hello, world4, BeforeShut" << std::endl;

	return true;
}

bool NFHelloWorld4Module::Shut()
{
	
	std::cout << "Hello, world4, Shut" << std::endl;

	return true;
}
