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

#ifndef NF_BLUE_PRINT_VIRTUAL_MACHINE_MODULE_H
#define NF_BLUE_PRINT_VIRTUAL_MACHINE_MODULE_H

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIBluePrintModule.h"
#include "NFComm/NFPluginModule/NFIBPVirtualMachineModule.h"

#include "BluePrintVM/NFBPVirtualMachine.h"

class NFBPVirtualMachineModule
    : public NFIBPVirtualMachineModule
{
public:
    NFBPVirtualMachineModule( NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual ~NFBPVirtualMachineModule() {};

    virtual bool Awake();
    virtual bool Init();
    virtual bool AfterInit();
    virtual bool CheckConfig();
    virtual bool ReadyExecute();
    virtual bool Execute();
    virtual bool BeforeShut();
    virtual bool Shut();
    virtual bool Finalize();
    virtual bool OnReloadPlugin();


    virtual void RunLogicBlock(const NFGUID& logicBlockID);
    virtual void StopLogicBlock(const NFGUID& logicBlockID);
    virtual bool CheckLogicBlockRefCircle(const NFGUID& logicBlockID);

private:
    NFMapEx<NFGUID, NFBPVirtualMachine> mVirtualMachine;

private:
    NFIBluePrintModule* m_pBluePrintModule;
};


#endif
