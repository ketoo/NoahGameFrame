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
	JUDGEMENT,
	EXECUTER,
	VARIABLE,
	DEBUGER,
)

///////////FOR MONITOR BEGIN///////////////////////////
NF_SMART_ENUM(NFMonitorType,
	GameEvent,
	NetworkEvent,
	NetworkMsgEvent,
	ObjectEvent,
	PropertyEvent,
	RecordEvent,
	HeartBeatEvent,
	SceneEvent,
	ItemEvent,
	SkillEvent,
	BuffEvent,
)


	//NetworkEvent INPUT
	//NetworkMsgEvent INPUT

//------------------------
//GameEvent INPUT 1
NF_SMART_ENUM(NFMonitorGameEventArgType,
	EventID,
	)
//ObjectEvent INPUT 1
NF_SMART_ENUM(NFMonitorObjectEventArgType,
	ClassName,
	)

//ObjectEvent INPUT 2
NF_SMART_ENUM(NFMonitorObjectEventType,
	CREATE_NODATA,
	CREATE_BEFORE_ATTACHDATA,
	CREATE_LOADDATA,
	CREATE_AFTER_ATTACHDATA,
	CREATE_BEFORE_EFFECT,
	CREATE_EFFECTDATA,
	CREATE_AFTER_EFFECT,
	CREATE_HASDATA,
	CREATE_FINISH,
	CREATE_CLIENT_FINISH,
	BEFOREDESTROY,
	DESTROY,
	)


//------------------------

//PropertyEvent INPUT
NF_SMART_ENUM(NFMonitorPropertyEventArgType,
	ClassName,
	PropertyName,
)

//------------------------

//RecordEvent INPUT 1
NF_SMART_ENUM(NFMonitorRecordEventArgType,
	ClassName,
	RecordName,
	OperateType,
)
//RecordEvent INPUT 2
NF_SMART_ENUM(NFMonitorRecordEventType,
	Add,
	Remove,
	Update,
	)

//------------------------

NF_SMART_ENUM(NFValueType,
	Int,
	Float,
	String,
	Vector2,
	Vector3,
	Object,
	)

NF_SMART_ENUM(NFVariableType,
	Input,
	ElementSystem,
	PropertySystem,
	RecordSystem,
	)


//------------------------
///////////FOR ACCESSOR BEGIN///////////////////////////
//THREE WAY TO GET A VALUE
//ONE IS TO GET A VALUE COME FROM ELEMENT SYSTEM
//TWO IS TO GET A VALUE COME FROM PROPERTY SYSTEM OR RECORD SYSTEM OF A PLAYER OR SCENE
//THREE IS TO GET A VALUE COME FROM VARIABLE SYSTEM


//INPUT FOR ONE:
//ELEMENT ID --> COME FROM INPUT CONTRLLER OR ANOTHER PLACE (MAYBE A VARIABLE, MAYBE A ELEMENT OR A PROPERTY)
//ELEMENT PROPERTY NAME

//2 args: string elementName, string propertyName
NF_SMART_ENUM(NFAccessorType,
	GetElementInt,
	GetElementFloat,
	GetElementString,
	GetElementVector2,
	GetElementVector3,
	GetPropertyInt,
	GetPropertyFloat,
	GetPropertyString,
	GetPropertyVector2,
	GetPropertyVector3,
	GetPropertyObject,
	GetRecordInt,
	GetRecordFloat,
	GetRecordString,
	GetRecordVector2,
	GetRecordVector3,
	GetRecordObject,
)

///////////FOR MODIFIER BEGIN///////////////////////////
//SetProperty 3 args: NFGUID objectID, string propertyName, int value
//SetRecord 5 args: NFGUID objectID, string recordName, int row, int col, int value
NF_SMART_ENUM(NFModifierType,
	SetPropertyInt,
	SetPropertyFloat,
	SetPropertyString,
	SetPropertyVector2,
	SetPropertyVector3,
	SetPropertyObject,
	SetRecordInt,
	SetRecordFloat,
	SetRecordString,
	SetRecordVector2,
	SetRecordVector3,
	SetRecordObject,
	AddRecordRow,
	ReemRecordRow,
)

///////////FOR OPERATOR BEGIN///////////////////////////
NF_SMART_ENUM(NFOperatorType,
	CreateObject,
	DestroyObject,
	MoveObject,
	EnterScene,
	LeaveScene,
	EnterGroup,
	LeaveGroup,
	AddHeartBeat,
	RemoveHeartBeat,
	AttackObject,
	UseSkill,
	UseItem
)

///////////FOR JUDGEMENT BEGIN///////////////////////////
NF_SMART_ENUM(NFJudgementCondition,
	Equal,
	EnEqual,
	MoreThen,
	LessThan,
	ExistElement,
	ExistObject,
	)

NF_SMART_ENUM(NFComparatorType,
	Equal,
	EnEqual,
	MoreThen,
	LessThan,
	ExistElement,
	ExistObject,
	)


///////////////////////////////////////////////////////////////////////////////////
class NFLogicBlock;
class NFIMonitor;
class NFJudgement;
class NFExecuter;
class NFIVariable;
class NFInputData;
class NFDataLink;

class NFInputData
{
public:
	NFGUID id;
	std::string name;
	int index;
	NFValueType valueType;
	std::string varData;
	NF_SHARE_PTR<NFDataLink> dataLink;
};

class NFBluePrintNodeBase
{
protected:
	NFIPluginManager* pPluginManager;

public:
	virtual ~NFBluePrintNodeBase(){}

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindBaseNode(const NFGUID& id) = 0;

	const int GetInputArgCount()
	{
		return inputArgs.size();
	}

	//start from 0
	NF_SHARE_PTR<NFInputData> GetInputArg(const int index)
	{
		if (inputArgs.size() > index)
		{
			return inputArgs[index];
		}

		return nullptr;
	}

	NF_SHARE_PTR<NFInputData> GetInputArg(const std::string name)
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

	bool enable = true;
	NFGUID logicBlockId;
	NFGUID id;
	std::string name;
	std::string desc;
	NF_SHARE_PTR<NFBluePrintNodeBase> parent;
	NFBlueprintType blueprintType = NFBlueprintType::LOGICBLOCK;

	std::vector<NF_SHARE_PTR<NFInputData>> inputArgs;
};

class NFDataLink
{
private:
	NFDataLink() {}

public:
	NFDataLink(NFGUID startNodeID, NFGUID endNodeID, NFGUID startAttr, NFGUID endAttr, const int index = -1)
	{
		this->index = index;

		this->startNode = startNodeID;
		this->endNode = endNodeID;

		this->startAttr = startAttr;
		this->endAttr = endAttr;
	}

	int index;
	NFGUID startAttr;
	NFGUID endAttr;
	NFGUID startNode;
	NFGUID endNode;
};


NF_SMART_ENUM(NFElementVariableInputArg,
	ClassName,
	ElementConfigID,
	ElementPropertyName,
	)

NF_SMART_ENUM(NFPropertyVariableInputArg,
	OnwerID,
	ClassName,
	PropertyName,
	)

NF_SMART_ENUM(NFRecordVariableInputArg,
	OnwerID,
	RecordName,
	RecordRow,
	RecordCol,
	)


class NFIVariable : public NFBluePrintNodeBase
{
protected:
	NFIVariable() {}
public:
	NFIVariable(NFIPluginManager* p, const NFGUID& blockID, const NFGUID& id, const std::string& name, NF_SHARE_PTR<NFBluePrintNodeBase> parent)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->parent = parent;
		this->logicBlockId = blockID;

		blueprintType = NFBlueprintType::VARIABLE;
	}

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindBaseNode(const NFGUID& id);

public:
	NFValueType valueType = NFValueType::String;
	NFVariableType variableType = NFVariableType::Input;


	NF_SHARE_PTR<NFDataLink> output;
};

//developer could define different executer by needs
//the executer defined by developer could be listed when the developer picking one executer or designing a executer
class NFExecuter : public NFBluePrintNodeBase
{
private:
	NFExecuter() {}
public:
	NFExecuter(NFIPluginManager* p, const NFGUID& id, const std::string& name, NF_SHARE_PTR<NFBluePrintNodeBase> parent)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->parent = parent;

		blueprintType = NFBlueprintType::EXECUTER;
	}
	
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindBaseNode(const NFGUID& id);


	//modifier

	NFModifierType modifierType = NFModifierType::SetPropertyString;
	NFOperatorType operatorType = NFOperatorType::CreateObject;

	//a executer could has a executer or a judgement
	NF_SHARE_PTR<NFExecuter> nextExecuter;
	std::list<NF_SHARE_PTR<NFJudgement>> judgements;
};

class NFComparator
{
public:

	//NFComparator is to campare the left variable and right variable
	class NFComparatorVariable
	{
		//VariableType valueType; //maybe the designer input the value, maybe come from accessor
		//NFAccessor
	};

	bool And = true;//false is or
	NFComparatorType compareType; // a > 0  ==> leftVariable > rightVariable
	NFComparatorVariable leftVariable;
	NFComparatorVariable rightVariable;
};

class NFJudgement : public NFBluePrintNodeBase
{
private:
	NFJudgement() {}

public:
	NFJudgement(NFIPluginManager* p, const NFGUID& id, const std::string& name, NF_SHARE_PTR<NFBluePrintNodeBase> parent)
	{
		this->name = name;
		this->id = id;
		this->pPluginManager = p;
		this->parent = parent;

		blueprintType = NFBlueprintType::JUDGEMENT;
	}

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindBaseNode(const NFGUID& id);

	std::string arg;

	//true node and false node, maybe a executer, maybe a judgement
	NF_SHARE_PTR<NFBluePrintNodeBase> trueBlueprintNode;
	NF_SHARE_PTR<NFBluePrintNodeBase> falseBlueprintNode;

	std::vector<NF_SHARE_PTR<NFComparator>> comparators;
 };

class NFIMonitor : public NFBluePrintNodeBase
{
public:
	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindBaseNode(const NFGUID& id)
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
		return nullptr;
	}

	virtual void InitInputArgs() = 0;

	NFMonitorType GetMonitorType()
	{
		return operatorType;
	}

	const int GetMonitorJudgementCount()
	{
		return judgements.size();
	}

	//start from 0
	NF_SHARE_PTR<NFJudgement> GetJudgement(const int index)
	{
		if (judgements.size() > index)
		{
			return judgements[index];
		}

		return nullptr;
	}

	void AddJudgement(NF_SHARE_PTR<NFJudgement> judgement)
	{
		judgements.push_back(judgement);
	}

protected:
	NFMonitorType operatorType = NFMonitorType::GameEvent;

	std::vector<NF_SHARE_PTR<NFJudgement>> judgements;

public:
};

//1 logic block must has at least 1 monitor, at least 1 judgement and at least 1 executer
//normally 1 judgement has 1 executer or more with different conditions
//blueprint block
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

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindBaseNode(const NFGUID& id);

	std::list<NF_SHARE_PTR<NFIMonitor>> monitors;
	std::list<NF_SHARE_PTR<NFIVariable>> variables;
	std::list<NF_SHARE_PTR<NFDataLink>> dataLinks;
};

class NFIBluePrintModule
    : public NFIModule
{
public:

	////////////////////////////////////////
	virtual void SetNodeModifyEventFunctor(std::function<void(const NFGUID&, const bool)> functor) = 0;
	virtual void SetLinkModifyEventFunctor(std::function<void(const NFGUID&, const const NFGUID&, const NFGUID&, const const NFGUID&, const bool)> functor) = 0;

    virtual NF_SHARE_PTR<NFLogicBlock> CreateLogicBlock(const NFGUID& logicBlockId, const std::string& name) = 0;
    virtual const std::list<NF_SHARE_PTR<NFLogicBlock>>& GetLogicBlocks() = 0;
	virtual NF_SHARE_PTR<NFLogicBlock>  GetLogicBlock(const NFGUID& logicBlockId) = 0;

	virtual NF_SHARE_PTR<NFBluePrintNodeBase>  FindBaseNode(const NFGUID& id) = 0;

	virtual NF_SHARE_PTR<NFIMonitor> AddMonitorForLogicBlock(const NFGUID& logicBlockId, const NFMonitorType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFIVariable> AddVariableForLogicBlock(const NFGUID& logicBlockId, const NFVariableType type, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFJudgement> AddJudgementForMonitor(const NFGUID& monitorId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFJudgement> AddTrueJudgementForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFJudgement> AddFalseJudgementForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFJudgement> AddJudgementForExecuter(const NFGUID& executerId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFExecuter> AddTrueExecuterForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFExecuter> AddFalseExecuterForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFExecuter> AddExecuterForExecuter(const NFGUID& executerId, const NFGUID& id, const std::string& name) = 0;

	virtual void AddLink(const NFGUID& logicBlockId, const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin) = 0;
	virtual NF_SHARE_PTR<NFDataLink> GetLink(const NFGUID& logicBlockId, const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin) = 0;
	virtual void DeleteLink(const NFGUID& logicBlockId, const NFGUID& startNode, const const NFGUID& endNode, const NFGUID& startPin, const const NFGUID& endPin) = 0;
	virtual std::list<NF_SHARE_PTR<NFDataLink>> GetLinks(const NFGUID& logicBlockId) = 0;

	virtual bool DeleteMonitor(const NFGUID& id) = 0;
	virtual bool DeleteJudgement(const NFGUID& id) = 0;
	virtual bool DeleteExecuter(const NFGUID& id) = 0;
};

#endif