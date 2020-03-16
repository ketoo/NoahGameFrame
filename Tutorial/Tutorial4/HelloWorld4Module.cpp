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

#include "HelloWorld4Module.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFCore/NFQueue.hpp"
#include "Dependencies/concurrentqueue/concurrentqueue.h"

bool NFHelloWorld4Module::Init()
{
	m_pActorModule = pPluginManager->FindModule<NFIActorModule>();
	m_pThreadPoolModule = pPluginManager->FindModule<NFIThreadPoolModule>();
	
	return true;
}

void NFHelloWorld4Module::RequestAsyEnd(NFActorMessage& actorMessage)
{
	//std::cout << "Main thread: " << std::this_thread::get_id() << " Actor: " << actorMessage.id.ToString() << " MsgID: " << actorMessage.msgID << " Data:" << actorMessage.data << std::endl;
}

bool NFHelloWorld4Module::AfterInit()
{
	std::cout << "Hello, world4, AfterInit, Main thread: " << std::this_thread::get_id() << std::endl;

	///////////////////////////
	std::cout << "start Benchmarks " << std::endl;
	//100M
	int messageCount = 1000000;
	{
		std::cout << "Test for ConcurrentQueue" << std::endl;
		moodycamel::ConcurrentQueue<int> q;

		std::thread threads[6];

		// Producers
		for (int i = 0; i != 4; ++i)
		{
			threads[i] = std::thread(
			[&]() 
			{

				int timeStart = NFGetTimeMS();

				for (int j = 0; j != messageCount; ++j)
				{
					q.enqueue(j);
				}

				int timeEnd = NFGetTimeMS();
				int timeCost = timeEnd - timeStart;
				if (timeCost > 0)
				{
					std::cout << "end Benchmarks, cost: " << timeCost << "ms for " << messageCount << ", qps: " << (messageCount / timeCost) * 1000 << std::endl;
				}
			});
		}

		// Consumers
		for (int i = 4; i != 6; ++i) {
			threads[i] = std::thread([&]() 
			{
				int item;
				for (int j = 0; j != 20; ++j)
				{
					//if (q.try_dequeue(item))
					{

					}
				}
			});
		}


		// Wait for all threads
		for (int i = 0; i != 6; ++i)
		{
			threads[i].join();
		}

		// Collect any leftovers (could be some if e.g. consumers finish before producers)
		int item;
		//while (q.try_dequeue(item))
		{

		}
	}

	{
		std::cout << "Test for NFQuene" << std::endl;
		NFQueue<int> q;

		std::thread threads[6];
		int threadCount = 2;
		// Producers
		for (int i = 0; i != threadCount; ++i)
		{
			threads[i] = std::thread(
				[&]()
			{

				int timeStart = NFGetTimeMS();

				for (int j = 0; j != messageCount; ++j)
				{
					q.Push(j);
				}

				int timeEnd = NFGetTimeMS();
				int timeCost = timeEnd - timeStart;
				if (timeCost > 0)
				{
					std::cout << "end Benchmarks, cost: " << timeCost << "ms for " << messageCount << ", qps: " << (messageCount / timeCost) * 1000 << std::endl;
				}
			});
		}

		// Wait for all threads
		for (int i = 0; i != threadCount; ++i)
		{
			threads[i].join();
		}
	}
	{
		std::cout << "Test for NFQuene std::string" << std::endl;
		NFQueue<std::string> q;

		std::thread threads[6];
		int threadCount = 2;
		// Producers
		for (int i = 0; i != threadCount; ++i)
		{
			threads[i] = std::thread(
				[&]()
			{

				int timeStart = NFGetTimeMS();

				for (int j = 0; j != messageCount; ++j)
				{
					q.Push(std::to_string(j * j));
				}

				int timeEnd = NFGetTimeMS();
				int timeCost = timeEnd - timeStart;
				if (timeCost > 0)
				{
					std::cout << "end Benchmarks, cost: " << timeCost << "ms for " << messageCount << ", qps: " << (messageCount / timeCost) * 1000 << std::endl;
				}
			});
		}

		// Wait for all threads
		for (int i = 0; i != threadCount; ++i)
		{
			threads[i].join();
		}
	}
	{
		std::cout << "Test for Task NO RESULT!" << std::endl;
		int timeStart = NFGetTimeMS();
		//example 4
		for (int i = 0; i < messageCount; ++i)
		{
			m_pThreadPoolModule->DoAsyncTask(NFGUID(), "sas",
				[&](NFThreadTask& task) -> void
			{
				//std::cout << "example 4 thread id: " << std::this_thread::get_id() << " task id:" << task.nTaskID.ToString() << " task data:" << task.data << std::endl;
				task.data = "aaaaaresulttttttt";
			});
		}

		int timeEnd = NFGetTimeMS();
		int timeCost = timeEnd - timeStart;
		if (timeCost > 0)
		{
			std::cout << "end Benchmarks, cost: " << timeCost << "ms for " << messageCount << ", qps: " << (messageCount / timeCost) * 1000 << std::endl;
		}
	}
	{
		std::cout << "Test for Task WITH RESULT!" << std::endl;
		int timeStart = NFGetTimeMS();
		//100M
		//example 4
		for (int i = 0; i < messageCount; ++i)
		{
			m_pThreadPoolModule->DoAsyncTask(NFGUID(), "sas",
				[&](NFThreadTask& task) -> void
			{
				//std::cout << "example 4 thread id: " << std::this_thread::get_id() << " task id:" << taskID.ToString() << " task data:" << strData << std::endl;
				task.data = "aaaaaresulttttttt";
			},
				[&](NFThreadTask& task) -> void
			{
				//std::cout << "example 4 thread id: " << std::this_thread::get_id() << " task id:" << taskID.ToString() << " task result:" << strData << std::endl;
			});
		}

		int timeEnd = NFGetTimeMS();
		int timeCost = timeEnd - timeStart;
		if (timeCost > 0)
		{
			std::cout << "end Benchmarks, cost: " << timeCost << "ms for " << messageCount << ", qps: " << (messageCount / timeCost) * 1000 << std::endl;
		}
	}

	//actor test
	{
		std::cout << "Test for actor mode" << std::endl;
		int timeStart = NFGetTimeMS();

		auto actorID1 = m_pActorModule->RequireActor();
		m_pActorModule->AddComponent<NFHttpComponent>(actorID1->ID());
		
		for (int i = 0; i < 5; ++i)
		{
			m_pActorModule->AddEndFunc(i, this, &NFHelloWorld4Module::RequestAsyEnd);
		}
		
		for (int i = 5; i < 10; ++i)
		{
			m_pActorModule->AddEndFunc(i, [](NFActorMessage& actorMessage) -> void
			{
				//std::cout << "example 2 AddEndFunc " << actorMessage.id.ToString() << " MSGID: " << actorMessage.msgID << std::endl;
			});
		}

		for (int i = 0; i < messageCount; ++i)
		{
			m_pActorModule->SendMsgToActor(actorID1->ID(), NFGUID(), i, "test");
			//m_pActorModule->SendMsgToActor(actorID1, i, std::to_string(i*i));
		}

		int timeEnd = NFGetTimeMS();
		int timeCost = timeEnd - timeStart;
		if (timeCost > 0)
		{
			std::cout << "end Benchmarks, cost: " << timeCost << "ms for " << messageCount << ", qps: " << (messageCount / timeCost) * 1000 << std::endl;
		}
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
