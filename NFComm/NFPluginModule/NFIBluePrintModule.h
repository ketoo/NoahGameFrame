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
#include "NFIModule.h"
#include "NFIKernelModule.h"

NF_SMART_ENUM(NFBlueprintType,
	LOGICBLOCK,
	MONITOR,
	JUDGEMENT,
	EXECUTER,
)

NF_SMART_ENUM(NFMonitorType,
	NONE,
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

NF_SMART_ENUM(NFMonitorObjectEventArgType,
	ClassName,
	)

NF_SMART_ENUM(NFMonitorPropertyEventArgType,
	ClassName,
	PropertyName,
)

NF_SMART_ENUM(NFMonitorRecordEventArgType,
	ClassName,
	RecordName,
	OperateType,
)

//ObjectEvent
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

//RecordEvent
NF_SMART_ENUM(NFMonitorRecordEventType,
	Add,
	Remove,
	Update,
)

//2 args: string elementName, string propertyName
NF_SMART_ENUM(NFAccessorType,
	NONE,
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

//SetProperty 3 args: NFGUID objectID, string propertyName, int value
//SetRecord 5 args: NFGUID objectID, string recordName, int row, int col, int value
NF_SMART_ENUM(NFModifierType,
	NONE,
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

NF_SMART_ENUM(NFOperatorType,
	NONE,
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

NF_SMART_ENUM(NFJudgementCondition,
	NONE,
	Equal,
	EnEqual,
	MoreThen,
	LessThan,
	ExistElement,
	ExistObject,
	)

NF_SMART_ENUM(NFComparatorType,
	NONE,
	Equal,
	EnEqual,
	MoreThen,
	LessThan,
	ExistElement,
	ExistObject,
	)

class NFBluePrintNodeBase
{
protected:
	NFIPluginManager* pPluginManager;

public:
	virtual ~NFBluePrintNodeBase(){}

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindBaseNode(const NFGUID& id) = 0;

	bool enable = true;
	NFGUID logicBlockId;
	NFGUID id;
	std::string name;
	std::string desc;
	NF_SHARE_PTR<NFBluePrintNodeBase> parent;
	NFBlueprintType blueprintType = NFBlueprintType::LOGICBLOCK;
};

class NFLogicBlock;
class NFMonitor;
class NFJudgement;
class NFExecuter;


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

	NFModifierType modifierType = NFModifierType::NONE;
	NFOperatorType operatorType = NFOperatorType::NONE;

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
		//VariableType variableType; //maybe the designer input the value, maybe come from accessor
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

class NFMonitor : public NFBluePrintNodeBase
{
private:
	NFMonitor() {}
public:
	NFMonitor(NFIPluginManager* p, const NFGUID& id, const std::string& name, NF_SHARE_PTR<NFBluePrintNodeBase> parent)
	{
		this->id = id;
		this->name = name;
		this->pPluginManager = p;
		this->parent = parent;

		blueprintType = NFBlueprintType::MONITOR;
	}

	virtual NF_SHARE_PTR<NFBluePrintNodeBase> FindBaseNode(const NFGUID& id);

public:
	NFDataList arg;
	NFMonitorType operatorType = NFMonitorType::NONE;
	int subType = 0;
	std::list<NF_SHARE_PTR<NFJudgement>> judgements;
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

	std::list<NF_SHARE_PTR<NFMonitor>> monitors;
};

class NFIBluePrintModule
    : public NFIModule
{
public:

	////////////////////////////////////////
	virtual void SetLogicBlockEventFunctor(std::function<void(const NFGUID&, const bool)> functor) = 0;

    virtual NF_SHARE_PTR<NFLogicBlock> CreateLogicBlock(const NFGUID& logicBlockId, const std::string& name) = 0;
    virtual const std::list<NF_SHARE_PTR<NFLogicBlock>>& GetLogicBlocks() = 0;
	virtual NF_SHARE_PTR<NFLogicBlock>  GetLogicBlock(const NFGUID& logicBlockId) = 0;

	virtual NF_SHARE_PTR<NFBluePrintNodeBase>  FindBaseNode(const NFGUID& id) = 0;

	virtual NF_SHARE_PTR<NFMonitor> AddMonitorForLogicBlock(const NFGUID& logicBlockId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFJudgement> AddJudgementForMonitor(const NFGUID& monitorId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFJudgement> AddTrueJudgementForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFJudgement> AddFalseJudgementForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFJudgement> AddJudgementForExecuter(const NFGUID& executerId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFExecuter> AddTrueExecuterForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFExecuter> AddFalseExecuterForJudgement(const NFGUID& judgementId, const NFGUID& id, const std::string& name) = 0;
	virtual NF_SHARE_PTR<NFExecuter> AddExecuterForExecuter(const NFGUID& executerId, const NFGUID& id, const std::string& name) = 0;

	virtual bool DeleteMonitor(const NFGUID& id) = 0;
	virtual bool DeleteJudgement(const NFGUID& id) = 0;
	virtual bool DeleteExecuter(const NFGUID& id) = 0;


};

#endif