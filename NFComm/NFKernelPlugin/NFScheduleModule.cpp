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

#include "NFScheduleModule.h"

void NFScheduleElement::DoHeartBeatEvent()
{
	if (self.IsNull())
	{
		MODULE_SCHEDULE_FUNCTOR_PTR cb;
		bool bRet = this->mxModuleFunctor.First(cb);
		while (bRet)
		{
			cb.get()->operator()(mstrScheduleName, mfIntervalTime, mnRemainCount);
			bRet = this->mxModuleFunctor.Next(cb);
		}
	}
	else
	{
		OBJECT_SCHEDULE_FUNCTOR_PTR cb;
		bool bRet = this->mxObjectFunctor.First(cb);
		while (bRet)
		{
			cb.get()->operator()(self, mstrScheduleName, mfIntervalTime, mnRemainCount);
			bRet = this->mxObjectFunctor.Next(cb);
		}
	}
}

NFScheduleModule::NFScheduleModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFScheduleModule::~NFScheduleModule()
{
	mObjectScheduleMap.ClearAll();
}

bool NFScheduleModule::Init()
{
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

	return true;
}
bool NFScheduleModule::Execute()
{
	NFPerformance performanceObject;

	//execute every schedule
	NF_SHARE_PTR<NFMapEx <std::string, NFScheduleElement >> xObjectSchedule = mObjectScheduleMap.First();
	while (xObjectSchedule)
	{
		std::string str;
		NF_SHARE_PTR<NFScheduleElement> pSchedule = xObjectSchedule->First();
		while (pSchedule)
		{
			NFINT64 nNow = NFGetTimeMS();
			if (nNow > pSchedule->mnNextTriggerTime)
			{
				if (pSchedule->mnRemainCount > 0 || pSchedule->mbForever == true)
				{
					if (!pSchedule->mbForever)
					{
						pSchedule->mnRemainCount--;
					}

					pSchedule->DoHeartBeatEvent();

					if (pSchedule->mnRemainCount <= 0 && pSchedule->mbForever == false)
					{
						mObjectRemoveList.insert(std::map<NFGUID, std::string>::value_type(pSchedule->self, pSchedule->mstrScheduleName));
					}
					else
					{
						NFINT64 nNextCostTime = NFINT64(pSchedule->mfIntervalTime * 1000);
						pSchedule->mnNextTriggerTime = nNow + nNextCostTime;
					}
				}
			}

			pSchedule = xObjectSchedule->Next();
		}

		xObjectSchedule = mObjectScheduleMap.Next();
	}

	//remove schedule
	for (std::map<NFGUID, std::string>::iterator it = mObjectRemoveList.begin(); it != mObjectRemoveList.end(); ++it)
	{
		NFGUID self = it->first;
		std::string scheduleName = it->second;
		auto findIter = mObjectScheduleMap.GetElement(self);
		if (NULL != findIter)
		{
			findIter->RemoveElement(scheduleName);
			if (findIter->Count() == 0)
			{
				mObjectScheduleMap.RemoveElement(self);
			}
		}
	}
	mObjectRemoveList.clear();

	//add schedule
	for (std::list<NFScheduleElement>::iterator iter = mObjectAddList.begin(); iter != mObjectAddList.end(); ++iter)
	{
		NF_SHARE_PTR< NFMapEx <std::string, NFScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(iter->self);
		if (NULL == xObjectScheduleMap)
		{
			xObjectScheduleMap = NF_SHARE_PTR< NFMapEx <std::string, NFScheduleElement >>(NF_NEW NFMapEx <std::string, NFScheduleElement >());
			mObjectScheduleMap.AddElement(iter->self, xObjectScheduleMap);
		}

		NF_SHARE_PTR<NFScheduleElement> xScheduleElement = xObjectScheduleMap->GetElement(iter->mstrScheduleName);
		if (NULL == xScheduleElement)
		{
			xScheduleElement = NF_SHARE_PTR<NFScheduleElement>(NF_NEW NFScheduleElement());
			*xScheduleElement = *iter;

			xObjectScheduleMap->AddElement(iter->mstrScheduleName, xScheduleElement);
		}
	}

	mObjectAddList.clear();

	if (performanceObject.CheckTimePoint(1))
	{
		std::ostringstream os;
		os << "---------------object scehdule  performance problem------------------- ";
		os << performanceObject.TimeScope();
		os << "---------- ";
		m_pLogModule->LogWarning(NFGUID(), os, __FUNCTION__, __LINE__);
	}
	////////////////////////////////////////////
	//execute every schedule

	NFPerformance performanceModule;

	NF_SHARE_PTR< NFScheduleElement > xModuleSchedule = mModuleScheduleMap.First();
	while (xModuleSchedule)
	{
		NFINT64 nNow = NFGetTimeMS();
		if (nNow > xModuleSchedule->mnNextTriggerTime)
		{
			if (xModuleSchedule->mnRemainCount > 0 || xModuleSchedule->mbForever == true)
			{
				if (!xModuleSchedule->mbForever)
				{
					xModuleSchedule->mnRemainCount--;
				}

				xModuleSchedule->DoHeartBeatEvent();

				if (xModuleSchedule->mnRemainCount <= 0 && xModuleSchedule->mbForever == false)
				{
					mModuleRemoveList.push_back(xModuleSchedule->mstrScheduleName);
				}
				else
				{
					NFINT64 nNextCostTime = NFINT64(xModuleSchedule->mfIntervalTime * 1000);
					xModuleSchedule->mnNextTriggerTime = nNow + nNextCostTime;
				}
			}
		}

		xModuleSchedule = mModuleScheduleMap.Next();
	}

	//remove schedule
	for (std::list<std::string>::iterator it = mModuleRemoveList.begin(); it != mModuleRemoveList.end(); ++it)
	{
		const std::string& strSheduleName = *it;;
		auto findIter = mModuleScheduleMap.GetElement(strSheduleName);
		if (NULL != findIter)
		{
			mModuleScheduleMap.RemoveElement(strSheduleName);
		}
	}
	mModuleRemoveList.clear();

	//add schedule
	for (std::list<NFScheduleElement>::iterator iter = mModuleAddList.begin(); iter != mModuleAddList.end(); ++iter)
	{
		NF_SHARE_PTR< NFScheduleElement > xModuleScheduleMap = mModuleScheduleMap.GetElement(iter->mstrScheduleName);
		if (NULL == xModuleScheduleMap)
		{
			xModuleScheduleMap = NF_SHARE_PTR< NFScheduleElement >(NF_NEW NFScheduleElement());
			mModuleScheduleMap.AddElement(iter->mstrScheduleName, xModuleScheduleMap);
		}

		*xModuleScheduleMap = *iter;
	}

	mModuleAddList.clear();

	if (performanceModule.CheckTimePoint(1))
	{
		std::ostringstream os;
		os << "---------------module scehdule performance problem------------------- ";
		os << performanceObject.TimeScope();
		os << "---------- ";
		m_pLogModule->LogWarning(NFGUID(), os, __FUNCTION__, __LINE__);
	}

	return true;
}

bool NFScheduleModule::AddSchedule(const std::string & strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR & cb, const float fTime, const int nCount)
{
	NFScheduleElement xSchedule;
	xSchedule.mstrScheduleName = strScheduleName;
	xSchedule.mfIntervalTime = fTime;
	xSchedule.mnNextTriggerTime = NFGetTimeMS() + (NFINT64)(fTime * 1000);
	xSchedule.mnStartTime = NFGetTimeMS();
	xSchedule.mnRemainCount = nCount;
	xSchedule.mnAllCount = nCount;
	xSchedule.self = NFGUID();
	if (nCount < 0)
	{
		xSchedule.mbForever = true;
	}

	xSchedule.mxModuleFunctor.Add(cb);

	mModuleAddList.push_back(xSchedule);

	return true;
}

bool NFScheduleModule::AddSchedule(const std::string & strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR & cb, const int nCount, const NFDateTime & date)
{
	return false;
}

bool NFScheduleModule::RemoveSchedule(const std::string & strScheduleName)
{
	mModuleRemoveList.push_back(strScheduleName);

	return true;
}

bool NFScheduleModule::ExistSchedule(const std::string & strScheduleName)
{
	return mModuleScheduleMap.ExistElement(strScheduleName);
}

bool NFScheduleModule::AddSchedule(const NFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount)
{
	NFScheduleElement xSchedule;
	xSchedule.mstrScheduleName = strScheduleName;
	xSchedule.mfIntervalTime = fTime;
	xSchedule.mnNextTriggerTime = NFGetTimeMS() + (NFINT64)(fTime * 1000);
	xSchedule.mnStartTime = NFGetTimeMS();
	xSchedule.mnRemainCount = nCount;
	xSchedule.mnAllCount = nCount;
	xSchedule.self = self;
	if (nCount < 0)
	{
		xSchedule.mbForever = true;
	}

	xSchedule.mxObjectFunctor.Add(cb);

	mObjectAddList.push_back(xSchedule);

	return true;
}

bool NFScheduleModule::AddSchedule(const NFGUID self, const std::string & strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR & cb, const int nCount, const NFDateTime & date)
{
	return false;
}

bool NFScheduleModule::RemoveSchedule(const NFGUID self)
{
	return mObjectScheduleMap.RemoveElement(self);
}

bool NFScheduleModule::RemoveSchedule(const NFGUID self, const std::string& strScheduleName)
{
	mObjectRemoveList.insert(std::map<NFGUID, std::string>::value_type(self, strScheduleName));
	return true;
}

bool NFScheduleModule::ExistSchedule(const NFGUID self, const std::string& strScheduleName)
{
	NF_SHARE_PTR< NFMapEx <std::string, NFScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(self);
	if (NULL == xObjectScheduleMap)
	{
		return false;
	}

	return xObjectScheduleMap->ExistElement(strScheduleName);
}
