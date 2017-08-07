// -------------------------------------------------------------------------
//    @FileName         :    NFCPropertyManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCPropertyManager
//
// -------------------------------------------------------------------------

#ifndef NFC_PROPERTYMANAGER_H
#define NFC_PROPERTYMANAGER_H

#include "NFMap.hpp"
#include "NFDataList.hpp"
#include "NFIPropertyManager.h"
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFCPropertyManager : public NFIPropertyManager
{
public:
    NFCPropertyManager(const NFGUID& self)
    {
        mSelf = self;
    };

    virtual ~NFCPropertyManager();

    virtual bool RegisterCallback(const std::string& strProperty, const PROPERTY_EVENT_FUNCTOR_PTR& cb);

    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const NFGUID& self, NF_SHARE_PTR<NFIProperty> pProperty);

    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const NFGUID& self, const std::string& strPropertyName, const NFDATA_TYPE varType);

    virtual const NFGUID& Self();

    virtual bool SetProperty(const std::string& strPropertyName, const NFData& TData);

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

private:
    NFGUID mSelf;
    std::map<std::string, int> mxPropertyIndexMap;
};


#endif
