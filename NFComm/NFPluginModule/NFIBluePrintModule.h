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

class NFIBluePrintModule
    : public NFIModule
{
public:
	NF_SMART_ENUM(MyEnum,
		ONE,
		TWO,
		THREE,
		TEN,
		ELEVEN
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
		BuffEvent
	)

	//PropertyEvent
	NF_SMART_ENUM(NFMonitorPropertyEventType,
		Update
	)
	
	//RecordEvent
	NF_SMART_ENUM(NFMonitorRecordEventType,
		Add,
		Remove,
		Update
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
		GetRecordObject
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
		SetRecordObject
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

	NF_SMART_ENUM(NFJudgementType,
		NONE,
		Equal,
		EnEqual,
		MoreThen,
		LessThan,
		ExistElement,
		ExistObject,
	)

	class BluePrintNodeBase
	{
	protected:
		NFIPluginManager* pPluginManager;

	public:
		NFGUID id;
		std::string name;
		std::string desc;
	};

	//developer could define different executer by needs
	//the executer defined by developer could be listed when the developer picking one executer or designing a executer
	class NFExecuter : public BluePrintNodeBase
	{	
	private:
		NFExecuter(){}
	public:
		NFExecuter(NFIPluginManager* p)
		{
			this->pPluginManager = p;
		}

		//modifier

		NFModifierType modifierType = NFModifierType::NONE;
		NFOperatorType operatorType = NFOperatorType::NONE;

		//if this executer has next executer, then do next executer first
		NF_SHARE_PTR<NFExecuter> nextExecuter;

		std::map<NFJudgementType, NF_SHARE_PTR<NFExecuter>> executers;
	};
	class NFMonitor ;
	class NFJudgement : public BluePrintNodeBase
	{	
	private:
		NFJudgement(){}

		NF_SHARE_PTR<NFMonitor> monitor;
	
	public:
		NFJudgement(NFIPluginManager* p, NF_SHARE_PTR<NFMonitor> monitor)
		{
			this->pPluginManager = p;
			this->monitor = monitor;
		}

		std::string arg;

		NFJudgementType judgementType = NFJudgementType::NONE;
		//if this judgment has next judgment, then do next judgment first
		NF_SHARE_PTR<NFJudgement> nextJudgement;
		std::map<NFJudgementType, NF_SHARE_PTR<NFExecuter>> executers;
	};

	class NFMonitor : public BluePrintNodeBase
	{	
	private:
		NFMonitor(){}
	public:
		NFMonitor(NFIPluginManager* p)
		{
			this->pPluginManager = p;
		}

		NF_SHARE_PTR<NFJudgement> AddJudgment(NFJudgementType judgementType, const std::string& name, const std::string& desc, const NFDataList& arg)
		{
			

			return nullptr;;
		}
public:
		NFDataList arg;
		NFMonitorType operatorType = NFMonitorType::NONE;
		std::list<NF_SHARE_PTR<NFJudgement>> judgements;
	};

	//1 logic block must has at least 1 monitor, at least 1 judgement and at least 1 executer
	//normally 1 judgement has 1 executer or more with different conditions
	//blueprint block
	class NFLogicBlock : public BluePrintNodeBase
	{
	private:
		NFLogicBlock(){}

	public:
		NFLogicBlock(NFIPluginManager* p, const std::string& name)
		{
			this->pPluginManager = p;
			this->name = name;
		}

		NF_SHARE_PTR<NFMonitor> AddMonitor(NFMonitorType operatorType, const std::string& desc, const NFDataList& arg)
		{
			NF_SHARE_PTR<NFMonitor> monitor = nullptr;

			//根据不同的monitor type，参数会不一样
			//必须先选择了monitor type，才知道需要什么参数
			switch(operatorType)
			{
				case NFMonitorType::NONE:
				break;
				case NFMonitorType::PropertyEvent:
				{
					//arg: property_name
					if (arg.GetCount() == 1)
					{
						monitor = NF_SHARE_PTR<NFMonitor>(NF_NEW NFMonitor(this->pPluginManager));
						monitor->id = this->pPluginManager->FindModule<NFIKernelModule>()->CreateGUID();
						//monitor->name = name;
						monitor->desc = desc;
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

public:
		std::list<NF_SHARE_PTR<NFMonitor>> monitors;
	};

	////////////////////////////////////////

    virtual NF_SHARE_PTR<NFLogicBlock> CreateLogicBlock(const std::string& name) = 0;
    virtual const std::list<NF_SHARE_PTR<NFLogicBlock>>& GetLogicBlocks() = 0;
    virtual NF_SHARE_PTR<NFLogicBlock>  GetLogicBlock(const NFGUID id) = 0;
    virtual NF_SHARE_PTR<NFLogicBlock>  GetLogicBlock(const std::string& name) = 0;

	
};

#endif