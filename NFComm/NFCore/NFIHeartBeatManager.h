// -------------------------------------------------------------------------
//    @FileName         :    NFIHeartBeatManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFIHeartBeatManager
//
// -------------------------------------------------------------------------

#ifndef NFI_HEARTBEATMANAGER_H
#define NFI_HEARTBEATMANAGER_H

#include <string>
#include "NFMap.h"
#include "NFList.h"
#include "NFDefine.h"
#include "NFCDataList.h"

class  NFCHeartBeatElement
    : public NFList<HEART_BEAT_FUNCTOR_PTR>
{
public:
    bool operator==(const NFCHeartBeatElement& src)
    {
        if (strBeatName == src.strBeatName)
        {
            return true;
        }

        return false;
    }

    NFCHeartBeatElement()
    {
        fBeatTime = 0.0f;
        //fTime = 0.0f;
		nTime = 0;
        nCount = 0;
        strBeatName = "";
    };

    virtual ~NFCHeartBeatElement()
    {
    }

    void DoHeartBeatEvent();

    float fBeatTime;
	NFINT64 nTime;
    int nCount;
    std::string strBeatName;
    NFGUID self;
};

class NFIHeartBeatManager
{
public:
    virtual ~NFIHeartBeatManager() {}

    virtual NFGUID Self() = 0;

    virtual bool Execute() = 0;
    virtual bool Exist(const std::string& strHeartBeatName) = 0;

    virtual bool AddHeartBeat(const NFGUID self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const float fTime, const int nCount) = 0;
    virtual bool RemoveHeartBeat(const std::string& strHeartBeatName) = 0;

    template<typename BaseType>
    bool AddHeartBeat(const NFGUID self, const std::string& strHeartBeatName, BaseType* pBase, int (BaseType::*handler)(const NFGUID&, const std::string&, const float, const int), const float fTime, const int nCount)
    {
        HEART_BEAT_FUNCTOR functor = std::bind(handler, pBase, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
        HEART_BEAT_FUNCTOR_PTR functorPtr(NF_NEW HEART_BEAT_FUNCTOR(functor));
        return AddHeartBeat(self, strHeartBeatName, functorPtr, fTime, nCount);
    }
};

#endif