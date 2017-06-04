// -------------------------------------------------------------------------
//    @FileName			:    NFCScheduleModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-05
//    @Module           :    NFCScheduleModule
//
// -------------------------------------------------------------------------

#include "NFCScheduleModule.h"

void NFCScheduleElement::DoHeartBeatEvent()
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

NFCScheduleModule::NFCScheduleModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCScheduleModule::~NFCScheduleModule()
{
	mObjectScheduleMap.ClearAll();
}

bool NFCScheduleModule::Init()
{
	return true;
}
bool NFCScheduleModule::Execute()
{
	//execute every schedule
	NF_SHARE_PTR<NFMapEx <std::string, NFCScheduleElement >> xObjectSchedule = mObjectScheduleMap.First();
	while (xObjectSchedule)
	{
		std::string str;
		NF_SHARE_PTR<NFCScheduleElement> pSchedule = xObjectSchedule->First();
		while (pSchedule)
		{
			NFINT64 nNow = NFGetTime();
			if (nNow > pSchedule->mnNextTriggerTime)
			{
				std::map<NFGUID, std::string>::iterator itRet = mObjectRemoveList.find(pSchedule->self);
				if (itRet != mObjectRemoveList.end())
				{
					if (itRet->second != pSchedule->mstrScheduleName)
					{
						if (pSchedule->mnRemainCount > 0 || pSchedule->mbForever == true)
						{
							pSchedule->mnRemainCount--;
							pSchedule->DoHeartBeatEvent();

							if (pSchedule->mnRemainCount <= 0 && pSchedule->mbForever == false)
							{
								mObjectRemoveList.insert(std::map<NFGUID, std::string>::value_type(pSchedule->self, pSchedule->mstrScheduleName));
							}
							else
							{
								NFINT64 nNextCostTime = NFINT64(pSchedule->mfIntervalTime * 1000) * (pSchedule->mnAllCount - pSchedule->mnRemainCount);
								pSchedule->mnNextTriggerTime = pSchedule->mnStartTime + nNextCostTime;
							}
						}
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
	for (std::list<NFCScheduleElement>::iterator iter = mObjectAddList.begin(); iter != mObjectAddList.end(); ++iter)
	{
		NF_SHARE_PTR< NFMapEx <std::string, NFCScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(iter->self);
		if (NULL == xObjectScheduleMap)
		{
			xObjectScheduleMap = NF_SHARE_PTR< NFMapEx <std::string, NFCScheduleElement >>(NF_NEW NFMapEx <std::string, NFCScheduleElement >());
			mObjectScheduleMap.AddElement(iter->self, xObjectScheduleMap);
		}

		NF_SHARE_PTR<NFCScheduleElement> xScheduleElement = xObjectScheduleMap->GetElement(iter->mstrScheduleName);
		if (NULL == xScheduleElement)
		{
			xScheduleElement = NF_SHARE_PTR<NFCScheduleElement>(NF_NEW NFCScheduleElement());
			*xScheduleElement = *iter;

			xObjectScheduleMap->AddElement(iter->mstrScheduleName, xScheduleElement);
		}
	}

	mObjectAddList.clear();

	////////////////////////////////////////////
	//execute every schedule
	NF_SHARE_PTR< NFCScheduleElement > xModuleSchedule = mModuleScheduleMap.First();
	while (xModuleSchedule)
	{
		NFINT64 nNow = NFGetTime();
		if (nNow > xModuleSchedule->mnNextTriggerTime)
		{
			if (xModuleSchedule->mnRemainCount > 0 || xModuleSchedule->mbForever == true)
			{
				xModuleSchedule->mnRemainCount--;
				xModuleSchedule->DoHeartBeatEvent();

				if (xModuleSchedule->mnRemainCount <= 0 && xModuleSchedule->mbForever == false)
				{
					mModuleRemoveList.push_back(xModuleSchedule->mstrScheduleName);
				}
				else
				{
					NFINT64 nNextCostTime = NFINT64(xModuleSchedule->mfIntervalTime * 1000) * (xModuleSchedule->mnAllCount - xModuleSchedule->mnRemainCount);
					xModuleSchedule->mnNextTriggerTime = xModuleSchedule->mnStartTime + nNextCostTime;
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
	for (std::list<NFCScheduleElement>::iterator iter = mModuleAddList.begin(); iter != mModuleAddList.end(); ++iter)
	{
		NF_SHARE_PTR< NFCScheduleElement > xModuleScheduleMap = mModuleScheduleMap.GetElement(iter->mstrScheduleName);
		if (NULL == xModuleScheduleMap)
		{
			xModuleScheduleMap = NF_SHARE_PTR< NFCScheduleElement >(NF_NEW NFCScheduleElement());
			mModuleScheduleMap.AddElement(iter->mstrScheduleName, xModuleScheduleMap);
		}

		*xModuleScheduleMap = *iter;
	}

	mModuleAddList.clear();
	return true;
}

bool NFCScheduleModule::AddSchedule(const std::string & strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR & cb, const float fTime, const int nCount)
{
	NFCScheduleElement xSchedule;
	xSchedule.mstrScheduleName = strScheduleName;
	xSchedule.mfIntervalTime = fTime;
	xSchedule.mnNextTriggerTime = NFGetTime() + (NFINT64)(fTime * 1000);
	xSchedule.mnStartTime = NFGetTime();
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

bool NFCScheduleModule::AddSchedule(const std::string & strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR & cb, const int nCount, const NFDateTime & date)
{
	return false;
}

bool NFCScheduleModule::RemoveSchedule(const std::string & strScheduleName)
{
	mModuleRemoveList.push_back(strScheduleName);

	return true;
}

bool NFCScheduleModule::ExistSchedule(const std::string & strScheduleName)
{
	return mModuleScheduleMap.ExistElement(strScheduleName);
}

bool NFCScheduleModule::AddSchedule(const NFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount)
{
	NFCScheduleElement xSchedule;
	xSchedule.mstrScheduleName = strScheduleName;
	xSchedule.mfIntervalTime = fTime;
	xSchedule.mnNextTriggerTime = NFGetTime() + (NFINT64)(fTime * 1000);
	xSchedule.mnStartTime = NFGetTime();
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

bool NFCScheduleModule::AddSchedule(const NFGUID self, const std::string & strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR & cb, const int nCount, const NFDateTime & date)
{
	return false;
}

bool NFCScheduleModule::RemoveSchedule(const NFGUID self)
{
	return mObjectScheduleMap.RemoveElement(self);
}

bool NFCScheduleModule::RemoveSchedule(const NFGUID self, const std::string& strScheduleName)
{
	mObjectRemoveList.insert(std::map<NFGUID, std::string>::value_type(self, strScheduleName));
	return true;
}

bool NFCScheduleModule::ExistSchedule(const NFGUID self, const std::string& strScheduleName)
{
	NF_SHARE_PTR< NFMapEx <std::string, NFCScheduleElement >> xObjectScheduleMap = mObjectScheduleMap.GetElement(self);
	if (NULL == xObjectScheduleMap)
	{
		return false;
	}

	return xObjectScheduleMap->ExistElement(strScheduleName);
}
