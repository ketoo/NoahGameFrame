// -------------------------------------------------------------------------
//    @FileName         :    NFCRecord.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCRecord
//
// -------------------------------------------------------------------------

#ifndef _GT_RECORD_H_
#define _GT_RECORD_H_

#include <vector>
#include "NFIRecord.h"
#include "NFCValueList.h"

class NFCRecord : public NFIRecord
{
public:
    NFCRecord();
    NFCRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIValueList& valueList, const NFIValueList& keyList, const NFIValueList& descList, int nMaxRow, bool bPublic,  bool bPrivate,  bool bSave, int nIndex);

    virtual ~NFCRecord();

    virtual bool IsUsed(const int nRow) const;
	virtual bool IsKey(const int nRow) const;

	virtual bool SetUsed(const int nRow, const int bUse);
    virtual bool SetKey(const int nRow, const int bKey);

    virtual int GetCols() const;

    virtual int GetRows() const;

    virtual VARIANT_TYPE GetColType(const int nCol) const;

    // 添加数据
    virtual int AddRow(const int nRow);

    virtual int AddRow(const int nRow, const NFIValueList& var);

    virtual bool SetInt(const int nRow, const int nCol, const int value);

    virtual bool SetFloat(const int nRow, const int nCol, const float value);

    virtual bool SetDouble(const int nRow, const int nCol, const double value);

    virtual bool SetString(const int nRow, const int nCol, const char* value);

    virtual bool SetObject(const int nRow, const int nCol, const NFIDENTID& value);

    virtual bool SetPointer(const int nRow, const int nCol, const void* value);

    virtual bool QueryRow(const int nRow, NFIValueList& varList);

    virtual bool SwapRowInfo(const int nOriginRow, const int nTargetRow);

    virtual int QueryInt(const int nRow, const int nCol) const;

    virtual float QueryFloat(const int nRow, const int nCol) const;

    virtual double QueryDouble(const int nRow, const int nCol) const;

    virtual const std::string& QueryString(const int nRow, const int nCol) const;

    virtual NFIDENTID QueryObject(const int nRow, const int nCol) const;

    virtual void* QueryPointer(const int nRow, const int nCol) const;

    virtual int FindRowByColValue(const int nCol, const NFIValueList& var, NFIValueList& varResult);

    virtual int FindInt(const int nCol, const int value, NFIValueList& varResult);

    virtual int FindFloat(const int nCol, const float value, NFIValueList& varResult);

    virtual int FindDouble(const int nCol, const double value, NFIValueList& varResult);

    virtual int FindString(const int nCol, const char* value, NFIValueList& varResult);

    virtual int FindObject(const int nCol, const NFIDENTID& value, NFIValueList& varResult);

    virtual int FindPointer(const int nCol, const void* value, NFIValueList& varResult);

    virtual bool Remove(const int nRow);

    virtual bool Clear();

    virtual void AddRecordHook(const RECORD_EVENT_FUNCTOR_PTR& cb);

    virtual const bool GetSave();

    virtual const bool GetPublic();

    virtual const bool GetPrivate();

    virtual const int GetIndex();

    virtual const std::string& GetName();

    virtual void SetSave(const bool bSave);

    virtual void SetPublic(const bool bPublic);

    virtual void SetPrivate(const bool bPrivate);

    virtual void SetName(const char* strName);

	virtual const NFIValueList& GetInitData();
    virtual const NFIValueList& GetKeyState();
    virtual const NFIValueList& GetInitDesc();

    virtual const TRECORDVEC& GetRecordVec() const
    {
        return mtRecordVec;
    }

protected:
    int GetPos(int nRow, int nCol) const;
    bool ValidPos(int nRow, int nCol) const;
    bool ValidRow(int nRow) const;
    bool ValidCol(int nCol) const;

    void OnEventHandler(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIValueList& oldVar, const NFIValueList& newVar);

protected:
    //记录这个表的Key类型，那样在读取和设置的时候才能保持正确
    NFCValueList mVarRecordType;//初始值类型--应该引用静态的(或者智能指针)，节约大量内存
	NFCValueList mVarRecordDesc;//初始值描述--应该引用静态的(或者智能指针)，节约大量内存
    NFCValueList mVarRecordKey;//各row是否是KEY--应该引用静态的(或者智能指针)，节约大量内存
    TRECORDVEC mtRecordVec;//真的数据
    std::vector<int> mVecUsedState;

    int mnMaxRow;

    NFIDENTID mSelf;
    bool mbSave;
    bool mbPublic;
    bool mbPrivate;
	bool mbHasKey;
    int mnIndex;
    std::string mstrRecordName;

    typedef std::vector<RECORD_EVENT_FUNCTOR_PTR> TRECORDCALLBACKEX;
    TRECORDCALLBACKEX mtRecordCallback;
};

#endif
