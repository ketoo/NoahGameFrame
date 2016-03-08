// -------------------------------------------------------------------------
//    @FileName         :    NFCPropertyManager.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCPropertyManager
//
// -------------------------------------------------------------------------

#include "NFCProperty.h"
#include "NFCPropertyManager.h"

NFCPropertyManager::~NFCPropertyManager()
{
    ClearAll();
}

bool NFCPropertyManager::RegisterCallback(const std::string& strProperty, const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFIProperty> pProperty = this->GetElement(strProperty);
    if (pProperty.get())
    {
        pProperty->RegisterCallback(cb);
        return true;
    }

    return false;
}

NF_SHARE_PTR<NFIProperty> NFCPropertyManager::AddProperty(const NFGUID& self, NF_SHARE_PTR<NFIProperty> pProperty)
{
    const std::string& strProperty = pProperty->GetKey();
    NF_SHARE_PTR<NFIProperty> pOldProperty = this->GetElement(strProperty);
    if (!pOldProperty.get())
    {
        NF_SHARE_PTR<NFIProperty> pNewProperty(NF_NEW NFCProperty(self, strProperty, pProperty->GetType(), pProperty->GetPublic(), pProperty->GetPrivate(), pProperty->GetSave(), pProperty->GetView(), pProperty->GetIndex(), pProperty->GetRelationValue()));
        this->AddElement(strProperty, pNewProperty);

        if (pProperty->GetIndex() > 0)
        {
            mxPropertyIndexMap.insert(std::map<std::string, int>::value_type(strProperty, pProperty->GetIndex()));
        }
    }

    return pOldProperty;
}

NF_SHARE_PTR<NFIProperty> NFCPropertyManager::AddProperty(const NFGUID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex, const std::string& strScriptFunction)
{
    NF_SHARE_PTR<NFIProperty> pProperty = this->GetElement(strPropertyName);
    if (!pProperty.get())
    {
        pProperty = NF_SHARE_PTR<NFIProperty>(NF_NEW NFCProperty(self, strPropertyName, varType, bPublic, bPrivate, bSave, bView, nIndex, strScriptFunction));
        this->AddElement(strPropertyName, pProperty);

        if (pProperty->GetIndex() > 0)
        {
            mxPropertyIndexMap.insert(std::map<std::string, int>::value_type(strPropertyName, nIndex));
        }
    }

    return pProperty;
}

bool NFCPropertyManager::SetProperty(const std::string& strPropertyName, const NFIDataList::TData& TData)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetElement(strPropertyName);
    if (pProperty.get())
    {
        pProperty->SetValue(TData);

        return true;
    }

    return false;
}

// bool NFCPropertyManager::SetProperty(const NFIProperty* pProperty)
// {
//     NF_SHARE_PTR<NFIProperty> pSelfProperty = GetElement(pProperty->GetKey());
//     if (pSelfProperty.get())
//     {
//         pSelfProperty->SetValue(pProperty);
//
//         return true;
//     }
//
//     return false;
// }

const NFGUID& NFCPropertyManager::Self()
{
    return mSelf;
}

const std::map<std::string, int>& NFCPropertyManager::GetPropertyIndex()
{
    return mxPropertyIndexMap;
}

const int NFCPropertyManager::GetPropertyIndex(const std::string& strProperty)
{
    std::map<std::string, int>::iterator it = mxPropertyIndexMap.find(strProperty);
    if (it != mxPropertyIndexMap.end())
    {
        return it->second;
    }

    return 0;
}

bool NFCPropertyManager::SetPropertyInt(const std::string& strPropertyName, const NFINT64 nValue)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetInt(nValue);
    }

    return false;
}

bool NFCPropertyManager::SetPropertyFloat(const std::string& strPropertyName, const double dwValue)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetFloat(dwValue);
    }

    return false;
}

bool NFCPropertyManager::SetPropertyString(const std::string& strPropertyName, const std::string& strValue)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetString(strValue);
    }

    return false;
}

bool NFCPropertyManager::SetPropertyObject(const std::string& strPropertyName, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->SetObject(obj);
    }

    return false;
}

NFINT64 NFCPropertyManager::GetPropertyInt(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetInt();
    }

    return 0;
}

double NFCPropertyManager::GetPropertyFloat(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetFloat();
    }

    return 0.0;
}

const std::string& NFCPropertyManager::GetPropertyString(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetString();
    }

    return NULL_STR;
}

const NFGUID& NFCPropertyManager::GetPropertyObject(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetElement(strPropertyName);
    if (pProperty.get())
    {
        return pProperty->GetObject();
    }

    return NULL_OBJECT;
}

