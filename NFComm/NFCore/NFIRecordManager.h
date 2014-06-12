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
    virtual NFIRecord* AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIValueList& varData, const NFIValueList& keyList,const NFIValueList& descData, const NFIValueList& tagData, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, int nIndex) = 0;
	virtual NFIDENTID Self() = 0;

};


#endif