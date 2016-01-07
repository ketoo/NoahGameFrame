// -------------------------------------------------------------------------
//    @FileName      :    NFCEventProcessModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventProcessModule
//
// -------------------------------------------------------------------------

#ifndef NFC_EVENTPROCESS_MODULE_H
#define NFC_EVENTPROCESS_MODULE_H

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

    virtual bool Execute();

    virtual bool RemoveEvent(const NFGUID& objectID);
    virtual bool RemoveEventCallBack(const NFGUID& objectID, const int nEventID);

    virtual bool DoEvent(const NFGUID& objectID, const int nEventID, const NFIDataList& valueList);
    virtual bool DoEvent(const NFGUID& objectID,  const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList);

    virtual bool AddEventCallBack(const NFGUID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb);
    virtual bool AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb);

protected:
	virtual bool HasEventCallBack(const NFGUID& objectID, const int nEventID);

private:

    NFList<NFGUID> mRemoveObjectListEx;
    NFMapEx<NFGUID, NFList<int> > mRemoveEventListEx;

	NFMapEx<std::string, NFCClassEventList> mxClassEventInfoEx;
    NFMapEx<NFGUID, NFCObjectEventInfo> mObjectEventInfoMapEx;
};


#endif