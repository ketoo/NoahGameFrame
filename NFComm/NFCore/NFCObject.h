// -------------------------------------------------------------------------
//    @FileName         :    NFCObject.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFCObject
//
// -------------------------------------------------------------------------

#ifndef NFC_OBJECT_H
#define NFC_OBJECT_H

#include <iostream>
#include <string>
#include "NFComm/NFCore/NFIObject.h"
#include "NFComm/NFCore/NFIRecordManager.h"
#include "NFComm/NFCore/NFIPropertyManager.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFCObject
    : public NFIObject
{
private:
    NFCObject() : NFIObject(NFGUID())
    {
		mObjectEventState = COE_CREATE_NODATA;
    }

public:
    NFCObject(NFGUID self, NFIPluginManager* pLuginManager);
    virtual ~NFCObject();

    virtual bool Init();
    virtual bool Shut();
    virtual bool Execute();

    ///////////////////////////////////////////////////////////////////////

    virtual NFGUID Self();

    /////////////////////////////////////////////////////////////////

	virtual CLASS_OBJECT_EVENT GetState();
	virtual bool SetState(const CLASS_OBJECT_EVENT eState);

    virtual bool FindProperty(const std::string& strPropertyName);

    virtual bool SetPropertyInt(const std::string& strPropertyName, const NFINT64 nValue);
    virtual bool SetPropertyFloat(const std::string& strPropertyName, const double dwValue);
    virtual bool SetPropertyString(const std::string& strPropertyName, const std::string& strValue);
    virtual bool SetPropertyObject(const std::string& strPropertyName, const NFGUID& obj);
	virtual bool SetPropertyVector2(const std::string& strPropertyName, const NFVector2& value);
	virtual bool SetPropertyVector3(const std::string& strPropertyName, const NFVector3& value);

    virtual NFINT64 GetPropertyInt(const std::string& strPropertyName);
	virtual int GetPropertyInt32(const std::string& strPropertyName);
    virtual double GetPropertyFloat(const std::string& strPropertyName);
    virtual const std::string& GetPropertyString(const std::string& strPropertyName);
    virtual const NFGUID& GetPropertyObject(const std::string& strPropertyName);
	virtual const NFVector2& GetPropertyVector2(const std::string& strPropertyName);
	virtual const NFVector3& GetPropertyVector3(const std::string& strPropertyName);

    virtual bool FindRecord(const std::string& strRecordName);

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue);
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& obj);
	virtual bool SetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol, const NFVector2& value);
	virtual bool SetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol, const NFVector3& value);

    virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value);
    virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value);
    virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value);
    virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value);
	virtual bool SetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector2& value);
	virtual bool SetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector3& value);


    virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol);
    virtual double GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol);
    virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const int nCol);
    virtual const NFGUID& GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol);
	virtual const NFVector2& GetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol);
	virtual const NFVector3& GetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol);

    virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual double GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag);
    virtual const NFGUID& GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const NFVector2& GetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const NFVector3& GetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag);

    virtual NF_SHARE_PTR<NFIRecordManager> GetRecordManager();
    virtual NF_SHARE_PTR<NFIPropertyManager> GetPropertyManager();

protected:
    virtual bool AddRecordCallBack(const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb);

    virtual bool AddPropertyCallBack(const std::string& strCriticalName, const PROPERTY_EVENT_FUNCTOR_PTR& cb);

private:
    NFGUID mSelf;
	CLASS_OBJECT_EVENT mObjectEventState;
    NF_SHARE_PTR<NFIRecordManager> m_pRecordManager;
    NF_SHARE_PTR<NFIPropertyManager> m_pPropertyManager;

};

#endif