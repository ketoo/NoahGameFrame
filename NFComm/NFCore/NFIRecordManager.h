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
    virtual NFIRecord* AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIValueList& varData, const NFIValueList& keyList,const NFIValueList& descData, const NFIValueList& tagData, const NFIValueList& relateRecordData, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, int nIndex) = 0;
	virtual NFIDENTID Self() = 0;
    virtual void GetRelationRows(const std::string& strScrRecord, const std::string& strSrcTag, const NFIValueList& var, const std::string& strRelatedRecord, OUT NFIValueList& outRowList) = 0;

    virtual const std::map<std::string, int>& GetRecordIndex() = 0;
    virtual const int GetRecordIndex(const std::string& strRecordName) = 0;
};


#endif