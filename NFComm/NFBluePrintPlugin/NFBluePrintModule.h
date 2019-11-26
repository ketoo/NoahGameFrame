/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2019 NoahFrame(NoahGameFrame)

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

#ifndef NF_BLUE_PRINT_MODULE_H
#define NF_BLUE_PRINT_MODULE_H

#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"
#include "NFComm/NFPluginModule/NFIClassModule.h"
#include "NFComm/NFPluginModule/NFIBluePrintModule.h"

class NFBluePrintModule
    : public NFIBluePrintModule
{
public:
    NFBluePrintModule( NFIPluginManager* p )
    {
        pPluginManager = p;

        MyEnum foo = MyEnum::TWO;
        std::cout << MyEnum::toString(foo);  // static method
        std::cout << foo.toString();         // member method
        std::cout << MyEnum::toString(MyEnum::TWO);
        std::cout << MyEnum::toString(10);
        MyEnum foo1 = MyEnum::fromString("TWO");

        // C++11 iteration over all values
        for( auto x : MyEnum::allValues() )
        {
            std::cout << x.toString() << std::endl;
        }
    }

    virtual ~NFBluePrintModule() {};

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

    virtual NF_SHARE_PTR<NFLogicBlock> CreateLogicBlock(const std::string& name)
    {
        auto p = NF_SHARE_PTR<NFLogicBlock>(NF_NEW NFLogicBlock(pPluginManager, name));
        mLogicBlocks.push_back(p);
        return p;
    }

    virtual const std::list<NF_SHARE_PTR<NFLogicBlock>>& GetLogicBlocks()
    {
        return mLogicBlocks;
    }

    virtual NF_SHARE_PTR<NFLogicBlock>  GetLogicBlock(const NFGUID id)
    {
        for (auto it : mLogicBlocks)
        {
            if (it->id == id)
            {
                return it;
            }
        }

        return nullptr;
    }

    virtual NF_SHARE_PTR<NFLogicBlock>  GetLogicBlock(const std::string& name)
    {
        for (auto it : mLogicBlocks)
        {
            if (it->name == name)
            {
                return it;
            }
        }

        return nullptr;
    }
    
private:

    std::list<NF_SHARE_PTR<NFLogicBlock>> mLogicBlocks;
};


#endif
