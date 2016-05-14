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

NFCProperty::NFCProperty(const NFGUID& self, const std::string& strPropertyName, const TDATA_TYPE varType, bool bPublic,  bool bPrivate,  bool bSave, const std::string& strRelationValue)
{

    mbPublic = bPublic;
    mbPrivate = bPrivate;
    mbSave = bSave;
    mSelf = self;

    msPropertyName = strPropertyName;
    mstrRelationValue = strRelationValue;
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

const bool NFCProperty::GetPublic() const
{
    return mbPublic;
}

const bool NFCProperty::GetPrivate() const
{
    return mbPrivate;
}

const std::string& NFCProperty::GetRelationValue() const
{
    return mstrRelationValue;
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

void NFCProperty::SetRelationValue(const std::string& strRelationValue)
{
    mstrRelationValue = strRelationValue;
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
            strData = GetString();
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

bool NFCProperty::DeSerialization()
{
    bool bRet = false;

    const TDATA_TYPE eType = GetType();
    if(eType == TDATA_STRING)
    {
        NFCDataList xDataList();
        const std::string& strData = mxData->GetString();

        xDataList.Split(strData.c_str(), ";")
        for(int i = 0; i < xDataList.GetCount(); ++i)
        {
            if(nullptr == mxEmbeddedList)
            {
                mxEmbeddedList = NF_SHARE_PTR<NFList<std::string>>(NF_NEW NFList<std::string>());
            }
            else
            {
                mxEmbeddedList->ClearAll();
            }

            if(xDataList.String(i).empty())
            {
                NFASSERT(0, strData, __FILE__, __FUNCTION__);
            }

            mxEmbeddedList->Add(xDataList.String(i));
        }

        if(nullptr != mxEmbeddedList && mxEmbeddedList->Count() > 0)
        {
            std::string strTemData;
            for(bool bListRet = mxEmbeddedList->First(strTemData); bListRet == true; bListRet = mxEmbeddedList->Next(strTemData))
            {
                NFCDataList xTemDataList();
                xTemDataList.Split(strTemData.c_str(), ",")
                if(xTemDataList.GetCount() > 0)
                {
                    if (xTemDataList.GetCount() != 2)
                    {
                        NFASSERT(0, strTemData, __FILE__, __FUNCTION__);
                    }

                    const std::string& strKey = xTemDataList.String(0);
                    const std::string& strValue = xTemDataList.String(0);

                    if(strKey.empty() || strValue.empty())
                    {
                        NFASSERT(0, strTemData, __FILE__, __FUNCTION__);
                    }

                    if(nullptr == mxEmbeddedMap)
                    {
                        mxEmbeddedMap = NF_SHARE_PTR<NFMapEx<std::string, std::string>>(NF_NEW NFMapEx<std::string, std::string>());
                    }
                    else
                    {
                        mxEmbeddedMap->ClearAll();
                    }

                    mxEmbeddedMap->AddElement(strKey, NF_SHARE_PTR<std::string>(NF_NEW std::string(strValue)))
                }
            }

            bRet = true;
        }
    }

    return bRet;
}

const NF_SHARE_PTR<NFList<std::string>> NFCProperty::GetEmbeddedList() const
{
    return this->mxEmbeddedList;
}

const NF_SHARE_PTR<NFMapEx<std::string, std::string>> NFCProperty::GetEmbeddedMap() const
{
    return this->mxEmbeddedMap;
}
