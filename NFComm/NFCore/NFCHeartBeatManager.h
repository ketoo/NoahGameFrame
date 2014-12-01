// -------------------------------------------------------------------------
//    @FileName         :    NFCHeartBeatManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFCHeartBeatManager
//
// -------------------------------------------------------------------------

#ifndef _GT_HEARTBEATMANAGER_H_
#define _GT_HEARTBEATMANAGER_H_

#include "NFMap.h"
#include "NFList.h"
#include "NFDefine.h"
#include "NFIDataList.h"
#include "NFIHeartBeatManager.h"

class NFCHeartBeatManager : public NFIHeartBeatManager
{
public:
    NFCHeartBeatManager()
    {
        mSelf = 0;
        fLastTotal = 0.0f;
    }

    NFCHeartBeatManager(const NFIDENTID& self)
    {
        mSelf = self;
        fLastTotal = 0.0f;
    }

    virtual ~NFCHeartBeatManager();

    virtual NFIDENTID Self();

    virtual bool Execute(const float fLastTime, const float fAllTime);
    virtual bool Exist(const std::string& strHeartBeatName);
    virtual bool AddHeartBeat(const NFIDENTID self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIDataList& var, const float fTime, const int nCount);
    virtual bool RemoveHeartBeat(const std::string& strHeartBeatName);

protected:
    NFIDENTID mSelf;
    float fLastTotal;

    NFList<std::string> mRemoveListEx;
    //NFList<NFCHeartBeatElement*> mAddListEx;
    std::list<NFCHeartBeatElement> mAddListEx;
    NFMapEx<std::string, NFCHeartBeatElement> mHeartBeatElementMapEx;
};

#endif