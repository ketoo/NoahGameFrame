// -------------------------------------------------------------------------
//    @FileName         :    NFCProperty.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCProperty
//
// -------------------------------------------------------------------------

#ifndef NFC_PROPERTY_H
#define NFC_PROPERTY_H

#include "NFDefine.h"
#include "NFMap.h"
#include "NFList.h"
#include "NFCDataList.h"
#include "NFIProperty.h"

class NFCProperty : public NFIProperty
{
public:
    NFCProperty();

    NFCProperty(const NFGUID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, const std::string& strRelativeValue);

    virtual ~NFCProperty();

    virtual void SetValue(const NFIDataList::TData& TData);
    virtual void SetValue(const NFIProperty* pProperty);

    virtual bool SetInt(const NFINT64 value);
    virtual bool SetFloat(const double value);
    virtual bool SetString(const std::string& value);
    virtual bool SetObject(const NFGUID& value);

    virtual const TDATA_TYPE GetType() const;
    virtual const bool GeUsed() const;
    virtual const std::string& GetKey() const;
    virtual const bool GetSave() const;
    virtual const bool GetPublic() const;
    virtual const bool GetPrivate() const;
    virtual const std::string& GetRelationValue() const;

    virtual void SetSave(bool bSave);
    virtual void SetPublic(bool bPublic);
    virtual void SetPrivate(bool bPrivate);
    virtual void SetRelationValue(const std::string& strRelation);

    virtual NFINT64 GetInt() const;
    virtual double GetFloat() const;
    virtual const std::string& GetString() const;
    virtual const NFGUID& GetObject() const;

    virtual bool Changed() const;

    virtual void RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb);

    virtual const NFIDataList::TData& GetValue() const;

    int OnEventHandler(const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

    virtual std::string ToString();
    virtual bool FromString(const std::string& strData);
private:
    typedef std::vector<PROPERTY_EVENT_FUNCTOR_PTR> TPROPERTYCALLBACKEX;
    TPROPERTYCALLBACKEX mtPropertyCallback;

    NFGUID mSelf;
    std::string msPropertyName;//可以想办法与基本类型共用
    std::string mstrRelationValue;
    TDATA_TYPE eType;//只有在不存在指针的时候才用这个判断类型--为节约内存

    NF_SHARE_PTR<NFIDataList::TData> mxData;
    NF_SHARE_PTR<NFMap<std::string, std::string>> mxEmbeddedMap;
    NF_SHARE_PTR<NFList<std::string>> mxEmbeddedList;

    bool mbPublic;
    bool mbPrivate;
    bool mbSave;
};

#endif
