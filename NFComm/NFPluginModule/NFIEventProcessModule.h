// -------------------------------------------------------------------------
//    @FileName         :    NFIEventProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIEventProcessModule
//
// -------------------------------------------------------------------------

#ifndef _NFI_EVENTPROCESS_MODULE_H
#define _NFI_EVENTPROCESS_MODULE_H

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


class NFIEventProcessModule
    : public NFILogicModule
{
public:

    virtual ~NFIEventProcessModule(){}

    template<typename BaseType>
    bool AddEventCallBack(const NFGUID& objectID, const int nEventID, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const NFIDataList&))
    {
        EVENT_PROCESS_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        EVENT_PROCESS_FUNCTOR_PTR functorPtr(new EVENT_PROCESS_FUNCTOR(functor));
        return AddEventCallBack(objectID, nEventID, functorPtr);
    }

    virtual bool RemoveEventCallBack(const NFGUID& objectID, const int nEventID) = 0;

    virtual bool DoEvent(const NFGUID& objectID, const int nEventID, const NFIDataList& valueList) = 0;

    virtual bool AddEventCallBack(const NFGUID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb) = 0;
	//////////////////////////////////////////////////////////////////////////

protected:
	virtual bool HasEventCallBack(const NFGUID& objectID, const int nEventID) = 0;

};

#endif
