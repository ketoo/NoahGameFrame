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

#include "NFBPVirtualMachineModule.h"
#include "NFBPVMEventModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"

bool NFBPVMEventModule::Awake()
{
    m_pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();
    m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
    m_pEventModule = pPluginManager->FindModule<NFIEventModule>();
    m_pNetModule = pPluginManager->FindModule<NFINetModule>();
    
	return true;
}

bool NFBPVMEventModule::Init()
{
    return true;
}

bool NFBPVMEventModule::AfterInit()
{
    m_pKernelModule->RegisterCommonClassEvent(this, &NFBPVMEventModule::OnClassCommonEvent);
    m_pKernelModule->RegisterCommonPropertyEvent(this, &NFBPVMEventModule::OnPropertyCommonEvent);
    m_pKernelModule->RegisterCommonRecordEvent(this, &NFBPVMEventModule::OnRecordCommonEvent);

    m_pEventModule->AddCommonEventCallBack(this, &NFBPVMEventModule::OnEventCommonEvent);

    m_pNetModule->AddReceiveCallBack(this, &NFBPVMEventModule::OnMessageEvent);

	return true;
}

bool NFBPVMEventModule::CheckConfig()
{
	return true;
}

bool NFBPVMEventModule::ReadyExecute()
{
	return true;
}

bool NFBPVMEventModule::Execute()
{
    return true;
}

bool NFBPVMEventModule::BeforeShut()
{
	return true;
}

bool NFBPVMEventModule::Shut()
{
	return true;
}

bool NFBPVMEventModule::Finalize()
{
	return true;
}

bool NFBPVMEventModule::OnReloadPlugin()
{
	return true;
}

int NFBPVMEventModule::OnEventCommonEvent(const NFGUID& self, const int eventID, const NFDataList& var)
{
    NFMapEx<std::string, NFData> emptyMap;

    auto item = mBluePrintBlockAction.First();
    while (item)
    {
        auto eventData = item->mGameEvent.GetElement(eventID);
        if (eventData)
        {
            NFGUID monitorID;
            auto monitorData =  eventData->First(monitorID);
            while (monitorData && !monitorID.IsNull())
            {
                //const NFGUID& objectID, const  NFGUID& monitorID, const int eventI
                monitorData->operator()(self, monitorID, eventID, emptyMap);

                monitorID.SetHead(0);
                monitorID.SetData(0);
                monitorData = eventData->Next(monitorID);
            }
        }

        item = mBluePrintBlockAction.Next();
    }

    return 0;
}

int NFBPVMEventModule::OnClassCommonEvent(const NFGUID& self, const std::string& strClassNames, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var)
{
    for (auto it = mBluePrintBlockAction.First(); it; it = mBluePrintBlockAction.Next())
    {
        for (auto handler = it->mGameObjectEvent.First(); handler; handler = it->mGameObjectEvent.Next())
        {
        }
    }

    return 0;
}

int NFBPVMEventModule::OnPropertyCommonEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar)
{
    return 0;
}

int NFBPVMEventModule::OnRecordCommonEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    return 0;
}

void NFBPVMEventModule::OnMessageEvent(const NFSOCK nSockIndex, const int nMsgID, const char* msg, const uint32_t nLen)
{
    // unpack
    NFGUID playerID(1, 1);
    NFMapEx<std::string, NFData> emptyMap;

    auto item = mBluePrintBlockAction.First();
    while (item)
    {
        auto eventData = item->mNetMsgEvent.GetElement(nMsgID);
        if (eventData)
        {
            NFGUID monitorID;
            auto monitorData = eventData->First(monitorID);
            while (monitorData && !monitorID.IsNull())
            {
                //const NFGUID& objectID, const  NFGUID& monitorID, const int eventID
                monitorData->operator()(playerID, monitorID, nMsgID, emptyMap);

                monitorID.SetHead(0);
                monitorID.SetData(0);
                monitorData = eventData->Next(monitorID);
            }
        }

        item = mBluePrintBlockAction.Next();
    }
}

bool NFBPVMEventModule::RegisterGameEventCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor)
{
	auto blockAction = mBluePrintBlockAction.GetElement(blockID);
	if (!blockAction)
	{
		blockAction = NF_SHARE_PTR<BluePrintBlockAction>(NF_NEW BluePrintBlockAction());
		mBluePrintBlockAction.AddElement(blockID, blockAction);
	}

	auto xEventListPtr = blockAction->mGameEvent.GetElement(eventID);
    if (!xEventListPtr)
    {
        xEventListPtr = NF_SHARE_PTR<NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>>(NF_NEW NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>());
		blockAction->mGameEvent.AddElement(eventID, xEventListPtr);
    }

    auto eventFunctor = xEventListPtr->GetElement(monitorID);
    if (!eventFunctor)
    {
        xEventListPtr->AddElement(monitorID, NF_SHARE_PTR<BLUEPRINT_EVENT_FUNCTOR>(NF_NEW BLUEPRINT_EVENT_FUNCTOR(functor)));
        return true;
    }

    return false;
}

bool NFBPVMEventModule::RegisterNetEventCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor)
{
	auto blockAction = mBluePrintBlockAction.GetElement(blockID);
	if (!blockAction)
	{
		blockAction = NF_SHARE_PTR<BluePrintBlockAction>(NF_NEW BluePrintBlockAction());
		mBluePrintBlockAction.AddElement(blockID, blockAction);
	}

	auto xEventListPtr = blockAction->mNetEvent.GetElement(eventID);
    if (!xEventListPtr)
    {
        xEventListPtr = NF_SHARE_PTR<NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>>(NF_NEW NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>());
		blockAction->mNetEvent.AddElement(eventID, xEventListPtr);
    }

    auto eventFunctor = xEventListPtr->GetElement(monitorID);
    if (!eventFunctor)
    {
        xEventListPtr->AddElement(monitorID, NF_SHARE_PTR<BLUEPRINT_EVENT_FUNCTOR>(NF_NEW BLUEPRINT_EVENT_FUNCTOR(functor)));
        return true;
    }

    return false;
}

bool NFBPVMEventModule::RegisterNetMsgEventCallBack(const NFGUID blockID, const int eventID, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor)
{
	auto blockAction = mBluePrintBlockAction.GetElement(blockID);
	if (!blockAction)
	{
		blockAction = NF_SHARE_PTR<BluePrintBlockAction>(NF_NEW BluePrintBlockAction());
		mBluePrintBlockAction.AddElement(blockID, blockAction);
	}

    auto xEventListPtr = blockAction->mNetMsgEvent.GetElement(eventID);
    if (!xEventListPtr)
    {
        xEventListPtr = NF_SHARE_PTR<NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>>(NF_NEW NFMapEx<NFGUID, BLUEPRINT_EVENT_FUNCTOR>());
		blockAction->mNetMsgEvent.AddElement(eventID, xEventListPtr);
    }

    auto eventFunctor = xEventListPtr->GetElement(monitorID);
    if (!eventFunctor)
    {
        xEventListPtr->AddElement(monitorID, NF_SHARE_PTR<BLUEPRINT_EVENT_FUNCTOR>(NF_NEW BLUEPRINT_EVENT_FUNCTOR(functor)));
        return true;
    }

    return false;
}

bool NFBPVMEventModule::RegisterGameObjectEventCallBack(const NFGUID blockID, const std::string& className, const NFGUID monitorID, const BLUEPRINT_EVENT_FUNCTOR& functor)
{

    return false;
}

bool NFBPVMEventModule::UnRegisterAllCallBack(const NFGUID blockID)
{
	return mBluePrintBlockAction.RemoveElement(blockID);
}