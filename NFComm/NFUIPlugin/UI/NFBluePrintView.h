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
#ifndef NF_BLUE_PRINT_VIEW_H
#define NF_BLUE_PRINT_VIEW_H

#include "NFComm/NFPluginModule/NFIUIModule.h"

class NFBluePrintView : public NFIView
{
public:
	NFBluePrintView(NFIPluginManager* p, NFViewType vt);

	//1 logic block must has at least 1 monitor, at least 1 judgement and at least 1 executer
	//normally 1 judgement has 1 executer
	enum LogicBlock
	{
		Monitor,
		Judgement,
		Executer
	};

	enum CommonInterfaces//access to NFrame for executer and judgement
	{
		//property
		//record
	};

	enum MonitorType
	{
		NetworkEvent,
		NetworkMsgEvent,
		ObjectEvent,
		PropertyEvent,
		HeartBeatEvent,
		SceneEvent,
		ItemEvent,
		SkillEvent,
		BuffEvent,
	};

	enum JudgementType
	{

	};

	enum ExecuterType
	{

	};

};

#endif