// -------------------------------------------------------------------------
//    @FileName      :    NFCEventProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventProcessModule
//
// -------------------------------------------------------------------------

#ifndef _NFC_EVENTPROCESS_MODULE_H_
#define _NFC_EVENTPROCESS_MODULE_H_

#include "NFComm/NFPluginModule/NFIEventProcessModule.h"

struct NFEventList
        : public NFList<EVENT_PROCESS_FUNCTOR_PTR>
{

};
struct NFClassEventList
        : public NFList<CLASS_EVENT_FUNCTOR_PTR>
{

};

class NFCObjectEventInfo
    : public NFMapEx<int, NFEventList>
{
};

class NFCClassEventInfo
    : public NFMapEx<std::string, NFClassEventList>
{
};

class NFCEventProcessModule
    : public NFIEventProcessModule
{
public:
    NFCEventProcessModule(NFIPluginManager* p);
    virtual ~NFCEventProcessModule();

    virtual bool Init();
    virtual bool Shut();

    virtual void OnReload(const char* strModuleName, NFILogicModule* pModule);


    virtual bool Execute(const float fLasFrametime, const float fStartedTime);


    virtual bool HasEventCallBack(const NFIDENTID& objectID, const int nEventID);
    virtual bool RemoveEvent(const NFIDENTID& objectID);
    virtual bool RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID/*, const EVENT_PROCESS_FUNCTOR_PTR& cb*/);

    virtual bool DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIDataList& valueList, const bool bSync = true);
    virtual bool DoEvent(const NFIDENTID& objectID,  const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList, const bool bSync = true);

    virtual bool AddEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb);
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);


private:
    NFList<NFIDENTID> mRemoveObjectListEx;
    NFMapEx<NFIDENTID, NFList<int> > mRemoveEventListEx;

    NFCClassEventInfo* m_pClassEventInfoEx;

    typedef NFMapEx<NFIDENTID, NFCObjectEventInfo> NFCObjectEventInfoMapEx;
    NFCObjectEventInfoMapEx mObjectEventInfoMapEx;
};


#endif