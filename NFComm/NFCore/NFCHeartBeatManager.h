// -------------------------------------------------------------------------
//    @FileName         :    NFCHeartBeatManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFCHeartBeatManager
//
// -------------------------------------------------------------------------

#ifndef _GT_HEARTBEATMANAGER_H
#define _GT_HEARTBEATMANAGER_H

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
        mSelf = NFGUID();
    }

    NFCHeartBeatManager(const NFGUID& self)
    {
        mSelf = self;
    }

    virtual ~NFCHeartBeatManager();

    virtual NFGUID Self();

    virtual bool Execute();
    virtual bool Exist(const std::string& strHeartBeatName);
    virtual bool AddHeartBeat(const NFGUID self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount);
    virtual bool RemoveHeartBeat(const std::string& strHeartBeatName);

protected:
    NFGUID mSelf;

    NFList<std::string> mRemoveListEx;
    std::list<NFCHeartBeatElement> mAddListEx;
    NFMapEx<std::string, NFCHeartBeatElement> mHeartBeatElementMapEx;
};

#endif