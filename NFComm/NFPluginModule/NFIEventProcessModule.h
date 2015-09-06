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


    virtual bool RemoveEvent(const NFIDENTID& objectID) = 0;
    virtual bool RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

    virtual bool DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIDataList& valueList) = 0;
    virtual bool DoEvent(const NFIDENTID& objectID,  const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList) = 0;

    virtual bool AddEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb) = 0;
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb) = 0;
	
	//////////////////////////////////////////////////////////////////////////

protected:
	virtual bool HasEventCallBack(const NFIDENTID& objectID, const int nEventID) = 0;

};

#endif
