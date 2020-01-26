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
#ifndef NFI_BLUE_PRINT_MODULE_H
#define NFI_BLUE_PRINT_MODULE_H

#include <iostream>
#include <map>
#include <list>
#include "NFIModule.h"
#include "NFIKernelModule.h"

NF_SMART_ENUM(NFBlueprintType,
	LOGICBLOCK,
	MONITOR,
	BRANCH,
	EXECUTER,
	VARIABLE,
	MODIFIER,
	ARITHMETIC,
	LOGGER,
	CUSTOM,
)

//--------FOR VARIABLE----------------
NF_SMART_ENUM(NFElementVariableInputArg,
	ClassName,
	ConfigID,
	PropertyName,
	)

	NF_SMART_ENUM(NFPropertyVariableInputArg,
		ObjectID,
		ClassName,
		PropertyName,
		)

	NF_SMART_ENUM(NFPropertyVariableOutputArg,
		ClassName,
		PropertyName,
		PropertyValue,
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
		ObjectID,
		)
	//------------------------------
	NF_SMART_ENUM(NFNetworkEventMonitorInputArg,
		EventID,
		)

	NF_SMART_ENUM(NFNetworkEventMonitorOutputArg,
		NextNode,
		ObjectID,
		)
	//------------------------------
	NF_SMART_ENUM(NFNetworkMsgMonitorInputArg,
		NetMsgID,
		)

	NF_SMART_ENUM(NFNetworkkMsgMonitorOutputArg,
		NextNode,
		ObjectID,
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
		PropertyName,
		)

	NF_SMART_ENUM(NFMonitorPropertyEventOutputArg,
		NextNode,
		ObjectID,
		PropertyName,
		PropertyValue,
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
	PropertyName,
	PropertyValue
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


///////////FOR EXECUTER BEGIN///////////////////////////
NF_SMART_ENUM(NFExecuterType,
	CreateObject,
	DestroyObject,
	MoveObject,
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

NF_SMART_ENUM(NFExecuterMoveObjectInputArg,
	LastNode,
	ObjectID,
	Position,
	MoveType,
	)

NF_SMART_ENUM(NFExecuterMoveObjectOutputArg,
	NextNode,
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
	std::string varData;
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

class NFBluePrintNodeBase
{
protected:
	NFIPluginManager* pPluginManager;

private:

public:

	virtual void InitInputArgs() = 0;
	virtual void InitOutputArgs() = 0;

	/*
	IMPORTANT:
	IF A NODE'S PIN LINKED WITH OTHER NODE'S PIN, WHICH MEANS THE DATA OF THIS PIN COME FROM OTHER NODE, 
	AS THE RESULT, WE NEED TO LOOP THE FUNCTION UpdateOutputData TO GET THE RIGHT VALUE OF THIS DATA.
	*/
	virtual void UpdateOutputData() = 0;

public:
	NFBluePrintNodeBase()
	{
	}

	virtual ~NFBluePrintNodeBase(){}

	void Init()
	{
		InitInputArgs();
		InitOutputArgs();
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

	NF_SHARE_PTR<NFIOData> GetOutputArg(const int index)
	{
		if (outputArgs.size() > index)
		{
			return outputArgs[index];
		}

		return nullptr;
	}

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

	bool enable = true;//only for logic block
	bool running = false;//only for logic block
	NFGUID logicBlockId;
	NFGUID id;
	std::string name;
	NFBlueprintType blueprintType = NFBlueprintType::LOGICBLOCK;

	std::vector<NF_SHARE_PTR<NFIOData>> inputArgs;
	std::vector<NF_SHARE_PTR<NFIOData>> outputArgs;
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

	NFBranchType branchType;
	NFComparatorType comparatorType;
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
	virtual void UpdateOutputData()
	{

	}

	std::list<NF_SHARE_PTR<NFIExecuter>> executers;
	std::list<NF_SHARE_PTR<NFIBranch>> branches;
	std::list<NF_SHARE_PTR<NFIMonitor>> monitors;
	std::list<NF_SHARE_PTR<NFIVariable>> variables;
	std::list<NF_SHARE_PTR<NFIModifier>> modifiers;
	std::list<NF_SHARE_PTR<NFIArithmetic>> arithmetics;

	std::list<NF_SHARE_PTR<NFDataLink>> dataLinks;

};

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

#endif