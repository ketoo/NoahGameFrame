// -------------------------------------------------------------------------
//    @FileName      :    NFCEventProcessModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-15
//    @Module           :    NFCEventProcessModule
//
// -------------------------------------------------------------------------

#include "NFCEventProcessModule.h"

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
    std::shared_ptr<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (!pObjectEventInfo.get())
    {
        pObjectEventInfo = std::shared_ptr<NFCObjectEventInfo>(NF_NEW NFCObjectEventInfo());
        mObjectEventInfoMapEx.AddElement(objectID, pObjectEventInfo);
    }
    assert(NULL != pObjectEventInfo);

    std::shared_ptr<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
    if (!pEventInfo)
    {
        pEventInfo = std::shared_ptr<NFEventList>(NF_NEW NFEventList());
        pObjectEventInfo->AddElement(nEventID, pEventInfo);
    }

    assert(NULL != pEventInfo);

    pEventInfo->Add(cb);

    return true;
}

bool NFCEventProcessModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    NFIDENTID ident;

    std::shared_ptr<NFList<int>> pList = mRemoveEventListEx.First(ident);
    while (pList.get())
    {
        //删除对象的某个事�?
        std::shared_ptr<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(ident);
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
    std::shared_ptr<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (pObjectEventInfo.get())
    {
        std::shared_ptr<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo.get())
        {
            std::shared_ptr<NFList<int>> pList = mRemoveEventListEx.GetElement(objectID);
            if (!pList.get())
            {
                pList = std::shared_ptr<NFList<int>>(NF_NEW NFList<int>());
                mRemoveEventListEx.AddElement(objectID, pList);
            }

            pList->Add(nEventID);
            return true;
        }
    }

    return false;
}

bool NFCEventProcessModule::DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIDataList& valueList)
{
    std::shared_ptr<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (!pObjectEventInfo.get())
    {
        return false;
    }


    std::shared_ptr<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
    if (!pEventInfo.get())
    {
        return false;
    }

    EVENT_PROCESS_FUNCTOR_PTR cb;// = std::shared_ptr<EVENT_PROCESS_FUNCTOR>(NULL);
    bool bRet = pEventInfo->First(cb);
    while (bRet)
    {
        cb.get()->operator()(objectID, nEventID,  valueList);

        bRet = pEventInfo->Next(cb);
    }

    return true;
}

bool NFCEventProcessModule::DoEvent(const NFIDENTID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& valueList)
{
    std::shared_ptr<NFClassEventList> pEventList = m_pClassEventInfoEx->GetElement(strClassName);
    if (pEventList.get())
    {
        CLASS_EVENT_FUNCTOR_PTR cb;// = std::shared_ptr<CLASS_EVENT_FUNCTOR>(NULL);
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
    std::shared_ptr<NFCObjectEventInfo> pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (pObjectEventInfo.get())
    {
        std::shared_ptr<NFEventList> pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo)
        {
            return true;//pEventInfo->Find(cb);
        }
    }
    return false;
}

bool NFCEventProcessModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
    std::shared_ptr<NFClassEventList> pEventList = m_pClassEventInfoEx->GetElement(strClassName);
    if (!pEventList.get())
    {
        pEventList = std::shared_ptr<NFClassEventList>(NF_NEW NFClassEventList());
        m_pClassEventInfoEx->AddElement(strClassName, pEventList);
    }

    assert(NULL != pEventList);

    pEventList->Add(cb);

    return true;
}