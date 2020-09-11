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


#include "NFRecordManager.h"

NFRecordManager::~NFRecordManager()
{
    ClearAll();
}

NF_SHARE_PTR<NFIRecord> NFRecordManager::AddRecord(const NFGUID& self, const std::string& recordName, const NF_SHARE_PTR<NFDataList>& ValueList, const NF_SHARE_PTR<NFDataList>& tagList, const int nRows)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (!pRecord)
    {
        //NF_SHARE_PTR<NFIRecord>
        pRecord = NF_SHARE_PTR<NFIRecord>(NF_NEW NFRecord(self, recordName, ValueList, tagList, nRows));
        this->AddElement(recordName, pRecord);
    }

    return pRecord;
}

const NFGUID& NFRecordManager::Self()
{
    return mSelf;
}

std::string NFRecordManager::ToString()
{
	std::stringstream stream;
	NF_SHARE_PTR<NFIRecord> pRecord = First();
	while (pRecord)
	{
		stream << pRecord->ToString() << std::endl;
		pRecord = Next();
	}

	return stream.str();
}

bool NFRecordManager::SetRecordInt(const std::string& recordName, const int row, const int col, const NFINT64 nValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetInt(row, col, nValue);
    }

    return false;
}

bool NFRecordManager::SetRecordInt(const std::string& recordName, const int row, const std::string& colTag, const NFINT64 value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetInt(row, colTag, value);
    }

    return false;
}

bool NFRecordManager::SetRecordFloat(const std::string& recordName, const int row, const int col, const double dwValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetFloat(row, col, dwValue);
    }

    return false;
}

bool NFRecordManager::SetRecordFloat(const std::string& recordName, const int row, const std::string& colTag, const double value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetFloat(row, colTag, value);
    }

    return false;
}

bool NFRecordManager::SetRecordString(const std::string& recordName, const int row, const int col, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetString(row, col, value);
    }

    return false;
}

bool NFRecordManager::SetRecordString(const std::string& recordName, const int row, const std::string& colTag, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetString(row, colTag, value);
    }

    return false;
}

bool NFRecordManager::SetRecordObject(const std::string& recordName, const int row, const int col, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetObject(row, col, obj);
    }

    return false;
}

bool NFRecordManager::SetRecordObject(const std::string& recordName, const int row, const std::string& colTag, const NFGUID& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->SetObject(row, colTag, value);
    }

    return false;
}

bool NFRecordManager::SetRecordVector2(const std::string& recordName, const int row, const int col, const NFVector2& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
	if (pRecord)
	{
		return pRecord->SetVector2(row, col, obj);
	}

	return false;
}

bool NFRecordManager::SetRecordVector2(const std::string& recordName, const int row, const std::string& colTag, const NFVector2& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
	if (pRecord)
	{
		return pRecord->SetVector2(row, colTag, value);
	}

	return false;
}

bool NFRecordManager::SetRecordVector3(const std::string& recordName, const int row, const int col, const NFVector3& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
	if (pRecord)
	{
		return pRecord->SetVector3(row, col, obj);
	}

	return false;
}

bool NFRecordManager::SetRecordVector3(const std::string& recordName, const int row, const std::string& colTag, const NFVector3& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
	if (pRecord)
	{
		return pRecord->SetVector3(row, colTag, value);
	}

	return false;
}

NFINT64 NFRecordManager::GetRecordInt(const std::string& recordName, const int row, const int col)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetInt(row, col);
    }

    return 0;
}

NFINT64 NFRecordManager::GetRecordInt(const std::string& recordName, const int row, const std::string& colTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetInt(row, colTag);
    }

    return 0;
}


double NFRecordManager::GetRecordFloat(const std::string& recordName, const int row, const int col)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetFloat(row, col);
    }

    return 0.0;
}

double NFRecordManager::GetRecordFloat(const std::string& recordName, const int row, const std::string& colTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetFloat(row, colTag);
    }

    return 0.0;
}

const std::string& NFRecordManager::GetRecordString(const std::string& recordName, const int row, const int col)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetString(row, col);
    }

    return NULL_STR;
}

const std::string& NFRecordManager::GetRecordString(const std::string& recordName, const int row, const std::string& colTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetString(row, colTag);
    }

    return NULL_STR;
}

const NFGUID& NFRecordManager::GetRecordObject(const std::string& recordName, const int row, const int col)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetObject(row, col);
    }

    return NULL_OBJECT;
}

const NFGUID& NFRecordManager::GetRecordObject(const std::string& recordName, const int row, const std::string& colTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
    if (pRecord)
    {
        return pRecord->GetObject(row, colTag);
    }

    return NULL_OBJECT;
}

const NFVector2& NFRecordManager::GetRecordVector2(const std::string& recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
	if (pRecord)
	{
		return pRecord->GetVector2(row, col);
	}

	return NULL_VECTOR2;
}

const NFVector2& NFRecordManager::GetRecordVector2(const std::string& recordName, const int row, const std::string& colTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
	if (pRecord)
	{
		return pRecord->GetVector2(row, colTag);
	}

	return NULL_VECTOR2;
}

const NFVector3& NFRecordManager::GetRecordVector3(const std::string& recordName, const int row, const int col)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
	if (pRecord)
	{
		return pRecord->GetVector3(row, col);
	}

	return NULL_VECTOR3;
}

const NFVector3& NFRecordManager::GetRecordVector3(const std::string& recordName, const int row, const std::string& colTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(recordName);
	if (pRecord)
	{
		return pRecord->GetVector3(row, colTag);
	}

	return NULL_VECTOR3;
}
