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
	if (mnRemainCount > 0 )
	{
		mnRemainCount--;
	}

	mnTriggerTime = nowTime + (NFINT64)(mfIntervalTime * 1000);

#if NF_PLATFORM != NF_PLATFORM_WIN
	NF_CRASH_TRY
#endif
	OBJECT_SCHEDULE_FUNCTOR_PTR cb;
	bool bRet = this->mxObjectFunctor.First(cb);
	while (bRet)
	{

		cb.get()->operator()(self, mstrScheduleName, mfIntervalTime, mnRemainCount);

		bRet = this->mxObjectFunctor.Next(cb);
	}
#if NF_PLATFORM != NF_PLATFORM_WIN
	NF_CRASH_END
#endif
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
	m_pSceneModule = pPluginManager->FindModule<NFISceneModule>();

	m_pKernelModule->RegisterCommonClassEvent(this, &NFScheduleModule::OnClassCommonEvent);
	m_pSceneModule->AddSceneGroupDestroyedCallBack(this, &NFScheduleModule::OnGroupCommonEvent);

	return true;
}

bool NFScheduleModule::Execute()
{
	NFPerformance performanceObject;
	NFINT64 nowTime = NFGetTimeMS();

	static std::vector<TickElement> elements;
	elements.clear();

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
					if(scheduleElement->mnRemainCount == 1){
						objectMap->RemoveElement(it->scheduleName);
					}
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

bool NFScheduleModule::AddSchedule(const NFGUID self, const std::string& scheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float time, const int count)
{
	auto objectMap = mObjectScheduleMap.GetElement(self);
	if (!objectMap)
	{
		objectMap = NF_SHARE_PTR< NFMapEx <std::string, NFScheduleElement >>(NF_NEW NFMapEx <std::string, NFScheduleElement >());
		mObjectScheduleMap.AddElement(self, objectMap);
	}

	auto scheduleObject = objectMap->GetElement(scheduleName);
	if (!scheduleObject)
	{
		scheduleObject = NF_SHARE_PTR<NFScheduleElement>(NF_NEW NFScheduleElement());
		scheduleObject->mstrScheduleName = scheduleName;
		scheduleObject->mfIntervalTime = time;
		scheduleObject->mnTriggerTime = NFGetTimeMS() + (NFINT64)(time * 1000);
		scheduleObject->mnRemainCount = count;
		scheduleObject->self = self;


		scheduleObject->mxObjectFunctor.Add(cb);

		objectMap->AddElement(scheduleName, scheduleObject);

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

bool NFScheduleModule::RemoveSchedule(const NFGUID self, const std::string& scheduleName)
{
	auto objectMap = mObjectScheduleMap.GetElement(self);
	if (objectMap)
	{
		return objectMap->RemoveElement(scheduleName);
	}

	return false;
}

bool NFScheduleModule::ExistSchedule(const NFGUID self, const std::string& scheduleName)
{
	auto objectScheduleMap = mObjectScheduleMap.GetElement(self);
	if (NULL == objectScheduleMap)
	{
		return false;
	}

	return objectScheduleMap->ExistElement(scheduleName);
}

NF_SHARE_PTR<NFScheduleElement> NFScheduleModule::GetSchedule(const NFGUID self, const std::string & scheduleName)
{
	NF_SHARE_PTR< NFMapEx <std::string, NFScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(self);
	if (NULL == xObjectScheduleMap)
	{
		return nullptr;
	}

	return xObjectScheduleMap->GetElement(scheduleName);
}


int NFScheduleModule::OnClassCommonEvent(const NFGUID & self, const std::string & className, const CLASS_OBJECT_EVENT classEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == classEvent)
	{
		this->RemoveSchedule(self);
	}

	return 0;
}

int NFScheduleModule::OnGroupCommonEvent(const NFGUID &self, const int scene, const int group, const int type, const NFDataList &arg)
{
	this->RemoveSchedule(NFGUID(scene, group));
	return 0;
}
