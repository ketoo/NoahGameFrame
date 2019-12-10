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
		/*
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
		*/
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

	virtual void SetLogicBlockEventFunctor(std::function<void(const NFGUID&, const bool)> functor);

	virtual NF_SHARE_PTR<NFLogicBlock> CreateLogicBlock(const NFGUID& logicBlockId, const std::string& name);
	virtual const std::list<NF_SHARE_PTR<NFLogicBlock>>& GetLogicBlocks();
	virtual NF_SHARE_PTR<NFLogicBlock> GetLogicBlock(const NFGUID& logicBlockId);

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindBaseNode(const NFGUID& id);

	virtual NF_SHARE_PTR<NFMonitor> AddMonitorForLogicBlock(const NFGUID& logicBlockId, const NFGUID& id, const std::string& name);
	virtual NF_SHARE_PTR<NFJudgement> AddJudgementForMonitor(const NFGUID& monitorId, const NFGUID& id, const std::string& name);
	virtual NF_SHARE_PTR<NFJudgement> AddTrueJudgementForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name);
	virtual NF_SHARE_PTR<NFJudgement> AddFalseJudgementForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name);
	virtual NF_SHARE_PTR<NFJudgement> AddJudgementForExecuter(const NFGUID& executerId, const NFGUID& id, const std::string& name);
	virtual NF_SHARE_PTR<NFExecuter> AddTrueExecuterForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name);
	virtual NF_SHARE_PTR<NFExecuter> AddFalseExecuterForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name);
	virtual NF_SHARE_PTR<NFExecuter> AddExecuterForExecuter(const NFGUID& executerId, const NFGUID& id, const std::string& name);

	virtual bool DeleteMonitor(const NFGUID& id);
	virtual bool DeleteJudgement(const NFGUID& id);
	virtual bool DeleteExecuter(const NFGUID& id);

protected:
    void ModifyEvent(const NFGUID& id, const bool create);

private:

    std::function<void(const NFGUID&, const bool)> mFunctor;
    std::list<NF_SHARE_PTR<NFLogicBlock>> mLogicBlocks;
};


#endif
