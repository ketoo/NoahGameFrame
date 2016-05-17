// -------------------------------------------------------------------------
//    @FileName         :    NFCHeartBeatManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFCHeartBeatManager
//
// -------------------------------------------------------------------------

#include "NFCHeartBeatManager.h"
#include "NFTimer.h"

NFCHeartBeatManager::~NFCHeartBeatManager()
{
    mHeartBeatElementMapEx.ClearAll();
}

void NFCHeartBeatElement::DoHeartBeatEvent()
{
    HEART_BEAT_FUNCTOR_PTR cb;
    bool bRet = First(cb);
    while (bRet)
    {
        cb.get()->operator()(self, strBeatName, fBeatTime, nCount);

        bRet = Next(cb);
    }
}
//////////////////////////////////////////////////////////////////////////
bool NFCHeartBeatManager::Execute()
{
    NF_SHARE_PTR<NFCHeartBeatElement> pElement = mHeartBeatElementMapEx.First();
    while (pElement.get())
    {
        //millisecond
        NFINT64 nTime = NFTimeEx::GetNowTimeMille();

        if (nTime > pElement->nNextTriggerTime && pElement->nCount > 0)
        {
            pElement->nCount--;

            pElement->DoHeartBeatEvent();

            if (pElement->nCount <= 0)
            {
                //等待删除
                mRemoveListEx.Add(pElement->strBeatName);
            }
            else
            {
                //Do Event
                pElement->nNextTriggerTime = nTime + NFINT64(pElement->fBeatTime * 1000);
            }
        }

        pElement = mHeartBeatElementMapEx.Next();
    }

    //删除所有过时心跳
    std::string strHeartBeatName;
    bool bRet = mRemoveListEx.First(strHeartBeatName);
    while (bRet)
    {
        mHeartBeatElementMapEx.RemoveElement(strHeartBeatName);

        bRet = mRemoveListEx.Next(strHeartBeatName);
    }

    mRemoveListEx.ClearAll();

    //////////////////////////////////////////////////////////////////////////
    //添加新心跳也是延时添加的
    for (std::list<NFCHeartBeatElement>::iterator iter = mAddListEx.begin(); iter != mAddListEx.end(); ++iter)
    {
        if (NULL == mHeartBeatElementMapEx.GetElement(iter->strBeatName))
        {
            NF_SHARE_PTR<NFCHeartBeatElement> pHeartBeatEx(NF_NEW NFCHeartBeatElement());
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

NFGUID NFCHeartBeatManager::Self()
{
    return mSelf;
}

//////////////////////////////////////////////////////////////////////////
bool NFCHeartBeatManager::AddHeartBeat(const NFGUID self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount)
{
    NFCHeartBeatElement xHeartBeat;
    xHeartBeat.nNextTriggerTime = NFTimeEx::GetNowTimeMille() + (NFINT64)(fTime * 1000);
    xHeartBeat.fBeatTime = fTime;
    xHeartBeat.nCount = nCount;
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
