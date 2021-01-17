/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/



#include "NFObject.h"
#include "NFRecordManager.h"
#include "NFPropertyManager.h"

NFObject::NFObject(NFGUID self, NFIPluginManager* pLuginManager)
    : NFIObject(self)
{
	mObjectEventState = COE_CREATE_NODATA;

    mSelf = self;
    m_pPluginManager = pLuginManager;

    m_pRecordManager = NF_SHARE_PTR<NFRecordManager>(NF_NEW NFRecordManager(mSelf));
    m_pPropertyManager = NF_SHARE_PTR<NFPropertyManager>(NF_NEW NFPropertyManager(mSelf));
}

NFObject::~NFObject()
{

}

bool NFObject::AddRecordCallBack(const std::string& recordName, const RECORD_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        pRecord->AddRecordHook(cb);

        return true;
    }

    return false;
}

bool NFObject::AddPropertyCallBack(const std::string& strCriticalName, const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(strCriticalName);
    if (pProperty)
    {
        pProperty->RegisterCallback(cb);

        return true;
    }

    return false;
}

CLASS_OBJECT_EVENT NFObject::GetState()
{
	return mObjectEventState;
}

void NFObject::SetState(const CLASS_OBJECT_EVENT eState)
{
	mObjectEventState = eState;
}

bool NFObject::ObjectReady()
{
    CLASS_OBJECT_EVENT state = GetState();
    if (state == COE_CREATE_HASDATA
        || state == COE_CREATE_FINISH
        || state == COE_CREATE_CLIENT_FINISH)
    {
        return true;
    }

    return false;
}

inline NF_SHARE_PTR<NFIProperty> NFObject::AddProperty(const std::string& propertyName, const NFDATA_TYPE varType)
{
   return this->GetPropertyManager()->AddProperty(this->Self(), propertyName, varType);
}

bool NFObject::FindProperty(const std::string& propertyName)
{
    if (GetPropertyManager()->GetElement(propertyName))
    {
        return true;
    }

    return false;
}

bool NFObject::SetPropertyInt(const std::string& propertyName, const NFINT64 nValue, const NFINT64 reason)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
    if (pProperty)
    {
        return pProperty->SetInt(nValue, reason);
    }

    return false;
}

bool NFObject::SetPropertyFloat(const std::string& propertyName, const double dwValue, const NFINT64 reason)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
    if (pProperty)
    {
        return pProperty->SetFloat(dwValue, reason);
    }

    return false;
}

bool NFObject::SetPropertyString(const std::string& propertyName, const std::string& value, const NFINT64 reason)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
    if (pProperty)
    {
        return pProperty->SetString(value, reason);
    }

    return false;
}

bool NFObject::SetPropertyObject(const std::string& propertyName, const NFGUID& obj, const NFINT64 reason)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
    if (pProperty)
    {
        return pProperty->SetObject(obj, reason);
    }

    return false;
}

bool NFObject::SetPropertyVector2(const std::string& propertyName, const NFVector2& value, const NFINT64 reason)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
	if (pProperty)
	{
		return pProperty->SetVector2(value, reason);
	}

	return false;
}

bool NFObject::SetPropertyVector3(const std::string& propertyName, const NFVector3& value, const NFINT64 reason)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
	if (pProperty)
	{
		return pProperty->SetVector3(value, reason);
	}

	return false;
}

NFINT64 NFObject::GetPropertyInt(const std::string& propertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
    if (pProperty)
    {
        return pProperty->GetInt();
    }

    return 0;
}

int NFObject::GetPropertyInt32(const std::string& propertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
	if (pProperty)
	{
		return pProperty->GetInt32();
	}

	return 0;
}

double NFObject::GetPropertyFloat(const std::string& propertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
    if (pProperty)
    {
        return pProperty->GetFloat();
    }

    return 0.0;
}

const std::string& NFObject::GetPropertyString(const std::string& propertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
    if (pProperty)
    {
        return pProperty->GetString();
    }

    return NULL_STR;
}

const NFGUID& NFObject::GetPropertyObject(const std::string& propertyName)
{
    NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
    if (pProperty)
    {
        return pProperty->GetObject();
    }

    return NULL_OBJECT;
}

const NFVector2& NFObject::GetPropertyVector2(const std::string& propertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
	if (pProperty)
	{
		return pProperty->GetVector2();
	}

	return NULL_VECTOR2;
}

const NFVector3& NFObject::GetPropertyVector3(const std::string& propertyName)
{
	NF_SHARE_PTR<NFIProperty> pProperty = GetPropertyManager()->GetElement(propertyName);
	if (pProperty)
	{
		return pProperty->GetVector3();
	}

	return NULL_VECTOR3;
}

NF_SHARE_PTR<NFIRecord> NFObject::FindRecord(const std::string& recordName)
{
	return GetRecordManager()->GetElement(recordName);
}

bool NFObject::SetRecordInt(const std::string& recordName, const int row, const int col, const NFINT64 nValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetInt(row, col, nValue);
    }

    return false;
}

bool NFObject::SetRecordInt(const std::string& recordName, const int row, const std::string& colTag, const NFINT64 value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetInt(row, colTag, value);
    }

    return false;
}

bool NFObject::SetRecordFloat(const std::string& recordName, const int row, const int col, const double dwValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetFloat(row, col, dwValue);
    }

    return false;
}

bool NFObject::SetRecordFloat(const std::string& recordName, const int row, const std::string& colTag, const double value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetFloat(row, colTag, value);
    }

    return false;
}

bool NFObject::SetRecordString(const std::string& recordName, const int row, const int col, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetString(row, col, value.c_str());
    }

    return false;
}

bool NFObject::SetRecordString(const std::string& recordName, const int row, const std::string& colTag, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetString(row, colTag, value.c_str());
    }

    return false;
}

bool NFObject::SetRecordObject(const std::string& recordName, const int row, const int col, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetObject(row, col, obj);
    }

    return false;
}

bool NFObject::SetRecordObject(const std::string& recordName, const int row, const std::string& colTag, const NFGUID& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetObject(row, colTag, value);
    }

    return false;
}

bool NFObject::SetRecordVector2(const std::string& recordName, const int row, const int col, const NFVector2& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
	if (pRecord)
	{
		return pRecord->SetVector2(row, col, obj);
	}

	return false;
}

bool NFObject::SetRecordVector2(const std::string& recordName, const int row, const std::string& colTag, const NFVector2& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
	if (pRecord)
	{
		return pRecord->SetVector2(row, colTag, value);
	}

	return false;
}

bool NFObject::SetRecordVector3(const std::string& recordName, const int row, const int col, const NFVector3& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
	if (pRecord)
	{
		return pRecord->SetVector3(row, col, obj);
	}

	return false;
}

bool NFObject::SetRecordVector3(const std::string& recordName, const int row, const std::string& colTag, const NFVector3& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
	if (pRecord)
	{
		return pRecord->SetVector3(row, colTag, value);
	}

	return false;
}

NFINT64 NFObject::GetRecordInt(const std::string& recordName, const int row, const int col)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetInt(row, col);
    }

    return 0;
}

NFINT64 NFObject::GetRecordInt(const std::string& recordName, const int row, const std::string& colTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetInt(row, colTag);
    }

    return 0;
}

double NFObject::GetRecordFloat(const std::string& recordName, const int row, const int col)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetFloat(row, col);
    }

    return 0.0;
}

double NFObject::GetRecordFloat(const std::string& recordName, const int row, const std::string& colTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetFloat(row, colTag);
    }

    return 0.0;
}

const std::string& NFObject::GetRecordString(const std::string& recordName, const int row, const int col)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetString(row, col);
    }

    return NULL_STR;
}

const std::string& NFObject::GetRecordString(const std::string& recordName, const int row, const std::string& colTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetString(row, colTag);
    }

    return NULL_STR;
}

const NFGUID& NFObject::GetRecordObject(const std::string& recordName, const int row, const int col)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetObject(row, col);
    }

    return NULL_OBJECT;
}

const NFGUID& NFObject::GetRecordObject(const std::string& recordName, const int row, const std::string& colTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetObject(row, colTag);
    }

    return NULL_OBJECT;
}

const NFVector2& NFObject::GetRecordVector2(const std::string& recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
	if (pRecord)
	{
		return pRecord->GetVector2(row, col);
	}

	return NULL_VECTOR2;
}

const NFVector2& NFObject::GetRecordVector2(const std::string& recordName, const int row, const std::string& colTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
	if (pRecord)
	{
		return pRecord->GetVector2(row, colTag);
	}

	return NULL_VECTOR2;
}

const NFVector3& NFObject::GetRecordVector3(const std::string& recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
	if (pRecord)
	{
		return pRecord->GetVector3(row, col);
	}

	return NULL_VECTOR3;
}

const NFVector3& NFObject::GetRecordVector3(const std::string& recordName, const int row, const std::string& colTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetRecordManager()->GetElement(recordName);
	if (pRecord)
	{
		return pRecord->GetVector3(row, colTag);
	}

	return NULL_VECTOR3;
}

NF_SHARE_PTR<NFIRecordManager> NFObject::GetRecordManager()
{
    return m_pRecordManager;
}

NF_SHARE_PTR<NFIPropertyManager> NFObject::GetPropertyManager()
{
    return m_pPropertyManager;
}

void NFObject::SetRecordManager(NF_SHARE_PTR<NFIRecordManager> xRecordManager)
{
	m_pRecordManager = xRecordManager;
}

void NFObject::SetPropertyManager(NF_SHARE_PTR<NFIPropertyManager> xPropertyManager)
{
	m_pPropertyManager = xPropertyManager;
}

NFGUID NFObject::Self()
{
    return mSelf;
}

void NFObject::ToMemoryCounterString(std::string& info)
{
    info.append(mSelf.ToString());
    info.append(":");
    info.append(m_pPropertyManager->GetPropertyString("ClassName"));
}