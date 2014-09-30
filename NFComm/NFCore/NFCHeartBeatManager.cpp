// -------------------------------------------------------------------------
//    @FileName         :    NFCHeartBeatManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFCHeartBeatManager
//
// -------------------------------------------------------------------------

#include "NFCHeartBeatManager.h"

NFCHeartBeatManager::~NFCHeartBeatManager()
{
    NFCHeartBeatElement* pElementEx = mHeartBeatElementMapEx.First();
    while (pElementEx)
    {
        delete pElementEx;
        pElementEx = mHeartBeatElementMapEx.Next();
    }
}

void NFCHeartBeatElement::DoHeartBeatEvent()
{
    HEART_BEAT_FUNCTOR_PTR cb;// = HEART_BEAT_FUNCTOR_PTR();
    bool bRet = First(cb);
    while (bRet)
    {
        cb.get()->operator()(self, strBeatName, fBeatTime, nCount, var);

        bRet = Next(cb);
    }
}
//////////////////////////////////////////////////////////////////////////
bool NFCHeartBeatManager::Execute(const float fLastTime, const float fAllTime)
{
    NFCHeartBeatElement* pElement = mHeartBeatElementMapEx.First();
    while (pElement)
    {
        int nCount = pElement->nCount;
        float fTime = pElement->fTime;

        if (fTime > 0.000000f)
        {
            fTime -= fLastTime;
        }

        if (fTime <= 0.001f)
        {
            nCount--;
        }
        else
        {
            pElement->fTime = fTime;
        }

        if (nCount <= 0)
        {
            //Do Event
            pElement->DoHeartBeatEvent();

            //等待删除
            mRemoveListEx.Add(pElement->strBeatName);
        }
        else
        {
            if (pElement->nCount != nCount)
            {
                pElement->nCount = nCount;
                //Do Event
                pElement->DoHeartBeatEvent();
                pElement->fTime = pElement->fBeatTime;
            }
        }

        pElement = mHeartBeatElementMapEx.Next();
    }

    //删除所有过时心跳
    std::string strHeartBeatName;
    bool bRet = mRemoveListEx.First(strHeartBeatName);
    while (bRet)
    {
        NFCHeartBeatElement* pElement = mHeartBeatElementMapEx.RemoveElement(strHeartBeatName);
        if (pElement)
        {
            HEART_BEAT_FUNCTOR_PTR heartFunctorPtr;
            bool bRemoveRet = pElement->First(heartFunctorPtr);
            while (bRemoveRet)
            {
                heartFunctorPtr.reset();

                bRemoveRet = pElement->Next(heartFunctorPtr);
            }

            pElement->ClearAll();
            delete pElement;
            pElement = NULL;
        }

        bRet = mRemoveListEx.Next(strHeartBeatName);
    }
    mRemoveListEx.ClearAll();
    //////////////////////////////////////////////////////////////////////////
    //添加新心跳也是延时添加的
    for (std::list<NFCHeartBeatElement>::iterator iter = mAddListEx.begin(); iter != mAddListEx.end(); ++iter)
    {
        if (NULL == mHeartBeatElementMapEx.GetElement(iter->strBeatName))
        {
            NFCHeartBeatElement* pHeartBeatEx = NF_NEW NFCHeartBeatElement();
            *pHeartBeatEx = *iter;
            mHeartBeatElementMapEx.AddElement(pHeartBeatEx->strBeatName, pHeartBeatEx);
        }
    }
    mAddListEx.clear();

    return true;
}

bool NFCHeartBeatManager::RemoveHeartBeat(const std::string& strHeartBeatName)
{
    return mRemoveListEx.Add(strHeartBeatName);
}

NFIDENTID NFCHeartBeatManager::Self()
{
    return mSelf;
}

//////////////////////////////////////////////////////////////////////////
bool NFCHeartBeatManager::AddHeartBeat(const NFIDENTID self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIDataList& var, const float fTime, const int nCount)
{
    //NFCHeartBeatElement* pHeartBeat = NF_NEW NFCHeartBeatElement();
    //pHeartBeat->fTime = fTime;
    //pHeartBeat->fBeatTime = fTime;
    //pHeartBeat->nCount = nCount;
    //pHeartBeat->var = var;
    //pHeartBeat->self = self;
    //pHeartBeat->strBeatName = strHeartBeatName;
    //pHeartBeat->Add(cb);
    //mAddListEx.Add(pHeartBeat);

    NFCHeartBeatElement xHeartBeat;
    xHeartBeat.fTime = fTime;
    xHeartBeat.fBeatTime = fTime;
    xHeartBeat.nCount = nCount;
    xHeartBeat.var = var;
    xHeartBeat.self = self;
    xHeartBeat.strBeatName = strHeartBeatName;
    xHeartBeat.Add(cb);
    mAddListEx.push_back(xHeartBeat);

    return true;
}

bool NFCHeartBeatManager::Exist(const std::string& strHeartBeatName)
{
    if (mHeartBeatElementMapEx.GetElement(strHeartBeatName))
    {
        return true;
    }

    return false;
}
