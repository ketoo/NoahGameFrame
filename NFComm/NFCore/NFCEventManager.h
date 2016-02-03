// -------------------------------------------------------------------------
//    @FileName      :    NFCEventManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventManager
//
// -------------------------------------------------------------------------

#ifndef NFC_EVENT_MNG_H
#define NFC_EVENT_MNG_H

#include "NFIEventManager.h"
#include "NFDefine.h"

class NFCEventManager
	: public NFIEventManager
{
public:
	NFCEventManager(NFGUID self);
    virtual ~NFCEventManager();

    virtual bool Init();
    virtual bool Shut();

    virtual bool Execute();

    virtual bool RemoveEventCallBack(const int nEventID);

    virtual bool DoEvent(const int nEventID, const NFIDataList& valueList);

    virtual bool AddEventCallBack(const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb);

protected:
	virtual bool HasEventCallBack(const int nEventID);

private:
	NFGUID mSelf;

    NFList<int> mRemoveEventListEx;
	NFMapEx<int, NFList<EVENT_PROCESS_FUNCTOR_PTR>> mObjectEventInfoMapEx;
};


#endif