// -------------------------------------------------------------------------
//    @FileName         :    NFIRecordManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFIRecordManager
//
// -------------------------------------------------------------------------

#ifndef _NFI_RECORDMANAGER_H_
#define _NFI_RECORDMANAGER_H_

#include "NFIRecord.h"
#include "NFMap.h"

class NFIRecordManager
    : public NFMap<std::string, NFIRecord>
{
public:
    virtual ~NFIRecordManager() {}
    virtual NFIRecord* AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIDataList& TData, const NFIDataList& keyList, const NFIDataList& descData, const NFIDataList& tagData, const NFIDataList& relateRecordData, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex) = 0;
    virtual NFIDENTID Self() = 0;
    virtual void GetRelationRows(const std::string& strScrRecord, const std::string& strSrcTag, const NFIDataList& var, const std::string& strRelatedRecord, NFIDataList& outRowList) = 0;

    virtual const std::map<std::string, int>& GetRecordIndex() = 0;
    virtual const int GetRecordIndex(const std::string& strRecordName) = 0;
};


#endif
