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

#ifndef NF_HELLO_WORLD2_H
#define NF_HELLO_WORLD2_H

#include "NFComm/NFPluginModule/NFIPlugin.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
class NFIHelloWorld2
	: public NFIModule
{

};

class NFHelloWorld2
    : public NFIHelloWorld2
{
public:
    NFHelloWorld2(NFIPluginManager* p)
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
    int OnPropertyCallBackEvent(const NFGUID& self, const std::string& strProperty, const NFData& oldVarList, const NFData& newVarList);



};

#endif
