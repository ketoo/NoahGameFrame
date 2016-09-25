#include "NFCScheduleModule.h"
#include "NFComm/NFCore/NFTime.h"

void NFCScheduleElement::DoHeartBeatEvent()
{
	HEART_BEAT_FUNCTOR_PTR cb;
	bool bRet = First(cb);
	while (bRet)
	{
		cb.get()->operator()(self, mstrScheduleName, mfIntervalTime, mnRemainCount);
		bRet = Next(cb);
	}
}

NFCScheduleModule::NFCScheduleModule(NFIPluginManager* p)
{
	pPluginManager = p;
}

NFCScheduleModule::~NFCScheduleModule()
{
	mScheduleMap.ClearAll();
}

bool NFCScheduleModule::Init()
{
	return true;
}
bool NFCScheduleModule::Execute()
{
	//execute every schedule
	NF_SHARE_PTR<NFMapEx <std::string, NFCScheduleElement >> xObjectSchedule = mScheduleMap.First();
	while (xObjectSchedule)
	{
		std::string str;
		NF_SHARE_PTR<NFCScheduleElement> pSchedule = xObjectSchedule->First();
		while (pSchedule)
		{
			NFINT64 nNow = NFTime::GetTime();
			if (nNow > pSchedule->mnNextTriggerTime && pSchedule->mnRemainCount > 0)
			{
				pSchedule->mnRemainCount--;
				pSchedule->DoHeartBeatEvent();

				if (pSchedule->mnRemainCount <= 0)
				{
					mRemoveList.insert(std::map<NFGUID, std::string>::value_type(pSchedule->self, pSchedule->mstrScheduleName));
				}
				else
				{
					NFINT64 nNextCostTime = NFINT64(pSchedule->mfIntervalTime * 1000) * (pSchedule->mnAllCount - pSchedule->mnRemainCount);
					pSchedule->mnNextTriggerTime = pSchedule->mnStartTime + nNextCostTime;
				}
			}

			pSchedule = xObjectSchedule->Next();
		}

		xObjectSchedule = mScheduleMap.Next();
	}

	//remove schedule
	for (std::map<NFGUID, std::string>::iterator it = mRemoveList.begin(); it != mRemoveList.end(); ++it)
	{
		NFGUID self = it->first;
		std::string scheduleName = it->second;
		auto findIter = mScheduleMap.GetElement(self);
		if (NULL != findIter)
		{
			findIter->RemoveElement(scheduleName);
			if (findIter->Count() == 0)
			{
				mScheduleMap.RemoveElement(self);
			}
		}
	}
	mRemoveList.clear();

	//add schedule
	for (std::list<NFCScheduleElement>::iterator iter = mAddList.begin(); iter != mAddList.end(); ++iter)
	{
		NF_SHARE_PTR< NFMapEx <std::string, NFCScheduleElement >> xObjectScheduleMap = mScheduleMap.GetElement(iter->self);
		if (NULL == xObjectScheduleMap)
		{
			xObjectScheduleMap = NF_SHARE_PTR< NFMapEx <std::string, NFCScheduleElement >>(NF_NEW NFMapEx <std::string, NFCScheduleElement >());
			mScheduleMap.AddElement(iter->self, xObjectScheduleMap);
		}

		NF_SHARE_PTR<NFCScheduleElement> xScheduleElement = xObjectScheduleMap->GetElement(iter->mstrScheduleName);
		if (NULL == xScheduleElement)
		{
			xScheduleElement = NF_SHARE_PTR<NFCScheduleElement>(NF_NEW NFCScheduleElement());
			*xScheduleElement = *iter;

			xObjectScheduleMap->AddElement(iter->mstrScheduleName, xScheduleElement);
		}
	}

	mAddList.clear();

	return true;
}

bool NFCScheduleModule::AddSchedule(const NFGUID self, const std::string& strScheduleName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount)
{
	NFCScheduleElement xSchedule;
	xSchedule.mstrScheduleName = strScheduleName;
	xSchedule.mfIntervalTime = fTime;
	xSchedule.mnNextTriggerTime = NFTime::GetTime() + (NFINT64)(fTime * 1000);
	xSchedule.mnStartTime = NFTime::GetTime();
	xSchedule.mnRemainCount = nCount;
	xSchedule.mnAllCount = nCount;
	xSchedule.self = self;
	xSchedule.Add(cb);

	mAddList.push_back(xSchedule);

	return true;
}

bool NFCScheduleModule::RemoveSchedule(const NFGUID self)
{
	return mScheduleMap.RemoveElement(self);
}

bool NFCScheduleModule::RemoveSchedule(const NFGUID self, const std::string& strScheduleName)
{
	mRemoveList.insert(std::map<NFGUID, std::string>::value_type(self, strScheduleName));
	return true;
}

bool NFCScheduleModule::ExistSchedule(const NFGUID self, const std::string& strScheduleName)
{
	NF_SHARE_PTR< NFMapEx <std::string, NFCScheduleElement >> xObjectScheduleMap = mScheduleMap.GetElement(self);
	if (NULL == xObjectScheduleMap)
	{
		return false;
	}

	return xObjectScheduleMap->ExistElement(strScheduleName);
}
