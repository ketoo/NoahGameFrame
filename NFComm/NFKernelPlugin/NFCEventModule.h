// -------------------------------------------------------------------------
//    @FileName         :    NFCEventModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventModule
//
// -------------------------------------------------------------------------

#ifndef NFC_EVENT_MODULE_H
#define NFC_EVENT_MODULE_H

#include <iostream>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFPluginModule/NFGUID.h"
#include "NFComm/NFPluginModule/NFIEventModule.h"
#include "NFComm/NFPluginModule/NFIKernelModule.h"

class NFCEventModule
    : public NFIEventModule
{
public:
    NFCEventModule(NFIPluginManager* p)
    {
        pPluginManager = p;
    }


    virtual ~NFCEventModule()
    {
    }

	virtual bool Init();
	virtual bool AfterInit();
	virtual bool BeforeShut();
	virtual bool Shut();
	virtual bool Execute();

	virtual bool DoEvent(const NFEventDefine nEventID, const NFDataList& valueList);

	virtual bool ExistEventCallBack(const NFEventDefine nEventID);

	virtual bool RemoveEventCallBack(const NFEventDefine nEventID);

	//////////////////////////////////////////////////////////
	virtual bool DoEvent(const NFGUID self, const NFEventDefine nEventID, const NFDataList& valueList);

	virtual bool ExistEventCallBack(const NFGUID self, const NFEventDefine nEventID);

	virtual bool RemoveEventCallBack(const NFGUID self, const NFEventDefine nEventID);
	virtual bool RemoveEventCallBack(const NFGUID self);

protected:

	virtual bool AddEventCallBack(const NFEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb);
	virtual bool AddEventCallBack(const NFGUID self, const NFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb);

private:

	NFIKernelModule* m_pKernelodule;

private:
	// for module
	NFList<NFEventDefine> mModuleRemoveListEx;
	NFMapEx<NFEventDefine, NFList<MODULE_EVENT_FUNCTOR_PTR>> mModuleEventInfoMapEx;

	//for object
	NFList<NFGUID> mObjectRemoveListEx;
	NFMapEx<NFGUID, NFMapEx<NFEventDefine, NFList<OBJECT_EVENT_FUNCTOR_PTR>>> mObjectEventInfoMapEx;
};

#endif