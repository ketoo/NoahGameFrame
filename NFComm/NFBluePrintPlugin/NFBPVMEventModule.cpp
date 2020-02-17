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

//#include "Dependencies/googletest-release-1.8.0/googletest/include/gtest/gtest.h"
#include "NFBPVirtualMachineModule.h"
#include "NFBPVMEventModule.h"

bool NFBPVirtualMachineModule::Awake()
{


	return true;
}

bool NFBPVirtualMachineModule::Init()
{
    m_pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();

    return true;
}

bool NFBPVirtualMachineModule::AfterInit()
{

	return true;
}

bool NFBPVirtualMachineModule::CheckConfig()
{
	
	return true;
}

bool NFBPVirtualMachineModule::ReadyExecute()
{
	

	return true;
}

bool NFBPVirtualMachineModule::Execute()
{
	
    return true;
}

bool NFBPVirtualMachineModule::BeforeShut()
{

	return true;
}

bool NFBPVirtualMachineModule::Shut()
{
	

	return true;
}

bool NFBPVirtualMachineModule::Finalize()
{
	

	return true;
}

bool NFBPVirtualMachineModule::OnReloadPlugin()
{
	
	return true;
}

void NFBPVirtualMachineModule::RunLogicBlock(const NFGUID& logicBlockID)
{
    auto block = m_pBluePrintModule->GetLogicBlock(logicBlockID);
    auto vm = NF_SHARE_PTR<NFBPVirtualMachine>(NF_NEW NFBPVirtualMachine(pPluginManager, block));
    mVirtualMachine.AddElement(logicBlockID, vm);
}

void NFBPVirtualMachineModule::StopLogicBlock(const NFGUID& logicBlockID)
{
    auto block = m_pBluePrintModule->GetLogicBlock(logicBlockID);
    mVirtualMachine.RemoveElement(logicBlockID);
    //destroy that VM
}

bool NFBPVirtualMachineModule::CheckLogicBlockRefCircle(const NFGUID& logicBlockID)
{
    return false;
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

bool NFBPVMEventModule::UnRegisterAllCallBack(const NFGUID blockID)
{
	return mBluePrintBlockAction.RemoveElement(blockID);
}