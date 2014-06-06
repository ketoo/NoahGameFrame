// -------------------------------------------------------------------------
//    @FileName         :    NFCObject.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFCObject
//
// -------------------------------------------------------------------------

#ifndef _NFC_OBJECT_H_
#define _NFC_OBJECT_H_

#include <iostream>
#include <string>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFCore/NFIRecordManager.h"
#include "NFComm/NFCore/NFIHeartBeatManager.h"
#include "NFComm/NFCore/NFIPropertyManager.h"

class NFCObject
    : public NFIObject
{
public:
    NFCObject(const NFIDENTID& self, NFIPluginManager* pLuginManager);
    virtual ~NFCObject();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute(const float fLastTime, const float fAllTime);

    ///////////////////////////////////////////////////////////////////////

    virtual NFIDENTID Self();

    virtual bool FindHeartBeat(const std::string& strHeartBeatName);

    virtual bool RemoveHeartBeat(const std::string& strHeartBeatName);

    /////////////////////////////////////////////////////////////////

    virtual bool FindProperty(const std::string& strPropertyName);

    virtual bool SetPropertyInt(const std::string& strPropertyName, const int nValue);
    virtual bool SetPropertyFloat(const std::string& strPropertyName,  const float fValue);
    virtual bool SetPropertyDouble(const std::string& strPropertyName, const double dwValue);
    virtual bool SetPropertyString(const std::string& strPropertyName, const std::string& strValue);
    virtual bool SetPropertyObject(const std::string& strPropertyName, const NFIDENTID& obj);

    virtual int QueryPropertyInt(const std::string& strPropertyName);
    virtual float QueryPropertyFloat(const std::string& strPropertyName);
    virtual double QueryPropertyDouble(const std::string& strPropertyName);
    virtual const std::string& QueryPropertyString(const std::string& strPropertyName);
    virtual NFIDENTID QueryPropertyObject(const std::string& strPropertyName);

    virtual bool FindRecord(const std::string& strRecordName);

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const int nValue);
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol,  const float fValue);
    virtual bool SetRecordDouble(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj);

    virtual bool SetRecordInt(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol, const int nValue);
    virtual bool SetRecordFloat(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol,  const float fValue);
    virtual bool SetRecordDouble(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
    virtual bool SetRecordString(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
    virtual bool SetRecordObject(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol, const NFIDENTID& obj);

    virtual int QueryRecordInt(const std::string& strRecordName, const int nRow, const int nCol);
    virtual float QueryRecordFloat(const std::string& strRecordName, const int nRow, const int nCol);
    virtual double QueryRecordDouble(const std::string& strRecordName, const int nRow, const int nCol);
    virtual const std::string& QueryRecordString(const std::string& strRecordName, const int nRow, const int nCol);
    virtual NFIDENTID QueryRecordObject(const std::string& strRecordName, const int nRow, const int nCol);

    virtual int QueryRecordInt(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol);
    virtual float QueryRecordFloat(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol);
    virtual double QueryRecordDouble(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol);
    virtual const std::string& QueryRecordString(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol);
    virtual NFIDENTID QueryRecordObject(NFIRecord* pRecord, const std::string& strRecordName, const int nRow, const int nCol);

    virtual NFIComponent* AddComponent(const std::string& strComponentName, const std::string& strLanguageName);
    virtual NFIComponent* FindComponent(const std::string& strComponentName);

    virtual NFIRecordManager* GetRecordManager();
    virtual NFIHeartBeatManager* GetHeartBeatManager();
    virtual NFIPropertyManager* GetPropertyManager();
    virtual NFIComponentManager* GetComponentManager();

protected:
    virtual bool AddRecordCallBack(const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb);

    virtual bool AddPropertyCallBack(const std::string& strCriticalName, const PROPERTY_EVENT_FUNCTOR_PTR& cb);

    virtual bool AddHeartBeat(const std::string& strHeartBeatName, const HEART_BEAT_FUNCTOR_PTR& cb, const NFIValueList& var, const float fTime, const int nCount);

private:
    NFIDENTID mSelf;

    NFIRecordManager* m_pRecordManager;
    NFIHeartBeatManager* m_pHeartBeatManager;
    NFIPropertyManager* m_pPropertyManager;
    NFIComponentManager* m_pComponentManager;

    NFIPluginManager* m_pPluginManager;
};

#endif