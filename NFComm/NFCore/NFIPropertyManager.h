// -------------------------------------------------------------------------
//    @FileName         :    NFIPropertyManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFIPropertyManager
//
// -------------------------------------------------------------------------

#ifndef _NFI_PROPERTYMANAGER_H_
#define _NFI_PROPERTYMANAGER_H_

#include "NFMap.h"
#include "NFIProperty.h"
#include <map>

class NFIPropertyManager
    : public NFMapEx<std::string, NFIProperty>
{
public:
    virtual ~NFIPropertyManager() {}

    virtual bool RegisterCallback(const std::string& strProperty, const PROPERTY_EVENT_FUNCTOR_PTR& cb, const NFIDataList& argVar) = 0;

    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const NFIDENTID& self, NF_SHARE_PTR<NFIProperty> pProperty) = 0;
    virtual NF_SHARE_PTR<NFIProperty> AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex, const std::string& strScriptFunction) = 0;

    virtual bool SetProperty(const std::string& strPropertyName, const NFIDataList::TData& TData) = 0;
    //virtual bool SetProperty(const NFIProperty* pProperty) = 0;

    virtual NFIDENTID Self() = 0;

    virtual const std::map<std::string, int>& GetPropertyIndex() = 0;
    virtual const int GetPropertyIndex(const std::string& strProperty) = 0;

private:
};

#endif
