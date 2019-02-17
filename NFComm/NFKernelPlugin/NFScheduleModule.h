/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2018 NoahFrame(NoahGameFrame)

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

#ifndef NF_SCHEDULE_MODULE_H
#define NF_SCHEDULE_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFPerformance.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"

class  NFScheduleElement
{
public:
	NFScheduleElement()
	{
		mstrScheduleName = "";
		mfIntervalTime = 0.0f;
		mnNextTriggerTime = 0;
		mnStartTime = 0;
		mnRemainCount = 0;
		mnAllCount = 0;
		mbForever = false;
	};

	virtual ~NFScheduleElement()
	{
	}

	void DoHeartBeatEvent();

	std::string mstrScheduleName;
	float mfIntervalTime;
	NFINT64 mnNextTriggerTime;
	NFINT64 mnStartTime;
	int mnRemainCount;
	int mnAllCount;
	bool mbForever;

	NFGUID self;

	NFList<OBJECT_SCHEDULE_FUNCTOR_PTR> mxObjectFunctor;
	NFList<MODULE_SCHEDULE_FUNCTOR_PTR> mxModuleFunctor;
};

class NFScheduleModule : public NFIScheduleModule
{
public:
	NFScheduleModule(NFIPluginManager* p);

	virtual ~NFScheduleModule();

	virtual bool Init();
	virtual bool Execute();

	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount);
	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const NFDateTime& date);
	virtual bool RemoveSchedule(const std::string& strScheduleName);
	virtual bool ExistSchedule(const std::string& strScheduleName);
	
	
	virtual bool AddSchedule(const NFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount);
	virtual bool AddSchedule(const NFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const NFDateTime& date);
	virtual bool RemoveSchedule(const NFGUID self);
	virtual bool RemoveSchedule(const NFGUID self, const std::string& strScheduleName);
	virtual bool ExistSchedule(const NFGUID self, const std::string& strScheduleName);



protected:
	NFMapEx<NFGUID, NFMapEx <std::string, NFScheduleElement >> mObjectScheduleMap;//guid_scheduleName_element
	std::list<NFScheduleElement> mObjectAddList;
	std::map<NFGUID, std::string> mObjectRemoveList;

	NFMapEx <std::string, NFScheduleElement > mModuleScheduleMap;//guid_scheduleName_element
	std::list<NFScheduleElement> mModuleAddList;
	std::list<std::string> mModuleRemoveList;

	NFILogModule* m_pLogModule;
	
};

#endif