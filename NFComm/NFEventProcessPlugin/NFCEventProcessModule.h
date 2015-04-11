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

class NFCObjectEventInfo
    : public NFMapEx<int, NFEventList>
{
};

class NFCObjectAsyncEventInfo
	: public NFMapEx<int, NFAsyncEventList>
{
};

class NFCClassEventInfo
    : public NFMapEx<std::string, NFClassEventList>
{
};

class NFCClassSyncEventInfo
	: public NFMapEx<std::string, NFClassAsyncEventList>
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

	//////////sync////////////////////////////////////////////////////////////////
	virtual bool AddAsyncEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_ASYNC_PROCESS_FUNCTOR_PTR& cb);
	virtual bool AddAsyncClassCallBack(const std::string& strClassName, const CLASS_ASYNC_EVENT_FUNCTOR_PTR& cb);
private:
    NFList<NFIDENTID> mRemoveObjectListEx;
    NFMapEx<NFIDENTID, NFList<int> > mRemoveEventListEx;

    NFCClassEventInfo* m_pClassEventInfoEx;

    typedef NFMapEx<NFIDENTID, NFCObjectEventInfo> NFCObjectEventInfoMapEx;
    NFCObjectEventInfoMapEx mObjectEventInfoMapEx;

	///////////////////sync event///////////////////////////////////////////////////////
	typedef NFMapEx<NFIDENTID, NFCObjectAsyncEventInfo> NFCObjectSyncEventInfoMapEx;
	NFCObjectSyncEventInfoMapEx mObjectSyncEventInfoMapEx;
};


#endif