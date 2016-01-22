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

class LinqData
{
public:
	void Init(NF_SHARE_PTR<NFIRecord> xRecord, const int nColSingleKey, TDATA_TYPE eColSingleType, const int nColMultiKey, TDATA_TYPE eColMultiType)
	{
		mxColData.xRecord = xRecord;

		mxColData.nColSingleKey = nColSingleKey;
		mxColData.eColSingleType = eColSingleType;
		mxColData.nColMultiKey = nColMultiKey;
		mxColData.eColMultiType = eColMultiType;
	}

	void OnEventHandler(const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar)
	{
		if (nCol == mxColData.nColSingleKey)
		{
			OnSinglekeyEventHandler(nOpType, nRow, nCol, oldVar, newVar);
		}

		if(nCol == mxColData.nColMultiKey)
		{
			OnMultiKeyEventHandler(nOpType, nRow, nCol, oldVar, newVar);
		}
	}

protected:
	void OnSinglekeyEventHandler(const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar)
	{
		TDATA_TYPE eColType = mxColData.xRecord->GetColType(nCol);

		switch (nOpType)
		{
		case NFIRecord::RecordOptype::Add:
			{
				switch (eColType)
				{
				case TDATA_TYPE::TDATA_STRING:
					{
						//新数据
						const std::string& strData = mxColData.xRecord->GetString(nRow, nCol);
						mxSingleKeyStr.AddElement(strData, NF_SHARE_PTR<int>(new int(nRow)));
					}
					break;
				case TDATA_TYPE::TDATA_INT:
					{
						const NFINT64 nData = mxColData.xRecord->GetInt(nRow, nCol);
						mxSingleKeyInt.AddElement(nData, NF_SHARE_PTR<int>(new int(nRow)));
					}
					break;
				case TDATA_TYPE::TDATA_OBJECT:
					{
						const NFGUID& xID = mxColData.xRecord->GetObject(nRow, nCol);
						mxSingleKeyObject.AddElement(xID, NF_SHARE_PTR<int>(new int(nRow)));
					}
					break;
				default:
					break;
				}
			}
			break;
		case NFIRecord::RecordOptype::Del:
			{
				switch (eColType)
				{
				case TDATA_TYPE::TDATA_STRING:
					{
						//新数据
						const std::string& strData = mxColData.xRecord->GetString(nRow, nCol);
						mxSingleKeyStr.RemoveElement(strData);
					}
					break;
				case TDATA_TYPE::TDATA_INT:
					{
						const NFINT64 nData = mxColData.xRecord->GetInt(nRow, nCol);
						mxSingleKeyInt.RemoveElement(nData);
					}
					break;
				case TDATA_TYPE::TDATA_OBJECT:
					{
						const NFGUID& xID = mxColData.xRecord->GetObject(nRow, nCol);
						mxSingleKeyObject.RemoveElement(xID);
					}
					break;
				default:
					break;
				}
			}
			break;
		case NFIRecord::RecordOptype::Swap:
		case NFIRecord::RecordOptype::UpData:
			{
				//既然是key，那么就只能单独添加和删除,不存在更新，否是是使用错误
				switch (eColType)
				{
				case TDATA_TYPE::TDATA_STRING:
					{

					}
					break;
				case TDATA_TYPE::TDATA_INT:
					{

					}
					break;
				case TDATA_TYPE::TDATA_OBJECT:
					{

					}
					break;
				default:
					break;
				}
			}
			break;
		case NFIRecord::RecordOptype::Create:
			break;
			{
				switch (eColType)
				{
				case TDATA_TYPE::TDATA_STRING:
					{

					}
					break;
				case TDATA_TYPE::TDATA_INT:
					{

					}
					break;
				case TDATA_TYPE::TDATA_OBJECT:
					{

					}
					break;
				default:
					break;
				}
			}
			break;
		case NFIRecord::RecordOptype::Cleared:
			break;
		case NFIRecord::RecordOptype::Sort:
			break;
		default:
			break;
		}
	}

	void OnMultiKeyEventHandler(const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar)
	{
		TDATA_TYPE eColType = mxColData.xRecord->GetColType(nCol);

		switch (nOpType)
		{
		case NFIRecord::RecordOptype::Add:
			{
				switch (eColType)
				{
				case TDATA_TYPE::TDATA_STRING:
					{
						//新数据
						const std::string& strData = mxColData.xRecord->GetString(nRow, nCol);
						mxSingleKeyStr.AddElement(strData, NF_SHARE_PTR<int>(new int(nRow)));
					}
					break;
				case TDATA_TYPE::TDATA_INT:
					{
						const NFINT64 nData = mxColData.xRecord->GetInt(nRow, nCol);
						mxSingleKeyInt.AddElement(nData, NF_SHARE_PTR<int>(new int(nRow)));
					}
					break;
				case TDATA_TYPE::TDATA_OBJECT:
					{
						const NFGUID& xID = mxColData.xRecord->GetObject(nRow, nCol);
						mxSingleKeyObject.AddElement(xID, NF_SHARE_PTR<int>(new int(nRow)));
					}
					break;
				default:
					break;
				}
			}
			break;
		case NFIRecord::RecordOptype::Del:
			{
			}
			break;
		case NFIRecord::RecordOptype::UpData:
			{
			}
			break;
		case NFIRecord::RecordOptype::Swap:
			{
			}
			break;
		case NFIRecord::RecordOptype::Create:
			break;
		case NFIRecord::RecordOptype::Cleared:
			break;
		case NFIRecord::RecordOptype::Sort:
			break;
		default:
			break;
		}
	}
protected:

	struct LinqColData
	{
		LinqColData()
		{
			nColSingleKey = 0;
			eColSingleType = TDATA_UNKNOWN;
			nColMultiKey = 0;
			eColMultiType = TDATA_UNKNOWN;
		};

		NF_SHARE_PTR<NFIRecord> xRecord;

		int nColSingleKey;
		TDATA_TYPE eColSingleType;
		int nColMultiKey;
		TDATA_TYPE eColMultiType;
	};

protected:

	LinqColData mxColData;

	//单key:col->row
	NFMapEx<std::string, int> mxSingleKeyStr;
	NFMapEx<NFINT64, int> mxSingleKeyInt;
	NFMapEx<NFGUID, int> mxSingleKeyObject;

	//多key:col->row_list<row, used_state>
	NFMapEx<std::string, NFMapEx<int, int> > mxMultiKeyStr;
	NFMapEx<NFINT64, NFMapEx<int, int> > mxultiKeyInt;
	NFMapEx<NFGUID, NFMapEx<int, int> > mxultiKeyObject;

private:
};

class NFCRecord : public NFIRecord
{
public:
    NFCRecord();
    NFCRecord(const NFGUID& self, const std::string& strRecordName, const NFIDataList& valueList, const NFIDataList& keyList, const NFIDataList& descList, const NFIDataList& tagList, const NFIDataList& relateRecordList, int nMaxRow, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex);

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
    virtual bool SetFloat(const int nRow, const int nCol, const double value);
    virtual bool SetString(const int nRow, const int nCol, const char* value);
    virtual bool SetObject(const int nRow, const int nCol, const NFGUID& value);

    virtual bool SetInt(const int nRow, const std::string& strColTag, const NFINT64 value);
    virtual bool SetFloat(const int nRow, const std::string& strColTag, const double value);
    virtual bool SetString(const int nRow, const std::string& strColTag, const char* value);
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
    virtual int FindObject(const int nCol, const NFGUID& value, NFIDataList& varResult);

    virtual int FindRowByColValue(const std::string& strColTag, const NFIDataList& var, NFIDataList& varResult);
    virtual int FindInt(const std::string& strColTag, const NFINT64 value, NFIDataList& varResult);
    virtual int FindFloat(const std::string& strColTag, const double value, NFIDataList& varResult);
    virtual int FindString(const std::string& strColTag, const char* value, NFIDataList& varResult);
    virtual int FindObject(const std::string& strColTag, const NFGUID& value, NFIDataList& varResult);

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

    void OnEventHandler(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar);

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

    NFGUID mSelf;
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
	std::map<NFGUID, int> mxObjectKeyMap;

    typedef std::vector<RECORD_EVENT_FUNCTOR_PTR> TRECORDCALLBACKEX;
    TRECORDCALLBACKEX mtRecordCallback;
};

#endif
