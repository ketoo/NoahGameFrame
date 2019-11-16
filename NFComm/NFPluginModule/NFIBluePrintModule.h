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

class NFIBluePrintModule
    : public NFIModule
{
public:
	NF_SMART_ENUM(MyEnum,
		ONE=1,
		TWO,
		THREE,
		TEN=10,
		ELEVEN
		)
		/*
	NF_SMART_ENUM(MonitorType,
		NetworkEvent,
		NetworkMsgEvent,
		ObjectEvent,
		PropertyEvent,
		HeartBeatEvent,
		SceneEvent,
		ItemEvent,
		SkillEvent,
		BuffEvent
	)
*/
	enum AccessorType
	{
		GetElementInt = 0,//2 args: string elementName, string propertyName
		GetElementFloat = 1,//2 args: string elementName, string propertyName
		GetElementString = 2,//2 args: string elementName, string propertyName
		GetElementVector2 = 3,//2 args: string elementName, string propertyName
		GetElementVector3 = 4,//2 args: string elementName, string propertyName

		GetPropertyInt = 10,//2 args: NFGUID objectID, string propertyName
		GetPropertyFloat = 11,//2 args: NFGUID objectID, string propertyName
		GetPropertyString = 12,//2 args: NFGUID objectID, string propertyName
		GetPropertyVector2 = 13,//2 args: NFGUID objectID, string propertyName
		GetPropertyVector3 = 14,//2 args: NFGUID objectID, string propertyName
		GetPropertyObject = 15,//2 args: NFGUID objectID, string propertyName

		GetRecordInt = 20,//4 args: NFGUID objectID, string recordName, int row, int col
		GetRecordFloat = 21,//4 args: NFGUID objectID, string recordName, int row, int col
		GetRecordString = 22,//4 args: NFGUID objectID, string recordName, int row, int col
		GetRecordVector2 = 23,//4 args: NFGUID objectID, string recordName, int row, int col
		GetRecordVector3 = 24,//4 args: NFGUID objectID, string recordName, int row, int col
		GetRecordObject = 25//4 args: NFGUID objectID, string recordName, int row, int col
	};

	enum ModifierType
	{
		SetPropertyInt = 10,//3 args: NFGUID objectID, string propertyName, int value
		SetPropertyFloat = 11,//3 args: NFGUID objectID, string propertyName, int value
		SetPropertyString = 12,//3 args: NFGUID objectID, string propertyName, int value
		SetPropertyVector2 = 13,//3 args: NFGUID objectID, string propertyName, int value
		SetPropertyVector3 = 14,//3 args: NFGUID objectID, string propertyName, int value
		SetPropertyObject = 15,//3 args: NFGUID objectID, string propertyName, int value

		SetRecordInt = 20,//5 args: NFGUID objectID, string recordName, int row, int col, int value
		SetRecordFloat = 21,//5 args: NFGUID objectID, string recordName, int row, int col, int value
		SetRecordString = 22,//5 args: NFGUID objectID, string recordName, int row, int col, int value
		SetRecordVector2 = 23,//5 args: NFGUID objectID, string recordName, int row, int col, int value
		SetRecordVector3 = 24,//5 args: NFGUID objectID, string recordName, int row, int col, int value
		SetRecordObject = 25//5 args: NFGUID objectID, string recordName, int row, int col, int value
	};

	enum OperatorType
	{
		CreateObject = 0,
		DestroyObject = 1,
		MoveObject = 2,

		EnterScene = 10,
		LeaveScene = 11,
		EnterGroup = 12,
		LeaveGroup = 13,

		AddHeartBeat = 20,
		RemoveHeartBeat = 21,

		AttackObject = 30,
		UseSkill = 31,
		UseItem = 32
	};

	enum JudgementType
	{
		Equal = 0,
		EnEqual = 1,

		MoreThen = 10,
		LessThan = 11,

		ExistElement = 20,
		ExistObject = 21,
	};

	//1 logic block must has at least 1 monitor, at least 1 judgement and at least 1 executer
	//normally 1 judgement has 1 executer or more with different conditions

	//blueprint block
	class LogicBlock
	{
		/*
		Monitor,
		Judgement,
		Executer
		*/

	};

	class Executer
	{
	public:

		//modifier
	};

	class Judgement
	{
	public:

		std::list<Executer> judgements;
	};

	class Monitor
	{
	public:

		std::list<Judgement> judgements;
	};

	

	
};

#endif