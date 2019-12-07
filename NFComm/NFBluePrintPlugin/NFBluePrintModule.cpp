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

#include "NFBluePrintModule.h"


NF_SHARE_PTR<NFBluePrintNodeBase> NFExecuter::FindBaseNode(const NFGUID& id)
{
	if (nextExecuter && nextExecuter->id == id)
	{
		return nextExecuter;
	}
	for (auto it : nextJudgement)
	{
		if (it->id == id)
		{
			return it;
		}
		auto baseNode = it->FindBaseNode(id);
		if (baseNode)
		{
			return baseNode;
		}
	}
	return nullptr;
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFJudgement::FindBaseNode(const NFGUID& id)
{
	for (auto it : judgements)
	{
		if (it->id == id)
		{
			return std::dynamic_pointer_cast<NFBluePrintNodeBase>(it);
		}
		auto baseNode = it->FindBaseNode(id);
		if (baseNode)
		{
			return baseNode;
		}
	}
	if (nextExecuter->id == id)
	{
		return nextExecuter;
	}
	
	return nullptr;
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFMonitor::FindBaseNode(const NFGUID& id)
{
	for (auto it : judgements)
	{
		if (it->id == id)
		{
			return it;
		}
		auto baseNode = it->FindBaseNode(id);
		if (baseNode)
		{
			return baseNode;
		}
	}
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFLogicBlock::FindBaseNode(const NFGUID& id)
{
	for (auto it : monitors)
	{
		if (it->id == id)
		{
			return it;
		}
		auto baseNode = it->FindBaseNode(id);
		if (baseNode)
		{
			return baseNode;
		}
	}
	return nullptr;
}
////////////////////

bool NFBluePrintModule::Awake()
{


	return true;
}

bool NFBluePrintModule::Init()
{
	

    return true;
}

bool NFBluePrintModule::AfterInit()
{

	return true;
}

bool NFBluePrintModule::CheckConfig()
{
	
	return true;
}

bool NFBluePrintModule::ReadyExecute()
{
	

	return true;
}

bool NFBluePrintModule::Execute()
{
	
    return true;
}

bool NFBluePrintModule::BeforeShut()
{

	return true;
}

bool NFBluePrintModule::Shut()
{
	

	return true;
}

bool NFBluePrintModule::Finalize()
{
	

	return true;
}

bool NFBluePrintModule::OnReloadPlugin()
{
	
	return true;
}

NF_SHARE_PTR<NFLogicBlock> NFBluePrintModule::CreateLogicBlock(const NFGUID& logicBlockId, const std::string& name)
{
	auto p = NF_SHARE_PTR<NFLogicBlock>(NF_NEW NFLogicBlock(pPluginManager, logicBlockId, name));
	mLogicBlocks.push_back(p);
	return p;
}

const std::list<NF_SHARE_PTR<NFLogicBlock>>& NFBluePrintModule::GetLogicBlocks()
{
	return mLogicBlocks;
}

NF_SHARE_PTR<NFLogicBlock> NFBluePrintModule::GetLogicBlock(const NFGUID& id)
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

NF_SHARE_PTR<NFBluePrintNodeBase> NFBluePrintModule::FindBaseNode(const NFGUID& id)
{
	for (auto it : mLogicBlocks)
	{
		if (it->id == id)
		{
			return it;
		}

		auto baseNode = it->FindBaseNode(id);
		if (baseNode)
		{
			return baseNode;
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFMonitor> NFBluePrintModule::AddMonitorForLogicBlock(const NFGUID& logicBlockId, const NFGUID& id, const std::string& name)
{
	for (auto it : mLogicBlocks)
	{
		if (it->id == logicBlockId)
		{
			auto monitor = NF_SHARE_PTR<NFMonitor>(NF_NEW NFMonitor(this->pPluginManager, id, name, it));
			it->monitors.push_front(monitor);

			return monitor;
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFJudgement> NFBluePrintModule::AddJudgementForMonitor(const NFGUID& monitorId, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindBaseNode(monitorId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::MONITOR)
	{
		NF_SHARE_PTR<NFMonitor> monitor = std::dynamic_pointer_cast<NFMonitor>(baseNode);

		auto newNode = NF_SHARE_PTR<NFJudgement>(NF_NEW NFJudgement(this->pPluginManager, id, name, baseNode));
		monitor->judgements.push_back(newNode);

		return newNode;
	}

	return nullptr;
}

NF_SHARE_PTR<NFJudgement> NFBluePrintModule::AddJudgementForJudgement(const NFGUID& monitorId, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindBaseNode(monitorId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::JUDGEMENT)
	{
		NF_SHARE_PTR<NFJudgement> judgement = std::dynamic_pointer_cast<NFJudgement>(baseNode);
		
		auto newNode = NF_SHARE_PTR<NFJudgement>(NF_NEW NFJudgement(this->pPluginManager, id, name, baseNode));
		judgement->judgements.push_back(newNode);

		return newNode;
	}

	return nullptr;
}

NF_SHARE_PTR<NFJudgement> NFBluePrintModule::AddJudgementForExecuter(const NFGUID& monitorId, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindBaseNode(monitorId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::EXECUTER)
	{
		NF_SHARE_PTR<NFExecuter> executer = std::dynamic_pointer_cast<NFExecuter>(baseNode);
		
		auto newNode = NF_SHARE_PTR<NFJudgement>(NF_NEW NFJudgement(this->pPluginManager, id, name, baseNode));
		executer->nextJudgement.push_back(newNode);

		return newNode;
	}

	return nullptr;
}

NF_SHARE_PTR<NFExecuter> NFBluePrintModule::AddExecuterForMonitor(const NFGUID& judgeMent, const NFGUID& id, const std::string& name)
{
	return NF_SHARE_PTR<NFExecuter>();
}

NF_SHARE_PTR<NFExecuter> NFBluePrintModule::AddExecuterForJudgement(const NFGUID& judgeMent, const NFGUID& id, const std::string& name)
{
	return NF_SHARE_PTR<NFExecuter>();
}

NF_SHARE_PTR<NFExecuter> NFBluePrintModule::AddExecuterForExecuter(const NFGUID& judgeMent, const NFGUID& id, const std::string& name)
{
	return NF_SHARE_PTR<NFExecuter>();
}

bool NFBluePrintModule::DeleteMonitor(const NFGUID& id)
{
	return false;
}

bool NFBluePrintModule::DeleteJudgement(const NFGUID& id)
{
	return false;
}

bool NFBluePrintModule::DeleteExecuter(const NFGUID& id)
{
	return false;
}

/*
NF_SHARE_PTR<NFMonitor> AddMonitor(NFMonitorType operatorType, const NFGUID& id, const std::string& name, const NFDataList& arg)
{
	NF_SHARE_PTR<NFMonitor> monitor = nullptr;

	//different parameters if you have different monitor type
	//you will know what is the monitor type if you picked the operatorType
	switch (operatorType)
	{
	case NFMonitorType::NONE:
		break;
	case NFMonitorType::PropertyEvent:
	{
		//arg: property_name
		if (arg.GetCount() == 1)
		{
			monitor = NF_SHARE_PTR<NFMonitor>(NF_NEW NFMonitor(this->pPluginManager, id, name));
			monitor->operatorType = operatorType;
		}
	}
	break;

	default:
		break;
	}

	if (monitor)
	{
		monitors.push_back(monitor);
	}

	return monitor;
}
*/
