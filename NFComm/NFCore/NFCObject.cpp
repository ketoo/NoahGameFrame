// -------------------------------------------------------------------------
//    @FileName         :    NFCObject.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCObject
//
// -------------------------------------------------------------------------

#include "NFCObject.h"
#include "NFCRecordManager.h"
#include "NFCPropertyManager.h"

NFCObject::NFCObject(NFGUID self, NFIPluginManager* pLuginManager)
    : NFIObject(self)
{
	mObjectEventState = COE_CREATE_NODATA;

    mSelf = self;
    m_pPluginManager = pLuginManager;

    m_pRecordManager = NF_SHARE_PTR<NFCRecordManager>(NF_NEW NFCRecordManager(mSelf));
    m_pPropertyManager = NF_SHARE_PTR<NFCPropertyManager>(NF_NEW NFCPropertyManager(mSelf));
}

NFCObject::~NFCObject()
{

}

bool NFCObject::Init()
{
    return true;
}

bool NFCObject::Shut()
{
    return true;
}

bool NFCObject::Execute()
{
    return true;
}

bool NFCObject::AddRecordCallBack(const std::string& strRecordName, const RECORD_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        pRecord->AddRecordHook(cb);

        return true;
    }

    return false;
}

bool NFCObject::AddPropertyCallBack(const std::string& strCriticalName, const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strCriticalName);
    if (pProperty)
    {
        pProperty->RegisterCallback(cb);

        return true;
    }

    return false;
}

CLASS_OBJECT_EVENT NFCObject::GetState()
{
	return mObjectEventState;
}

bool NFCObject::SetState(const CLASS_OBJECT_EVENT eState)
{
	mObjectEventState = eState;
	return true;
}

bool NFCObject::FindProperty(const std::string& strPropertyName)
{
    if (GetPropertyManager()->GetElement(strPropertyName))
    {
        return true;
    }

    return false;
}

bool NFCObject::SetPropertyInt(const std::string& strPropertyName, const NFINT64 nValue)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        return pProperty->SetInt(nValue);
    }

    return false;
}

bool NFCObject::SetPropertyFloat(const std::string& strPropertyName, const double dwValue)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        return pProperty->SetFloat(dwValue);
    }

    return false;
}

bool NFCObject::SetPropertyString(const std::string& strPropertyName, const std::string& strValue)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        return pProperty->SetString(strValue);
    }

    return false;
}

bool NFCObject::SetPropertyObject(const std::string& strPropertyName, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        return pProperty->SetObject(obj);
    }

    return false;
}

bool NFCObject::SetPropertyVector2(const std::string& strPropertyName, const NFVector2& value)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
	if (pProperty)
	{
		return pProperty->SetVector2(value);
	}

	return false;
}

bool NFCObject::SetPropertyVector3(const std::string& strPropertyName, const NFVector3& value)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
	if (pProperty)
	{
		return pProperty->SetVector3(value);
	}

	return false;
}

NFINT64 NFCObject::GetPropertyInt(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        return pProperty->GetInt();
    }

    return 0;
}

int NFCObject::GetPropertyInt32(const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
	if (pProperty)
	{
		return pProperty->GetInt32();
	}

	return 0;
}

double NFCObject::GetPropertyFloat(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        return pProperty->GetFloat();
    }

    return 0.0;
}

const std::string& NFCObject::GetPropertyString(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        return pProperty->GetString();
    }

    return NULL_STR;
}

const NFGUID& NFCObject::GetPropertyObject(const std::string& strPropertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
    if (pProperty)
    {
        return pProperty->GetObject();
    }

    return NULL_OBJECT;
}

const NFVector2& NFCObject::GetPropertyVector2(const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
	if (pProperty)
	{
		return pProperty->GetVector2();
	}

	return NULL_VECTOR2;
}

const NFVector3& NFCObject::GetPropertyVector3(const std::string& strPropertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strPropertyName);
	if (pProperty)
	{
		return pProperty->GetVector3();
	}

	return NULL_VECTOR3;
}

bool NFCObject::FindRecord(const std::string& strRecordName)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return true;
    }

    return false;
}

bool NFCObject::SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetInt(nRow, nCol, nValue);
    }

    return false;
}

bool NFCObject::SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetInt(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetFloat(nRow, nCol, dwValue);
    }

    return false;
}

bool NFCObject::SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetFloat(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetString(nRow, nCol, strValue.c_str());
    }

    return false;
}

bool NFCObject::SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetString(nRow, strColTag, value.c_str());
    }

    return false;
}

bool NFCObject::SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetObject(nRow, nCol, obj);
    }

    return false;
}

bool NFCObject::SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetObject(nRow, strColTag, value);
    }

    return false;
}

bool NFCObject::SetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol, const NFVector2& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector2(nRow, nCol, obj);
	}

	return false;
}

bool NFCObject::SetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector2& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector2(nRow, strColTag, value);
	}

	return false;
}

bool NFCObject::SetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol, const NFVector3& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector3(nRow, nCol, obj);
	}

	return false;
}

bool NFCObject::SetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector3& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector3(nRow, strColTag, value);
	}

	return false;
}

NFINT64 NFCObject::GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetInt(nRow, nCol);
    }

    return 0;
}

NFINT64 NFCObject::GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetInt(nRow, strColTag);
    }

    return 0;
}

double NFCObject::GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetFloat(nRow, nCol);
    }

    return 0.0;
}

double NFCObject::GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetFloat(nRow, strColTag);
    }

    return 0.0;
}

const std::string& NFCObject::GetRecordString(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetString(nRow, nCol);
    }

    return NULL_STR;
}

const std::string& NFCObject::GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetString(nRow, strColTag);
    }

    return NULL_STR;
}

const NFGUID& NFCObject::GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetObject(nRow, nCol);
    }

    return NULL_OBJECT;
}

const NFGUID& NFCObject::GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetObject(nRow, strColTag);
    }

    return NULL_OBJECT;
}

const NFVector2& NFCObject::GetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector2(nRow, nCol);
	}

	return NULL_VECTOR2;
}

const NFVector2& NFCObject::GetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector2(nRow, strColTag);
	}

	return NULL_VECTOR2;
}

const NFVector3& NFCObject::GetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector3(nRow, nCol);
	}

	return NULL_VECTOR3;
}

const NFVector3& NFCObject::GetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector3(nRow, strColTag);
	}

	return NULL_VECTOR3;
}

NF_SHARE_PTR<NFIRecordManager> NFCObject::GetRecordManager()
{
    return m_pRecordManager;
}

NF_SHARE_PTR<NFIPropertyManager> NFCObject::GetPropertyManager()
{
    return m_pPropertyManager;
}

NFGUID NFCObject::Self()
{
    return mSelf;
}