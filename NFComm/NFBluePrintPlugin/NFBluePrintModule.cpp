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

#include "NFBluePrintModule.h"
#include "BluePrintNode/NFMonitor.h"
#include "BluePrintNode/NFVariable.h"
#include "BluePrintNode/NFBranch.h"
#include "NFComm/NFBluePrintPlugin/BluePrintNode/NFExecutor.h"
#include "BluePrintNode/NFModifier.h"
#include "BluePrintNode/NFArithmetic.h"
#include "BluePrintNode/NFDebugger.h"


NF_SHARE_PTR<NFBluePrintNodeBase> NFLogicBlock::FindNode(const NFGUID& id)
{
	for (auto it : monitors)
	{
		if (it->id == id)
		{
			return it;
		}
	}

	for (auto it : variables)
	{
		if (it->id == id)
		{
			return it;
		}
	}
	for (auto it : executers)
	{
		if (it->id == id)
		{
			return it;
		}
	}
	for (auto it : branches)
	{
		if (it->id == id)
		{
			return it;
		}
	}
	for (auto it : modifiers)
	{
		if (it->id == id)
		{
			return it;
		}
	}
	for (auto it : arithmetics)
	{
		if (it->id == id)
		{
			return it;
		}
	}
	for (auto it : debuggers)
	{
		if (it->id == id)
		{
			return it;
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

void NFBluePrintModule::SetLinkModifyEventFunctor(std::function<void(const NFGUID&, const bool)> functor)
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

NF_SHARE_PTR<NFBluePrintNodeBase> NFBluePrintModule::FindNode(const NFGUID& id)
{
	for (auto it : mLogicBlocks)
	{
		if (it->id == id)
		{
			return it;
		}

		auto baseNode = it->FindNode(id);
		if (baseNode)
		{
			return baseNode;
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFIMonitor> NFBluePrintModule::AddMonitor(const NFGUID& logicBlockId, const NFMonitorType type, const NFGUID& id, const std::string& name)
{
	for (auto it : mLogicBlocks)
	{
		if (it->id == logicBlockId)
		{
			NF_SHARE_PTR<NFIMonitor> monitor;

			switch (type)
			{
			case NFMonitorType::GameEvent:
				monitor = NF_SHARE_PTR<NFIMonitor>(NF_NEW NFGameEventMonitor(this->pPluginManager, logicBlockId, id, name));
				break;
			case NFMonitorType::NetworkEvent:
				monitor = NF_SHARE_PTR<NFIMonitor>(NF_NEW NFNetworkEventMonitor(this->pPluginManager, logicBlockId, id, name));
				break;
			case NFMonitorType::NetworkMsgEvent:
				monitor = NF_SHARE_PTR<NFIMonitor>(NF_NEW NFNetworkMsgEventMonitor(this->pPluginManager, logicBlockId, id, name));
				break;
			case NFMonitorType::ObjectEvent:
				monitor = NF_SHARE_PTR<NFIMonitor>(NF_NEW NFObjectEventMonitor(this->pPluginManager, logicBlockId, id, name));
				break;
			case NFMonitorType::PropertyEvent:
				monitor = NF_SHARE_PTR<NFIMonitor>(NF_NEW NFPropertyEventMonitor(this->pPluginManager, logicBlockId, id, name));
				break;
			case NFMonitorType::RecordEvent:
				monitor = NF_SHARE_PTR<NFIMonitor>(NF_NEW NFRecordEventMonitor(this->pPluginManager, logicBlockId, id, name));
				break;
			case NFMonitorType::SceneEvent:
				monitor = NF_SHARE_PTR<NFIMonitor>(NF_NEW NFSceneEventMonitor(this->pPluginManager, logicBlockId, id, name));
				break;
			default:
				break;
			}
			
			if (monitor)
			{
				it->monitors.push_back(monitor);

				NodeModifyEvent(id, true);

				return monitor;
			}
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFIVariable> NFBluePrintModule::AddVariable(const NFGUID& logicBlockId, const NFVariableType type, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindNode(logicBlockId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::LOGICBLOCK)
	{
		NF_SHARE_PTR<NFLogicBlock> logicBlock = std::dynamic_pointer_cast<NFLogicBlock>(baseNode);

		NF_SHARE_PTR<NFIVariable> variable;
		switch (type)
		{
		case NFVariableType::BasicVariable:
			variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFInputVariable(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFVariableType::ElementSystem:
			variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFElementVariable(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFVariableType::PropertySystem:
			variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFPropertyVariable(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFVariableType::RecordSystem:
			variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFRecordVariable(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFVariableType::Array:
			variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFArrayVariable(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFVariableType::Dictionary:
			variable = NF_SHARE_PTR<NFIVariable>(NF_NEW NFDictionaryVariable(this->pPluginManager, logicBlockId, id, name));
			break;
		default:
			break;
		}

		if (variable)
		{
			logicBlock->variables.push_front(variable);

			NodeModifyEvent(id, true);

			return variable;
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFIBranch> NFBluePrintModule::AddBranch(const NFGUID& logicBlockId, NFBranchType type, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindNode(logicBlockId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::LOGICBLOCK)
	{
		NF_SHARE_PTR<NFLogicBlock> logicBlock = std::dynamic_pointer_cast<NFLogicBlock>(baseNode);
		NF_SHARE_PTR<NFIBranch> branch;
		switch (type)
		{
		case NFBranchType::IntBranch:
			branch = NF_SHARE_PTR<NFIBranch>(NF_NEW NFIntBranch(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFBranchType::StringBranch:
			branch = NF_SHARE_PTR<NFIBranch>(NF_NEW NFStringBranch(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFBranchType::FloatBranch:
			branch = NF_SHARE_PTR<NFIBranch>(NF_NEW NFFloatBranch(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFBranchType::ObjectBranch:
			branch = NF_SHARE_PTR<NFIBranch>(NF_NEW NFObjectBranch(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFBranchType::Vector2Branch:
			branch = NF_SHARE_PTR<NFIBranch>(NF_NEW NFVector2Branch(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFBranchType::Vector3Branch:
			branch = NF_SHARE_PTR<NFIBranch>(NF_NEW NFVector3Branch(this->pPluginManager, logicBlockId, id, name));
			break;
		default:
			break;
		}

		logicBlock->branches.push_back(branch);

		NodeModifyEvent(id, true);

		return branch;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIExecutor> NFBluePrintModule::AddExecutor(const NFGUID& logicBlockId, const NFExecutorType type, const NFGUID& id, const std::string& name)
{	
	auto baseNode = FindNode(logicBlockId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::LOGICBLOCK)
	{
		NF_SHARE_PTR<NFLogicBlock> logicBlock = std::dynamic_pointer_cast<NFLogicBlock>(baseNode);
		NF_SHARE_PTR<NFIExecutor> executer;
		switch (type)
		{
		case NFExecutorType::CreateObject:
			executer = NF_SHARE_PTR<NFIExecutor>(NF_NEW NFCreateObjectExecutor(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFExecutorType::DestroyObject:
			executer = NF_SHARE_PTR<NFIExecutor>(NF_NEW NFDestroyObjectExecutor(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFExecutorType::GameEvent:
			executer = NF_SHARE_PTR<NFIExecutor>(NF_NEW NFGameEventExecutor(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFExecutorType::MoveObject:
			executer = NF_SHARE_PTR<NFIExecutor>(NF_NEW NFMoveObjectExecutor(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFExecutorType::Sleep:
			executer = NF_SHARE_PTR<NFIExecutor>(NF_NEW NFSleepExecutor(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFExecutorType::SendMessage:
			executer = NF_SHARE_PTR<NFIExecutor>(NF_NEW NFSendMessageExecutor(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFExecutorType::EnterScene:
			executer = NF_SHARE_PTR<NFIExecutor>(NF_NEW NFEnterSceneExecutor(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFExecutorType::AddHeartBeat:
			executer = NF_SHARE_PTR<NFIExecutor>(NF_NEW NFAddHeartBeatExecutor(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFExecutorType::AttackObject:
			executer = NF_SHARE_PTR<NFIExecutor>(NF_NEW NFAttackObjectExecutor(this->pPluginManager, logicBlockId, id, name));
			break;
		default:
			break;
		}

		if (executer)
		{
			logicBlock->executers.push_back(executer);

			NodeModifyEvent(id, true);

			return executer;
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFIModifier> NFBluePrintModule::AddModifier(const NFGUID& logicBlockId, const NFModifierType type, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindNode(logicBlockId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::LOGICBLOCK)
	{
		NF_SHARE_PTR<NFLogicBlock> logicBlock = std::dynamic_pointer_cast<NFLogicBlock>(baseNode);

		NF_SHARE_PTR<NFIModifier> modifier;
		switch (type)
		{
		case NFModifierType::SetProperty:
			modifier = NF_SHARE_PTR<NFIModifier>(NF_NEW NFPropertyModifier(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFModifierType::SetRecord:
			modifier = NF_SHARE_PTR<NFIModifier>(NF_NEW NFRecordModifier(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFModifierType::AddRecordRow:
			//modifier = NF_SHARE_PTR<NFIModifier>(NF_NEW NFPropertyVariable(this->pPluginManager, logicBlockId, id, name, it));
			break;
		case NFModifierType::RemRecordRow:
			modifier = NF_SHARE_PTR<NFIModifier>(NF_NEW NFRecordRemover(this->pPluginManager, logicBlockId, id, name));
			break;
		default:
			break;
		}

		if (modifier)
		{
			logicBlock->modifiers.push_back(modifier);
			NodeModifyEvent(id, true);

			return modifier;

		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFIArithmetic> NFBluePrintModule::AddArithmetic(const NFGUID& logicBlockId, const NFArithmeticType type, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindNode(logicBlockId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::LOGICBLOCK)
	{
		NF_SHARE_PTR<NFLogicBlock> logicBlock = std::dynamic_pointer_cast<NFLogicBlock>(baseNode);

		NF_SHARE_PTR<NFIArithmetic> arithmetic;
		switch (type)
		{
		case NFArithmeticType::Add:
			arithmetic = NF_SHARE_PTR<NFIArithmetic>(NF_NEW NFArithmetic(this->pPluginManager, logicBlockId, id, name));
			break;
		case NFArithmeticType::Sub:
			arithmetic = NF_SHARE_PTR<NFIArithmetic>(NF_NEW NFArithmetic(this->pPluginManager, logicBlockId, id, name));
			break;
		default:
			break;
		}

		if (arithmetic)
		{
			logicBlock->arithmetics.push_back(arithmetic);
			NodeModifyEvent(id, true);

			return arithmetic;

		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFIDebugger> NFBluePrintModule::AddDebugger(const NFGUID& logicBlockId, const NFGUID& id, const std::string& name)
{
	auto baseNode = FindNode(logicBlockId);
	if (baseNode && baseNode->blueprintType == NFBlueprintType::LOGICBLOCK)
	{
		NF_SHARE_PTR<NFLogicBlock> logicBlock = std::dynamic_pointer_cast<NFLogicBlock>(baseNode);
		NF_SHARE_PTR<NFIDebugger> debuger = NF_SHARE_PTR<NFIDebugger>(NF_NEW NFDebugger(this->pPluginManager, logicBlockId, id, name));
		
		if (debuger)
		{
			logicBlock->debuggers.push_back(debuger);

			NodeModifyEvent(id, true);

			return debuger;
		}
	}

	return nullptr;
}

void NFBluePrintModule::AddLink(const NFGUID& logicBlockId, const NFGUID& id, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin)
{
	auto block = GetLogicBlock(logicBlockId);
	if (block)
	{
		if (GetLink(logicBlockId, startNode, endNode, startPin, endPin))
		{
			return;
		}

		auto start = FindNode(startNode);
		auto end = FindNode(endNode);
		if (start && end)
		{
			auto starNodePin = start->GetOutputArg(startPin);
			auto endNodePin = end->GetInputArg(endPin);
			if (starNodePin && endNodePin)
			{
				auto link = NF_SHARE_PTR<NFDataLink>(NF_NEW NFDataLink(id, startNode, endNode, startPin, endPin));
				block->dataLinks.push_back(link);
				starNodePin->SetLinkID(link->selfID);
				endNodePin->SetLinkID(link->selfID);

				LinkModifyEvent(id, true);
			}
		}
	}
}

NF_SHARE_PTR<NFDataLink> NFBluePrintModule::GetLink(const NFGUID& logicBlockId, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin)
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

NF_SHARE_PTR<NFDataLink> NFBluePrintModule::GetLink(const NFGUID& logicBlockId, const NFGUID& id)
{
	auto block = GetLogicBlock(logicBlockId);
	if (block)
	{
		for (auto it = block->dataLinks.begin(); it != block->dataLinks.end(); ++it)
		{
			if ((*it)->selfID == id)
			{
				return *it;
			}
		}
	}

	return nullptr;
}

NF_SHARE_PTR<NFDataLink> NFBluePrintModule::GetLink(const NFGUID& id)
{
	for (auto it : mLogicBlocks)
	{
		auto logicBlock = it;
		for (auto link : logicBlock->dataLinks)
		{
			if (link->selfID == id)
			{
				return link;
			}
		}
	}

	return nullptr;
}

bool NFBluePrintModule::DeleteLink(const NFGUID& logicBlockId, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin)
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

				LinkModifyEvent((*it)->selfID, false);

				block->dataLinks.erase(it);
				return true;
			}
		}
	}

	return false;
}

bool NFBluePrintModule::DeleteLink(const NFGUID& logicBlockId, const NFGUID& id)
{
	auto block = GetLogicBlock(logicBlockId);
	if (block)
	{
		for (auto it = block->dataLinks.begin(); it != block->dataLinks.end(); ++it)
		{
			if ((*it)->selfID == id)
			{
				LinkModifyEvent((*it)->selfID, false);

				block->dataLinks.erase(it);
				return true;
			}
		}
	}

	return false;
}


bool NFBluePrintModule::DeleteLink(const NFGUID& id)
{
	for (auto block : mLogicBlocks)
	{
		for (auto it = block->dataLinks.begin(); it != block->dataLinks.end(); ++it)
		{
			auto link = (*it);
			if (link->selfID == id)
			{
				LinkModifyEvent(link->selfID, false);

				auto startNode = FindNode(link->startNode);
				auto endNode = FindNode(link->endNode);

				auto outputData = startNode->GetOutputArg(link->startAttr);
				auto inputData = endNode->GetInputArg(link->endAttr);
				outputData->SetLinkID(NFGUID());
				inputData->SetLinkID(NFGUID());

				block->dataLinks.erase(it);
				return true;
			}
		}
	}

	return false;
}

bool NFBluePrintModule::CheckLink(const NFGUID &id)
{
	auto linkData = GetLink(id);
	if (linkData && !linkData->startNode.IsNull() && !linkData->endNode.IsNull())
	{
		auto startNode = FindNode(linkData->startNode);
		auto endNode = FindNode(linkData->endNode);
		if (startNode && endNode)
		{
			auto startAttr = startNode->GetOutputArg(linkData->startAttr);
			auto endAttr = endNode->GetInputArg(linkData->endAttr);
			if (startAttr && endAttr)
			{
				if (startAttr->GetValueType() == endAttr->GetValueType())
				{
					return true;
				}
			}
		}
	}

	return false;
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

bool NFBluePrintModule::DeleteExecutor(const NFGUID& id)
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

void NFBluePrintModule::LinkModifyEvent(const NFGUID& id, const bool create)
{
	if (mLinkModifyFunctor)
	{
		mLinkModifyFunctor(id, create);
	}
}