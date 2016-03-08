// -------------------------------------------------------------------------
//    @FileName         :    NFIEventManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFIEventManager
//
// -------------------------------------------------------------------------

#ifndef NFI_EVENT_MNG_H
#define NFI_EVENT_MNG_H

#include <iostream>
#include <functional>
#include "NFDefine.h"
#include "NFList.h"
#include "NFMapEx.h"
#include "NFComm/NFCore/NFIDataList.h"
#include "NFComm/NFEventDefine/NFEventDefine.h"

class NFIEventManager
{
public:

    virtual ~NFIEventManager() {}
    virtual bool Execute() = 0;

    template<typename BaseType>
    bool AddEventCallBack(const int nEventID, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const int, const NFIDataList&))
    {
        EVENT_PROCESS_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
        EVENT_PROCESS_FUNCTOR_PTR functorPtr(new EVENT_PROCESS_FUNCTOR(functor));
        return AddEventCallBack(nEventID, functorPtr);
    }

    virtual bool RemoveEventCallBack(const int nEventID) = 0;

    virtual bool DoEvent(const int nEventID, const NFIDataList& valueList) = 0;

    virtual bool AddEventCallBack(const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb) = 0;
    //////////////////////////////////////////////////////////////////////////

protected:
    virtual bool HasEventCallBack(const int nEventID) = 0;

};

#endif
