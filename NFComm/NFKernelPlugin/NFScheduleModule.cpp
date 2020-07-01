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

#include "NFScheduleModule.h"

void NFScheduleElement::DoHeartBeatEvent(NFINT64 nowTime)
{
	mnRemainCount--;
	mnTriggerTime = nowTime + (NFINT64)(mfIntervalTime * 1000);

	OBJECT_SCHEDULE_FUNCTOR_PTR cb;
	bool bRet = this->mxObjectFunctor.First(cb);
	while (bRet)
	{
		cb.get()->operator()(self, mstrScheduleName, mfIntervalTime, mnRemainCount);
		bRet = this->mxObjectFunctor.Next(cb);
	}
}

NFScheduleModule::NFScheduleModule(NFIPluginManager* p)
{
	pPluginManager = p;
    m_bIsExecute = true;
}

NFScheduleModule::~NFScheduleModule()
{
	mObjectScheduleMap.ClearAll();
}

bool NFScheduleModule::Init()
{
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	
	m_pKernelModule->RegisterCommonClassEvent(this, &NFScheduleModule::OnClassCommonEvent);

/*
	std::set<TickElement> test;
	for (int i = 0 ; i < 100;  ++i)
	{
		TickElement element;
		element.triggerTime = m_pKernelModule->Random(1, 50);
		test.insert(element);
	}

	for (auto it = test.begin(); it != test.end(); ++it)
	{
		std::cout << it->triggerTime << std::endl;
	}
 */
	return true;
}

bool NFScheduleModule::Execute()
{
	NFPerformance performanceObject;
	NFINT64 nowTime = NFGetTimeMS();

	std::list<TickElement> elements;

	for (auto it = mScheduleMap.begin(); it != mScheduleMap.end();)
	{
		if (nowTime >= it->triggerTime)
		{
			//std::cout << nowTime << it->scheduleName << ">>>>>" << it->triggerTime << std::endl;

			auto objectMap = mObjectScheduleMap.GetElement(it->self);
			if (objectMap)
			{
				auto scheduleElement = objectMap->GetElement(it->scheduleName);
				if (scheduleElement)
				{
					scheduleElement->DoHeartBeatEvent(nowTime);

					if (scheduleElement->mnRemainCount != 0)
					{
						TickElement element;
						element.scheduleName = scheduleElement->mstrScheduleName;
						element.triggerTime = scheduleElement->mnTriggerTime;
						element.self = scheduleElement->self;

						elements.push_back(element);
					}
				}
			}

			it = mScheduleMap.erase(it);
		}
		else
		{
			break;
		}
	}

	for (auto& item : elements)
	{
		mScheduleMap.insert(item);
	}

	if (performanceObject.CheckTimePoint(1))
	{
		std::ostringstream os;
		os << "---------------module schedule performance problem ";
		os << performanceObject.TimeScope();
		os << "---------- ";
		m_pLogModule->LogWarning(NFGUID(), os, __FUNCTION__, __LINE__);
	}

	return true;
}

bool NFScheduleModule::AddSchedule(const NFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount)
{
	auto objectMap = mObjectScheduleMap.GetElement(self);
	if (!objectMap)
	{
		objectMap = NF_SHARE_PTR< NFMapEx <std::string, NFScheduleElement >>(NF_NEW NFMapEx <std::string, NFScheduleElement >());
		mObjectScheduleMap.AddElement(self, objectMap);
	}

	auto scheduleObject = objectMap->GetElement(strScheduleName);
	if (!scheduleObject)
	{
		scheduleObject = NF_SHARE_PTR<NFScheduleElement>(NF_NEW NFScheduleElement());
		scheduleObject->mstrScheduleName = strScheduleName;
		scheduleObject->mfIntervalTime = fTime;
		scheduleObject->mnTriggerTime = NFGetTimeMS() + (NFINT64)(fTime * 1000);
		scheduleObject->mnRemainCount = nCount;
		scheduleObject->self = self;


		scheduleObject->mxObjectFunctor.Add(cb);

		objectMap->AddElement(strScheduleName, scheduleObject);

		TickElement tickElement;
		tickElement.scheduleName = scheduleObject->mstrScheduleName;
		tickElement.triggerTime = scheduleObject->mnTriggerTime;
		tickElement.self = scheduleObject->self;

		mScheduleMap.insert(tickElement);
	}

	return true;
}

bool NFScheduleModule::RemoveSchedule(const NFGUID self)
{
	return mObjectScheduleMap.RemoveElement(self);
}

bool NFScheduleModule::RemoveSchedule(const NFGUID self, const std::string& strScheduleName)
{
	auto objectMap = mObjectScheduleMap.GetElement(self);
	if (objectMap)
	{
		return objectMap->RemoveElement(strScheduleName);
	}

	return false;
}

bool NFScheduleModule::ExistSchedule(const NFGUID self, const std::string& strScheduleName)
{
	auto objectScheduleMap = mObjectScheduleMap.GetElement(self);
	if (NULL == objectScheduleMap)
	{
		return false;
	}

	return objectScheduleMap->ExistElement(strScheduleName);
}

NF_SHARE_PTR<NFScheduleElement> NFScheduleModule::GetSchedule(const NFGUID self, const std::string & strScheduleName)
{
	NF_SHARE_PTR< NFMapEx <std::string, NFScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(self);
	if (NULL == xObjectScheduleMap)
	{
		return nullptr;
	}

	return xObjectScheduleMap->GetElement(strScheduleName);
}


int NFScheduleModule::OnClassCommonEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		this->RemoveSchedule(self);
	}

	return 0;
}