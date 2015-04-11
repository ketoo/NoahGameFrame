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

struct NFEventList
	: public NFList<EVENT_PROCESS_FUNCTOR_PTR>
{

};

struct NFAsyncEventList
	: public NFList<EVENT_ASYNC_PROCESS_FUNCTOR_PTR>
{

};

struct NFClassEventList
	: public NFList<CLASS_EVENT_FUNCTOR_PTR>
{

};

struct NFClassAsyncEventList
	: public NFList<CLASS_ASYNC_EVENT_FUNCTOR_PTR>
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
	bool AddAsyncEventCallBack(const NFIDENTID& objectID, const int nEventID, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const int, const std::string&))
	{
		EVENT_ASYNC_PROCESS_FUNCTOR functor = boost::bind(handler, pBase, _1, _2, _3);
		EVENT_ASYNC_PROCESS_FUNCTOR_PTR functorPtr(new EVENT_ASYNC_PROCESS_FUNCTOR(functor));
		return AddAsyncEventCallBack(objectID, nEventID, functorPtr);
	}

	template<typename BaseType>
	bool AddAsyncClassCallBack(const std::string& strClassName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const std::string&, const CLASS_OBJECT_EVENT, const std::string&))
	{
		CLASS_ASYNC_EVENT_FUNCTOR functor = boost::bind(handler, pBase, _1, _2, _3, _4);
		CLASS_ASYNC_EVENT_FUNCTOR_PTR functorPtr(new CLASS_ASYNC_EVENT_FUNCTOR(functor));
		return AddAsyncClassCallBack(strClassName, functorPtr);
	}

    virtual bool HasEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

    virtual bool RemoveEvent(const NFIDENTID& objectID) = 0;
    virtual bool RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

    virtual bool DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIDataList& valueList, const bool bSync = true) = 0;
    virtual bool DoEvent(const NFIDENTID& objectID,  const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList, const bool bSync = true) = 0;

    virtual bool AddEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb) = 0;
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;

	virtual bool AddAsyncEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_ASYNC_PROCESS_FUNCTOR_PTR& cb) = 0;
	virtual bool AddAsyncClassCallBack(const std::string& strClassName, const CLASS_ASYNC_EVENT_FUNCTOR_PTR& cb) = 0;
};

#endif
