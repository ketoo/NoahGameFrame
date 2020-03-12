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
    if (block)
    {
        auto vm = NF_SHARE_PTR<NFBPVirtualMachine>(NF_NEW NFBPVirtualMachine(pPluginManager, block));
        if (vm)
        {
            mVirtualMachine.AddElement(logicBlockID, vm);

            vm->Awake();
            vm->Init();
            vm->AfterInit();
            vm->ReadyExecute();
        }
    }
}

void NFBPVirtualMachineModule::StopLogicBlock(const NFGUID& logicBlockID)
{
    auto block = m_pBluePrintModule->GetLogicBlock(logicBlockID);
    if (block)
    {
    }
    auto vm = mVirtualMachine.GetElement(logicBlockID);
    if (vm)
    {
        vm->BeforeShut();
        vm->Shut();

        mVirtualMachine.RemoveElement(logicBlockID);
    }
    //destroy that VM
}

bool NFBPVirtualMachineModule::CheckLogicBlockRefCircle(const NFGUID& logicBlockID)
{
    return false;
}
