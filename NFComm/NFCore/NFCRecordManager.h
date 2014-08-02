// -------------------------------------------------------------------------
//    @FileName         :    NFCRecordManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFCRecordManager
//
// -------------------------------------------------------------------------

#ifndef _NFC_RECORDMANAGER_H_
#define _NFC_RECORDMANAGER_H_

#include <map>
#include <list>
#include <string>
#include <vector>
#include "NFCRecord.h"
#include "NFIRecordManager.h"

class NFCRecordManager : public NFIRecordManager
{
public:
	NFCRecordManager(const NFIDENTID& self )
	{
		mSelf = self;
	}

    virtual ~NFCRecordManager();

    virtual NFIRecord* AddRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIDataList& TData, const NFIDataList& keyList, const NFIDataList& descData, const NFIDataList& tagData, const NFIDataList& relateRecordData, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, int nIndex);

	virtual NFIDENTID Self();

    virtual void GetRelationRows(const std::string& strScrRecord, const std::string& strSrcTag, const NFIDataList& var, const std::string& strRelatedRecord, OUT NFIDataList& outRowList);

    virtual const std::map<std::string, int>& GetRecordIndex();
    virtual const int GetRecordIndex(const std::string& strRecordName);

private:
	NFIDENTID mSelf;
    std::map<std::string, int> mxRecordIndexMap;

};

#endif