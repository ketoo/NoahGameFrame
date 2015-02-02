// -------------------------------------------------------------------------
//    @FileName         :    NFIRecord.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFIRecord
//
// -------------------------------------------------------------------------

#ifndef _NFI_RECORD_H_
#define _NFI_RECORD_H_

#include "NFDefine.h"
#include "NFIDataList.h"

class NFIRecord
{
public:
    enum RecordOptype
    {
        Add = 0,
        Del,
        Changed,//swap
        Create,
        UpData,
        Cleared,
        Sort,
    };

    typedef std::vector<NF_SHARE_PTR<NFIDataList::TData>> TRECORDVEC;
    typedef TRECORDVEC::const_iterator TRECORDVECCONSTITER;

    virtual ~NFIRecord() {}

    virtual bool IsUsed(const int nRow) const  = 0;
    virtual bool IsKey(const int nRow) const  = 0;
    virtual bool SetUsed(const int nRow, const int bUse)  = 0;
    virtual bool SetKey(const int nRow, const int bKey) = 0;

    virtual int GetCols() const  = 0;
    virtual int GetRows() const  = 0;

    virtual TDATA_TYPE GetColType(const int nCol) const = 0;
    virtual const std::string& GetColTag(const int nCol) const = 0;

    // 添加数据
    virtual int AddRow(const int nRow) = 0;
    virtual int AddRow(const int nRow, const NFIDataList& var) = 0;

    virtual bool SetInt(const int nRow, const int nCol, const NFINT64 value) = 0;
    virtual bool SetFloat(const int nRow, const int nCol, const float value) = 0;
    virtual bool SetDouble(const int nRow, const int nCol, const double value) = 0;
    virtual bool SetString(const int nRow, const int nCol, const char* value) = 0;
    virtual bool SetObject(const int nRow, const int nCol, const NFIDENTID& value) = 0;

    virtual bool SetInt(const int nRow, const std::string& strColTag, const NFINT64 value) = 0;
    virtual bool SetFloat(const int nRow, const std::string& strColTag, const float value) = 0;
    virtual bool SetDouble(const int nRow, const std::string& strColTag, const double value) = 0;
    virtual bool SetString(const int nRow, const std::string& strColTag, const char* value) = 0;
    virtual bool SetObject(const int nRow, const std::string& strColTag, const NFIDENTID& value) = 0;
    virtual bool SetPointer(const int nRow, const std::string& strColTag, const void* value) = 0;

    // 获得数据
    virtual bool QueryRow(const int nRow, NFIDataList& varList) = 0;
    virtual bool SwapRowInfo(const int nOriginRow, const int nTargetRow) = 0;

    virtual NFINT64 GetInt(const int nRow, const int nCol) const = 0;
    virtual float GetFloat(int const nRow, const int nCol) const = 0;
    virtual double GetDouble(const int nRow, const int nCol) const = 0;
    virtual const std::string& GetString(const int nRow, const int nCol) const = 0;
    virtual NFIDENTID GetObject(const int nRow, const int nCol) const = 0;
    virtual void* GetPointer(const int nRow, const int nCol) const = 0;

    virtual NFINT64 GetInt(const int nRow, const std::string& strColTag) const = 0;
    virtual float GetFloat(const int nRow, const std::string& strColTag) const = 0;
    virtual double GetDouble(const int nRow, const std::string& strColTag) const = 0;
    virtual const std::string& GetString(const int nRow, const std::string& strColTag) const = 0;
    virtual NFIDENTID GetObject(const int nRow, const std::string& strColTag) const = 0;
    virtual void* GetPointer(const int nRow, const std::string& strColTag) const = 0;

    virtual int FindRowByColValue(const int nCol, const NFIDataList& var, NFIDataList& varResult) = 0;
    virtual int FindInt(const int nCol, const NFINT64 value, NFIDataList& varResult) = 0;
    virtual int FindFloat(const int nCol, const float value, NFIDataList& varResult) = 0;
    virtual int FindDouble(const int nCol, const double value, NFIDataList& varResult) = 0;
    virtual int FindString(const int nCol, const char* value, NFIDataList& varResult) = 0;
    virtual int FindObject(const int nCol, const NFIDENTID& value, NFIDataList& varResult) = 0;
    virtual int SortByCol(const int nCol, const bool bOrder, NFIDataList& varResult)
    {
        return 0;
    };

    virtual int FindRowByColValue(const std::string& strColTag, const NFIDataList& var, NFIDataList& varResult) = 0;
    virtual int FindInt(const std::string& strColTag, const NFINT64 value, NFIDataList& varResult) = 0;
    virtual int FindFloat(const std::string& strColTag, const float value, NFIDataList& varResult) = 0;
    virtual int FindDouble(const std::string& strColTag, const double value, NFIDataList& varResult) = 0;
    virtual int FindString(const std::string& strColTag, const char* value, NFIDataList& varResult) = 0;
    virtual int FindObject(const std::string& strColTag, const NFIDENTID& value, NFIDataList& varResult) = 0;
    virtual int FindPointer(const std::string& strColTag, const void* value, NFIDataList& varResult) = 0;
    virtual int SortByTag(const std::string& strColTag, const bool bOrder,  NFIDataList& varResult)
    {
        return 0;
    };

    virtual bool Remove(const int nRow) = 0;
    virtual bool Remove(NFIDataList& varRows) //need to optimize
    {
        for (int i  = 0; i < varRows.GetCount(); ++i)
        {
            Remove(varRows.Int(i));
        }

        return true;
    }

    virtual bool Clear() = 0;

    virtual void AddRecordHook(const RECORD_EVENT_FUNCTOR_PTR& cb) = 0;

    virtual const bool GetSave() = 0;
    virtual const bool GetView() = 0;
    virtual const bool GetPublic() = 0;
    virtual const bool GetPrivate() = 0;
    virtual const int GetIndex() = 0;
    virtual const std::string& GetName() const = 0;
    virtual const NFIDataList& GetInitData() const = 0;
    virtual const NFIDataList& GetKeyState() const = 0;
    virtual const NFIDataList& GetInitDesc() const = 0;
    virtual const NFIDataList& GetTag() const = 0;

    virtual void SetSave(const bool bSave) = 0;
    virtual void SetView(const bool bView) = 0;
    virtual void SetPublic(const bool bPublic) = 0;
    virtual void SetPrivate(const bool bPrivate) = 0;
    virtual void SetName(const char* strName) = 0;

    virtual const TRECORDVEC& GetRecordVec() const = 0;

    virtual const NFIDataList& GetRelatedRecord() const = 0;
    virtual bool GetRelatedTag(const std::string& strSrcTag, const std::string& strRelatedRecord, std::string& strRelatedTag) = 0;
};

#endif
