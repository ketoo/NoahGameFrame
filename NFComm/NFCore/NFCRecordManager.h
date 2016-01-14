// -------------------------------------------------------------------------
//    @FileName         :    NFCRecordManager.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-12-17
//    @Module           :    NFCRecordManager
//
// -------------------------------------------------------------------------

#ifndef NFC_RECORDMANAGER_H
#define NFC_RECORDMANAGER_H

#include <map>
#include <list>
#include <string>
#include <vector>
#include "NFCRecord.h"
#include "NFIRecordManager.h"

class NFCRecordManager : public NFIRecordManager
{
public:
    NFCRecordManager(const NFGUID& self)
    {
        mSelf = self;
    }

    virtual ~NFCRecordManager();

    virtual NF_SHARE_PTR<NFIRecord> AddRecord(const NFGUID& self, const std::string& strRecordName, const NFIDataList& TData, const NFIDataList& keyList, const NFIDataList& descData, const NFIDataList& tagData, const NFIDataList& relateRecordData, const int nRows, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex);

    virtual const NFGUID& Self();

    virtual void GetRelationRows(const std::string& strScrRecord, const std::string& strSrcTag, const NFIDataList& var, const std::string& strRelatedRecord, NFIDataList& outRowList);

    virtual const std::map<std::string, int>& GetRecordIndex();
    virtual const int GetRecordIndex(const std::string& strRecordName);

	//////////////////////////////////////////////////////////////////////////

	virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const int nCol, const NFINT64 nValue);
	virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol, const double dwValue);
	virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const int nCol, const std::string& strValue);
	virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const int nCol, const NFGUID& obj);

	virtual bool SetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFINT64 value);
	virtual bool SetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag, const double value);
	virtual bool SetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag, const std::string& value);
	virtual bool SetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag, const NFGUID& value);

	virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const int nCol);
	virtual double GetRecordFloat(const std::string& strRecordName, const int nRow, const int nCol);
	virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const int nCol);
	virtual const NFGUID& GetRecordObject(const std::string& strRecordName, const int nRow, const int nCol);

	virtual NFINT64 GetRecordInt(const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual double GetRecordFloat(const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const std::string& GetRecordString(const std::string& strRecordName, const int nRow, const std::string& strColTag);
	virtual const NFGUID& GetRecordObject(const std::string& strRecordName, const int nRow, const std::string& strColTag);
	//////////////////////////////////////////////////////////////////////////
private:
    NFGUID mSelf;
    std::map<std::string, int> mxRecordIndexMap;

};

#endif
