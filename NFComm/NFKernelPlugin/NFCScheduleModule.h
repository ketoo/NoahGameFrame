// -------------------------------------------------------------------------
//    @FileName			:    NFCScheduleModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2016-12-05
//    @Module           :    NFCScheduleModule
//
// -------------------------------------------------------------------------

#ifndef NFC_SCHEDULE_MODULE_H
#define NFC_SCHEDULE_MODULE_H

#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFIScheduleModule.h"

class  NFCScheduleElement
{
public:
	NFCScheduleElement()
	{
		mstrScheduleName = "";
		mfIntervalTime = 0.0f;
		mnNextTriggerTime = 0;
		mnStartTime = 0;
		mnRemainCount = 0;
		mnAllCount = 0;
		mbForever = false;
	};

	virtual ~NFCScheduleElement()
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

class NFCScheduleModule : public NFIScheduleModule
{
public:
	NFCScheduleModule(NFIPluginManager* p);

	virtual ~NFCScheduleModule();

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
	NFMapEx<NFGUID, NFMapEx <std::string, NFCScheduleElement >> mObjectScheduleMap;//guid_scheduleName_element
	std::list<NFCScheduleElement> mObjectAddList;
	std::map<NFGUID, std::string> mObjectRemoveList;

	NFMapEx <std::string, NFCScheduleElement > mModuleScheduleMap;//guid_scheduleName_element
	std::list<NFCScheduleElement> mModuleAddList;
	std::list<std::string> mModuleRemoveList;
	
};

#endif