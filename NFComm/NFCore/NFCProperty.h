// -------------------------------------------------------------------------
//    @FileName         :    NFCProperty.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCProperty
//
// -------------------------------------------------------------------------

#ifndef NFC_PROPERTY_H
#define NFC_PROPERTY_H

#include "NFMap.hpp"
#include "NFList.hpp"
#include "NFCDataList.h"
#include "NFIProperty.h"
#include "NFComm/NFPluginModule/NFPlatform.h"
class _NFExport NFCProperty : public NFIProperty
{
private:
    NFCProperty();

public:
    NFCProperty(const NFGUID& self, const std::string& strPropertyName, const TDATA_TYPE varType);

    virtual ~NFCProperty();

    virtual void SetValue(const NFIDataList::TData& TData);
    virtual void SetValue(const NFIProperty* pProperty);

    virtual bool SetInt(const NFINT64 value);
    virtual bool SetFloat(const double value);
    virtual bool SetString(const std::string& value);
    virtual bool SetObject(const NFGUID& value);
	virtual bool SetVector2(const NFVector2& value);
	virtual bool SetVector3(const NFVector3& value);

    virtual const TDATA_TYPE GetType() const;
    virtual const bool GeUsed() const;
    virtual const std::string& GetKey() const;
    virtual const bool GetSave() const;
    virtual const bool GetPublic() const;
    virtual const bool GetPrivate() const;
    virtual const bool GetCache() const;
    virtual const bool GetRef() const;
	virtual const bool GetUpload() const;

    virtual void SetSave(bool bSave);
    virtual void SetPublic(bool bPublic);
    virtual void SetPrivate(bool bPrivate);
    virtual void SetCache(bool bCache);
	virtual void SetRef(bool bRef);
	virtual void SetUpload(bool bUpload);

    virtual NFINT64 GetInt() const;
    virtual double GetFloat() const;
    virtual const std::string& GetString() const;
    virtual const NFGUID& GetObject() const;
	virtual const NFVector2& GetVector2() const;
	virtual const NFVector3& GetVector3() const;

    virtual const NFIDataList::TData& GetValue() const;
    virtual const NF_SHARE_PTR<NFList<std::string>> GetEmbeddedList() const;
    virtual const NF_SHARE_PTR<NFMapEx<std::string, std::string>> GetEmbeddedMap() const;

    virtual bool Changed() const;

    virtual std::string ToString();
    virtual bool FromString(const std::string& strData);
    virtual bool DeSerialization();

    virtual void RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb);

private:
    int OnEventHandler(const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

private:
    typedef std::vector<PROPERTY_EVENT_FUNCTOR_PTR> TPROPERTYCALLBACKEX;
    TPROPERTYCALLBACKEX mtPropertyCallback;

    NFGUID mSelf;
    std::string msPropertyName;
    TDATA_TYPE eType;

    NF_SHARE_PTR<NFIDataList::TData> mxData;
    NF_SHARE_PTR<NFMapEx<std::string, std::string>> mxEmbeddedMap;
    NF_SHARE_PTR<NFList<std::string>> mxEmbeddedList;

    bool mbPublic;
    bool mbPrivate;
    bool mbSave;
    bool mbCache;
    bool mbRef;
	bool mbUpload;
};

#endif
