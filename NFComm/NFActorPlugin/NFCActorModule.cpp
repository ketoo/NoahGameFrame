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


#include "NFCActorModule.h"

NFCActorModule::NFCActorModule(NFIPluginManager* p)
	:mFramework(NF_ACTOR_THREAD_COUNT)
{
	pPluginManager = p;

    srand((unsigned)time(NULL));

    m_pMainActor = NF_SHARE_PTR<NFIActor>(NF_NEW NFCActor(mFramework, this));
}

NFCActorModule::~NFCActorModule()
{
	m_pMainActor.reset();
	m_pMainActor = nullptr;
}

bool NFCActorModule::Init()
{
    return true;
}

bool NFCActorModule::AfterInit()
{

    return true;
}

bool NFCActorModule::BeforeShut()
{
	mxActorMap.ClearAll();
    return true;
}

bool NFCActorModule::Shut()
{
 
    return true;
}

bool NFCActorModule::Execute()
{
	ExecuteEvent();
    return true;
}


int NFCActorModule::RequireActor()
{
    NF_SHARE_PTR<NFIActor> pActor(NF_NEW NFCActor(mFramework, this));
	mxActorMap.AddElement(pActor->GetAddress().AsInteger(), pActor);

    return pActor->GetAddress().AsInteger();
}

NF_SHARE_PTR<NFIActor> NFCActorModule::GetActor(const int nActorIndex)
{
	return mxActorMap.GetElement(nActorIndex);
}

bool NFCActorModule::HandlerEx(const NFIActorMessage & message, const int from)
{
	if (message.msgType != NFIActorMessage::ACTOR_MSG_TYPE_COMPONENT)
	{
		return mxQueue.Push(message);
	}

	return false;
}

bool NFCActorModule::ExecuteEvent()
{
	NFIActorMessage xMsg;
	bool bRet = false;
	bRet = mxQueue.Pop(xMsg);
	while (bRet)
	{
		if (xMsg.msgType != NFIActorMessage::ACTOR_MSG_TYPE_COMPONENT && xMsg.xEndFuncptr != nullptr)
		{
			ACTOR_PROCESS_FUNCTOR* pFun = xMsg.xEndFuncptr.get();
			pFun->operator()(xMsg.nFormActor, xMsg.nMsgID, xMsg.data);

			//Actor can be reused in ActorPool mode, so we don't release it.
			//m_pActorManager->ReleaseActor(xMsg.nFormActor);
		}

		bRet = mxQueue.Pop(xMsg);
	}

	return true;
}

bool NFCActorModule::SendMsgToActor(const int nActorIndex, const int nEventID, const std::string& strArg)
{
    NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
    if (nullptr != pActor)
    {
        NFIActorMessage xMessage;

		xMessage.msgType = NFIActorMessage::ACTOR_MSG_TYPE_COMPONENT;
        xMessage.data = strArg;
        xMessage.nMsgID = nEventID;
        xMessage.nFormActor = m_pMainActor->GetAddress().AsInteger();

        return mFramework.Send(xMessage, m_pMainActor->GetAddress(), pActor->GetAddress());
    }

    return false;
}

bool NFCActorModule::AddComponent(const int nActorIndex, NF_SHARE_PTR<NFIComponent> pComponent)
{
    NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
    if (nullptr != pActor)
    {
        pActor->AddComponent(pComponent);

        return true;
    }

    return false;
}

bool NFCActorModule::ReleaseActor(const int nActorIndex)
{
	return mxActorMap.RemoveElement(nActorIndex);
}

bool NFCActorModule::AddEndFunc(const int nActorIndex, const int nSubMsgID, ACTOR_PROCESS_FUNCTOR_PTR functorPtr)
{
    NF_SHARE_PTR<NFIActor> pActor = GetActor(nActorIndex);
    if (nullptr != pActor)
    {
		return pActor->AddEndFunc(nSubMsgID, functorPtr);
    }

    return false;
}