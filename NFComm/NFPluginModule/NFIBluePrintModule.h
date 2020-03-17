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
#ifndef NFI_BLUE_PRINT_MODULE_H
#define NFI_BLUE_PRINT_MODULE_H

#include <iostream>
#include <map>
#include <list>
#include "NFIModule.h"
#include "NFIKernelModule.h"
#include "NFIClassModule.h"
#include "NFIElementModule.h"
#include "NFILogModule.h"

class NFBranchType;
class NFMonitorType;
class NFVariableType;
class NFExecuterType;
class NFModifierType;
class NFArithmeticType;

class NFBluePrintNodeBase;
class NFLogicBlock;
class NFIBranch;
class NFIMonitor;
class NFIVariable;
class NFIExecuter;
class NFIModifier;
class NFIArithmetic;
class NFIDebugger;

class NFDataLink;

class NFIBluePrintModule
	: public NFIModule
{
public:

	////////////////////////////////////////
	virtual void SetNodeModifyEventFunctor(std::function<void(const NFGUID&, const bool)> functor) = 0;
	virtual void SetLinkModifyEventFunctor(std::function<void(const NFGUID&, const bool)> functor) = 0;

	virtual NF_SHARE_PTR<NFLogicBlock> CreateLogicBlock(const NFGUID& logicBlockId, const std::string& name) = 0;
	virtual const std::list<NF_SHARE_PTR<NFLogicBlock>>& GetLogicBlocks() = 0;
	virtual NF_SHARE_PTR<NFLogicBlock>  GetLogicBlock(const NFGUID& logicBlockId) = 0;

	virtual NF_SHARE_PTR<NFBluePrintNodeBase>  FindNode(const NFGUID& id) = 0;

	virtual NF_SHARE_PTR<NFIBranch> AddBranch(const NFGUID& logicBlockId, const NFBranchType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIMonitor> AddMonitor(const NFGUID& logicBlockId, const NFMonitorType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIVariable> AddVariable(const NFGUID& logicBlockId, const NFVariableType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIExecuter> AddExecuter(const NFGUID& logicBlockId, const NFExecuterType TYPE, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIModifier> AddModifier(const NFGUID& logicBlockId, const NFModifierType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIArithmetic> AddArithmetic(const NFGUID& logicBlockId, const NFArithmeticType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIDebugger> AddDebugger(const NFGUID& logicBlockId, const NFGUID& id, const std::string& name) = 0;

	virtual void AddLink(const NFGUID& logicBlockId, const NFGUID& id, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin) = 0;
	virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& logicBlockId, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin) = 0;
	virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& logicBlockId, const NFGUID& id) = 0;
	virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& id) = 0;
	virtual bool DeleteLink(const NFGUID& logicBlockId, const NFGUID& startNode, const NFGUID& endNode, const NFGUID& startPin, const NFGUID& endPin) = 0;
	virtual bool DeleteLink(const NFGUID& logicBlockId, const NFGUID& id) = 0;
	virtual bool DeleteLink(const NFGUID& id) = 0;
	virtual std::list<NF_SHARE_PTR<NFDataLink>> GetLinks(const NFGUID& logicBlockId) = 0;

	virtual bool DeleteMonitor(const NFGUID& id) = 0;
	virtual bool DeleteJudgement(const NFGUID& id) = 0;
	virtual bool DeleteExecuter(const NFGUID& id) = 0;
};

NF_SMART_ENUM(NFBlueprintType,
	LOGICBLOCK,
	MONITOR,
	BRANCH,
	EXECUTER,
	VARIABLE,
	MODIFIER,
	ARITHMETIC,
	DEBUGER,
	CUSTOM,
)

//--------FOR VARIABLE----------------
NF_SMART_ENUM(NFElementVariableInputArg,
	ClassName,
	ConfigID,
	PropName,
	)

	NF_SMART_ENUM(NFElementVariableOutputArg,
		ClassName,
		ConfigID,
		PropName,
		PropValue,
		)

	NF_SMART_ENUM(NFPropertyVariableInputArg,
		ObjectID,
		ClassName,
		PropName,
		)

	NF_SMART_ENUM(NFPropertyVariableOutputArg,
		ClassName,
		PropName,
		PropValue,
		)

	NF_SMART_ENUM(NFPropertyListVariableInputArg,
		ObjectID,
		ClassName,
		)

	NF_SMART_ENUM(NFPropertyListVariableOutputArg,
		ObjectID,
		ClassName,
		ConfigID,
		SceneID,
		GroupID,
		Position,
		)

	NF_SMART_ENUM(NFRecordVariableInputArg,
		ObjectID,
		ClassName,
		RecordName,
		RecordRow,
		RecordCol,
		)

//------------------------
//-------FOR MONITOR-----------------

NF_SMART_ENUM(NFMonitorType,
	GameEvent,
	NetworkEvent,
	NetworkMsgEvent,
	ObjectEvent,
	PropertyEvent,
	RecordEvent,
	SceneEvent,
	ItemEvent,
	SkillEvent,
	BuffEvent,
	)
	//------------------------------
	NF_SMART_ENUM(NFGameEventMonitorInputArg,
		EventID,
		)

	NF_SMART_ENUM(NFGameEventMonitorOutputArg,
		NextNode,
		EventID,
		ObjectID,
		)
	//------------------------------
	NF_SMART_ENUM(NFNetworkEventMonitorInputArg,
		EventID,
		)

	NF_SMART_ENUM(NFNetworkEventMonitorOutputArg,
		NextNode,
		EventID,
		ObjectID,
		)
	//------------------------------
	NF_SMART_ENUM(NFNetworkMsgMonitorInputArg,
		NetMsgID,
		)

	NF_SMART_ENUM(NFNetworkkMsgMonitorOutputArg,
		NextNode,
		ObjectID,
		NetMsgID,
		Dictionary,
		)
	//message data will includes a hashmap, developers could add more args to get the data by key
	//------------------------------
	NF_SMART_ENUM(NFMonitorObjectEventInputArg,
		ClassName,
		ClassEvent,
		)

	NF_SMART_ENUM(NFMonitorObjectEventOutputArg,
		NextNode,
		ObjectID,
		ClassName,
		ConfigID,
		SceneID,
		GroupID,
		Position,
		)

	//------------------------------
	NF_SMART_ENUM(NFMonitorPropertyEventInputArg,
		ClassName,
		PropName,
		)

	NF_SMART_ENUM(NFMonitorPropertyEventOutputArg,
		NextNode,
		ObjectID,
		PropName,
		OldValue,
		NewValue,
		)

	//------------------------------
	NF_SMART_ENUM(NFMonitorRecordEventInputArg,
		ClassName,
		RecordName,
		OperateType,
		)

	NF_SMART_ENUM(NFMonitorRecordEventOutArg,
		NextNode,
		ClassName,
		RecordName,
		Row,
		Col,
		)

	//------------------------------
	NF_SMART_ENUM(NFMonitorSceneEventInputArg,
		ClassName,
		SceneID,
		OperateType,
		)

	NF_SMART_ENUM(NFMonitorSceneEventOutArg,
		NextNode,
		ClassName,
		SceneID,
		GroupID,
		ObjectID,
		)

	//------------------------------
//------------------------
//----------FOR VARIABLE--------------
NF_SMART_ENUM(NFValueType,
	UNKNOW,
	Node,
	Int,
	Float,
	String,
	Vector2,
	Vector3,
	Object,
	Array,
	Dictionary,
	)

NF_SMART_ENUM(NFVariableType,
	Input,
	ElementSystem,
	PropertySystem,
	PropertyList,
	RecordSystem,
	Array,
	Dictionary,
	)

//----------FOR DEBUGER--------------

NF_SMART_ENUM(NFDebuggerLevel,
	Debug_Level,
	Info_Level,
	Waring_Level,
	Error_Level,
	)

NF_SMART_ENUM(NFDebuggerInputArg,
	LastNode,
	LogLevel,
	LogData,
)

NF_SMART_ENUM(NFDebuggerOutputArg,
	NextNode,
)
///////////FOR MODIFIER BEGIN///////////////////////////
//SetProperty 3 args: NFGUID objectID, string propertyName, int value
//SetRecord 5 args: NFGUID objectID, string recordName, int row, int col, int value
NF_SMART_ENUM(NFModifierType,
	SetProperty,
	SetRecord,
	AddRecordRow,
	RemRecordRow,
)

NF_SMART_ENUM(NFPropertyModifierInputArg,
	LastNode,
	ObjectID,
	ClassName,
	PropName,
	PropValue
	)

NF_SMART_ENUM(NFPropertyModifierOutputArg,
		NextNode,
		)

NF_SMART_ENUM(NFRecordModifierInputArg,
	LastNode,
	ObjectID,
	RecordName,
	RecordRow,
	RecordCol,
	RecordValue
	)
NF_SMART_ENUM(NFRecordModifierOutputArg,
	NextNode,
	)

NF_SMART_ENUM(NFRecordRemoverrInputArg,
	LastNode,
	ObjectID,
	RecordName,
	RecordRow,
)

NF_SMART_ENUM(NFRecordRemoverOutputArg,
	NextNode,
	)


///////////FOR EXECUTER BEGIN///////////////////////////
NF_SMART_ENUM(NFExecuterType,
	CreateObject,
	DestroyObject,
	GameEvent,
	MoveObject,
	Sleep,
	EnterScene,
	AddHeartBeat,
	AttackObject,
	UseSkill,
	UseItem
)

NF_SMART_ENUM(NFExecuterCreateObjectInputArg,
	LastNode,
	ConfigID,
	SceneID,
	GroupID,
	Position,
)
NF_SMART_ENUM(NFExecuterCreateObjectOutputputArg,
	NextNode,
	ObjectID,
	)

NF_SMART_ENUM(NFExecuterDestroyObjectInputArg,
	LastNode,
	ObjectID,
	)

NF_SMART_ENUM(NFExecuterDestroyObjectOutputputArg,
	NextNode,
	)

NF_SMART_ENUM(NFExecuterGameEventInputArg,
	LastNode,
	ObjectID,
	)

NF_SMART_ENUM(NFExecuterGameEventOutputArg,
	NextNode,
	)

NF_SMART_ENUM(NFExecuterMoveObjectInputArg,
	LastNode,
	ObjectID,
	Position,
	MoveType,
	)

NF_SMART_ENUM(NFExecuterMoveObjectOutputArg,
	NextNode,
	)

NF_SMART_ENUM(NFExecuterSleepInputArg,
	LastNode,
	ObjectID,
	SleepTime,
	)

NF_SMART_ENUM(NFExecuterSleepOutputArg,
	NextNode,
	ObjectID,
	)

NF_SMART_ENUM(NFExecuterEnterSceneInputArg,
	LastNode,
	ObjectID,
	SceneID,
	GroupID,
	Position,
	)

NF_SMART_ENUM(NFExecuterEnterSceneOutputArg,
	NextNode,
	)

NF_SMART_ENUM(NFExecuterAddHeartBeatInputArg,
	LastNode,
	ObjectID,
	BeatName,
	BeatTime,
	BeatCount,
	)

NF_SMART_ENUM(NFExecuterAddHeartBeatOutputArg,
	NextNode,
	)

///////////FOR BRANCH BEGIN///////////////////////////
NF_SMART_ENUM(NFBranchType,
	IntBranch,
	StringBranch,
	FloatBranch,
	ObjectBranch,
	Vector2Branch,
	Vector3Branch,
)

NF_SMART_ENUM(NFComparatorType,
	Equal,
	EnEqual,
	MoreThen,
	LessThan,
	)

NF_SMART_ENUM(NFBranchInputArg,
	LastNode,
	LeftInput,
	Comparator,
	RightInput
)

NF_SMART_ENUM(NFBranchOutputArg,
	TrueOut,
	FalseOut,
)

///////////FOR ARITHMETIC BEGIN///////////////////////////

NF_SMART_ENUM(NFArithmeticType,
	Add,
	Sub,
	Multiply,
	Division,
	)

NF_SMART_ENUM(NFArithmeticInputArg,
	LeftInput,
	ALU,
	RightInput
)
///////////////////////////////////////////////////////////////////////////////////
class NFLogicBlock;
class NFIMonitor;
class NFIBranch;
class NFIExecuter;
class NFIVariable;
class NFIOData;
class NFDataLink;

//BOTH MEANS: INTERNAL AND EXTERNAL
//PARAMETER
NF_SMART_ENUM(NFIODataComFromType,
	BOTH,
	INTERNAL,
	EXTERNAL
)

class NFIOData
{
public:
	NFGUID id;
	std::string name;//arg name
	NFValueType valueType;
	NFData varData;

	std::map<std::string, NFData> dictionaryData;

	NFIODataComFromType fromType = NFIODataComFromType::EXTERNAL;

	NFGUID GetLinkID()
	{
		return linkID;
	}

	void SetLinkID(NFGUID id)
	{
		linkID = id;
	}

private:
	NFGUID linkID;
};

class NFDataLink
{
private:
	NFDataLink() {}

public:
	NFDataLink(NFGUID selfID, NFGUID startNodeID, NFGUID endNodeID, NFGUID startAttr, NFGUID endAttr, const int index = -1, const int color = -1)
	{
		this->selfID = selfID;
		this->index = index;
		this->color = color;

		this->startNode = startNodeID;
		this->endNode = endNodeID;

		this->startAttr = startAttr;
		this->endAttr = endAttr;
	}

	int index;
	int color;
	NFGUID selfID;
	NFGUID startAttr;
	NFGUID endAttr;
	NFGUID startNode;
	NFGUID endNode;
};

class NFBluePrintNodeBase
{
protected:
	NFIPluginManager* pPluginManager;

	NFIBluePrintModule* pBluePrintModule;
	NFIElementModule* pElementModule;
	NFIClassModule* pClassModule;
	NFIKernelModule* m_pKernelModule;
	NFILogModule* m_pLogModule;

public:
	NFBluePrintNodeBase()
	{
	}

	virtual ~NFBluePrintNodeBase(){}

	void Init()
	{
		pBluePrintModule = pPluginManager->FindModule<NFIBluePrintModule>();
		pElementModule = this->pPluginManager->FindModule<NFIElementModule>();
		pClassModule = this->pPluginManager->FindModule<NFIClassModule>();
		m_pKernelModule = this->pPluginManager->FindModule<NFIKernelModule>();
		m_pLogModule = this->pPluginManager->FindModule<NFILogModule>();

		InitInputArgs();
		InitOutputArgs();

		//check input args count and out args count
	}

	const int GetInputArgCount()
	{
		return inputArgs.size();
	}

	const int GetOutputArgCount()
	{
		return outputArgs.size();
	}

	NF_SHARE_PTR<NFIOData> GetInputArg(const NFGUID& id)
	{
		for (int i = 0; i < inputArgs.size(); ++i)
		{
			auto input = inputArgs[i];
			if (input->id == id)
			{
				return input;
			}
		}

		return nullptr;
	}

	NF_SHARE_PTR<NFIOData> GetOutputArg(const NFGUID& id)
	{
		for (int i = 0; i < outputArgs.size(); ++i)
		{
			auto output = outputArgs[i];
			if (output->id == id)
			{
				return output;
			}
		}

		return nullptr;
	}

	//start from 0
	NF_SHARE_PTR<NFIOData> GetInputArg(const int index)
	{
		if (inputArgs.size() > index)
		{
			return inputArgs[index];
		}

		return nullptr;
	}

	//start from 0
	NF_SHARE_PTR<NFIOData> GetInputArg(const std::string name)
	{
		for (int i = 0; i < inputArgs.size(); ++i)
		{
			auto input = inputArgs[i];
			if (input->name == name)
			{
				return input;
			}
		}

		return nullptr;
	}

	//start from 0
	NF_SHARE_PTR<NFIOData> GetOutputArg(const int index)
	{
		if (outputArgs.size() > index)
		{
			return outputArgs[index];
		}

		return nullptr;
	}

	//start from 0
	NF_SHARE_PTR<NFIOData> GetOutputArg(const std::string name)
	{
		for (int i = 0; i < outputArgs.size(); ++i)
		{
			auto input = outputArgs[i];
			if (input->name == name)
			{
				return input;
			}
		}

		return nullptr;
	}

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() = 0;

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNextNode()
	{
		auto outputIO = FindOutputNodeIOData();
		if (outputIO)
		{
			NFGUID linkID = outputIO->GetLinkID();
			if (!linkID.IsNull())
			{
				auto linkData = this->pBluePrintModule->GetLink(linkID);
				if (linkData)
				{
					return this->pBluePrintModule->FindNode(linkData->endNode);
				}
			}
		}

		return nullptr;
	}

	virtual void InitInputArgs() = 0;
	virtual void InitOutputArgs() = 0;

	void Execute(const NFGUID& runTimeOnwer)
	{
		PrepareInputData(runTimeOnwer, false);
		UpdateOutputData(runTimeOnwer, false);
		auto nextNode = FindNextNode();
		if (nextNode)
		{
			nextNode->Execute(runTimeOnwer);
		}
	}

	//if iteration == true, only update the input and output data, dont create object or do things like that
	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration) = 0;
	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration) = 0;

	virtual void PrepareInputParameterData(NF_SHARE_PTR<NFIOData> inputParameter, const NFGUID& runTimeOnwer, const bool iteration = true)
	{
		if (inputParameter)
		{
			NFGUID linkID = inputParameter->GetLinkID();
			if (!linkID.IsNull())
			{
				auto linkData = this->pBluePrintModule->GetLink(linkID);
				if (linkData && !linkData->startNode.IsNull())
				{
					auto lastNode = this->pBluePrintModule->FindNode(linkData->startNode);
					if (lastNode)
					{
						auto lastAttr = lastNode->GetOutputArg(linkData->startAttr);
						if (lastAttr && lastAttr->valueType == inputParameter->valueType)
						{
							lastNode->PrepareInputData(runTimeOnwer, iteration);
							lastNode->UpdateOutputData(runTimeOnwer, iteration);


							switch (inputParameter->valueType)
							{
							case NFValueType::Int:
							case NFValueType::Float:
							case NFValueType::String:
							case NFValueType::Object:
							case NFValueType::Vector2:
							case NFValueType::Vector3:
							case NFValueType::Array:
							case NFValueType::Dictionary:
								inputParameter->varData = lastAttr->varData;
								inputParameter->dictionaryData = lastAttr->dictionaryData;
								break;
							default:
								break;
							}
						}
					}
				}
			}
		}
	}

	bool enable = true;//only for logic block
	bool running = false;//only for logic block
	NFGUID logicBlockId;
	NFGUID id;
	std::string name;
	NFBlueprintType blueprintType = NFBlueprintType::LOGICBLOCK;

	std::vector<NF_SHARE_PTR<NFIOData>> inputArgs;
	std::vector<NF_SHARE_PTR<NFIOData>> outputArgs;
};

class NFIArithmetic : public NFBluePrintNodeBase
{
public:
	NFIArithmetic()
	{
		blueprintType = NFBlueprintType::ARITHMETIC;
	}

	NFArithmeticType arithmeticType;
};

class NFIBranch : public NFBluePrintNodeBase
{
public:
	NFIBranch()
	{
		blueprintType = NFBlueprintType::BRANCH;
	}

	NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override
	{
		NF_SHARE_PTR<NFIOData> output = nullptr;
		if (result)
		{
			output = GetOutputArg(NFBranchOutputArg::TrueOut);
		}
		else
		{
			output = GetOutputArg(NFBranchOutputArg::FalseOut);
		}

		return output;
	}

	NFBranchType branchType;
	NFComparatorType comparatorType;
	bool result = false;
};

class NFIVariable : public NFBluePrintNodeBase
{
public:
	NFIVariable()
	{
		blueprintType = NFBlueprintType::VARIABLE;
	}

	NFVariableType variableType;
};

class NFIMonitor : public NFBluePrintNodeBase
{
public:
	NFIMonitor()
	{
		blueprintType = NFBlueprintType::MONITOR;
	}

	NFMonitorType monitorType;

public:
};

class NFIModifier : public NFBluePrintNodeBase
{
public:
	NFIModifier()
	{
		blueprintType = NFBlueprintType::MODIFIER;
	}

	NFModifierType modifierType;
};

class NFIExecuter : public NFBluePrintNodeBase
{
public:
	NFIExecuter()
	{
		blueprintType = NFBlueprintType::EXECUTER;
	}
	NFExecuterType executerType;
};

class NFIDebugger : public NFBluePrintNodeBase
{
public:
	NFIDebugger()
	{
		blueprintType = NFBlueprintType::DEBUGER;
	}
	//NFExecuterType executerType;
};

class NFLogicBlock : public NFBluePrintNodeBase
{
private:
	NFLogicBlock() {}

public:
	NFLogicBlock(NFIPluginManager* p, const NFGUID& id, const std::string& name)
	{
		this->logicBlockId = id;
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->blueprintType = NFBlueprintType::LOGICBLOCK;
	}

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindNode(const NFGUID& id);

	virtual void InitInputArgs()
	{

	}

	virtual void InitOutputArgs()
	{

	}

	virtual void PrepareInputData(const NFGUID& runTimeOnwer, const bool iteration)
	{

	}

	virtual void UpdateOutputData(const NFGUID& runTimeOnwer, const bool iteration)
	{

	}

	virtual NF_SHARE_PTR<NFIOData> FindOutputNodeIOData() override
	{
		return nullptr;
	}

	std::list<NF_SHARE_PTR<NFIExecuter>> executers;
	std::list<NF_SHARE_PTR<NFIBranch>> branches;
	std::list<NF_SHARE_PTR<NFIMonitor>> monitors;
	std::list<NF_SHARE_PTR<NFIVariable>> variables;
	std::list<NF_SHARE_PTR<NFIModifier>> modifiers;
	std::list<NF_SHARE_PTR<NFIArithmetic>> arithmetics;
	std::list<NF_SHARE_PTR<NFIDebugger>> debuggers;

	std::list<NF_SHARE_PTR<NFDataLink>> dataLinks;

};

#endif