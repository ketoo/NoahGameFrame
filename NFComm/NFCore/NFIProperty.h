// -------------------------------------------------------------------------
//    @FileName         :    NFIProperty.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFIProperty
//
// -------------------------------------------------------------------------

#ifndef NFI_PROPERTY_H
#define NFI_PROPERTY_H

#include "NFDefine.h"
#include "NFIDataList.h"
#include "NFList.h"

class NFIProperty
{
public:
    virtual ~NFIProperty() {}

    virtual void SetValue(const NFIDataList::TData& TData) = 0;
    virtual void SetValue(const NFIProperty* pProperty) = 0;
    virtual bool SetInt(const NFINT64 value) = 0;
    virtual bool SetFloat(const double value) = 0;
    virtual bool SetString(const std::string& value) = 0;
    virtual bool SetObject(const NFGUID& value) = 0;

    virtual const TDATA_TYPE GetType() const = 0;
    virtual const bool GeUsed() const = 0;
    virtual const std::string& GetKey() const = 0;
    virtual const bool GetSave() const = 0;
    virtual const bool GetPublic() const = 0;
    virtual const bool GetPrivate() const = 0;
    virtual const std::string& GetRelationValue() const = 0;

    virtual void SetSave(bool bSave) = 0;
    virtual void SetPublic(bool bPublic) = 0;
    virtual void SetPrivate(bool bPrivate) = 0;
    virtual void SetRelationValue(const std::string& strRelation) = 0;

    virtual NFINT64 GetInt() const = 0;
    virtual double GetFloat() const = 0;
    virtual const std::string& GetString() const = 0;
    virtual const NFGUID& GetObject() const = 0;

    virtual const NFIDataList::TData& GetValue() const = 0;
    virtual const NF_SHARE_PTR<NFList<std::string>> GetEmbeddedList() const = 0;
    virtual const NF_SHARE_PTR<NFMapEx<std::string, std::string>> GetEmbeddedMap() const = 0;

    virtual bool Changed() const = 0;

    virtual std::string ToString() = 0;
    virtual bool FromString(const std::string& strData) = 0;
    virtual bool DeSerialization() = 0;

    virtual void RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb) = 0;
};

#endif
