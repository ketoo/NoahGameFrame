#pragma once

// -------------------------------------------------------------------------
//    �ļ���      :    NFIEventManager.h
//    ������      :    LvSheng.Huang
//    ����ʱ��    :    2012-07-31 22:14
//    ��������    :    �¼�������
//
// -------------------------------------------------------------------------

#include "../NFCore/NFDefine.h"
#include "../NFCore/NFIdentID.h"
#include "../NFCore/NFCValueList.h"
#include "../NFPluginModule/NFIEventProcessModule.h"

class NFIEventManager
{
public:

    virtual bool AddEventCallBack(const NFIDENTID& objectID, const int nEventID, EVENT_PROCESS_FUNC cb) = 0;
    virtual bool AddClassCallBack(const std::string& strClassName, CLASS_EVENT_FUNC cb) = 0;

    virtual bool Execute(const float fLasFrametime, const float fStartedTime) = 0;

    virtual bool HasEventCallBack(const NFIDENTID& objectID, const int nEventID, EVENT_PROCESS_FUNC cb) = 0;

    virtual bool RemoveEvent(const NFIDENTID& objectID) = 0;
    virtual bool RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID, EVENT_PROCESS_FUNC cb) = 0;
    virtual bool RemoveClassCallBack(const std::string& strClassName, CLASS_EVENT_FUNC cb) = 0;

    virtual bool DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIValueList& valueList) = 0;
    virtual bool DoEvent(NFIDENTID& objectID, const  std::string& strClassName, const int nClassEvent, const NFIValueList& valueList) = 0;

};