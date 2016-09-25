#ifndef NFC_SCHEDULE_MODULE_H
#define NFC_SCHEDULE_MODULE_H

#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFDefine.h"
#include "NFComm/NFCore/NFIDataList.h"
#include "NFIScheduleModule.h"

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

class NFCScheduleModule : public NFIScheduleModule
{
public:
	NFCScheduleModule(NFIPluginManager* p);

	virtual ~NFCScheduleModule();

	virtual bool Init();
	virtual bool Execute();

	virtual bool AddSchedule(const NFGUID self, const std::string& strScheduleName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount);
	virtual bool RemoveSchedule(const NFGUID self, const std::string& strScheduleName);
	virtual bool ExistSchedule(const NFGUID self, const std::string& strScheduleName);

protected:
	std::map<std::string, NFMapEx <std::string, NFCScheduleElement >> mScheduleMap;//guid_scheduleName_element
	std::list<NFCScheduleElement> mAddList;
	std::map<std::string, std::string> mRemoveList;
	
};

#endif