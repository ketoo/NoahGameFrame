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
    eType = TDATA_UNKNOWN;

    msPropertyName = "";
}

NFCProperty::NFCProperty(const NFIDENTID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, int nIndex, const std::string& strScriptFunction)
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
    for (TPROPERTYCALLBACKEX::iterator iter = mtPropertyCallback.begin(); iter != mtPropertyCallback.end(); ++iter)
    {
        iter->reset();
    }

    mtPropertyCallback.clear();
    m_pTData.reset();
}

void NFCProperty::SetValue(const NFIValueList::TData& TData)
{
    if (eType != TData.nType)
    {
        return;
    }

    if (!m_pTData.get())
    {
    	if (!NFIValueList::Valid(TData))
    	{
        	return;
    	}

        m_pTData = std::shared_ptr<NFIValueList::TData>(NF_NEW NFIValueList::TData());
    	m_pTData->nType = TData.nType;
        m_pTData->variantData = TData.variantData;
    }

    NFCValueList oldValue;
    oldValue.Append(*m_pTData);

    m_pTData->variantData = TData.variantData;

    NFCValueList newValue;
    newValue.Append(*m_pTData);

    OnEventHandler(oldValue , newValue);
}

void NFCProperty::SetValue(const NFIProperty* pProperty)
{
    const NFCProperty* p = dynamic_cast<const NFCProperty*>(pProperty);
    SetValue(p->GetValue());
    //SetValue(pProperty->GetValue());
}

NFIValueList::TData NFCProperty::GetValue() const
{
    if (m_pTData.get())
    {
        return *m_pTData;
    }

    return NFIValueList::TData();
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

const std::string& NFCProperty::GetRelationValue() const
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

int NFCProperty::GetInt() const
{
    if (!m_pTData.get())
    {
        return 0;
    }

    if (TDATA_INT == m_pTData->nType)
    {
        return boost::get<int>(m_pTData->variantData);
    }

    return 0;
}

float NFCProperty::GetFloat() const
{
    if (!m_pTData.get())
    {
        return 0.0f;
    }

    if (TDATA_FLOAT == m_pTData->nType)
    {
        return boost::get<float>(m_pTData->variantData);
    }

    return 0.0f;
}

double NFCProperty::GetDouble() const
{
    if (!m_pTData.get())
    {
        return 0.0f;
    }

    if (TDATA_DOUBLE == m_pTData->nType)
    {
        return boost::get<double>(m_pTData->variantData);
    }

    return 0.0f;
}

const std::string& NFCProperty::GetString() const
{
    if (!m_pTData.get())
    {
        return NULL_STR;
    }

    if (TDATA_STRING == m_pTData->nType)
    {
        return boost::get<const std::string&>(m_pTData->variantData);
    }

    return NULL_STR;
}

NFIDENTID NFCProperty::GetObject() const
{
    if (!m_pTData.get())
    {
        return NFIDENTID();
    }

    if (TDATA_OBJECT == m_pTData->nType)
    {
        return boost::get<NFINT64>(m_pTData->variantData);
    }

    return NFIDENTID();
}

void* NFCProperty::GetPointer() const
{
    if (!m_pTData.get())
    {
        return NULL;
    }

    if (TDATA_POINTER == m_pTData->nType)
    {
        return boost::get<void*>(m_pTData->variantData);
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
    if (eType != TDATA_INT)
    {
        return false;
    }

    NFIValueList::TData TData;
    TData.variantData = value;
    TData.nType = TDATA_INT;

    if (!m_pTData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(TData))
        {
            return false;
        }

        m_pTData = std::shared_ptr<NFIValueList::TData>(NF_NEW NFIValueList::TData());
        m_pTData->nType = TDATA_INT;
        m_pTData->variantData = (int)0;
    }

    if (TData.variantData == m_pTData->variantData)
    {
        return false;
    }

    if (TDATA_INT == m_pTData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pTData);

        m_pTData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pTData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetFloat(float value)
{
    if (eType != TDATA_FLOAT)
    {
        return false;
    }

    NFIValueList::TData TData;
    TData.variantData = value;
    TData.nType = TDATA_FLOAT;

    if (!m_pTData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(TData))
        {
            return false;
        }

        m_pTData = std::shared_ptr<NFIValueList::TData>(NF_NEW NFIValueList::TData());
        m_pTData->nType = TDATA_FLOAT;
        m_pTData->variantData = (float)0.0f;
    }

    if (TData.variantData == m_pTData->variantData)
    {
        return false;
    }

    if (TDATA_FLOAT == m_pTData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pTData);

        m_pTData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pTData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetDouble(double value)
{
    if (eType != TDATA_DOUBLE)
    {
        return false;
    }

    NFIValueList::TData TData;
    TData.variantData = value;
    TData.nType = TDATA_DOUBLE;

    if (!m_pTData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(TData))
        {
            return false;
        }

        m_pTData = std::shared_ptr<NFIValueList::TData>(NF_NEW NFIValueList::TData());
        m_pTData->nType = TDATA_DOUBLE;
        m_pTData->variantData = (double)0.0f;

    }

    if (TData.variantData == m_pTData->variantData)
    {
        return false;
    }

    if (TDATA_DOUBLE == m_pTData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pTData);

        m_pTData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pTData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetString(const std::string& value)
{
    if (eType != TDATA_STRING)
    {
        return false;
    }

    NFIValueList::TData TData;
    TData.variantData = value;
    TData.nType = TDATA_STRING;

    if (!m_pTData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(TData))
        {
            return false;
        }

        m_pTData = std::shared_ptr<NFIValueList::TData>(NF_NEW NFIValueList::TData());
        m_pTData->nType = TDATA_STRING;
        m_pTData->variantData = NULL_STR;
    }

    if (TData.variantData == m_pTData->variantData)
    {
        return false;
    }

    if (TDATA_STRING == m_pTData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pTData);

        m_pTData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pTData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetObject(const NFIDENTID& value)
{
    if (eType != TDATA_OBJECT)
    {
        return false;
    }

    NFIValueList::TData TData;
    TData.variantData = value.nData64;
    TData.nType = TDATA_OBJECT;

    if (!m_pTData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(TData))
        {
            return false;
        }

        m_pTData = std::shared_ptr<NFIValueList::TData>(NF_NEW NFIValueList::TData());
        m_pTData->nType = TDATA_OBJECT;
        m_pTData->variantData = (NFINT64)0;

    }

    if (TData.variantData == m_pTData->variantData)
    {
        return false;
    }

    if (TDATA_OBJECT == m_pTData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pTData);

        m_pTData->variantData = value.nData64;

        NFCValueList newValue;
        newValue.Append(*m_pTData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::SetPointer(void* value)
{
    if (eType != TDATA_POINTER)
    {
        return false;
    }

    NFIValueList::TData TData;
    TData.variantData = value;
    TData.nType = TDATA_POINTER;

    if (!m_pTData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (!NFIValueList::Valid(TData))
        {
            return false;
        }

        m_pTData = std::shared_ptr<NFIValueList::TData>(NF_NEW NFIValueList::TData());
        m_pTData->nType = TDATA_POINTER;
        m_pTData->variantData = (void*)NULL;
    }

    if (TData.variantData == m_pTData->variantData)
    {
        return false;
    }

    if (TDATA_POINTER == m_pTData->nType)
    {
        NFCValueList oldValue;
        oldValue.Append(*m_pTData);

        m_pTData->variantData = value;

        NFCValueList newValue;
        newValue.Append(*m_pTData);

        OnEventHandler(oldValue , newValue);

        return true;
    }

    return false;
}

bool NFCProperty::Changed() const
{
    return NFIValueList::Valid(GetValue());
}

const TDATA_TYPE NFCProperty::GetType() const
{
    return eType;
}

const bool NFCProperty::GeUsed() const
{
    if (m_pTData.get())
    {
        return true;
    }

    return false;
}
