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

    bool Init();
    bool AfterInit();
    bool BeforeShut();
    bool Shut();
    bool Execute();

	bool DoEvent(const NFEventDefine nEventID, const NFIDataList& valueList);

	bool ExistEventCallBack(const NFEventDefine nEventID);

	bool RemoveEventCallBack(const NFEventDefine nEventID);

	//////////////////////////////////////////////////////////
	bool DoEvent(const NFGUID self, const NFEventDefine nEventID, const NFIDataList& valueList);

	bool ExistEventCallBack(const NFGUID self, const NFEventDefine nEventID);

	bool RemoveEventCallBack(const NFGUID self, const NFEventDefine nEventID);

private:

	bool AddEventCallBack(const NFEventDefine nEventID, const MODULE_EVENT_FUNCTOR_PTR cb);
	bool AddEventCallBack(const NFGUID self, const NFEventDefine nEventID, const OBJECT_EVENT_FUNCTOR_PTR cb);

private:

	NFList<NFEventDefine> mModuleRemoveListEx;
	NFMapEx<NFEventDefine, NFList<MODULE_EVENT_FUNCTOR_PTR>> mModuleEventInfoMapEx;

	NFMapEx<NFGUID, NFList<NFEventDefine>> mObjectRemoveListEx;
	NFMapEx<NFGUID, NFMapEx<NFEventDefine, NFList<OBJECT_EVENT_FUNCTOR_PTR>>> mObjectEventInfoMapEx;
};

#endif