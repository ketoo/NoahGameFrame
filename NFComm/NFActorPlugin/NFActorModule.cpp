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


#include "NFActorModule.h"

NFActorModule::NFActorModule(NFIPluginManager* p)
{
    m_bIsExecute = true;
	pPluginManager = p;

    srand((unsigned)time(NULL));
}

NFActorModule::~NFActorModule()
{
}

bool NFActorModule::Init()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pThreadPoolModule = pPluginManager->FindModule<NFIThreadPoolModule>();

    return true;
}

bool NFActorModule::AfterInit()
{

    return true;
}

bool NFActorModule::BeforeShut()
{
	mxActorMap.clear();
    return true;
}

bool NFActorModule::Shut()
{
 
    return true;
}

bool NFActorModule::Execute()
{
	ExecuteEvent();
	ExecuteResultEvent();

    return true;
}


NF_SHARE_PTR<NFIActor> NFActorModule::RequireActor()
{
	NF_SHARE_PTR<NFIActor> pActor = NF_SHARE_PTR<NFIActor>(NF_NEW NFActor(m_pKernelModule->CreateGUID(), this));
	mxActorMap.insert(std::map<NFGUID, NF_SHARE_PTR<NFIActor>>::value_type(pActor->ID(), pActor));

	return pActor;
}

NF_SHARE_PTR<NFIActor> NFActorModule::GetActor(const NFGUID nActorIndex)
{
	auto it = mxActorMap.find(nActorIndex);
	if (it != mxActorMap.end())
	{
		return it->second;
	}

	return nullptr;
}

bool NFActorModule::AddResult(const NFActorMessage & message)
{
	return mxResultQueue.Push(message);
}

bool NFActorModule::ExecuteEvent()
{
	static int64_t lastTime = 0;
	int64_t nowTime = NFGetTimeMS();
	if (nowTime < lastTime + 10)
	{
		return false;
	}

	lastTime = nowTime;

	for (auto it : mxActorMap)
	{
		NF_SHARE_PTR<NFIActor> pActor = it.second;
		if (pActor)
		{
			if (test)
			{
				pActor->Execute();
			}
			else
			{
				m_pThreadPoolModule->DoAsyncTask(pActor->ID(), "",
					[pActor](NFThreadTask& threadTask) -> void
					{
						pActor->Execute();
					});
			}
		}
	}

	return true;
}

bool NFActorModule::ExecuteResultEvent()
{
	NFActorMessage actorMessage;
	while (mxResultQueue.try_dequeue(actorMessage))
	{
		ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end = mxEndFunctor.GetElement(actorMessage.msgID);
		if (functorPtr_end)
		{
			functorPtr_end->operator()(actorMessage);
		}
	}
	
	return true;
}

bool NFActorModule::SendMsgToActor(const NFGUID actorIndex, const NFGUID who, const int eventID, const std::string& data, const std::string& arg)
{
	static uint64_t index = 0;
    NF_SHARE_PTR<NFIActor> pActor = GetActor(actorIndex);
    if (nullptr != pActor)
    {
        NFActorMessage xMessage;

		xMessage.id = who;
		xMessage.index	= index++;
        xMessage.data = data;
		xMessage.msgID = eventID;
		xMessage.arg = arg;


		return this->SendMsgToActor(actorIndex, xMessage);
    }

    return false;
}

bool NFActorModule::ReleaseActor(const NFGUID nActorIndex)
{
	auto it = mxActorMap.find(nActorIndex);
	if (it != mxActorMap.end())
	{
		mxActorMap.erase(it);
	
		return true;
	}
	
	return false;
}

bool NFActorModule::AddEndFunc(const int subMessageID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end)
{
	return mxEndFunctor.AddElement(subMessageID, functorPtr_end);
}

bool NFActorModule::SendMsgToActor(const NFGUID actorIndex, const NFActorMessage &message)
{
	auto it = mxActorMap.find(actorIndex);
	if (it != mxActorMap.end())
	{
		//std::cout << "send message " << message.msgID << " to " << actorIndex.ToString() << " and msg index is " << message.index << std::endl;
		return it->second->SendMsg(message);
	}

	return false;
}
