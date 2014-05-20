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
    NFIRecord* pRecord = this->First();
    while (pRecord)
    {
        delete pRecord;
        pRecord = NULL;

        pRecord = this->Next();
    }
}

NFIRecord* NFCRecordManager::AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIValueList& ValueList, const NFIValueList& keyList, const NFIValueList& descList, const int nRows, bool bPublic /*= false*/, bool bPrivate /*= false*/, bool bSave /*= false */, int nIndex)
{
    NFIRecord* pRecord = GetElement(strRecordName);
    if (!pRecord)
    {
        pRecord = new NFCRecord(self, strRecordName, ValueList, keyList, descList, nRows, bPublic, bPrivate, bSave, nIndex);
        this->AddElement(strRecordName, pRecord);
    }

    return pRecord;
}

NFIDENTID NFCRecordManager::Self()
{
	return mSelf;
}

