// -------------------------------------------------------------------------
//    @FileName         :    NFCRecord.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCRecord
//
// -------------------------------------------------------------------------

#ifndef NFC_RECORD_H
#define NFC_RECORD_H

#include <vector>
#include "NFIRecord.h"
#include "NFDataList.hpp"
#include "NFMapEx.hpp"
#include "NFComm/NFPluginModule/NFPlatform.h"

class _NFExport NFCRecord : public NFIRecord
{
public:
    NFCRecord();
    NFCRecord(const NFGUID& self, const std::string& strRecordName, const NF_SHARE_PTR<NFDataList>& valueList, const NF_SHARE_PTR<NFDataList>& tagList, const int nMaxRow);

    virtual ~NFCRecord();

    virtual bool IsUsed(const int nRow) const;

    virtual bool SetUsed(const int nRow, const int bUse);

	virtual bool PreAllocMemoryForRow(const int nRow);

    virtual int GetCols() const;

    virtual int GetRows() const;

    virtual NFDATA_TYPE GetColType(const int nCol) const;
    virtual const std::string& GetColTag(const int nCol) const;

    
    virtual int AddRow(const int nRow);

    virtual int AddRow(const int nRow, const NFDataList& var);

    virtual bool SetInt(const int nRow, const int nCol, const NFINT64 value);
    virtual bool SetFloat(const int nRow, const int nCol, const double value);
    virtual bool SetString(const int nRow, const int nCol, const std::string& value);
    virtual bool SetObject(const int nRow, const int nCol, const NFGUID& value);
	virtual bool SetVector2(const int nRow, const int nCol, const NFVector2& value);
	virtual bool SetVector3(const int nRow, const int nCol, const NFVector3& value);

    virtual bool SetInt(const int nRow, const std::string& strColTag, const NFINT64 value);
    virtual bool SetFloat(const int nRow, const std::string& strColTag, const double value);
    virtual bool SetString(const int nRow, const std::string& strColTag, const std::string& value);
    virtual bool SetObject(const int nRow, const std::string& strColTag, const NFGUID& value);
	virtual bool SetVector2(const int nRow, const std::string& strColTag, const NFVector2& value);
	virtual bool SetVector3(const int nRow, const std::string& strColTag, const NFVector3& value);

    virtual bool QueryRow(const int nRow, NFDataList& varList);

    virtual bool SwapRowInfo(const int nOriginRow, const int nTargetRow);

    virtual NFINT64 GetInt(const int nRow, const int nCol) const;
    virtual double GetFloat(const int nRow, const int nCol) const;
    virtual const std::string& GetString(const int nRow, const int nCol) const;
    virtual const NFGUID& GetObject(const int nRow, const int nCol) const;
	virtual const NFVector2& GetVector2(const int nRow, const int nCol) const;
	virtual const NFVector3& GetVector3(const int nRow, const int nCol) const;

    virtual NFINT64 GetInt(const int nRow, const std::string& strColTag) const;
    virtual double GetFloat(const int nRow, const std::string& strColTag) const;
    virtual const std::string& GetString(const int nRow, const std::string& strColTag) const;
    virtual const NFGUID& GetObject(const int nRow, const std::string& strColTag) const;
	virtual const NFVector2& GetVector2(const int nRow, const std::string& strColTag) const;
	virtual const NFVector3& GetVector3(const int nRow, const std::string& strColTag) const;

    virtual int FindRowByColValue(const int nCol, const NFDataList& var, NFDataList& varResult);
    virtual int FindInt(const int nCol, const NFINT64 value, NFDataList& varResult);
    virtual int FindFloat(const int nCol, const double value, NFDataList& varResult);
	virtual int FindString(const int nCol, const std::string& value, NFDataList& varResult);
    virtual int FindObject(const int nCol, const NFGUID& value, NFDataList& varResult);
	virtual int FindVector2(const int nCol, const NFVector2& value, NFDataList& varResult);
	virtual int FindVector3(const int nCol, const NFVector3& value, NFDataList& varResult);


    virtual int FindRowByColValue(const std::string& strColTag, const NFDataList& var, NFDataList& varResult);
    virtual int FindInt(const std::string& strColTag, const NFINT64 value, NFDataList& varResult);
    virtual int FindFloat(const std::string& strColTag, const double value, NFDataList& varResult);
	virtual int FindString(const std::string& strColTag, const std::string& value, NFDataList& varResult);
    virtual int FindObject(const std::string& strColTag, const NFGUID& value, NFDataList& varResult);
	virtual int FindVector2(const std::string& strColTag, const NFVector2& value, NFDataList& varResult);
	virtual int FindVector3(const std::string& strColTag, const NFVector3& value, NFDataList& varResult);

    virtual bool Remove(const int nRow);

    virtual bool Clear();

    virtual void AddRecordHook(const RECORD_EVENT_FUNCTOR_PTR& cb);

    virtual const bool GetSave();

    virtual const bool GetCache();

	virtual const bool GetUpload();

    virtual const bool GetPublic();

    virtual const bool GetPrivate();

    virtual const std::string& GetName() const;

    virtual void SetSave(const bool bSave);

    virtual void SetCache(const bool bCache);

	virtual void SetUpload(const bool bUpload);

    virtual void SetPublic(const bool bPublic);

    virtual void SetPrivate(const bool bPrivate);

    virtual void SetName(const std::string& strName);

    virtual const NF_SHARE_PTR<NFDataList> GetInitData() const;
    virtual const NF_SHARE_PTR<NFDataList> GetTag() const;

    virtual const TRECORDVEC& GetRecordVec() const;
protected:
    int GetPos(int nRow, int nCol) const;

    int GetCol(const std::string& strTag) const;

    bool ValidPos(int nRow, int nCol) const;
    bool ValidRow(int nRow) const;
    bool ValidCol(int nCol) const;

    void OnEventHandler(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar);

protected:
    
	NF_SHARE_PTR<NFDataList> mVarRecordType;
	NF_SHARE_PTR<NFDataList> mVarRecordTag;

    std::map<std::string, int> mmTag;

	////////////////////////////
	
	TRECORDVEC mtRecordVec;
	std::vector<int> mVecUsedState;
    int mnMaxRow;

    NFGUID mSelf;
    bool mbSave;
    bool mbPublic;
    bool mbPrivate;
    bool mbCache;
	bool mbUpload;
    std::string mstrRecordName;

    typedef std::vector<RECORD_EVENT_FUNCTOR_PTR> TRECORDCALLBACKEX;
    TRECORDCALLBACKEX mtRecordCallback;
};

#endif
