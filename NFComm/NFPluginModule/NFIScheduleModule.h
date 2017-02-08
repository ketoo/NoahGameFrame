#ifndef NFI_SCHEDULE_MODULE_H
#define NFI_SCHEDULE_MODULE_H

#include <string>
#include "NFComm/NFCore/NFMap.hpp"
#include "NFComm/NFCore/NFList.hpp"
#include "NFComm/NFCore/NFDataList.hpp"
#include "NFComm/NFCore/NFDateTime.hpp"
#include "NFComm/NFPluginModule/NFIModule.h"

typedef std::function<int(const NFGUID&, const std::string&, const float, const int)> OBJECT_SCHEDULE_FUNCTOR;
typedef std::function<int(const std::string&, const float, const int)> MODULE_SCHEDULE_FUNCTOR;

typedef NF_SHARE_PTR<OBJECT_SCHEDULE_FUNCTOR> OBJECT_SCHEDULE_FUNCTOR_PTR;//HEART
typedef NF_SHARE_PTR<MODULE_SCHEDULE_FUNCTOR> MODULE_SCHEDULE_FUNCTOR_PTR;//HEART

class NFIScheduleModule
	:public  NFIModule
{
public:
	virtual ~NFIScheduleModule() {}

	///for module
	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
	virtual bool AddSchedule(const std::string& strScheduleName, const MODULE_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const NFDateTime& date) = 0;
	virtual bool RemoveSchedule(const std::string& strScheduleName) = 0;
	virtual bool ExistSchedule(const std::string& strScheduleName) = 0;

	template<typename BaseType>
	bool AddSchedule(const std::string& strScheduleName, BaseType* pBase, int (BaseType::*handler)(const std::string&, const float, const int), const float fIntervalTime, const int nCount)
	{
		MODULE_SCHEDULE_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		MODULE_SCHEDULE_FUNCTOR_PTR functorPtr(NF_NEW MODULE_SCHEDULE_FUNCTOR(functor));
		return AddSchedule(strScheduleName, functorPtr, fIntervalTime, nCount);
	}
	
	///for object
	virtual bool AddSchedule(const NFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
	virtual bool AddSchedule(const NFGUID self, const std::string& strScheduleName, const OBJECT_SCHEDULE_FUNCTOR_PTR& cb, const int nCount, const NFDateTime& date) = 0;
	virtual bool RemoveSchedule(const NFGUID self) = 0;
	virtual bool RemoveSchedule(const NFGUID self, const std::string& strScheduleName) = 0;
	virtual bool ExistSchedule(const NFGUID self, const std::string& strScheduleName) = 0;

	template<typename BaseType>
	bool AddSchedule(const NFGUID self, const std::string& strScheduleName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const float, const int), const float fIntervalTime, const int nCount)
	{
		OBJECT_SCHEDULE_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		OBJECT_SCHEDULE_FUNCTOR_PTR functorPtr(NF_NEW OBJECT_SCHEDULE_FUNCTOR(functor));
		return AddSchedule(self, strScheduleName, functorPtr, fIntervalTime, nCount);
	}
};

#endif
