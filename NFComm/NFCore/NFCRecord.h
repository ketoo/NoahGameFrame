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
#include "NFCDataList.h"

class NFCRecord : public NFIRecord
{
public:
    NFCRecord();
    NFCRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIDataList& valueList, const NFIDataList& keyList, const NFIDataList& descList, const NFIDataList& tagList, const NFIDataList& relateRecordList, int nMaxRow, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex);

    virtual ~NFCRecord();

    virtual bool IsUsed(const int nRow) const;
    virtual bool IsKey(const int nCol) const;

    virtual bool SetUsed(const int nRow, const int bUse);
    virtual bool SetKey(const int nCol, const int bKey);

    virtual int GetCols() const;

    virtual int GetRows() const;

    virtual TDATA_TYPE GetColType(const int nCol) const;
    virtual const std::string& GetColTag(const int nCol) const;

    // 添加数据
    virtual int AddRow(const int nRow);

    virtual int AddRow(const int nRow, const NFIDataList& var);

    virtual bool SetInt(const int nRow, const int nCol, const NFINT64 value);
    virtual bool SetFloat(const int nRow, const int nCol, const float value);
    virtual bool SetDouble(const int nRow, const int nCol, const double value);
    virtual bool SetString(const int nRow, const int nCol, const char* value);
    virtual bool SetObject(const int nRow, const int nCol, const NFIDENTID& value);
    virtual bool SetPointer(const int nRow, const int nCol, const void* value);

    virtual bool SetInt(const int nRow, const std::string& strColTag, const NFINT64 value);
    virtual bool SetFloat(const int nRow, const std::string& strColTag, const float value);
    virtual bool SetDouble(const int nRow, const std::string& strColTag, const double value);
    virtual bool SetString(const int nRow, const std::string& strColTag, const char* value);
    virtual bool SetObject(const int nRow, const std::string& strColTag, const NFIDENTID& value);
    virtual bool SetPointer(const int nRow, const std::string& strColTag, const void* value);

    virtual bool QueryRow(const int nRow, NFIDataList& varList);

    virtual bool SwapRowInfo(const int nOriginRow, const int nTargetRow);

    virtual NFINT64 GetInt(const int nRow, const int nCol) const;
    virtual float GetFloat(const int nRow, const int nCol) const;
    virtual double GetDouble(const int nRow, const int nCol) const;
    virtual const std::string& GetString(const int nRow, const int nCol) const;
    virtual NFIDENTID GetObject(const int nRow, const int nCol) const;
    virtual void* GetPointer(const int nRow, const int nCol) const;

    virtual NFINT64 GetInt(const int nRow, const std::string& strColTag) const;
    virtual float GetFloat(const int nRow, const std::string& strColTag) const;
    virtual double GetDouble(const int nRow, const std::string& strColTag) const;
    virtual const std::string& GetString(const int nRow, const std::string& strColTag) const;
    virtual NFIDENTID GetObject(const int nRow, const std::string& strColTag) const;
    virtual void* GetPointer(const int nRow, const std::string& strColTag) const;

    virtual int FindRowByColValue(const int nCol, const NFIDataList& var, NFIDataList& varResult);
    virtual int FindInt(const int nCol, const NFINT64 value, NFIDataList& varResult);
    virtual int FindFloat(const int nCol, const float value, NFIDataList& varResult);
    virtual int FindDouble(const int nCol, const double value, NFIDataList& varResult);
    virtual int FindString(const int nCol, const char* value, NFIDataList& varResult);
    virtual int FindObject(const int nCol, const NFIDENTID& value, NFIDataList& varResult);
    virtual int FindPointer(const int nCol, const void* value, NFIDataList& varResult);

    virtual int FindRowByColValue(const std::string& strColTag, const NFIDataList& var, NFIDataList& varResult);
    virtual int FindInt(const std::string& strColTag, const NFINT64 value, NFIDataList& varResult);
    virtual int FindFloat(const std::string& strColTag, const float value, NFIDataList& varResult);
    virtual int FindDouble(const std::string& strColTag, const double value, NFIDataList& varResult);
    virtual int FindString(const std::string& strColTag, const char* value, NFIDataList& varResult);
    virtual int FindObject(const std::string& strColTag, const NFIDENTID& value, NFIDataList& varResult);
    virtual int FindPointer(const std::string& strColTag, const void* value, NFIDataList& varResult);

    virtual bool Remove(const int nRow);

    virtual bool Clear();

    virtual void AddRecordHook(const RECORD_EVENT_FUNCTOR_PTR& cb);

    virtual const bool GetSave();

    virtual const bool GetView();

    virtual const bool GetPublic();

    virtual const bool GetPrivate();

    virtual const int GetIndex();

    virtual const std::string& GetName() const;

    virtual void SetSave(const bool bSave);

    virtual void SetView(const bool bView);

    virtual void SetPublic(const bool bPublic);

    virtual void SetPrivate(const bool bPrivate);

    virtual void SetName(const char* strName);

    virtual const NFIDataList& GetInitData() const;
    virtual const NFIDataList& GetKeyState() const;
    virtual const NFIDataList& GetInitDesc() const;
    virtual const NFIDataList& GetTag() const;
    virtual const TRECORDVEC& GetRecordVec() const;
    virtual bool GetRelatedTag(const std::string& strSrcTag, const std::string& strRelatedRecord, std::string& strRelatedTag);
    virtual const NFIDataList& GetRelatedRecord() const;
protected:
    int GetPos(int nRow, int nCol) const;

    int GetCol(const std::string& strTag) const;

    bool ValidPos(int nRow, int nCol) const;
    bool ValidRow(int nRow) const;
    bool ValidCol(int nCol) const;

    void OnEventHandler(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar);

protected:
    //记录这个表的Key类型，那样在读取和设置的时候才能保持正确
    NFCDataList mVarRecordType;//初始值类型--应该引用静态的(或者智能指针)，节约大量内存
    NFCDataList mVarRecordTag;//col的tag值--应该引用静态的(或者智能指针)，节约大量内存
    NFCDataList mVarRecordDesc;//初始值描述--应该引用静态的(或者智能指针)，节约大量内存
    NFCDataList mVarRecordKey;//各row是否是KEY--应该引用静态的(或者智能指针)，节约大量内存
    NFCDataList mVarRecordRelation; // 表中某个字段与另外一张表中某字段对应关系
    TRECORDVEC mtRecordVec;//真的数据
    std::vector<int> mVecUsedState;

    std::map<std::string, int> mmTag;//tag->col转换

    typedef std::pair<int, std::string> RelationRecordColType;
    typedef std::map<RelationRecordColType, std::string> RelationRecordMap;
    RelationRecordMap mmRelationRecord; // std::map<pair<col, RelationRecord, RelationRecord>, RelationRecordColTag>

    int mnMaxRow;

    NFIDENTID mSelf;
    bool mbSave;
    bool mbView;
    bool mbPublic;
    bool mbPrivate;
    int mnKeyCol;
    int mnIndex;
    std::string mstrRecordName;

	//col-><int_key, row_value>//整形key,暂时只支持一个key
	std::map<NFINT64, int> mxIntKeyMap;
	//col-><string_key, row_value>//字符串key,暂时只支持一个key
	std::map<std::string, int> mxStringKeyMap;
	//col-><object_key, row_value>//对象key,暂时只支持一个key
	std::map<NFIDENTID, int> mxObjectKeyMap;

    typedef std::vector<RECORD_EVENT_FUNCTOR_PTR> TRECORDCALLBACKEX;
    TRECORDCALLBACKEX mtRecordCallback;
};

#endif
