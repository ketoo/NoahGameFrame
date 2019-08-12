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


#include "NFActorModule.h"

NFActorModule::NFActorModule(NFIPluginManager* p)
{
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
	mxActorMap.ClearAll();
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


NFGUID NFActorModule::RequireActor()
{
	NF_SHARE_PTR<NFIActor> pActor = NF_SHARE_PTR<NFIActor>(NF_NEW NFActor(m_pKernelModule->CreateGUID(), this));
	mxActorMap.AddElement(pActor->ID(), pActor);

	return pActor->ID();
}

NF_SHARE_PTR<NFIActor> NFActorModule::GetActor(const NFGUID nActorIndex)
{
	return mxActorMap.GetElement(nActorIndex);
}

bool NFActorModule::AddResult(const NFActorMessage & message)
{
	return mxResultQueue.Push(message);
}

bool NFActorModule::ExecuteEvent()
{
	for (auto it : mActorMessageCount)
	{
		const NFGUID id = it.first;
		NF_SHARE_PTR<NFIActor> pActor = GetActor(id);
		if (pActor)
		{
			m_pThreadPoolModule->DoAsyncTask("",
				[pActor](NFThreadTask& threadTask) -> void
			{
				pActor->Execute();
			});
		}
	}
	
	mActorMessageCount.clear();

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

bool NFActorModule::SendMsgToActor(const NFGUID nActorIndex, const int nEventID, const std::string& strArg)
{
    NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
    if (nullptr != pActor)
    {
        NFActorMessage xMessage;

		xMessage.id	= nActorIndex;
        xMessage.data = strArg;
        xMessage.msgID = nEventID;

		mActorMessageCount[pActor->ID()] = 1;

		return pActor->SendMsg(xMessage);
    }

    return false;
}

bool NFActorModule::AddComponent(const NFGUID nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent)
{
    NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
    if (nullptr != pActor)
    {
        pActor->AddComponent(pComponent);

        return true;
    }

    return false;
}

bool NFActorModule::RemoveComponent(const NFGUID nActorIndex, const std::string& strComponentName)
{
	return false;
}

NF_SHARE_PTR<NFIComponent> NFActorModule::FindComponent(const NFGUID nActorIndex, const std::string& strComponentName)
{
	return nullptr;
}

bool NFActorModule::ReleaseActor(const NFGUID nActorIndex)
{
	return mxActorMap.RemoveElement(nActorIndex);
}

bool NFActorModule::AddEndFunc(const int subMessageID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr_end)
{
	return mxEndFunctor.AddElement(subMessageID, functorPtr_end);
}