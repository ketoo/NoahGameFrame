#ifndef NFI_SCHEDULE_MODULE_H
#define NFI_SCHEDULE_MODULE_H

#include <string>
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFDefine.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFPluginModule/NFIModule.h"

class  NFCScheduleElement
	: public NFList<HEART_BEAT_FUNCTOR_PTR> 
{
public:
	NFCScheduleElement()
	{
		mstrScheduleName = "";
		mfIntervalTime = 0.0f;
		mnNextTriggerTime = 0;
		mnRemainCount = 0;
	};

	virtual ~NFCScheduleElement()
	{
	}

	void DoHeartBeatEvent();

	std::string mstrScheduleName;
	float mfIntervalTime;
	NFINT64 mnNextTriggerTime;
	int mnRemainCount;
	
	NFGUID self;
};

class NFIScheduleModule
	:public  NFIModule
{
public:
	virtual ~NFIScheduleModule() {}

	virtual bool AddSchedule(const NFGUID self, const std::string& strScheduleName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
	virtual bool RemoveSchedule(const NFGUID self, const std::string& strScheduleName) = 0;
	virtual bool FindSchedule(const NFGUID self, const std::string& strScheduleName) = 0;

	template<typename BaseType>
	bool AddSchedule(const NFGUID self, const std::string& strScheduleName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const float, const int), const float fIntervalTime, const int nCount)
	{
		HEART_BEAT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		HEART_BEAT_FUNCTOR_PTR functorPtr(NF_NEW HEART_BEAT_FUNCTOR(functor));
		return AddSchedule(self, strScheduleName, functorPtr, fIntervalTime, nCount);
	}
};

#endif
