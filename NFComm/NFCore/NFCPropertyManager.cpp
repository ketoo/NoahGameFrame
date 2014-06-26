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
    NFIProperty* pProperty = this->First();
    while (pProperty)
    {
        delete pProperty;

        pProperty = this->Next();
    }
}

bool NFCPropertyManager::RegisterCallback(const std::string& strProperty, const PROPERTY_EVENT_FUNCTOR_PTR& cb, const NFIValueList& argVar)
{
    NFIProperty* pProperty = this->GetElement(strProperty);
    if (pProperty)
    {
        pProperty->RegisterCallback(cb, argVar);
        return true;
    }

    return false;
}

NFIProperty* NFCPropertyManager::AddProperty(const NFIDENTID& self, NFIProperty* pProperty)
{
    const std::string& strProperty = pProperty->GetKey();
    NFIProperty* pOldProperty = GetElement(strProperty);
    if (!pOldProperty)
    {
        NFIProperty* pNewProperty = new NFCProperty(self, strProperty, pProperty->GetType(), pProperty->GetPublic(), pProperty->GetPrivate(), pProperty->GetSave(), pProperty->GetIndex(), pProperty->GetRelationValue());
        this->AddElement(strProperty, pNewProperty);

        if (pProperty->GetIndex() > 0)
        {
            mxPropertyIndexMap.insert(std::map<std::string, int>::value_type(strProperty, pProperty->GetIndex()));
        }
    }
    
    return pOldProperty;
}

NFIProperty* NFCPropertyManager::AddProperty(const NFIDENTID& self, const std::string& strPropertyName, const VARIANT_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, int nIndex, const std::string& strScriptFunction)
{
    NFIProperty* pProperty = GetElement(strPropertyName);
    if (!pProperty)
    {
        pProperty = new NFCProperty(self, strPropertyName, varType, bPublic, bPrivate, bSave, nIndex, strScriptFunction);
        this->AddElement(strPropertyName, pProperty);

        if (pProperty->GetIndex() > 0)
        {
            mxPropertyIndexMap.insert(std::map<std::string, int>::value_type(strPropertyName, nIndex));
        }
    }

    return pProperty;
}

bool NFCPropertyManager::SetProperty(const std::string& strPropertyName, const NFIValueList::VarData& varData)
{
    NFIProperty* pProperty = GetElement(strPropertyName);
    if (pProperty)
    {
        pProperty->SetValue(varData);

        return true;
    }

    return false;
}

bool NFCPropertyManager::SetProperty(const NFIProperty* pProperty )
{
    NFIProperty* pSelfProperty = GetElement(pProperty->GetKey());
    if (pSelfProperty)
    {
        pSelfProperty->SetValue(pProperty);

        return true;
    }

    return false;
}

NFIDENTID NFCPropertyManager::Self()
{
	return mSelf;
}

const std::map<std::string, int>& NFCPropertyManager::GetPropertyIndex()
{
    return mxPropertyIndexMap;
}

const int NFCPropertyManager::GetPropertyIndex( const std::string& strProperty )
{
    std::map<std::string, int>::iterator it = mxPropertyIndexMap.find(strProperty);
    if (it != mxPropertyIndexMap.end())
    {
        return it->second;
    }

    return 0;
}

