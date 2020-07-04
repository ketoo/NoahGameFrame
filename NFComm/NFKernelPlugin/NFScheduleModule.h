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

#ifndef NF_SCHEDULE_MODULE_H
#define NF_SCHEDULE_MODULE_H

#include <iostream>
#include <set>
#include <algorithm>
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFPerformance.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"
#include "NFComm/NFPluginModule/NFILogModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

class TickElement
{
public:

	bool operator< (const TickElement& b) const
	{
		return this->triggerTime < b.triggerTime;
	}

	std::string scheduleName;
	NFINT64 triggerTime;
	NFGUID self;
};

class NFScheduleModule : public NFIScheduleModule
{
public:
    NFScheduleModule(NFIPluginManager* p);

	virtual ~NFScheduleModule();

	virtual bool Init();
	virtual bool Execute();

	virtual bool AddSchedule(const NFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount);
	virtual bool RemoveSchedule(const NFGUID self);
	virtual bool RemoveSchedule(const NFGUID self, const std::string& strScheduleName);
	virtual bool ExistSchedule(const NFGUID self, const std::string& strScheduleName);
	virtual NF_SHARE_PTR<NFScheduleElement> GetSchedule(const NFGUID self, const std::string& strScheduleName);

protected:
	int OnClassCommonEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var);

protected:
	NFMapEx<NFGUID, NFMapEx<std::string, NFScheduleElement >> mObjectScheduleMap;

	std::multiset<TickElement> mScheduleMap;

	NFILogModule* m_pLogModule;
	NFIKernelModule* m_pKernelModule;
	
};

#endif