// -------------------------------------------------------------------------
//    @FileName      :    NFCEventProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventProcessModule
//
// -------------------------------------------------------------------------

#include "NFCEventManager.h"

NFCEventManager::NFCEventManager( NFGUID self )
{
	mSelf = self;
}

NFCEventManager::~NFCEventManager()
{
	mRemoveEventListEx.ClearAll();

	mObjectEventInfoMapEx.ClearAll();
}

bool NFCEventManager::Init()
{
    return true;
}

bool NFCEventManager::Shut()
{
    mRemoveEventListEx.ClearAll();
    mObjectEventInfoMapEx.ClearAll();

    return true;
}

bool NFCEventManager::AddEventCallBack(const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFList<EVENT_PROCESS_FUNCTOR_PTR>> pEventInfo = mObjectEventInfoMapEx.GetElement(nEventID);
    if (nullptr == pEventInfo)
    {
        pEventInfo = NF_SHARE_PTR<NFList<EVENT_PROCESS_FUNCTOR_PTR>>(NF_NEW NFList<EVENT_PROCESS_FUNCTOR_PTR>());
        mObjectEventInfoMapEx.AddElement(nEventID, pEventInfo);
    }

    assert(nullptr != pEventInfo);

    pEventInfo->Add(cb);

    return true;
}

bool NFCEventManager::Execute()
{
	int nEvent = 0;
	bool bRet = mRemoveEventListEx.First(nEvent);
	while (bRet)
	{
		mObjectEventInfoMapEx.RemoveElement(nEvent);

		bRet = mRemoveEventListEx.Next(nEvent);
	}

    mRemoveEventListEx.ClearAll();

    return true;
}

bool NFCEventManager::RemoveEventCallBack(const int nEventID/*, const EVENT_PROCESS_FUNCTOR_PTR& cb*/)
{
	mRemoveEventListEx.Add(nEventID);
	return true;
}

bool NFCEventManager::DoEvent(const int nEventID, const NFIDataList& valueList)
{
	NF_SHARE_PTR<NFList<EVENT_PROCESS_FUNCTOR_PTR>> pEventInfo = mObjectEventInfoMapEx.GetElement(nEventID);
	if (nullptr == pEventInfo)
	{
		return false;
	}

	EVENT_PROCESS_FUNCTOR_PTR cb;
	bool bRet = pEventInfo->First(cb);
	while (bRet)
	{
		cb->operator()(mSelf, nEventID,  valueList);

		bRet = pEventInfo->Next(cb);
	}

    return true;
}

bool NFCEventManager::HasEventCallBack(const int nEventID)
{
	NF_SHARE_PTR<NFList<EVENT_PROCESS_FUNCTOR_PTR>> pEventInfo = mObjectEventInfoMapEx.GetElement(nEventID);
	if (nullptr != pEventInfo)
	{
		return true;
	}

    return false;
}
