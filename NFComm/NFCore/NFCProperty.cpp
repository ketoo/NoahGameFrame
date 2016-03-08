// -------------------------------------------------------------------------
//    @FileName         :    NFCProperty.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCProperty
//
// -------------------------------------------------------------------------

#include "NFCProperty.h"
#include <complex>

NFCProperty::NFCProperty()
{
    mbPublic = false;
    mbPrivate = false;
    mbSave = false;
    mSelf = NFGUID();
    eType = TDATA_UNKNOWN;

    msPropertyName = "";
}

NFCProperty::NFCProperty(const NFGUID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex, const std::string& strScriptFunction)
{

    mbPublic = bPublic;
    mbPrivate = bPrivate;
    mbSave = bSave;
    mbView = bView;
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
    mxData.reset();
}

void NFCProperty::SetValue(const NFIDataList::TData& TData)
{
    if (eType != TData.GetType())
    {
        return;
    }

    if (!mxData.get())
    {
        if (!TData.IsNullValue())
        {
            return;
        }

        mxData = NF_SHARE_PTR<NFIDataList::TData>(NF_NEW NFIDataList::TData(TData));
    }

    NFCDataList::TData oldValue;
    oldValue = *mxData;

    mxData->variantData = TData.variantData;

    NFCDataList::TData newValue;
    newValue = *mxData;

    OnEventHandler(oldValue , newValue);
}

void NFCProperty::SetValue(const NFIProperty* pProperty)
{
    SetValue(pProperty->GetValue());
}

const NFIDataList::TData& NFCProperty::GetValue() const
{
    if (mxData.get())
    {
        return *mxData;
    }

    return NULL_TDATA;
}

const std::string&  NFCProperty::GetKey() const
{
    return msPropertyName;
}

const bool NFCProperty::GetSave() const
{
    return mbSave;
}

const bool NFCProperty::GetView() const
{
    return mbView;
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

void NFCProperty::SetView(bool bView)
{
    mbView = bView;
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

NFINT64 NFCProperty::GetInt() const
{
    if (!mxData.get())
    {
        return 0;
    }

    return mxData->GetInt();
}

double NFCProperty::GetFloat() const
{
    if (!mxData.get())
    {
        return 0.0;
    }

    return mxData->GetFloat();
}

const std::string& NFCProperty::GetString() const
{
    if (!mxData.get())
    {
        return NULL_STR;
    }

    return mxData->GetString();
}

const NFGUID& NFCProperty::GetObject() const
{
    if (!mxData.get())
    {
        return NULL_OBJECT;
    }

    return mxData->GetObject();
}

void NFCProperty::RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
    mtPropertyCallback.push_back(cb);
}

int NFCProperty::OnEventHandler(const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    if (mtPropertyCallback.size() <= 0)
    {
        return 0;
    }

    TPROPERTYCALLBACKEX::iterator it = mtPropertyCallback.begin();
    TPROPERTYCALLBACKEX::iterator end = mtPropertyCallback.end();
    for (it; it != end; ++it)
    {
        //NFIDataList参数:属性名，OLD属性值，NEW属性值, ARG参数(pKernel,self)
        PROPERTY_EVENT_FUNCTOR_PTR& pFunPtr = *it;
        PROPERTY_EVENT_FUNCTOR* pFunc = pFunPtr.get();
        int nTemRet = pFunc->operator()(mSelf, msPropertyName, oldVar, newVar);
    }

    return 0;
}

bool NFCProperty::SetInt(const NFINT64 value)
{
    if (eType != TDATA_INT)
    {
        return false;
    }

    if (!mxData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (0 == value)
        {
            return false;
        }

        mxData = NF_SHARE_PTR<NFIDataList::TData>(NF_NEW NFIDataList::TData(TDATA_INT));
        mxData->SetInt(0);
    }

    if (value == mxData->GetInt())
    {
        return false;
    }

    NFCDataList::TData oldValue;
    oldValue = *mxData;

    mxData->SetInt(value);

    OnEventHandler(oldValue, *mxData);

    return true;
}

bool NFCProperty::SetFloat(const double value)
{
    if (eType != TDATA_FLOAT)
    {
        return false;
    }

    if (!mxData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (std::abs(value) < 0.001)
        {
            return false;
        }

        mxData = NF_SHARE_PTR<NFIDataList::TData>(NF_NEW NFIDataList::TData(TDATA_FLOAT));
        mxData->SetFloat(0.0);
    }

    if (value - mxData->GetFloat() < 0.001)
    {
        return false;
    }


    NFCDataList::TData oldValue;
    oldValue = *mxData;

    mxData->SetFloat(value);


    OnEventHandler(oldValue, *mxData);

    return true;
}

bool NFCProperty::SetString(const std::string& value)
{
    if (eType != TDATA_STRING)
    {
        return false;
    }

    if (!mxData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (value.empty())
        {
            return false;
        }

        mxData = NF_SHARE_PTR<NFIDataList::TData>(NF_NEW NFIDataList::TData(TDATA_STRING));
        mxData->SetString(NULL_STR);
    }

    if (value == mxData->GetString())
    {
        return false;
    }

    NFCDataList::TData oldValue;
    oldValue = *mxData;

    mxData->SetString(value);

    OnEventHandler(oldValue, *mxData);

    return true;
}

bool NFCProperty::SetObject(const NFGUID& value)
{
    if (eType != TDATA_OBJECT)
    {
        return false;
    }

    if (!mxData.get())
    {
        //本身是空就是因为没数据，还来个没数据的就不存了
        if (value.IsNull())
        {
            return false;
        }

        mxData = NF_SHARE_PTR<NFIDataList::TData>(NF_NEW NFIDataList::TData(TDATA_OBJECT));
        mxData->SetObject(NFGUID());

    }

    if (value == mxData->GetObject())
    {
        return false;
    }

    NFCDataList::TData oldValue;
    oldValue = *mxData;

    mxData->SetObject(value);

    OnEventHandler(oldValue , *mxData);

    return true;
}

bool NFCProperty::Changed() const
{
    return GetValue().IsNullValue();
}

const TDATA_TYPE NFCProperty::GetType() const
{
    return eType;
}

const bool NFCProperty::GeUsed() const
{
    if (mxData.get())
    {
        return true;
    }

    return false;
}

std::string NFCProperty::ToString()
{
    std::string strData;
    const TDATA_TYPE eType = GetType();
    switch (eType)
    {
        case TDATA_INT:
            strData = lexical_cast<std::string> (GetInt());
            break;

        case TDATA_FLOAT:
            strData = lexical_cast<std::string> (GetFloat());
            break;

        case TDATA_STRING:
            strData = lexical_cast<std::string> (GetString());
            break;
        case TDATA_OBJECT:
            strData = GetObject().ToString();
            break;
        default:

            strData = NULL_STR;
            break;
    }

    return strData;
}

bool NFCProperty::FromString(const std::string& strData)
{
    const TDATA_TYPE eType = GetType();
    bool bRet = false;
    switch (eType)
    {
        case TDATA_INT:
        {
            NFINT64  nValue = 0;
            bRet = NF_StrTo(strData, nValue);
            SetInt(nValue);
        }
        break;

        case TDATA_FLOAT:
        {
            double  dValue = 0;
            bRet = NF_StrTo(strData, dValue);
            SetFloat(dValue);
        }
        break;

        case TDATA_STRING:
        {
            SetString(strData);
            bRet = true;
        }
        break;
        case TDATA_OBJECT:
        {
            NFGUID xID;

            bRet = xID.FromString(strData);
            SetObject(xID);
        }
        break;
        default:
            break;
    }

    return bRet;
}
