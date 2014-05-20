// -------------------------------------------------------------------------
//    @FileName         :    NFIHeartBeatManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFIHeartBeatManager
//
// -------------------------------------------------------------------------

#ifndef _NFI_HEARTBEATMANAGER_H_
#define _NFI_HEARTBEATMANAGER_H_

#include <string>
#include "NFMap.h"
#include "NFList.h"
#include "NFDefine.h"
#include "NFCValueList.h"

class  NFCHeartBeatElementEx
    : public NFList<HEART_BEAT_FUNCTOR_PTR>
{
public:
    bool operator==(const NFCHeartBeatElementEx& src)
    {
        if (strBeatName == src.strBeatName)
        {
            return true;
        }

        return false;
    }

    NFCHeartBeatElementEx()
    {
        fBeatTime = 0.0f;
        fTime = 0.0f;
        nCount = 0;
        strBeatName = "";
        self = 0;
    };

    ~NFCHeartBeatElementEx() {};
    void DoHeartBeatEvent();

    float fBeatTime;

    float fTime;
    int nCount;
    std::string strBeatName;
    NFIDENTID self;
    NFCValueList var;
};

class NFIHeartBeatManager
{
public:
    virtual ~NFIHeartBeatManager() {}
	
	virtual NFIDENTID Self() = 0;

    virtual bool Execute(const float fLastTime, const float fAllTime) = 0;
    virtual bool Exist(const std::string& strHeartBeatName) = 0;
    
    virtual bool AddHeartBeat(const NFIDENTID self, const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIValueList& var, const float fTime, const int nCount) = 0;
    virtual bool RemoveHeartBeat(const std::string& strHeartBeatName) = 0;

    template<typename BaseType>
    bool AddHeartBeat(const NFIDENTID self, const std::string& strHeartBeatName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const NFIValueList&), const NFIValueList& var, const float fTime, const int nCount)
    {
        HEART_BEAT_FUNCTOR functor = boost::bind(handler, pBase, _1, _2);
        HEART_BEAT_FUNCTOR_PTR functorPtr(new HEART_BEAT_FUNCTOR(functor));
        return AddHeartBeat(self, strHeartBeatName, functorPtr, var, fTime, nCount);
    }
};

#endif