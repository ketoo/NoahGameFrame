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

#ifndef NFI_BP_VIRTUAL_MACHINE_MODULE_H
#define NFI_BP_VIRTUAL_MACHINE_MODULE_H

#include <list>
#include <iostream>
#include <functional>
#include <algorithm>
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFIRecord.h"
#include "NFComm/NFCore/NFIProperty.h"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFCore/NFIRecordManager.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIModule.h"

class NFIBPVirtualMachineModule
	: public NFIModule
{
public:

    virtual void RunLogicBlock(const NFGUID& logicBlockID) = 0;
    virtual void StopLogicBlock(const NFGUID& logicBlockID) = 0;
    virtual bool CheckLogicBlockRefCircle(const NFGUID& logicBlockID) = 0;
};


#endif
