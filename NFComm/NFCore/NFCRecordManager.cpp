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

NF_SHARE_PTR<NFIRecord> NFCRecordManager::AddRecord(const NFGUID& self, const std::string& strRecordName, const NFIDataList& ValueList, const NFIDataList& keyList, const NFIDataList& descList, const NFIDataList& tagList, const NFIDataList& relateRecordData, const int nRows, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (!pRecord.get())
    {
        //NF_SHARE_PTR<NFIRecord>
        pRecord = NF_SHARE_PTR<NFIRecord>(NF_NEW NFCRecord(self, strRecordName, ValueList, keyList, descList, tagList, relateRecordData, nRows, bPublic, bPrivate, bSave, bView, nIndex));
        this->AddElement(strRecordName, pRecord);

        if (nIndex > 0)
        {
            mxRecordIndexMap.insert(std::map<std::string, int>::value_type(strRecordName, nIndex));
        }
    }

    return pRecord;
}

const NFGUID& NFCRecordManager::Self()
{
    return mSelf;
}

void NFCRecordManager::GetRelationRows(const std::string& strSrcRecord, const std::string& strSrcTag, const NFIDataList& var, const std::string& strRelatedRecord, NFIDataList& outRowList)
{
    NF_SHARE_PTR<NFIRecord> pSrcRecord = GetElement(strSrcRecord);
    NF_SHARE_PTR<NFIRecord> pRelatedRecord = GetElement(strRelatedRecord);
    if (NULL == pSrcRecord.get() || NULL == pRelatedRecord.get())
    {
        return;
    }

    std::string strRelatedTag;
    if (!pSrcRecord->GetRelatedTag(strSrcTag, strRelatedRecord, strRelatedTag))
    {
        return;
    }

    pRelatedRecord->FindRowByColValue(strRelatedTag, var, outRowList);
}

const std::map<std::string, int>& NFCRecordManager::GetRecordIndex()
{
    return mxRecordIndexMap;
}

const int NFCRecordManager::GetRecordIndex(const std::string& strRecordName)
{
    std::map<std::string, int>::iterator it = mxRecordIndexMap.find(strRecordName);
    if (it != mxRecordIndexMap.end())
    {
        return it->second;
    }

    return 0;
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
        return pRecord->SetString(nRow, nCol, strValue.c_str());
    }

    return false;
}

bool NFCRecordManager::SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value)
{
    NF_SHARE_PTR<NFIRecord> pRecord = GetElement(strRecordName);
    if (pRecord.get())
    {
        return pRecord->SetString(nRow, strColTag, value.c_str());
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