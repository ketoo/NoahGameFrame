// -------------------------------------------------------------------------
//    @FileName         :    NFCRecordManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFCRecordManager
//
// -------------------------------------------------------------------------

#include "NFCRecordManager.h"

NFCRecordManager::~NFCRecordManager()
{
    ClearAll();
}

NF_SHARE_PTR<NFIRecord> NFCRecordManager::AddRecord(const NFGUID& self, const std::string& strRecordName, const NF_SHARE_PTR<NFDataList>& ValueList, const NF_SHARE_PTR<NFDataList>& tagList, const int nRows)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (!pRecord)
    {
        //NF_SHARE_PTR<NFIRecord>
        pRecord = NF_SHARE_PTR<NFIRecord>(NF_NEW NFCRecord(self, strRecordName, ValueList, tagList, nRows));
        this->AddElement(strRecordName, pRecord);
    }

    return pRecord;
}

const NFGUID& NFCRecordManager::Self()
{
    return mSelf;
}

bool NFCRecordManager::SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetInt(nRow, nCol, nValue);
    }

    return false;
}

bool NFCRecordManager::SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetInt(nRow, strColTag, value);
    }

    return false;
}

bool NFCRecordManager::SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetFloat(nRow, nCol, dwValue);
    }

    return false;
}

bool NFCRecordManager::SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetFloat(nRow, strColTag, value);
    }

    return false;
}

bool NFCRecordManager::SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetString(nRow, nCol, strValue);
    }

    return false;
}

bool NFCRecordManager::SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetString(nRow, strColTag, value);
    }

    return false;
}

bool NFCRecordManager::SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetObject(nRow, nCol, obj);
    }

    return false;
}

bool NFCRecordManager::SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->SetObject(nRow, strColTag, value);
    }

    return false;
}

bool NFCRecordManager::SetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol, const NFVector2& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector2(nRow, nCol, obj);
	}

	return false;
}

bool NFCRecordManager::SetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector2& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector2(nRow, strColTag, value);
	}

	return false;
}

bool NFCRecordManager::SetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol, const NFVector3& obj)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector3(nRow, nCol, obj);
	}

	return false;
}

bool NFCRecordManager::SetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFVector3& value)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->SetVector3(nRow, strColTag, value);
	}

	return false;
}

NFINT64 NFCRecordManager::GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetInt(nRow, nCol);
    }

    return 0;
}

NFINT64 NFCRecordManager::GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetInt(nRow, strColTag);
    }

    return 0;
}


double NFCRecordManager::GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetFloat(nRow, nCol);
    }

    return 0.0;
}

double NFCRecordManager::GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetFloat(nRow, strColTag);
    }

    return 0.0;
}

const std::string& NFCRecordManager::GetRecordString(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetString(nRow, nCol);
    }

    return NULL_STR;
}

const std::string& NFCRecordManager::GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetString(nRow, strColTag);
    }

    return NULL_STR;
}

const NFGUID& NFCRecordManager::GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetObject(nRow, nCol);
    }

    return NULL_OBJECT;
}

const NFGUID& NFCRecordManager::GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord)
    {
        return pRecord->GetObject(nRow, strColTag);
    }

    return NULL_OBJECT;
}

const NFVector2& NFCRecordManager::GetRecordVector2(const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector2(nRow, nCol);
	}

	return NULL_VECTOR2;
}

const NFVector2& NFCRecordManager::GetRecordVector2(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector2(nRow, strColTag);
	}

	return NULL_VECTOR2;
}

const NFVector3& NFCRecordManager::GetRecordVector3(const std::string& strRecordName, const int nRow, const int nCol)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector3(nRow, nCol);
	}

	return NULL_VECTOR3;
}

const NFVector3& NFCRecordManager::GetRecordVector3(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
	NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
	if (pRecord)
	{
		return pRecord->GetVector3(nRow, strColTag);
	}

	return NULL_VECTOR3;
}
