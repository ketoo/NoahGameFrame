// -------------------------------------------------------------------------
//    @FileName         :    NFIObject.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFIObject
//
// -------------------------------------------------------------------------

#ifndef _NFI_OBJECT_H_
#define _NFI_OBJECT_H_


#include "NFPlatform.h"
#include "NFIValueList.h"
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
    bool AddHeartBeat(const std::string& strHeartBeatName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const NFIValueList&), const NFIValueList& var, const float fTime, const int nCount)
    {
        HEART_BEAT_FUNCTOR functor = boost::bind(handler, pBase, _1, _2);
        HEART_BEAT_FUNCTOR_PTR functorPtr(new HEART_BEAT_FUNCTOR(functor));
        return AddHeartBeat(strHeartBeatName, functorPtr, var, fTime, nCount);
    }

    virtual bool FindHeartBeat(const std::string& strHeartBeatName) = 0;

    virtual bool RemoveHeartBeat(const std::string& strHeartBeatName) = 0;

    template<typename BaseType>
    bool AddRecordCallBack(const std::string& strRecordName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const std::string&, const int, const int, const int, const NFIValueList&, const NFIValueList&, const NFIValueList&))
    {
        RECORD_EVENT_FUNCTOR functor = boost::bind(handler, pBase, _1, _2, _3, _4, _5, _6, _7, _8);
        RECORD_EVENT_FUNCTOR_PTR functorPtr(new RECORD_EVENT_FUNCTOR(functor));
        return AddRecordCallBack(strRecordName, functorPtr);
    }

    template<typename BaseType>
    bool AddPropertyCallBack(const std::string& strPropertyName, BaseType* pBase, int (BaseType::*handler)(const NFIDENTID&, const std::string&, const NFIValueList&, const NFIValueList&, const NFIValueList&))
    {
        PROPERTY_EVENT_FUNCTOR functor = boost::bind(handler, pBase, _1, _2, _3, _4, _5);
        PROPERTY_EVENT_FUNCTOR_PTR functorPtr(new PROPERTY_EVENT_FUNCTOR(functor));
        return AddPropertyCallBack(strPropertyName, functorPtr);
    }

    /////////////////////////////////////////////////////////////////

    virtual bool LinkObject(const NFIDENTID& childObject, const NFIDENTID& targetObject) = 0;

    virtual bool UnLinkObject(const NFIDENTID& childObject, const NFIDENTID& targetObject) = 0;

    virtual bool FindProperty(const std::string& strPropertyName) = 0;

    virtual bool SetPropertyInt(const std::string& strPropertyName, const int nValue) = 0;
    virtual bool SetPropertyFloat(const std::string& strPropertyName,  const float fValue) = 0;
    virtual bool SetPropertyDouble(const std::string& strPropertyName, const double dwValue) = 0;
    virtual bool SetPropertyString(const std::string& strPropertyName, const std::string& strValue) = 0;
    virtual bool SetPropertyObject(const std::string& strPropertyName, const NFIDENTID& obj) = 0;

    virtual int QueryPropertyInt(const std::string& strPropertyName) = 0;
    virtual float QueryPropertyFloat(const std::string& strPropertyName) = 0;
    virtual double QueryPropertyDouble(const std::string& strPropertyName) = 0;
    virtual const std::string& QueryPropertyString(const std::string& strPropertyName) = 0;
    virtual NFIDENTID QueryPropertyObject(const std::string& strPropertyName) = 0;

    virtual bool FindRecord(const std::string& strRecordName) = 0;

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const int nValue) = 0;
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol,  const float fValue) = 0;
    virtual bool SetRecordDouble(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue) = 0;
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue) = 0;
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj) = 0;

    virtual bool SetRecordInt(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol, const int nValue) = 0;
    virtual bool SetRecordFloat(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol,  const float fValue) = 0;
    virtual bool SetRecordDouble(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue) = 0;
    virtual bool SetRecordString(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue) = 0;
    virtual bool SetRecordObject(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj) = 0;

    virtual int QueryRecordInt(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual float QueryRecordFloat(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual double QueryRecordDouble(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual const std::string& QueryRecordString(const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual NFIDENTID QueryRecordObject(const std::string& strRecordName, const int nRow, const int nCol) = 0;

    virtual int QueryRecordInt(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol) = 0;
    virtual float QueryRecordFloat(NFIRecord* pRecord, const std::string& strPropertyName, const int nRow, const int nCol) = 0;
    virtual double QueryRecordDouble(NFIRecord* pRecord, const std::string& strPropertyName, const int nRow, const int nCol) = 0;
    virtual const std::string& QueryRecordString(NFIRecord* pRecord, const std::string& strPropertyName, const int nRow, const int nCol) = 0;
    virtual NFIDENTID QueryRecordObject(NFIRecord* pRecord, const std::string& strPropertyName, const int nRow, const int nCol) = 0;

    virtual NFIRecordManager* GetRecordManager() = 0;
    virtual NFIHeartBeatManager* GetHeartBeatManager() = 0;
    virtual NFIPropertyManager* GetPropertyManager() = 0;
    virtual NFIComponentManager* GetComponentManager() = 0;
    
protected:
    virtual bool AddRecordCallBack(const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual bool AddPropertyCallBack(const std::string& strPropertyName, const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;
    
    virtual bool AddHeartBeat(const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIValueList& var, const float fTime, const int nCount) = 0;

private:
};

#endif