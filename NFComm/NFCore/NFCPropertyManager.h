// -------------------------------------------------------------------------
//    @FileName         :    NFCPropertyManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCPropertyManager
//
// -------------------------------------------------------------------------

#ifndef _NFC_PROPERTYMANAGER_H_
#define _NFC_PROPERTYMANAGER_H_

#include "NFDefine.h"
#include "NFMap.h"
#include "NFIDataList.h"
#include "NFIPropertyManager.h"

class NFCPropertyManager : public NFIPropertyManager
{
public:
    NFCPropertyManager(const NFIDENTID& self)
    {
        mSelf = self;
    };

    virtual ~NFCPropertyManager();

    virtual bool RegisterCallback(const std::string& strProperty, const PROPERTY_EVENT_FUNCTOR_PTR& cb, const NFIDataList& argVar);

    virtual NFIProperty* AddProperty(const NFIDENTID& self, NFIProperty* pProperty);

    virtual NFIProperty* AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex, const std::string& strScriptFunction);

    virtual bool SetProperty(const std::string& strPropertyName, const NFIDataList::TData& TData);

    virtual bool SetProperty(const NFIProperty* pProperty);

    virtual NFIDENTID Self();

    virtual const std::map<std::string, int>& GetPropertyIndex();

    virtual const int GetPropertyIndex(const std::string& strProperty);

private:
    NFIDENTID mSelf;
    std::map<std::string, int> mxPropertyIndexMap;
};


#endif
