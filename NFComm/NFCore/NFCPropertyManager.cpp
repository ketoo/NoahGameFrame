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

bool NFCPropertyManager::RegisterCallback(const std::string& strProperty, const PROPERTY_EVENT_FUNCTOR_PTR& cb, const NFIDataList& argVar)
{
    NF_SHARE_PTR<NFIProperty> pProperty = this->GetElement(strProperty);
    if (pProperty.get())
    {
        pProperty->RegisterCallback(cb, argVar);
        return true;
    }

    return false;
}

NF_SHARE_PTR<NFIProperty> NFCPropertyManager::AddProperty(const NFIDENTID& self, NF_SHARE_PTR<NFIProperty> pProperty)
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

NF_SHARE_PTR<NFIProperty> NFCPropertyManager::AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex, const std::string& strScriptFunction)
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

NFIDENTID NFCPropertyManager::Self()
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

