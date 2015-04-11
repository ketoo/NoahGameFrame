// -------------------------------------------------------------------------
//    @FileName         :    NFIObject.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFIObject
//
// -------------------------------------------------------------------------

#ifndef _NFI_OBJECT_H_
#define _NFI_OBJECT_H_


#include "NFComm/NFPluginModule/NFPlatform.h"
#include "NFIDataList.h"
#include "NFIRecord.h"
#include "NFIRecordManager.h"
#include "NFIHeartBeatManager.h"
#include "NFIPropertyManager.h"
#include "NFIComponentManager.h"

class NFIObject
{
public:
    virtual ~NFIObject() {}
    virtual bool Init() = 0;
    virtual bool Shut() = 0;

    virtual bool Execute(const float fLastTime, const float fAllTime) = 0;

    virtual NFIDENTID Self() = 0;

    template<typename BaseType>
    bool AddPropertyCallBack(const std::string& strPropertyName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const std::string&, const NFIDataList&, const NFIDataList&, const NFIDataList&))
    {
        PROPERTY_EVENT_FUNCTOR functor = boost::bind(handler, pBase, _1, _2, _3, _4, _5);
        PROPERTY_EVENT_FUNCTOR_PTR functorPtr(NF_NEW PROPERTY_EVENT_FUNCTOR(functor));
        return AddPropertyCallBack(strPropertyName, functorPtr);
    }

    template<typename BaseType>
    bool AddRecordCallBack(const std::string& strRecordName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const std::string&, const int, const int, const int, const NFIDataList&, const NFIDataList&, const NFIDataList&))
    {
        RECORD_EVENT_FUNCTOR functor = boost::bind(handler, pBase, _1, _2, _3, _4, _5, _6, _7, _8);
        RECORD_EVENT_FUNCTOR_PTR functorPtr(NF_NEW RECORD_EVENT_FUNCTOR(functor));
        return AddRecordCallBack(strRecordName, functorPtr);
    }

    template<typename BaseType>
    bool AddHeartBeat(const std::string& strHeartBeatName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const std::string&, const float, const int, const NFIDataList&), const NFIDataList& var, const float fTime, const int nCount)
    {
        HEART_BEAT_FUNCTOR functor = boost::bind(handler, pBase, _1, _2, _3, _4, _5);
        HEART_BEAT_FUNCTOR_PTR functorPtr(NF_NEW HEART_BEAT_FUNCTOR(functor));
        return AddHeartBeat(strHeartBeatName, functorPtr, var, fTime, nCount);
    }

    virtual bool AddRecordCallBack(const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual bool AddPropertyCallBack(const std::string& strPropertyName, const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual bool AddHeartBeat(const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIDataList& var, const float fTime, const int nCount) = 0;

    virtual bool FindHeartBeat(const std::string& strHeartBeatName) = 0;

    virtual bool RemoveHeartBeat(const std::string& strHeartBeatName) = 0;

    /////////////////////////////////////////////////////////////////

    virtual bool FindProperty(const std::string& strPropertyName) = 0;

    virtual bool SetPropertyInt(const std::string& strPropertyName, const NFINT64 nValue) = 0;
    virtual bool SetPropertyFloat(const std::string& strPropertyName,  const float fValue) = 0;
    virtual bool SetPropertyDouble(const std::string& strPropertyName, const double dwValue) = 0;
    virtual bool SetPropertyString(const std::string& strPropertyName, const std::string& strValue) = 0;
    virtual bool SetPropertyObject(const std::string& strPropertyName, const NFIDENTID& obj) = 0;

    virtual NFINT64 GetPropertyInt(const std::string& strPropertyName) = 0;
    virtual float GetPropertyFloat(const std::string& strPropertyName) = 0;
    virtual double GetPropertyDouble(const std::string& strPropertyName) = 0;
    virtual const std::string& GetPropertyString(const std::string& strPropertyName) = 0;
    virtual NFIDENTID GetPropertyObject(const std::string& strPropertyName) = 0;

    virtual bool FindRecord(const std::string& strRecordName) = 0;

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue) = 0;
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol,  const float fValue) = 0;
    virtual bool SetRecordDouble(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue) = 0;
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue) = 0;
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj) = 0;

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value) = 0;
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const float value) = 0;
    virtual bool SetRecordDouble(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value) = 0;
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value) = 0;
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFIDENTID& value) = 0;

    virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual float GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual double GetRecordDouble(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual NFIDENTID GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol) = 0;

    virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual float GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual double GetRecordDouble(const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;
    virtual NFIDENTID GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag) = 0;

    virtual NF_SHARE_PTR<NFIComponent> AddComponent(const std::string& strComponentName, const std::string& strLanguageName) = 0;
    virtual NF_SHARE_PTR<NFIComponent> FindComponent(const std::string& strComponentName) = 0;

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager() = 0;
    virtual NF_SHARE_PTR<NFIHeartBeatManager> GetHeartBeatManager() = 0;
    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager() = 0;
    virtual NF_SHARE_PTR<NFIComponentManager> GetComponentManager() = 0;



private:
};

#endif