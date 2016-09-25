#ifndef NFI_SCHEDULE_MODULE_H
#define NFI_SCHEDULE_MODULE_H

#include <string>
#include "NFComm/NFCore/NFMap.h"
#include "NFComm/NFCore/NFList.h"
#include "NFComm/NFCore/NFDefine.h"
#include "NFComm/NFCore/NFCDataList.h"
#include "NFComm/NFPluginModule/NFIModule.h"

typedef std::function<int(const NFGUID&, const std::string&, const float, const int)> HEART_BEAT_FUNCTOR;
typedef std::function<void()> MODULE_HEART_BEAT_FUNCTOR;

typedef NF_SHARE_PTR<HEART_BEAT_FUNCTOR> HEART_BEAT_FUNCTOR_PTR;//HEART
typedef NF_SHARE_PTR<MODULE_HEART_BEAT_FUNCTOR> MODULE_HEART_BEAT_FUNCTOR_PTR;//HEART

class NFIScheduleModule
	:public  NFIModule
{
public:
	virtual ~NFIScheduleModule() {}
	/*
	///for module
	virtual bool AddSchedule(const std::string& strScheduleName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
	virtual bool RemoveSchedule(const std::string& strScheduleName) = 0;
	virtual bool ExistSchedule(const std::string& strScheduleName) = 0;

	template<typename BaseType>
	bool AddSchedule(const std::string& strScheduleName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const float, const int), const float fIntervalTime, const int nCount)
	{
		HEART_BEAT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		HEART_BEAT_FUNCTOR_PTR functorPtr(NF_NEW HEART_BEAT_FUNCTOR(functor));
		return AddSchedule(self, strScheduleName, functorPtr, fIntervalTime, nCount);
	}
	*/
	///for object
	virtual bool AddSchedule(const NFGUID self, const std::string& strScheduleName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
	virtual bool RemoveSchedule(const NFGUID self, const std::string& strScheduleName) = 0;
	virtual bool ExistSchedule(const NFGUID self, const std::string& strScheduleName) = 0;

	template<typename BaseType>
	bool AddSchedule(const NFGUID self, const std::string& strScheduleName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const float, const int), const float fIntervalTime, const int nCount)
	{
		HEART_BEAT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		HEART_BEAT_FUNCTOR_PTR functorPtr(NF_NEW HEART_BEAT_FUNCTOR(functor));
		return AddSchedule(self, strScheduleName, functorPtr, fIntervalTime, nCount);
	}
};

#endif
