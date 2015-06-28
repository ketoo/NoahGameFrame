// -------------------------------------------------------------------------
//    @FileName         :    NFIEventProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIEventProcessModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_EVENTPROCESS_MODULE_H_
#define _NFI_EVENTPROCESS_MODULE_H_

#include <iostream>
#include "NFILogicModule.h"
#include "NFComm/NFCore/NFIDataList.h"
#include "NFComm/NFEventDefine/NFEventDefine.h"
#include "NFComm/NFPluginModule/NFIActorManager.h"

class NFIComponent;

struct NFEventList
	: public NFList<EVENT_PROCESS_FUNCTOR_PTR>
{

};

class NFCObjectEventInfo
	: public NFMapEx<int, NFEventList>
{
};

struct NFCClassEventList
	: public NFList<CLASS_EVENT_FUNCTOR_PTR>
{

};

class NFIEventProcessModule
    : public NFILogicModule
{
public:

    virtual ~NFIEventProcessModule(){}

    template<typename BaseType>
    bool AddEventCallBack(const NFIDENTID& objectID, const int nEventID, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const int, const NFIDataList&))
    {
        EVENT_PROCESS_FUNCTOR functor = boost::bind(handler, pBase, _1, _2, _3);
        EVENT_PROCESS_FUNCTOR_PTR functorPtr(new EVENT_PROCESS_FUNCTOR(functor));
        return AddEventCallBack(objectID, nEventID, functorPtr);
    }

    template<typename BaseType>
    bool AddClassCallBack(const std::string& strClassName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const std::string&, const CLASS_OBJECT_EVENT, const NFIDataList&))
    {
        CLASS_EVENT_FUNCTOR functor = boost::bind(handler, pBase, _1, _2, _3, _4);
        CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
        return AddClassCallBack(strClassName, functorPtr);
    }

	template<typename BaseType>
	bool AddAsyncEventCallBack(const NFIDENTID& objectID, const int nEventID, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const int, std::string&), int (BaseType::*handler_end)(const NFIDENTID&, const int, const std::string&))
	{
		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR functor_begin = boost::bind(handler, pBase, _1, _2, _3);
		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR functorPtr_begin(new EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR(functor_begin));

		EVENT_ASYNC_PROCESS_END_FUNCTOR functor_end = boost::bind(handler_end, pBase, _1, _2, _3);
		EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end(new EVENT_ASYNC_PROCESS_END_FUNCTOR(functor_end));

		return AddAsyncEventCallBack(objectID, nEventID, functorPtr_begin, functorPtr_end);
	}

	template<typename BaseTypeComponent, typename BaseType>
	bool AddActorEventCallBack(const NFIDENTID& objectID, const int nEventID, BaseTypeComponent* pBaseComponent, int (BaseTypeComponent::*handler)(const NFIDENTID&, const int, std::string&), BaseType* pBase, int (BaseType::*handler_end)(const NFIDENTID&, const int, const std::string&))
	{
		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR functor_begin = boost::bind(handler, pBaseComponent, _1, _2, _3);
		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR functorPtr_begin(new EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR(functor_begin));

		EVENT_ASYNC_PROCESS_END_FUNCTOR functor_end = boost::bind(handler_end, pBase, _1, _2, _3);
		EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end(new EVENT_ASYNC_PROCESS_END_FUNCTOR(functor_end));

		return AddActorEventCallBack(objectID, nEventID, NF_SHARE_PTR<NFIComponent>((NFIComponent*)pBaseComponent), functorPtr_begin, functorPtr_end);
	}

	template<typename BaseType>
	bool AddAsyncClassCallBack(const std::string& strClassName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const std::string&, const CLASS_OBJECT_EVENT, const std::string&), int (BaseType::*handler_end)(const NFIDENTID&, const std::string&, const CLASS_OBJECT_EVENT, const std::string&))
	{
		CLASS_ASYNC_EVENT_FUNCTOR functor_begin = boost::bind(handler, pBase, _1, _2, _3, _4);
		CLASS_ASYNC_EVENT_FUNCTOR_PTR functorPtr_begin(new CLASS_ASYNC_EVENT_FUNCTOR(functor_begin));

		CLASS_ASYNC_EVENT_FUNCTOR functor_end = boost::bind(handler_end, pBase, _1, _2, _3, _4);
		CLASS_ASYNC_EVENT_FUNCTOR_PTR functorPtr_end(new CLASS_ASYNC_EVENT_FUNCTOR(functor_end));

		return AddAsyncClassCallBack(strClassName, functorPtr_begin, functorPtr_end);
	}

    virtual bool HasEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

    virtual bool RemoveEvent(const NFIDENTID& objectID) = 0;
    virtual bool RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

    virtual bool DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIDataList& valueList, const bool bSync = true) = 0;
    virtual bool DoEvent(const NFIDENTID& objectID,  const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList, const bool bSync = true) = 0;

    virtual bool AddEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb) = 0;
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool AddAsyncEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR& cb_begin, const EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR& cb_end) = 0;
	virtual bool AddActorEventCallBack(const NFIDENTID& objectID, const int nEventID, NF_SHARE_PTR<NFIComponent> pComponent, const EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR& cb_begin, const EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR& cb_end) = 0;
	virtual bool AddAsyncClassCallBack(const std::string& strClassName, const CLASS_ASYNC_EVENT_FUNCTOR_PTR& cb_begin, const CLASS_ASYNC_EVENT_FUNCTOR_PTR& cb_end) = 0;
};

#endif
