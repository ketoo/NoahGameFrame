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

#ifndef NF_HELLO_WORLD3_H
#define NF_HELLO_WORLD3_H

#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIElementModule.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"

class NFIHelloWorld3Module
	: public NFIModule
{

};

class NFHelloWorld3Module
    : public NFIHelloWorld3Module
{
public:
    NFHelloWorld3Module(NFIPluginManager* p)
    {
        m_bIsExecute = true;
        pPluginManager = p;
    }

    virtual bool Init();
    virtual bool AfterInit();

    virtual bool Execute();

    virtual bool BeforeShut();
    virtual bool Shut();

protected:
    int OnEvent(const NFGUID& self, const int event, const NFDataList& arg);
    int OnClassCallBackEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT event, const NFDataList& arg);
    int OnPropertyCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFData& oldVarList, const NFData& newVarList);
    int OnPropertyStrCallBackEvent( const NFGUID& self, const std::string& strProperty, const NFData& oldVarList, const NFData& newVarList);

    int OnHeartBeat(const NFGUID& self, const std::string& strHeartBeat, const float fTime, const int nCount);

protected:
    int64_t mLastTime;
protected:
    NFIKernelModule* m_pKernelModule;
    NFIElementModule* m_pElementModule;
	NFIEventModule* m_pEventModule;
	NFIScheduleModule* m_pScheduleModule;
};

#endif
