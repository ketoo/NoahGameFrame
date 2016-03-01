// -------------------------------------------------------------------------
//    @FileName         :    NFCPropertyManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCPropertyManager
//
// -------------------------------------------------------------------------

#ifndef NFC_PROPERTYMANAGER_H
#define NFC_PROPERTYMANAGER_H

#include "NFDefine.h"
#include "NFMap.h"
#include "NFIDataList.h"
#include "NFIPropertyManager.h"

class NFCPropertyManager : public NFIPropertyManager
{
public:
    NFCPropertyManager(const NFGUID& self)
    {
        mSelf = self;
    };

    virtual ~NFCPropertyManager();

    virtual bool RegisterCallback(const std::string& strProperty, const PROPERTY_EVENT_FUNCTOR_PTR& cb);

    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const NFGUID& self, NF_SHARE_PTR<NFIProperty> pProperty);

    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const NFGUID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex, const std::string& strScriptFunction);

    virtual bool SetProperty(const std::string& strPropertyName, const NFIDataList::TData& TData);

    //virtual bool SetProperty(const NFIProperty* pProperty);

    virtual const NFGUID& Self();

    virtual const std::map<std::string, int>& GetPropertyIndex();

    virtual const int GetPropertyIndex(const std::string& strProperty);

    virtual bool SetPropertyInt(const std::string& strPropertyName, const NFINT64 nValue);
    virtual bool SetPropertyFloat(const std::string& strPropertyName, const double dwValue);
    virtual bool SetPropertyString(const std::string& strPropertyName, const std::string& strValue);
    virtual bool SetPropertyObject(const std::string& strPropertyName, const NFGUID& obj);

    virtual NFINT64 GetPropertyInt(const std::string& strPropertyName);
    virtual double GetPropertyFloat(const std::string& strPropertyName);
    virtual const std::string& GetPropertyString(const std::string& strPropertyName);
    virtual const NFGUID& GetPropertyObject(const std::string& strPropertyName);


private:
    NFGUID mSelf;
    std::map<std::string, int> mxPropertyIndexMap;
};


#endif
