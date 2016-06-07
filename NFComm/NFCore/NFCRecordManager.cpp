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

NF_SHARE_PTR<NFIRecord> NFCRecordManager::AddRecord(const NFGUID& self, const std::string& strRecordName, const NF_SHARE_PTR<NFIDataList>& ValueList, const NF_SHARE_PTR<NFIDataList>& tagList, const int nRows)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (!pRecord.get())
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
    if (pRecord.get())
    {
        return pRecord->SetInt(nRow, nCol, nValue);
    }

    return false;
}

bool NFCRecordManager::SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetInt(nRow, strColTag, value);
    }

    return false;
}

bool NFCRecordManager::SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetFloat(nRow, nCol, dwValue);
    }

    return false;
}

bool NFCRecordManager::SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetFloat(nRow, strColTag, value);
    }

    return false;
}

bool NFCRecordManager::SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetString(nRow, nCol, strValue);
    }

    return false;
}

bool NFCRecordManager::SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetString(nRow, strColTag, value);
    }

    return false;
}

bool NFCRecordManager::SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& obj)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetObject(nRow, nCol, obj);
    }

    return false;
}

bool NFCRecordManager::SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetObject(nRow, strColTag, value);
    }

    return false;
}

NFINT64 NFCRecordManager::GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetInt(nRow, nCol);
    }

    return 0;
}

NFINT64 NFCRecordManager::GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetInt(nRow, strColTag);
    }

    return 0;
}


double NFCRecordManager::GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetFloat(nRow, nCol);
    }

    return 0.0;
}

double NFCRecordManager::GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetFloat(nRow, strColTag);
    }

    return 0.0;
}

const std::string& NFCRecordManager::GetRecordString(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetString(nRow, nCol);
    }

    return NULL_STR;
}

const std::string& NFCRecordManager::GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetString(nRow, strColTag);
    }

    return NULL_STR;
}

const NFGUID& NFCRecordManager::GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetObject(nRow, nCol);
    }

    return NULL_OBJECT;
}

const NFGUID& NFCRecordManager::GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->GetObject(nRow, strColTag);
    }

    return NULL_OBJECT;
}
