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

#ifndef NF_DATA_TRAIL_MODULE_H
#define NF_DATA_TRAIL_MODULE_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIPropertyModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIPropertyConfigModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIDataTailModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class NFDataTailModule
    : public NFIDataTailModule
{
public:
    NFDataTailModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }
    virtual ~NFDataTailModule() {};

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();
    virtual bool AfterInit();

	virtual void LogObjectData(const NFGUID& self);
    virtual void StartTrail(const NFGUID& self);

protected:
    void PrintStackTrace();

    int TrailObjectData(const NFGUID& self);

    int OnClassObjectEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList& var);

    int OnObjectPropertyEvent(const NFGUID& self, const std::string& strPropertyName, const NFData& oldVar, const NFData& newVar);

    int OnObjectRecordEvent(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

private:

    NFIKernelModule* m_pKernelModule;
    NFIElementModule* m_pElementModule;
    NFIClassModule* m_pClassModule;
    NFILogModule* m_pLogModule;
};


#endif
