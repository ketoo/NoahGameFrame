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


#ifndef NF_COOLDOW_MODULE_H
#define NF_COOLDOW_MODULE_H


#include <NFComm/NFPluginModule/NFIKernelModule.h>
#include <NFComm/NFPluginModule/NFIElementModule.h>
#include <NFComm/NFPluginModule/NFICooldownModule.h>


class NFCooldownModule
        : public NFICooldownModule
{
public:

    NFCooldownModule(NFIPluginManager* p )
    {
        pPluginManager = p;
    }

    virtual bool AfterInit() override;

    virtual void AddCooldown(const NFGUID& self, const std::string& configID );
    virtual bool ExistCooldown(const NFGUID& self, const std::string& configID );

private:

    NFIKernelModule* m_pKernelModule;
    NFIElementModule* m_pElementModule;
};

#endif
