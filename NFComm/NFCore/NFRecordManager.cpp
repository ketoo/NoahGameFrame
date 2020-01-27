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

NF_SHARE_PTR<NFIRecord> NFRecordManager::AddRecord(const NFGUID& self, const std::string& strRecordName, const NF_SHARE_PTR<NFDataList>& ValueList, const NF_SHARE_PTR<NFDataList>& tagList, const int nRows)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (!pRecord)
    {
        //NF_SHARE_PTR<NFIRecord>
        pRecord = NF_SHARE_PTR<NFIRecord>(NF_NEW NFRecord(self, strRecordName, ValueList, tagList, nRows));
        this->AddElement(strRecordName, pRecord);
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

bool NFRecordManager::SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetInt(nRow, nCol, nValue);
    }

    return false;
}

bool NFRecordManager::SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetInt(nRow, strColTag, value);
    }

    return false;
}

bool NFRecordManager::SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetFloat(nRow, nCol, dwValue);
    }

    return false;
}

bool NFRecordManager::SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetFloat(nRow, strColTag, value);
    }

    return false;
}

bool NFRecordManager::SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetString(nRow, nCol, strValue);
    }

    return false;
}

bool NFRecordManager::SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetString(nRow, strColTag, value);
    }

    return false;
}

bool NFRecordManager::SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetObject(nRow, nCol, obj);
    }

    return false;
}

bool NFRecordManager::SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetObject(nRow, strColTag, value);
    }

    return false;
}

bool NFRecordManager::SetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol, const NFVector2& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector2(nRow, nCol, obj);
	}

	return false;
}

bool NFRecordManager::SetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector2& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector2(nRow, strColTag, value);
	}

	return false;
}

bool NFRecordManager::SetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol, const NFVector3& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector3(nRow, nCol, obj);
	}

	return false;
}

bool NFRecordManager::SetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector3& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector3(nRow, strColTag, value);
	}

	return false;
}

NFINT64 NFRecordManager::GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetInt(nRow, nCol);
    }

    return 0;
}

NFINT64 NFRecordManager::GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetInt(nRow, strColTag);
    }

    return 0;
}


double NFRecordManager::GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetFloat(nRow, nCol);
    }

    return 0.0;
}

double NFRecordManager::GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetFloat(nRow, strColTag);
    }

    return 0.0;
}

const std::string& NFRecordManager::GetRecordString(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetString(nRow, nCol);
    }

    return NULL_STR;
}

const std::string& NFRecordManager::GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetString(nRow, strColTag);
    }

    return NULL_STR;
}

const NFGUID& NFRecordManager::GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetObject(nRow, nCol);
    }

    return NULL_OBJECT;
}

const NFGUID& NFRecordManager::GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetObject(nRow, strColTag);
    }

    return NULL_OBJECT;
}

const NFVector2& NFRecordManager::GetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector2(nRow, nCol);
	}

	return NULL_VECTOR2;
}

const NFVector2& NFRecordManager::GetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector2(nRow, strColTag);
	}

	return NULL_VECTOR2;
}

const NFVector3& NFRecordManager::GetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector3(nRow, nCol);
	}

	return NULL_VECTOR3;
}

const NFVector3& NFRecordManager::GetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector3(nRow, strColTag);
	}

	return NULL_VECTOR3;
}
