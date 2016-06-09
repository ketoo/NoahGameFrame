// -------------------------------------------------------------------------
//    @FileName         :    NFCRecord.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCRecord
//
// -------------------------------------------------------------------------

#ifndef _GT_RECORD_H
#define _GT_RECORD_H

#include <vector>
#include "NFIRecord.h"
#include "NFCDataList.h"
#include "NFMapEx.h"

class NFCRecord : public NFIRecord
{
public:
    NFCRecord();
    NFCRecord(const NFGUID& self, const std::string& strRecordName, const NF_SHARE_PTR<NFIDataList>& valueList, const NF_SHARE_PTR<NFIDataList>& tagList, const int nMaxRow);

    virtual ~NFCRecord();

    virtual bool IsUsed(const int nRow) const;

    virtual bool SetUsed(const int nRow, const int bUse);

    virtual int GetCols() const;

    virtual int GetRows() const;

    virtual TDATA_TYPE GetColType(const int nCol) const;
    virtual const std::string& GetColTag(const int nCol) const;

    // 添加数据
    virtual int AddRow(const int nRow);

    virtual int AddRow(const int nRow, const NFIDataList& var);

    virtual bool SetInt(const int nRow, const int nCol, const NFINT64 value);
    virtual bool SetFloat(const int nRow, const int nCol, const double value);
    virtual bool SetString(const int nRow, const int nCol, const std::string& value);
    virtual bool SetObject(const int nRow, const int nCol, const NFGUID& value);

    virtual bool SetInt(const int nRow, const std::string& strColTag, const NFINT64 value);
    virtual bool SetFloat(const int nRow, const std::string& strColTag, const double value);
    virtual bool SetString(const int nRow, const std::string& strColTag, const std::string& value);
    virtual bool SetObject(const int nRow, const std::string& strColTag, const NFGUID& value);

    virtual bool QueryRow(const int nRow, NFIDataList& varList);

    virtual bool SwapRowInfo(const int nOriginRow, const int nTargetRow);

    virtual NFINT64 GetInt(const int nRow, const int nCol) const;
    virtual double GetFloat(const int nRow, const int nCol) const;
    virtual const std::string& GetString(const int nRow, const int nCol) const;
    virtual const NFGUID& GetObject(const int nRow, const int nCol) const;

    virtual NFINT64 GetInt(const int nRow, const std::string& strColTag) const;
    virtual double GetFloat(const int nRow, const std::string& strColTag) const;
    virtual const std::string& GetString(const int nRow, const std::string& strColTag) const;
    virtual const NFGUID& GetObject(const int nRow, const std::string& strColTag) const;

    virtual int FindRowByColValue(const int nCol, const NFIDataList& var, NFIDataList& varResult);
    virtual int FindInt(const int nCol, const NFINT64 value, NFIDataList& varResult);
    virtual int FindFloat(const int nCol, const double value, NFIDataList& varResult);
	virtual int FindString(const int nCol, const char* value, NFIDataList& varResult);
	virtual int FindString(const int nCol, const std::string& value, NFIDataList& varResult);
    virtual int FindObject(const int nCol, const NFGUID& value, NFIDataList& varResult);

    virtual int FindRowByColValue(const std::string& strColTag, const NFIDataList& var, NFIDataList& varResult);
    virtual int FindInt(const std::string& strColTag, const NFINT64 value, NFIDataList& varResult);
    virtual int FindFloat(const std::string& strColTag, const double value, NFIDataList& varResult);
	virtual int FindString(const std::string& strColTag, const char* value, NFIDataList& varResult);
	virtual int FindString(const std::string& strColTag, const std::string& value, NFIDataList& varResult);
    virtual int FindObject(const std::string& strColTag, const NFGUID& value, NFIDataList& varResult);

    virtual bool Remove(const int nRow);

    virtual bool Clear();

    virtual void AddRecordHook(const RECORD_EVENT_FUNCTOR_PTR& cb);

    virtual const bool GetSave();

    virtual const bool GetCache();

    virtual const bool GetPublic();

    virtual const bool GetPrivate();

    virtual const std::string& GetName() const;

    virtual void SetSave(const bool bSave);

    virtual void SetCache(const bool bCache);

    virtual void SetPublic(const bool bPublic);

    virtual void SetPrivate(const bool bPrivate);

    virtual void SetName(const char* strName);

    virtual const NF_SHARE_PTR<NFIDataList> GetInitData() const;
    virtual const NF_SHARE_PTR<NFIDataList> GetTag() const;

    virtual const TRECORDVEC& GetRecordVec() const;
protected:
    int GetPos(int nRow, int nCol) const;

    int GetCol(const std::string& strTag) const;

    bool ValidPos(int nRow, int nCol) const;
    bool ValidRow(int nRow) const;
    bool ValidCol(int nCol) const;

    void OnEventHandler(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

protected:
    //记录这个表的Key类型，那样在读取和设置的时候才能保持正确
	NF_SHARE_PTR<NFIDataList> mVarRecordType;//初始值类型--应该引用静态的(或者智能指针)，节约大量内存
	NF_SHARE_PTR<NFIDataList> mVarRecordTag;//col的tag值--应该引用静态的(或者智能指针)，节约大量内存

    std::map<std::string, int> mmTag;//tag->col转换

	////////////////////////////
	
	TRECORDVEC mtRecordVec;//真的数据
	std::vector<int> mVecUsedState;
    int mnMaxRow;

    NFGUID mSelf;
    bool mbSave;
    bool mbPublic;
    bool mbPrivate;
    bool mbCache;
    std::string mstrRecordName;

    typedef std::vector<RECORD_EVENT_FUNCTOR_PTR> TRECORDCALLBACKEX;
    TRECORDCALLBACKEX mtRecordCallback;
};

#endif
