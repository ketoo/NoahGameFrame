// -------------------------------------------------------------------------
//    @FileName         :    NFCProperty.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCProperty
//
// -------------------------------------------------------------------------

#include "NFCProperty.h"

NFCProperty::NFCProperty()
{
    mbPublic = false;
    mbPrivate = false;
    mbSave = false;
    mSelf = 0;
    eType = VTYPE_UNKNOWN;

    msPropertyName = "";
}

NFCProperty::NFCProperty(const NFIDENTID& self, const std::string& strPropertyName, const VARIANT_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, int nIndex, const std::string& strScriptFunction)
{

    mbPublic = bPublic;
    mbPrivate = bPrivate;
    mbSave = bSave;
    mnIndex = nIndex;
    mSelf = self;

    msPropertyName = strPropertyName;
    eType = varType;
}

NFCProperty::~NFCProperty()
{
    mtPropertyCallback.clear();
    m_pVarData.reset();
}

void NFCProperty::SetValue(const NFIValueList::VarData& varData)
{
    if (eType != varData.nType)
    {
        return;
    }

    if (!m_pVarData.get())
    {
    	if (!NFIValueList::Valid(varData))
    	{
        	return;
    	}

        m_pVarData = std::shared_ptr<NFIValueList::VarData>(new NFIValueList::VarData());
    	m_pVarData->nType = varData.nType;
        m_pVarData->variantData = varData.variantData;
    }

    NFCValueList oldValue;
    oldValue.Append(*m_pVarData);

    m_pVarData->variantData = varData.variantData;

    NFCValueList newValue;
    newValue.Append(*m_pVarData);

    OnEventHandler(oldValue , newValue);
}

void NFCProperty::SetValue(const NFIProperty* pProperty)
{
    const NFCProperty* p = dynamic_cast<const NFCProperty*>(pProperty);
    SetValue(p->GetValue());
    //SetValue(pProperty->GetValue());
}

NFIValueList::VarData NFCProperty::GetValue() const
{
    if (m_pVarData.get())
    {
        return *m_pVarData;
    }

    return NFIValueList::VarData();
}

const std::string&  NFCProperty::GetKey() const
{
    return msPropertyName;
}

const bool NFCProperty::GetSave() const
{
    return mbSave;
}

const bool NFCProperty::GetPublic() const
{
    return mbPublic;
}

const bool NFCProperty::GetPrivate() const
{
    return mbPrivate;
}

const int NFCProperty::GetIndex() const
{
    return mnIndex;
};

const std::string& NFCProperty::GetScriptFunction() const
{
    return NULL_STR;//msScriptFunction;
}

void NFCProperty::SetSave(bool bSave)
{
    mbSave = bSave;
}

void NFCProperty::SetPublic(bool bPublic)
{
    mbPublic = bPublic;
}

void NFCProperty::SetPrivate(bool bPrivate)
{
    mbPrivate = bPrivate;
}

void NFCProperty::SetScriptFunction(const std::string& strScriptFunction)
{
    //msScriptFunction = strScriptFunction;
}

int NFCProperty::QueryInt() const
{
    if (!m_pVarData.get())
    {
        return 0;
    }

    if (VTYPE_INT == m_pVarData->nType)
    {
        return boost::get<int>(m_pVarData->variantData);
    }

    return 0;
}

float NFCProperty::QueryFloat() const
{
    if (!m_pVarData.get())
    {
        return 0.0f;
    }

    if (VTYPE_FLOAT == m_pVarData->nType)
    {
        return boost::get<float>(m_pVarData->variantData);
    }

    return 0.0f;
}

double NFCProperty::QueryDouble() const
{
    if (!m_pVarData.get())
    {
        return 0.0f;
    }

    if (VTYPE_DOUBLE == m_pVarData->nType)
    {
        return boost::get<double>(m_pVarData->variantData);
    }

    return 0.0f;
}

const std::string& NFCProperty::QueryString() const
{
    if (!m_pVarData.get())
    {
        return NULL_STR;
    }

    if (VTYPE_STRING == m_pVarData->nType)
    {
        return boost::get<const std::string&>(m_pVarData->variantData);
    }

    return NULL_STR;
}

NFIDENTID NFCProperty::QueryObject() const
{
    if (!m_pVarData.get())
    {
        return NFIDENTID();
    }

    if (VTYPE_OBJECT == m_pVarData->nType)
    {
        return boost::get<NFINT64>(m_pVarData->variantData);
    }

    return NFIDENTID();
}

void* NFCProperty::QueryPointer() const
{
    if (!m_pVarData.get())
    {
        return NULL;
    }

    if (VTYPE_POINTER == m_pVarData->nType)
    {
        return boost::get<void*>(m_pVarData->variantData);
    }

    return NULL;
}

void NFCProperty::RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb, const NFIValueList& argVar)
{
    mtPropertyCallback.push_back(cb);
}

int NFCProperty::OnEventHandler(const NFIValueList& oldVar, const NFIValueList& newVar)
{
    if (mtPropertyCallback.size() <= 0)
    {
        return 0;
    }

    TPROPERTYCALLBACKEX::iterator it = mtPropertyCallback.begin();
    TPROPERTYCALLBACKEX::iterator end = mtPropertyCallback.end();
    for (it; it != end; ++it)
    {
        //NFIValueList参数:属性名，OLD属性值，NEW属性值, ARG参数(pKernel,self)
        PROPERTY_EVENT_FUNCTOR_PTR pFunPtr = *it;
        PROPERTY_EVENT_FUNCTOR* pFunc = pFunPtr.get();
        int nTemRet = pFunc->operator()(mSelf, msPropertyName, oldVar, newVar, NFCValueList());
    }

    return 0;
}

bool NFCProperty::SetInt(int value)
{
    if (eType != VTYPE_INT)
    {
        return false;
    }

    NFIValueList::VarData varData;
    varData.variantData = value;
    varData.nType = VTYPE_INT;

    if (!m_pVarData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(varData))
        {
            return false;
        }

        m_pVarData = std::shared_ptr<NFIValueList::VarData>(new NFIValueList::VarData());
        m_pVarData->nType = VTYPE_INT;
        m_pVarData->variantData = (int)0;
    }

    if (varData.variantData == m_pVarData->variantData)
    {
        return false;
    }

    if (VTYPE_INT == m_pVarData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pVarData);

        m_pVarData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pVarData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetFloat(float value)
{
    if (eType != VTYPE_FLOAT)
    {
        return false;
    }

    NFIValueList::VarData varData;
    varData.variantData = value;
    varData.nType = VTYPE_FLOAT;

    if (!m_pVarData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(varData))
        {
            return false;
        }

        m_pVarData = std::shared_ptr<NFIValueList::VarData>(new NFIValueList::VarData());
        m_pVarData->nType = VTYPE_FLOAT;
        m_pVarData->variantData = (float)0.0f;
    }

    if (varData.variantData == m_pVarData->variantData)
    {
        return false;
    }

    if (VTYPE_FLOAT == m_pVarData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pVarData);

        m_pVarData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pVarData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetDouble(double value)
{
    if (eType != VTYPE_DOUBLE)
    {
        return false;
    }

    NFIValueList::VarData varData;
    varData.variantData = value;
    varData.nType = VTYPE_DOUBLE;

    if (!m_pVarData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(varData))
        {
            return false;
        }

        m_pVarData = std::shared_ptr<NFIValueList::VarData>(new NFIValueList::VarData());
        m_pVarData->nType = VTYPE_DOUBLE;
        m_pVarData->variantData = (double)0.0f;

    }

    if (varData.variantData == m_pVarData->variantData)
    {
        return false;
    }

    if (VTYPE_DOUBLE == m_pVarData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pVarData);

        m_pVarData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pVarData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetString(const std::string& value)
{
    if (eType != VTYPE_STRING)
    {
        return false;
    }

    NFIValueList::VarData varData;
    varData.variantData = value;
    varData.nType = VTYPE_STRING;

    if (!m_pVarData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(varData))
        {
            return false;
        }

        m_pVarData = std::shared_ptr<NFIValueList::VarData>(new NFIValueList::VarData());
        m_pVarData->nType = VTYPE_STRING;
        m_pVarData->variantData = NULL_STR;
    }

    if (varData.variantData == m_pVarData->variantData)
    {
        return false;
    }

    if (VTYPE_STRING == m_pVarData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pVarData);

        m_pVarData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pVarData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetObject(const NFIDENTID& value)
{
    if (eType != VTYPE_OBJECT)
    {
        return false;
    }

    NFIValueList::VarData varData;
    varData.variantData = value.nData64;
    varData.nType = VTYPE_OBJECT;

    if (!m_pVarData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(varData))
        {
            return false;
        }

        m_pVarData = std::shared_ptr<NFIValueList::VarData>(new NFIValueList::VarData());
        m_pVarData->nType = VTYPE_OBJECT;
        m_pVarData->variantData = (NFINT64)0;

    }

    if (varData.variantData == m_pVarData->variantData)
    {
        return false;
    }

    if (VTYPE_OBJECT == m_pVarData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pVarData);

        m_pVarData->variantData = value.nData64;

        NFCValueList newValue;
        newValue.Append(*m_pVarData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetPointer(void* value)
{
    if (eType != VTYPE_POINTER)
    {
        return false;
    }

    NFIValueList::VarData varData;
    varData.variantData = value;
    varData.nType = VTYPE_POINTER;

    if (!m_pVarData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(varData))
        {
            return false;
        }

        m_pVarData = std::shared_ptr<NFIValueList::VarData>(new NFIValueList::VarData());
        m_pVarData->nType = VTYPE_POINTER;
        m_pVarData->variantData = (void*)NULL;
    }

    if (varData.variantData == m_pVarData->variantData)
    {
        return false;
    }

    if (VTYPE_POINTER == m_pVarData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pVarData);

        m_pVarData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pVarData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::Changed() const
{
    return NFIValueList::Valid(GetValue());
}

const VARIANT_TYPE NFCProperty::GetType() const
{
    return eType;
}

const bool NFCProperty::GeUsed() const
{
    if (m_pVarData.get())
    {
        return true;
    }

    return false;
}
