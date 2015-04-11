// -------------------------------------------------------------------------
//    @FileName      :    NFCEventProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventProcessModule
//
// -------------------------------------------------------------------------

#include "NFCEventProcessModule.h"
#include "NFComm/NFPluginModule/NFIPluginManager.h"

#ifdef NF_USE_ACTOR
#include "NFComm/NFPluginModule/NFIActorManager.h"
#endif

NFCEventProcessModule::NFCEventProcessModule(NFIPluginManager* p)
{
    pPluginManager = p;

}

NFCEventProcessModule::~NFCEventProcessModule()
{

}

bool NFCEventProcessModule::Init()
{
    m_pClassEventInfoEx = NF_NEW NFCClassEventInfo();

    return true;
}

bool NFCEventProcessModule::Shut()
{
    m_pClassEventInfoEx->ClearAll();

    mRemoveEventListEx.ClearAll();

    mObjectEventInfoMapEx.ClearAll();

    return true;
}

void NFCEventProcessModule::OnReload(const char* strModuleName, NFILogicModule* pModule)
{

}

bool NFCEventProcessModule::AddEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (!pObjectEventInfo.get())
    {
        pObjectEventInfo = NF_SHARE_PTR<NFCObjectEventInfo>(NF_NEW NFCObjectEventInfo());
        mObjectEventInfoMapEx.AddElement(objectID, pObjectEventInfo);
    }
    assert(NULL != pObjectEventInfo);

    NF_SHARE_PTR<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
    if (!pEventInfo)
    {
        pEventInfo = NF_SHARE_PTR<NFEventList>(NF_NEW NFEventList());
        pObjectEventInfo->AddElement(nEventID, pEventInfo);
    }

    assert(NULL != pEventInfo);

    pEventInfo->Add(cb);

    return true;
}

bool NFCEventProcessModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    NFIDENTID ident;

    NF_SHARE_PTR<NFList<int>> pList = mRemoveEventListEx.First(ident);
    while (pList.get())
    {
        //鍒犻櫎瀵硅薄鐨勬煇涓簨浠?
        NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(ident);
        if (pObjectEventInfo.get())
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

    mRemoveObjectListEx.ClearAll();

    return true;
}

bool NFCEventProcessModule::RemoveEvent(const NFIDENTID& objectID)
{
    return mRemoveObjectListEx.Add(objectID);
}

bool NFCEventProcessModule::RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID/*, const EVENT_PROCESS_FUNCTOR_PTR& cb*/)
{
    NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (pObjectEventInfo.get())
    {
        NF_SHARE_PTR<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo.get())
        {
            NF_SHARE_PTR<NFList<int>> pList = mRemoveEventListEx.GetElement(objectID);
            if (!pList.get())
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

bool NFCEventProcessModule::DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIDataList& valueList, const bool bSync)
{
	if (bSync)
	{
		NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
		if (!pObjectEventInfo.get())
		{
			return false;
		}


		NF_SHARE_PTR<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
		if (!pEventInfo.get())
		{
			return false;
		}

		EVENT_PROCESS_FUNCTOR_PTR cb;// = NF_SHARE_PTR<EVENT_PROCESS_FUNCTOR>(NULL);
		bool bRet = pEventInfo->First(cb);
		while (bRet)
		{
			cb.get()->operator()(objectID, nEventID,  valueList);

			bRet = pEventInfo->Next(cb);
		}

	}
	else
	{
		
#ifdef NF_USE_ACTOR
		//pPluginManager->GetActorID()
		//处理之前，还得把所有的回调函数对象发给actor，以便他调用这些函数进行处理
		//pEventInfo
		NF_SHARE_PTR<NFCObjectAsyncEventInfo> pObjectEventInfo = mObjectSyncEventInfoMapEx.GetElement(objectID);
		if (!pObjectEventInfo.get())
		{
			return false;
		}


		NF_SHARE_PTR<NFAsyncEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
		if (!pEventInfo.get())
		{
			return false;
		}


		NFIActorManager* pActorManager = pPluginManager->GetActorManager();
		std::string strArg;
		valueList.ToString(strArg, "|");
		pActorManager->OnRequireActor(objectID, nEventID, strArg, pEventInfo);
#endif
	}

    return true;
}

bool NFCEventProcessModule::DoEvent(const NFIDENTID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList, const bool bSync)
{
    NF_SHARE_PTR<NFClassEventList> pEventList = m_pClassEventInfoEx->GetElement(strClassName);
    if (pEventList.get())
    {
        CLASS_EVENT_FUNCTOR_PTR cb;// = NF_SHARE_PTR<CLASS_EVENT_FUNCTOR>(NULL);
        bool bRet = pEventList->First(cb);
        while (bRet)
        {
            cb.get()->operator()(objectID, strClassName, eClassEvent,  valueList);

            bRet = pEventList->Next(cb);
        }
    }

    return false;
}

bool NFCEventProcessModule::HasEventCallBack(const NFIDENTID& objectID, const int nEventID)
{
    NF_SHARE_PTR<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (pObjectEventInfo.get())
    {
        NF_SHARE_PTR<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo)
        {
            return true;//pEventInfo->Find(cb);
        }
    }
    return false;
}

bool NFCEventProcessModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFClassEventList> pEventList = m_pClassEventInfoEx->GetElement(strClassName);
    if (!pEventList.get())
    {
        pEventList = NF_SHARE_PTR<NFClassEventList>(NF_NEW NFClassEventList());
        m_pClassEventInfoEx->AddElement(strClassName, pEventList);
    }

    assert(NULL != pEventList);

    pEventList->Add(cb);

    return true;
}

bool NFCEventProcessModule::AddAsyncEventCallBack( const NFIDENTID& objectID, const int nEventID, const EVENT_ASYNC_PROCESS_FUNCTOR_PTR& cb )
{
	NF_SHARE_PTR<NFCObjectAsyncEventInfo> pObjectSyncEventInfo = mObjectSyncEventInfoMapEx.GetElement(objectID);
	if (!pObjectSyncEventInfo.get())
	{
		pObjectSyncEventInfo = NF_SHARE_PTR<NFCObjectAsyncEventInfo>(NF_NEW NFCObjectAsyncEventInfo());
		mObjectSyncEventInfoMapEx.AddElement(objectID, pObjectSyncEventInfo);
	}

	assert(NULL != pObjectSyncEventInfo);

	NF_SHARE_PTR<NFAsyncEventList> pSyncEventInfo = pObjectSyncEventInfo->GetElement(nEventID);
	if (!pSyncEventInfo)
	{
		pSyncEventInfo = NF_SHARE_PTR<NFAsyncEventList>(NF_NEW NFAsyncEventList());
		pObjectSyncEventInfo->AddElement(nEventID, pSyncEventInfo);
	}

	assert(NULL != pSyncEventInfo);

	pSyncEventInfo->Add(cb);

	return true;
}

bool NFCEventProcessModule::AddAsyncClassCallBack( const std::string& strClassName, const CLASS_ASYNC_EVENT_FUNCTOR_PTR& cb )
{
	return true;
}
