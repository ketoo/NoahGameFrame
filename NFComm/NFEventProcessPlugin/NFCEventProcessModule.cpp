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
    m_pClassEventInfoEx = new NFCClassEventInfo();
}

NFCEventProcessModule::~NFCEventProcessModule()
{
    delete m_pClassEventInfoEx;
}

bool NFCEventProcessModule::Init()
{

    return true;
}

bool NFCEventProcessModule::Shut()
{
    NFClassEventList* pEventList = m_pClassEventInfoEx->First();
    while (pEventList)
    {
        pEventList->ClearAll();
        delete pEventList;
        pEventList = NULL;

        pEventList = m_pClassEventInfoEx->Next();
    }

    NFCObjectEventInfo* pObjectEventInfo = mObjectEventInfoMapEx.First();
    while (pObjectEventInfo)
    {
        NFEventList* pEventListEx = pObjectEventInfo->First();
        while (pEventListEx)
        {
            pEventListEx->ClearAll();
            delete pEventListEx;
            pEventListEx = NULL;

            pEventListEx = pObjectEventInfo->Next();
        }

        pObjectEventInfo->ClearAll();
        delete pObjectEventInfo;
        pObjectEventInfo = NULL;

        pObjectEventInfo = mObjectEventInfoMapEx.Next();
    }

    return true;
}

void NFCEventProcessModule::OnReload(const char* strModuleName, NFILogicModule* pModule)
{

}

bool NFCEventProcessModule::AddEventCallBack(const NFIDENTID& objectID, const int nEventID, const EVENT_PROCESS_FUNCTOR_PTR& cb)
{
    NFCObjectEventInfo* pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (!pObjectEventInfo)
    {
        pObjectEventInfo = new NFCObjectEventInfo();
        mObjectEventInfoMapEx.AddElement(objectID, pObjectEventInfo);
    }
    assert(NULL != pObjectEventInfo);

    NFEventList* pEventInfo = pObjectEventInfo->GetElement(nEventID);
    if (!pEventInfo)
    {
        pEventInfo = new NFEventList();
        pObjectEventInfo->AddElement(nEventID, pEventInfo);
    }

    assert(NULL != pEventInfo);

    pEventInfo->Add(cb);

    return true;
}

bool NFCEventProcessModule::Execute(const float fLasFrametime, const float fStartedTime)
{
    NFIDENTID ident;

    NFList<int>* pList = mRemoveEventListEx.First(ident);
    while (pList)
    {
        //删除对象的某个事件
        NFCObjectEventInfo* pObjectEventInfo = mObjectEventInfoMapEx.GetElement(ident);
        if (pObjectEventInfo)
        {
            int nEvent = 0;
            bool bRet = pList->First(nEvent);
            while (bRet)
            {
                NFEventList* pEventList = pObjectEventInfo->RemoveElement(nEvent);
                if (pEventList)
                {
                    pEventList->ClearAll();
                    delete pEventList;

                    pEventList = NULL;
                }

                bRet = pList->Next(nEvent);
            }
        }

        if (pList->Count() == 0)
        {
            delete pList;
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
        NFCObjectEventInfo* pObjectEventInfo = mObjectEventInfoMapEx.RemoveElement(ident);
        if (pObjectEventInfo)
        {
            NFEventList* pEventList = pObjectEventInfo->First();
            while (pEventList)
            {
                delete pEventList;
                pEventList = NULL;

                pEventList = pObjectEventInfo->Next();
            }

            delete pObjectEventInfo;
            pObjectEventInfo = NULL;
        }

        bRet = mRemoveObjectListEx.Next(ident);
    }

    mRemoveObjectListEx.ClearAll();

    return true;
}

bool NFCEventProcessModule::RemoveEvent(const NFIDENTID& objectID)
{
    return mRemoveObjectListEx.Add(objectID);
}

bool NFCEventProcessModule::RemoveEventCallBack(const NFIDENTID& objectID, const int nEventID/*, const EVENT_PROCESS_FUNCTOR_PTR& cb*/)
{
    NFCObjectEventInfo* pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (pObjectEventInfo)
    {
        NFEventList* pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo)
        {
            NFList<int>* pList = mRemoveEventListEx.GetElement(objectID);
            if (!pList)
            {
                pList = new NFList<int>();
                mRemoveEventListEx.AddElement(objectID, pList);
            }

            pList->Add(nEventID);
            return true;
        }
    }

    return false;
}

bool NFCEventProcessModule::DoEvent(const NFIDENTID& objectID, const int nEventID, const NFIValueList& valueList)
{
    NFCObjectEventInfo* pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (!pObjectEventInfo)
    {
        return false;
    }


    NFEventList* pEventInfo = pObjectEventInfo->GetElement(nEventID);
    if (!pEventInfo)
    {
        return false;
    }

    EVENT_PROCESS_FUNCTOR_PTR cb = std::shared_ptr<EVENT_PROCESS_FUNCTOR>(NULL);
    bool bRet = pEventInfo->First(cb);
    while (bRet)
    {
        cb.get()->operator()(objectID, nEventID,  valueList);

        bRet = pEventInfo->Next(cb);
    }

    return true;
}

bool NFCEventProcessModule::DoEvent(const NFIDENTID& objectID, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIValueList& valueList)
{
    NFClassEventList* pEventList = m_pClassEventInfoEx->GetElement(strClassName);
    if (pEventList)
    {
        CLASS_EVENT_FUNCTOR_PTR cb = std::shared_ptr<CLASS_EVENT_FUNCTOR>(NULL);
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
    NFCObjectEventInfo* pObjectEventInfo = mObjectEventInfoMapEx.GetElement(objectID);
    if (pObjectEventInfo)
    {
        NFEventList* pEventInfo = pObjectEventInfo->GetElement(nEventID);
        if (pEventInfo)
        {
            return true;//pEventInfo->Find(cb);
        }
    }
    return false;
}

bool NFCEventProcessModule::AddClassCallBack(const std::string& strClassName, const CLASS_EVENT_FUNCTOR_PTR& cb)
{
    NFClassEventList* pEventList = m_pClassEventInfoEx->GetElement(strClassName);
    if (!pEventList)
    {
        pEventList = new NFClassEventList();
        m_pClassEventInfoEx->AddElement(strClassName, pEventList);
    }

    assert(NULL != pEventList);

    pEventList->Add(cb);

    return true;
}