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
#include "BluePrintNode/NFMonitor.h"
#include "BluePrintNode/NFVariable.h"

NF_SHARE_PTR<NFBluePrintNodeBase> NFIVariable::FindBaseNode(const NFGUID& id)
{
	return nullptr;
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFExecuter::FindBaseNode(const NFGUID& id)
{
	if (nextExecuter)
	{
		if (nextExecuter->id == id)
		{
			return nextExecuter;
		}

		auto baseNode = nextExecuter->FindBaseNode(id);
		if (baseNode)
		{
			return baseNode;
		}
	}

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
	return nullptr;
}

NF_SHARE_PTR<NFBluePrintNodeBase> NFJudgement::FindBaseNode(const NFGUID& id)
{
	/*
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
*/
	if (trueBlueprintNode)
	{
		if (trueBlueprintNode->id == id)
		{
			return trueBlueprintNode;
		}
		
		auto baseNode = trueBlueprintNode->FindBaseNode(id);
		if (baseNode)
		{
			return baseNode;
		}
	}

	if (falseBlueprintNode)
	{
		if (falseBlueprintNode->id == id)
		{
			return falseBlueprintNode;
		}
		
		auto baseNode = falseBlueprintNode->FindBaseNode(id);
		if (baseNode)
		{
			return baseNode;
		}
	}

	return nullptr;
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

	for (auto it : variables)
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

void NFBluePrintModule::SetNodeModifyEventFunctor(std::function<void(const NFGUID&, const bool)> functor)
{
	mNodeModifyFunctor = functor;
}

void NFBluePrintModule::SetLinkModifyEventFunctor(std::function<void(const NFGUID &, const const NFGUID &, const NFGUID &, const const NFGUID &, const bool)> functor)
{
	mLinkModifyFunctor = functor;
}

NF_SHARE_PTR<NFLogicBlock> NFBluePrintModule::CreateLogicBlock(const NFGUID& logicBlockId, const std::string& name)
{
	auto p = NF_SHARE_PTR<NFLogicBlock>(NF_NEW NFLogicBlock(pPluginManager, logicBlockId, name));
	mLogicBlocks.push_back(p);

	NodeModifyEvent(logicBlockId, true);

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

NF_SHARE_PTR<NFIMonitor> NFBluePrintModule::AddMonitorForLogicBlock(const NFGUID& logicBlockId, const NFMonitorType type, const NFGUID& id, const std::string& name)
{
	for (auto it : mLogicBlocks)
	{
		if (it->id == logicBlockId)
		{
			NF_SHARE_PTR<NFIMonitor> monitor;

			switch (type)
			{
			case NFMonitorType::GameEvent:
				monitor = NF_SHARE_PTR<NFIMonitor>(NF_NEW NFGameEventMonitor(this->pPluginManager, logicBlockId, id, name, it));
				break;
			default:
				break;
			}
			
			it->monitors.push_back(monitor);

			NodeModifyEvent(id, true);

			return monitor;
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFIVariable> NFBluePrintModule::AddVariableForLogicBlock(const NFGUID& logicBlockId, const NFVariableType type, const NFGUID& id, const std::string& name)
{
	for (auto it : mLogicBlocks)
	{
		if (it->id == logicBlockId)
		{
			NF_SHARE_PTR<NFIVariable> variable;
			switch (type)
			{
			case NFVariableType::Input:
				variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFIVariable(this->pPluginManager, logicBlockId, id, name, it));
				break;
			case NFVariableType::ElementSystem:
				variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFElementVariable(this->pPluginManager, logicBlockId, id, name, it));
				break;
			case NFVariableType::PropertySystem:
				variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFPropertyVariable(this->pPluginManager, logicBlockId, id, name, it));
				break;
			case NFVariableType::RecordSystem:
				variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFRecordVariable(this->pPluginManager, logicBlockId, id, name, it));
				break;
			default:
				break;
			}

			it->variables.push_front(variable);

			NodeModifyEvent(id, true);

			return variable;
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFJudgement> NFBluePrintModule::AddJudgementForMonitor(const NFGUID& monitorId, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindBaseNode(monitorId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::MONITOR)
	{
		NF_SHARE_PTR<NFIMonitor> monitor = std::dynamic_pointer_cast<NFIMonitor>(baseNode);

		auto newNode = NF_SHARE_PTR<NFJudgement>(NF_NEW NFJudgement(this->pPluginManager, id, name, baseNode));
		monitor->AddJudgement(newNode);

		NodeModifyEvent(id, true);

		return newNode;
	}

	return nullptr;
}

NF_SHARE_PTR<NFJudgement> NFBluePrintModule::AddTrueJudgementForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindBaseNode(judgementId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::JUDGEMENT)
	{
		NF_SHARE_PTR<NFJudgement> judgement = std::dynamic_pointer_cast<NFJudgement>(baseNode);
		
		auto newNode = NF_SHARE_PTR<NFJudgement>(NF_NEW NFJudgement(this->pPluginManager, id, name, baseNode));
		judgement->trueBlueprintNode = newNode;

		NodeModifyEvent(id, true);

		return newNode;
	}

	return nullptr;
}

NF_SHARE_PTR<NFJudgement> NFBluePrintModule::AddFalseJudgementForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindBaseNode(judgementId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::JUDGEMENT)
	{
		NF_SHARE_PTR<NFJudgement> judgement = std::dynamic_pointer_cast<NFJudgement>(baseNode);
		
		auto newNode = NF_SHARE_PTR<NFJudgement>(NF_NEW NFJudgement(this->pPluginManager, id, name, baseNode));
		judgement->falseBlueprintNode = newNode;

		NodeModifyEvent(id, true);

		return newNode;
	}

	return nullptr;
}

NF_SHARE_PTR<NFJudgement> NFBluePrintModule::AddJudgementForExecuter(const NFGUID& executerId, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindBaseNode(executerId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::EXECUTER)
	{
		NF_SHARE_PTR<NFExecuter> executer = std::dynamic_pointer_cast<NFExecuter>(baseNode);
		
		auto newNode = NF_SHARE_PTR<NFJudgement>(NF_NEW NFJudgement(this->pPluginManager, id, name, baseNode));
		executer->judgements.push_back(newNode);

		NodeModifyEvent(id, true);

		return newNode;
	}

	return nullptr;
}


NF_SHARE_PTR<NFExecuter> NFBluePrintModule::AddTrueExecuterForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name)
{	
	auto baseNode = FindBaseNode(judgementId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::JUDGEMENT)
	{
		NF_SHARE_PTR<NFJudgement> judgement = std::dynamic_pointer_cast<NFJudgement>(baseNode);
		
		auto newNode = NF_SHARE_PTR<NFExecuter>(NF_NEW NFExecuter(this->pPluginManager, id, name, baseNode));
		judgement->trueBlueprintNode = newNode;

		NodeModifyEvent(id, true);

		return newNode;
	}

	return nullptr;
}

NF_SHARE_PTR<NFExecuter> NFBluePrintModule::AddFalseExecuterForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name)
{	
	auto baseNode = FindBaseNode(judgementId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::JUDGEMENT)
	{
		NF_SHARE_PTR<NFJudgement> judgement = std::dynamic_pointer_cast<NFJudgement>(baseNode);
		
		auto newNode = NF_SHARE_PTR<NFExecuter>(NF_NEW NFExecuter(this->pPluginManager, id, name, baseNode));
		judgement->falseBlueprintNode = newNode;

		NodeModifyEvent(id, true);

		return newNode;
	}

	return nullptr;
}

NF_SHARE_PTR<NFExecuter> NFBluePrintModule::AddExecuterForExecuter(const NFGUID& executerId, const NFGUID& id, const std::string& name)
{	
	auto baseNode = FindBaseNode(executerId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::EXECUTER)
	{
		NF_SHARE_PTR<NFExecuter> judgement = std::dynamic_pointer_cast<NFExecuter>(baseNode);
		
		auto newNode = NF_SHARE_PTR<NFExecuter>(NF_NEW NFExecuter(this->pPluginManager, id, name, baseNode));
		judgement->nextExecuter = newNode;

		NodeModifyEvent(id, true);

		return newNode;
	}

	return nullptr;
}

void NFBluePrintModule::AddLink(const NFGUID& logicBlockId, const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin)
{
	auto block = GetLogicBlock(logicBlockId);
	if (block)
	{
		if (GetLink(logicBlockId, startNode, endNode, startPin, endPin))
		{
			return;
		}

		auto link = NF_SHARE_PTR<NFDataLink>(NF_NEW NFDataLink(startNode, endNode, startPin, endPin));
		block->dataLinks.push_back(link);

		LinkModifyEvent(startNode, endNode, startPin, endPin, true);
	}
}

NF_SHARE_PTR<NFDataLink> NFBluePrintModule::GetLink(const NFGUID& logicBlockId, const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin)
{
	auto block = GetLogicBlock(logicBlockId);
	if (block)
	{
		for (auto it = block->dataLinks.begin(); it != block->dataLinks.end(); ++it)
		{
			if ((*it)->startNode == startNode
				&& (*it)->endNode == endNode
				&& (*it)->startAttr == startPin
				&& (*it)->endAttr == endPin)
			{
				return *it;
			}
		}
	}

	return nullptr;
}

void NFBluePrintModule::DeleteLink(const NFGUID& logicBlockId, const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin)
{
	auto block = GetLogicBlock(logicBlockId);
	if (block)
	{
		for (auto it = block->dataLinks.begin(); it != block->dataLinks.end(); ++it)
		{
			if ((*it)->startNode == startNode
				&& (*it)->endNode == endNode
				&& (*it)->startAttr == startPin
				&& (*it)->endAttr == endPin)
			{

				LinkModifyEvent(startNode, endNode, startPin, endPin, false);

				block->dataLinks.erase(it);
				return;
			}
		}
	}
}

std::list<NF_SHARE_PTR<NFDataLink>> NFBluePrintModule::GetLinks(const NFGUID& logicBlockId)
{
	auto block = GetLogicBlock(logicBlockId);
	if (block)
	{
		return block->dataLinks;
	}

	return std::list<NF_SHARE_PTR<NFDataLink>>();
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

void NFBluePrintModule::NodeModifyEvent(const NFGUID& id, const bool create)
{
	if (mNodeModifyFunctor)
	{
		mNodeModifyFunctor(id, create);
	}
}

void NFBluePrintModule::LinkModifyEvent(const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin, const bool create)
{
	if (mLinkModifyFunctor)
	{
		mLinkModifyFunctor(startNode, endNode, startPin, endPin, create);
	}
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
