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

    virtual void SetNodeModifyEventFunctor(std::function<void(const NFGUID&, const bool)> functor);
    virtual void SetLinkModifyEventFunctor(std::function<void(const NFGUID&, const bool)> functor);

	virtual NF_SHARE_PTR<NFLogicBlock> CreateLogicBlock(const NFGUID& logicBlockId, const std::string& name);
	virtual const std::list<NF_SHARE_PTR<NFLogicBlock>>& GetLogicBlocks();
	virtual NF_SHARE_PTR<NFLogicBlock> GetLogicBlock(const NFGUID& logicBlockId);

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNode(const NFGUID& id);

    virtual NF_SHARE_PTR<NFIMonitor> AddMonitor(const NFGUID& logicBlockId, const NFMonitorType type, const NFGUID& id, const std::string& name);
    virtual NF_SHARE_PTR<NFIVariable> AddVariable(const NFGUID& logicBlockId, const NFVariableType type, const NFGUID& id, const std::string& name);
    virtual NF_SHARE_PTR<NFIBranch> AddBranch(const NFGUID& logicBlockId, const NFBranchType type, const NFGUID& id, const std::string& name);
    virtual NF_SHARE_PTR<NFIExecutor> AddExecutor(const NFGUID& logicBlockId, const NFExecutorType type, const NFGUID& id, const std::string& name);
    virtual NF_SHARE_PTR<NFIModifier> AddModifier(const NFGUID& logicBlockId, const NFModifierType type, const NFGUID& id, const std::string& name);
    virtual NF_SHARE_PTR<NFIArithmetic> AddArithmetic(const NFGUID& logicBlockId, const NFArithmeticType type, const NFGUID& id, const std::string& name);
    virtual NF_SHARE_PTR<NFIDebugger> AddDebugger(const NFGUID& logicBlockId, const NFGUID& id, const std::string& name);

    virtual void AddLink(const NFGUID& logicBlockId, const NFGUID& id, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin);
    virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& logicBlockId, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin);
    virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& logicBlockId, const NFGUID& id);
    virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& id);
    virtual bool DeleteLink(const NFGUID& logicBlockId, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin);
    virtual bool DeleteLink(const NFGUID& logicBlockId, const NFGUID& id);
    virtual bool DeleteLink(const NFGUID& id);
	virtual bool CheckLink(const NFGUID& id);
    virtual std::list<NF_SHARE_PTR<NFDataLink>> GetLinks(const NFGUID& logicBlockId);

	virtual bool DeleteMonitor(const NFGUID& id);
	virtual bool DeleteJudgement(const NFGUID& id);
	virtual bool DeleteExecutor(const NFGUID& id);

protected:
    void NodeModifyEvent(const NFGUID& id, const bool create);
    void LinkModifyEvent(const NFGUID& id, const bool create);

private:

    std::function<void(const NFGUID&, const bool)> mNodeModifyFunctor;
    std::function<void(const NFGUID&, const bool)> mLinkModifyFunctor;
    std::list<NF_SHARE_PTR<NFLogicBlock>> mLogicBlocks;
};


#endif
