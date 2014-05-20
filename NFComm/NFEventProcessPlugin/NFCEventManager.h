#pragma once

// -------------------------------------------------------------------------
//    文件名      :    NFCEventManager.h
//    创建者      :    LvSheng.Huang
//    创建时间    :    2012-07-31 22:14
//    功能描述    :    事件管理器
//
// -------------------------------------------------------------------------

#include <map>
#include <list>
#include "NFIEventManager.h"
#include "../NFCore/GTSingleton.h"
#include "../NFCore/NFCValueList.h"
#include "../NFCore/GTList.h"
#include "../NFCore/GTMap.h"



class NFCEventManager
    : public NFIEventManager,
  public GTMap<NFIDENTID, NFCObjectEventInfo>
{
public:
    NFCEventManager() {};
    virtual ~NFCEventManager() {};

    //virtual bool AddEvent( const NFIDENTID& objectID, const int nEventID, const int nSamsaraTime, const float fProcessTime, const NFIValueList& valueList, const bool bSyschro = true );
    virtual bool AddEventCallBack(const NFIDENTID& objectID, const int nEventID, EVENT_PROCESS_FUNC cb);
    virtual bool AddClassCallBack(const std::string& strClassName, CLASS_EVENT_FUNC cb);

    virtual bool Execute(const float fLasFrametime, const float fStartedTime);

    //virtual bool HasEvent( const NFIDENTID& objectID, const int nEventID );
    virtual bool HasEventCallBack(const NFIDENTID& objectID, const int nEventID, EVENT_PROCESS_FUNC cb);

    virtual bool RemoveEvent(const NFIDENTID& objectID);
    //  virtual bool RemoveEvent( const NFIDENTID& objectID, const int nEventID );
    virtual bool RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID, EVENT_PROCESS_FUNC cb);
    virtual bool RemoveClassCallBack(const std::string& strClassName, CLASS_EVENT_FUNC cb);

    virtual bool DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIValueList& valueList);
    virtual bool DoEvent(NFIDENTID& objectID, const  std::string& strClassName, const int nClassEvent, const NFIValueList& valueList);

private:
    //virtual bool AddEvent( const NFIDENTID& objectID, const int nEventID, const NFEventList& info );

    //virtual bool DoEvent( const NFIDENTID& objectID, NFEventList* info );

private:
    NFCClassEventInfo mClassEventInfo;
};