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
	virtual bool AddAsyncEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR& cb_begin, const EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR& cb_end);
	virtual bool AddActorEventCallBack( const NFIDENTID& objectID, const int nEventID, NF_SHARE_PTR<NFIComponent> pComponent, const EVENT_ASYNC_PROCESS_BEGIN_FUNCTOR_PTR& cb, const EVENT_ASYNC_PROCESS_END_FUNCTOR_PTR& cb_end );
	virtual bool AddAsyncClassCallBack(const std::string& strClassName, const CLASS_ASYNC_EVENT_FUNCTOR_PTR& cb_begin, const CLASS_ASYNC_EVENT_FUNCTOR_PTR& cb_end);
private:

    NFList<NFIDENTID> mRemoveObjectListEx;
    NFMapEx<NFIDENTID, NFList<int> > mRemoveEventListEx;

	NFMapEx<std::string, NFCClassEventList> mxClassEventInfoEx;
    NFMapEx<NFIDENTID, NFCObjectEventInfo> mObjectEventInfoMapEx;

	///////////////////async event///////////////////////////////////////////////////////
	NFMapEx<NFIDENTID, NFCObjectAsyncEventInfo> mObjectSyncEventInfoMapEx;
	NFMapEx<std::string, NFClassAsyncEventList> mClassSyncEventInfoMapEx;


};


#endif