// -------------------------------------------------------------------------
//    @FileName      :    NFCEventProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventProcessModule
//
// -------------------------------------------------------------------------

#include "NFCEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"
#include "NFComm/NFPluginModule/NFIActorManager.h"

NFCEventProcessModule::NFCEventProcessModule(NFIPluginManager* p)
{
    pPluginManager = p;
}

NFCEventProcessModule::~NFCEventProcessModule()
{
	mRemoveObjectListEx.ClearAll();
	mRemoveEventListEx.ClearAll();

	mxClassEventInfoEx.ClearAll();
	mObjectEventInfoMapEx.ClearAll();
}

bool NFCEventProcessModule::Init()
{
    return true;
}

bool NFCEventProcessModule::Shut()
{
    mxClassEventInfoEx.ClearAll();
    mRemoveEventListEx.ClearAll();
    mObjectEventInfoMapEx.ClearAll();

    return true;
}

void NFCEventProcessModule::OnReload(const char* strModuleName, NFILogicModule* pModule)
{
}

bool NFCEventProcessModule::AddEventCallBack(const NFGUID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (nullptr == pObjectEventInfo)
    {
        pObjectEventInfo = NF_SHARE_PTR<NFCObjectEventInfo>(NF_NEW NFCObjectEventInfo());
        mObjectEventInfoMapEx.AddElement(objectID, pObjectEventInfo);
    }

    assert(nullptr != pObjectEventInfo);

    NF_SHARE_PTR<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
    if (nullptr == pEventInfo)
    {
        pEventInfo = NF_SHARE_PTR<NFEventList>(NF_NEW NFEventList());
        pObjectEventInfo->AddElement(nEventID, pEventInfo);
    }

    assert(nullptr != pEventInfo);

    pEventInfo->Add(cb);

    return true;
}

bool NFCEventProcessModule::Execute()
{
    NFGUID ident;

    NF_SHARE_PTR<NFList<int>> pList = mRemoveEventListEx.First(ident);
    while (nullptr != pList)
    {
        NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(ident);
        if (pObjectEventInfo)
        {
            int nEvent = 0;
            bool bRet = pList->First(nEvent);
            while (bRet)
            {
                pObjectEventInfo->RemoveElement(nEvent);

                bRet = pList->Next(nEvent);
            }
        }

        pList = NULL;
        pList = mRemoveEventListEx.Next();
    }

    mRemoveEventListEx.ClearAll();

    //////////////////////////////////////////////////////////////////////////
    //删除事件对象
    bool bRet = mRemoveObjectListEx.First(ident);
    while (bRet)
    {
        mObjectEventInfoMapEx.RemoveElement(ident);

        bRet = mRemoveObjectListEx.Next(ident);
    }

    mRemoveObjectListEx.ClearAll();

    return true;
}

bool NFCEventProcessModule::RemoveEvent(const NFGUID& objectID)
{
    return mRemoveObjectListEx.Add(objectID);
}

bool NFCEventProcessModule::RemoveEventCallBack(const NFGUID& objectID, const int nEventID/*, const EVENT_PROCESS_FUNCTOR_PTR& cb*/)
{
    NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (nullptr != pObjectEventInfo)
    {
        NF_SHARE_PTR<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (nullptr != pEventInfo)
        {
            NF_SHARE_PTR<NFList<int>> pList = mRemoveEventListEx.GetElement(objectID);
            if (nullptr != pList)
            {
                pList = NF_SHARE_PTR<NFList<int>>(NF_NEW NFList<int>());
                mRemoveEventListEx.AddElement(objectID, pList);
            }

            pList->Add(nEventID);
            return true;
        }
    }

    return false;
}

bool NFCEventProcessModule::DoEvent(const NFGUID& objectID, const int nEventID, const NFIDataList& valueList)
{
	NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
	if (nullptr == pObjectEventInfo)
	{
		return false;
	}


	NF_SHARE_PTR<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
	if (nullptr == pEventInfo)
	{
		return false;
	}

	EVENT_PROCESS_FUNCTOR_PTR cb;
	bool bRet = pEventInfo->First(cb);
	while (bRet)
	{
		cb->operator()(objectID, nEventID,  valueList);

		bRet = pEventInfo->Next(cb);
	}

    return true;
}

bool NFCEventProcessModule::DoEvent(const NFGUID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList)
{
    NF_SHARE_PTR<NFCClassEventList> pEventList = mxClassEventInfoEx.GetElement(strClassName);
    if (nullptr != pEventList)
    {
        CLASS_EVENT_FUNCTOR_PTR cb;
        bool bRet = pEventList->First(cb);
        while (bRet)
        {
            cb->operator()(objectID, strClassName, eClassEvent,  valueList);

            bRet = pEventList->Next(cb);
        }
    }

    return false;
}

bool NFCEventProcessModule::HasEventCallBack(const NFGUID& objectID, const int nEventID)
{
    NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (nullptr != pObjectEventInfo)
    {
        NF_SHARE_PTR<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (nullptr != pEventInfo)
        {
            return true;
        }
    }

    return false;
}

bool NFCEventProcessModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFCClassEventList> pEventList = mxClassEventInfoEx.GetElement(strClassName);
    if (nullptr == pEventList)
    {
        pEventList = NF_SHARE_PTR<NFCClassEventList>(NF_NEW NFCClassEventList());
        mxClassEventInfoEx.AddElement(strClassName, pEventList);
    }

    assert(NULL != pEventList);

    pEventList->Add(cb);

    return true;
}