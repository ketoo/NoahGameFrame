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

NF_SHARE_PTR<NFIRecord> NFCRecordManager::AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIDataList& ValueList, const NFIDataList& keyList, const NFIDataList& descList, const NFIDataList& tagList, const NFIDataList& relateRecordData, const int nRows, bool bPublic, bool bPrivate, bool bSave, bool bView, int nIndex)
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

NFIDENTID NFCRecordManager::Self()
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
