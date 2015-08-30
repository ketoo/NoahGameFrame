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
#include <functional>
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
        EVENT_PROCESS_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        EVENT_PROCESS_FUNCTOR_PTR functorPtr(new EVENT_PROCESS_FUNCTOR(functor));
        return AddEventCallBack(objectID, nEventID, functorPtr);
    }

    template<typename BaseType>
    bool AddClassCallBack(const std::string& strClassName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const std::string&, const CLASS_OBJECT_EVENT, const NFIDataList&))
    {
        CLASS_EVENT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        CLASS_EVENT_FUNCTOR_PTR functorPtr(new CLASS_EVENT_FUNCTOR(functor));
        return AddClassCallBack(strClassName, functorPtr);
    }

	template<typename BaseType>
	bool AddAsyncEventCallBack(const NFIDENTID& objectID, const int nEventID, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const int, std::string&), int (BaseType::*handler_end)(const NFIDENTID&, const int, const int, const std::string&))
	{
		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR functor_begin = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR functorPtr_begin(new EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR(functor_begin));

		EVENT_ASYNC_PROCESS_END_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end(new EVENT_ASYNC_PROCESS_END_FUNCTOR(functor_end));

		return AddAsyncEventCallBack(objectID, nEventID, functorPtr_begin, functorPtr_end);
	}

    template<typename DerivedType , typename BaseType> class TIsDerived
    {
    public:
        static int AnyFunction(BaseType* base)
        {
            return 1;
        }

        static  char AnyFunction(void* t2)
        {
            return 0;
        }

        enum 
        {
            Result = ( sizeof(int) == sizeof(AnyFunction( (DerivedType*)NULL) ) ), 
        };
    };

	template<typename BaseTypeComponent, typename BaseType>
	int AddActorEventCallBack(BaseType* pBase, int (BaseType::*handler_end)(const NFIDENTID&, const int, const int, const std::string&))
	{
		if (!TIsDerived<BaseTypeComponent, NFIComponent>::Result)
        {
            //BaseTypeComponent must inherit from NFIComponent;
            return 0;
        }

		NF_SHARE_PTR<BaseTypeComponent> pComponent = NF_SHARE_PTR<BaseTypeComponent>(NF_NEW BaseTypeComponent(pPluginManager));

		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR functor_begin = std::bind(&BaseTypeComponent::OnASyncEvent, pComponent, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR functorPtr_begin(new EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR(functor_begin));

		EVENT_ASYNC_PROCESS_END_FUNCTOR functor_end = std::bind(handler_end, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR functorPtr_end(new EVENT_ASYNC_PROCESS_END_FUNCTOR(functor_end));        
        
		return AddActorEventCallBack(pComponent, functorPtr_begin, functorPtr_end);
	}

    virtual bool RemoveEvent(const NFIDENTID& objectID) = 0;
    virtual bool RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

    virtual bool DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIDataList& valueList, const bool bSync = true) = 0;
    virtual bool DoEvent(const NFIDENTID& objectID,  const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList, const bool bSync = true) = 0;

    virtual bool AddEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb) = 0;
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;
	
	//////////////////////////////////////////////////////////////////////////
	virtual bool SendActorMsg(const int nActorID, const int nEventID, const NFIDENTID self, const std::string& strData) = 0;

protected:
	virtual bool HasEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

	virtual bool AddAsyncEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR& cb_begin, const EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR& cb_end) = 0;
	virtual int AddActorEventCallBack(NF_SHARE_PTR<NFIComponent> pComponent, const EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR& cb_begin, const EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR& cb_end) = 0;

};

#endif
