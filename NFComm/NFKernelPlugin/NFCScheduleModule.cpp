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
	mScheduleMap.clear();
}

bool NFCScheduleModule::Init()
{
	return true;
}
bool NFCScheduleModule::Execute()
{
	//execute every schedule
	for (std::map<std::string, NFMapEx <std::string, NFCScheduleElement >>::iterator it = mScheduleMap.begin(); it != mScheduleMap.end(); ++it)
	{
		NFMapEx<std::string, NFCScheduleElement> nameScheduleMap = it->second;
		NF_SHARE_PTR<NFCScheduleElement> pSchedule = nameScheduleMap.First();
		while (pSchedule)
		{
			NFINT64 nNow = NFTime::GetTime();
			if (nNow > pSchedule->mnNextTriggerTime && pSchedule->mnRemainCount > 0)
			{
				pSchedule->mnRemainCount--;
				pSchedule->DoHeartBeatEvent();
				if (pSchedule->mnRemainCount <= 0)
				{
					mRemoveList.insert(std::map<std::string, std::string>::value_type(pSchedule->self.ToString(),pSchedule->mstrScheduleName));
				}
				else
				{
					pSchedule->mnNextTriggerTime += NFINT64(pSchedule->mfIntervalTime * 1000);
				}
			}
			pSchedule = nameScheduleMap.Next();
		}
	}
	//remove schedule
	for (std::map<std::string, std::string>::iterator it = mRemoveList.begin(); it != mRemoveList.end(); ++it)
	{
		std::string guid = it->first;
		std::string scheduleName = it->second;
		auto findIter = mScheduleMap.find(guid);
		if (findIter != mScheduleMap.end())
		{
			findIter->second.RemoveElement(scheduleName);
			if (findIter->second.Count() == 0)
			{
				mScheduleMap.erase(guid);
			}
		}
	}
	mRemoveList.clear();

	//add schedule
	for (std::list<NFCScheduleElement>::iterator iter = mAddList.begin(); iter != mAddList.end(); ++iter)
	{
		std::string guid = iter->self.ToString();
		std::string scheduleName = iter->mstrScheduleName;
		auto findIter = mScheduleMap.find(guid);
		if (findIter == mScheduleMap.end())
		{
			NFMapEx <std::string, NFCScheduleElement > nameScheduleMap;
			mScheduleMap.insert(std::map<std::string, NFMapEx <std::string, NFCScheduleElement >>::value_type(guid, nameScheduleMap));
			findIter = mScheduleMap.find(guid);
		}
		if (NULL == findIter->second.GetElement(scheduleName))
		{
			NF_SHARE_PTR<NFCScheduleElement> pSchedule(NF_NEW NFCScheduleElement());
			*pSchedule = *iter;
			findIter->second.AddElement(pSchedule->mstrScheduleName, pSchedule);
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
	xSchedule.mnRemainCount = nCount;
	xSchedule.self = self;
	xSchedule.Add(cb);
	mAddList.push_back(xSchedule);

	return true;
}

bool NFCScheduleModule::RemoveSchedule(const NFGUID self, const std::string& strScheduleName)
{
	mRemoveList.insert(std::map<std::string, std::string>::value_type(self.ToString(), strScheduleName));
	return true;
}

bool NFCScheduleModule::ExistSchedule(const NFGUID self, const std::string& strScheduleName)
{
	auto findIter = mScheduleMap.find(self.ToString());
	if (findIter == mScheduleMap.end())
	{
		return false;
	}
	if (NULL == findIter->second.GetElement(strScheduleName))
	{
		return false;
	}
	return true;
}
