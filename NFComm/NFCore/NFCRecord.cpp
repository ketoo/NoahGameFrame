// -------------------------------------------------------------------------
//    @FileName         :    NFCRecord.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCRecord
//
// -------------------------------------------------------------------------

#include <exception>
#include "NFCRecord.h"

NFCRecord::NFCRecord()
{
    mSelf = NFIDENTID();
    mbSave = false;
    mbPublic = false;
    mbPrivate = false;
    mnKeyCol = -1;
    mstrRecordName = "";
    mnMaxRow = 0;

}

NFCRecord::NFCRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIDataList& valueList, const NFIDataList& keyList, const NFIDataList& descList, const NFIDataList& tagList, const NFIDataList& relateRecordList, int nMaxRow, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex)
{
    mVarRecordType = valueList;
    mVarRecordDesc = descList;
    mVarRecordKey = keyList;
    mVarRecordTag = tagList;
    mVarRecordRelation = relateRecordList;

    mSelf = self;
    mbSave = bSave;
    mbView = bView;
    mbPublic = bPublic;
    mbPrivate = bPrivate;
    mnIndex = nIndex;

    mstrRecordName = strRecordName;

    mnMaxRow = nMaxRow;
    //确定大小
    mVecUsedState.resize(mnMaxRow);

    for (int i = 0; i < mnMaxRow; i++)
    {
        mVecUsedState[i] = 0;
    }

    for (int i = 0; i < keyList.GetCount(); ++i)
    {
        if (keyList.Int(i) > 0)
        {
            mnKeyCol = i;
            break;
        }
    }

    // TODO:可以考虑在此处直接加入默认值
    for (int i = 0; i < GetRows() * GetCols(); i++)
    {
        mtRecordVec.push_back(NF_SHARE_PTR<NFIDataList::TData>());
    }

    for (int i = 0; i < mVarRecordTag.GetCount(); ++i)
    {
        if (!mVarRecordTag.String(i).empty())
        {
            mmTag[mVarRecordTag.String(i)] = i;
        }
    }

    for (int i = 0; i < mVarRecordRelation.GetCount(); ++i)
    {
        const std::string& strRelationData = mVarRecordRelation.String(i);
        if (!strRelationData.empty())
        {
            continue;
        }

        NFCDataList relationDataList;
        relationDataList.Split(strRelationData.c_str(), ";");
        for (int j = 0; j < relationDataList.GetCount(); ++j)
        {
            const std::string& strSingleRelation = relationDataList.String(j);
            if (strSingleRelation.empty())
            {
                continue;
            }

            NFCDataList singleRelationList;
            singleRelationList.Split(strRelationData.c_str(), ",");
            const std::string& strRecord = singleRelationList.String(0);
            const std::string& strRecordTag = singleRelationList.String(1);
            mmRelationRecord.insert(RelationRecordMap::value_type(RelationRecordColType(i, strRecord), strRecordTag));
        }
    }
}

NFCRecord::~NFCRecord()
{
    for (auto iter = mtRecordVec.begin(); iter != mtRecordVec.end(); ++iter)
    {
        iter->reset();
    }

    for (auto iter = mtRecordCallback.begin(); iter != mtRecordCallback.end(); ++iter)
    {
        iter->reset();
    }

    mtRecordVec.clear();
    mVecUsedState.clear();
    mtRecordCallback.clear();
}

int NFCRecord::GetCols() const
{
    return mVarRecordType.GetCount();
}

int NFCRecord::GetRows() const
{
    return mnMaxRow;
}

TDATA_TYPE NFCRecord::GetColType(const int nCol) const
{
    return mVarRecordType.Type(nCol);
}

const std::string& NFCRecord::GetColTag(const int nCol) const
{
    return mVarRecordTag.String(nCol);
}

// 添加数据
int NFCRecord::AddRow(const int nRow)
{
    return AddRow(nRow, mVarRecordType);
}

//类型，新值，原值==给原值赋新值，没有就new
bool ValidAdd(TDATA_TYPE eType, const NFIDataList::TData& var, NF_SHARE_PTR<NFIDataList::TData>& pVar)
{
    if (var.nType != eType)
    {
        return false;
    }

    if (!pVar.get())
    {
        if (!NFIDataList::Valid(var))
        {
            return false;
        }

        pVar = NF_SHARE_PTR<NFIDataList::TData>(NF_NEW NFIDataList::TData());
        pVar->nType = eType;
        switch (eType)
        {
        case TDATA_UNKNOWN:
            break;
        case TDATA_INT:
            pVar->variantData = (NFINT64)0;
            break;
        case TDATA_FLOAT:
            pVar->variantData = (float)0.0f;
            break;
        case TDATA_DOUBLE:
            pVar->variantData = (double)0.0f;
            break;
        case TDATA_STRING:
            pVar->variantData = std::string("");
            break;
        case TDATA_OBJECT:
            pVar->variantData = NFIDENTID();
            break;
        case TDATA_MAX:
            break;
        default:
            break;
        }
    }

    if (pVar->nType != eType)
    {
        return false;
    }

    if (pVar->variantData == var.variantData)
    {
        return false;
    }

    return true;
}

bool ValidSet(TDATA_TYPE eType, const NFIDataList::TData& var, NF_SHARE_PTR<NFIDataList::TData>& pVar)
{
    if (var.nType != eType)
    {
        return false;
    }

    if (!pVar.get())
    {
        if (!NFIDataList::Valid(var))
        {
            return false;
        }

        pVar = NF_SHARE_PTR<NFIDataList::TData>(NF_NEW NFIDataList::TData());
        pVar->nType = eType;
    }

    if (pVar->nType != eType)
    {
        return false;
    }

    if (pVar->variantData == var.variantData)
    {
        return false;
    }

    pVar->variantData = var.variantData;

    return true;
}

int NFCRecord::AddRow(const int nRow, const NFIDataList& var)
{
    int nFindRow = nRow;
    if (nFindRow >= mnMaxRow)
    {
        return -1;
    }

    if (var.GetCount() != GetCols())
    {
        return -1;
    }

    if (nFindRow < 0)
    {
        for (int i = 0; i < mnMaxRow; i++)
        {
			if(!IsUsed(i))
			{
				nFindRow = i;
				break;
			}
        }
    }

    if (nFindRow < 0)
    {
        return -1;
    }

    bool bType = true;
    for (int i = 0; i < GetCols(); ++i)
    {
        if (var.Type(i) != GetColType(i))
        {
            bType = false;
            break;
        }

		if (IsKey(i))
		{
			//为key,value->Row，而key不能重复
			if (TDATA_TYPE::TDATA_INT == var.Type(i))
			{
				if (mxIntKeyMap.find(var.Int(i)) == mxIntKeyMap.end())
				{
					mxIntKeyMap.insert(std::map<NFINT64, int>::value_type(var.Int(i), nFindRow));
				}
				else
				{
					return -1;
				}
			}
			else if (TDATA_TYPE::TDATA_STRING == var.Type(i))
			{
				if (mxStringKeyMap.find(var.String(i)) == mxStringKeyMap.end())
				{
					mxStringKeyMap.insert(std::map<std::string, int>::value_type(var.String(i), nFindRow));
				}
				else
				{
					return -1;
				}
			}
			else if (TDATA_TYPE::TDATA_OBJECT == var.Type(i))
			{
				if (mxObjectKeyMap.find(var.Object(i)) == mxObjectKeyMap.end())
				{
					mxObjectKeyMap.insert(std::map<NFIDENTID, int>::value_type(var.Object(i), nFindRow));
				}
				else
				{
					return -1;
				}
			}
		}
    }

    if (bType)
    {
		SetUsed(nFindRow, 1);
        for (int i = 0; i < GetCols(); ++i)
        {
            NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nFindRow, i));//GetTData(nFindRow, i);
            if (!ValidSet(GetColType(i), *var.GetStackConst(i), pVar))
            {
                //添加失败--不存在这样的情况，因为类型上面已经监测过，如果返回的话，那么添加的数据是0的话就会返回，导致结果错误
                //                 mVecUsedState[nFindRow] = 0;
                //                 pVar.reset();
                //                 return -1;
            }
        }

        OnEventHandler(mSelf, mstrRecordName, Add, nFindRow, 0, mVarRecordType, var);
    }

    return nFindRow;
}

bool NFCRecord::SetInt(const int nRow, const int nCol, const NFINT64 value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

	if (TDATA_TYPE::TDATA_INT != GetColType(nCol))
	{
		return false;
	}

	if (!IsUsed(nRow))
	{
		return false;
	}

    NFIDataList::TData var;
    var.nType = TDATA_INT;
    var.variantData = value;

    NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

    if (!ValidAdd(TDATA_INT, var, pVar))
    {
        return false;
    }

    //还要把以前的key的内容删除
    if (IsKey(nCol))
    {
        NFINT64 nOldValue = boost::get<NFINT64>(pVar->variantData);
        mxIntKeyMap.erase(nOldValue);

        if (mxIntKeyMap.find(value) != mxIntKeyMap.end())
        {
            return false;
        }

        mxIntKeyMap.insert(std::map<NFINT64, int>::value_type(value, nRow));
    }

    NFCDataList oldValue;
    NFCDataList newValue;

    oldValue.Append(*pVar);
    newValue.Add(value);

    pVar->variantData = value;

    OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

    return true;
}

bool NFCRecord::SetInt(const int nRow, const std::string& strColTag, const NFINT64 value)
{
    int nCol = GetCol(strColTag);
    return SetInt(nRow, nCol, value);
}

bool NFCRecord::SetFloat(const int nRow, const int nCol, const float value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

	if (TDATA_TYPE::TDATA_FLOAT != GetColType(nCol))
	{
		return false;
	}

	if (!IsUsed(nRow))
	{
		return false;
	}

    NFIDataList::TData var;
    var.nType = TDATA_FLOAT;
    var.variantData = value;

    NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

    if (!ValidAdd(TDATA_FLOAT, var, pVar))
    {
        return false;
    }

    NFCDataList oldValue;
    NFCDataList newValue;

    oldValue.Append(*pVar);
    newValue.Add(value);

    pVar->variantData = value;

    OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

    return true;
}

bool NFCRecord::SetFloat(const int nRow, const std::string& strColTag, const float value)
{
    int nCol = GetCol(strColTag);
    return SetFloat(nRow, nCol, value);
}

bool NFCRecord::SetDouble(const int nRow, const int nCol, const double value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

	if (TDATA_TYPE::TDATA_DOUBLE != GetColType(nCol))
	{
		return false;
	}

	if (!IsUsed(nRow))
	{
		return false;
	}

    NFIDataList::TData var;
    var.nType = TDATA_DOUBLE;
    var.variantData = value;

    NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

    if (!ValidAdd(TDATA_DOUBLE, var, pVar))
    {
        return false;
    }

    NFCDataList oldValue;
    NFCDataList newValue;

    oldValue.Append(*pVar);
    newValue.Add(value);

    pVar->variantData = value;

    OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

    return true;
}

bool NFCRecord::SetDouble(const int nRow, const std::string& strColTag, const double value)
{
    int nCol = GetCol(strColTag);
    return SetDouble(nRow, nCol, value);
}

bool NFCRecord::SetString(const int nRow, const int nCol, const char* value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

	if (TDATA_TYPE::TDATA_STRING != GetColType(nCol))
	{
		return false;
	}

	if (!IsUsed(nRow))
	{
		return false;
	}

    NFIDataList::TData var;
    var.nType = TDATA_STRING;
    var.variantData = std::string(value);

    NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

    if (!ValidAdd(TDATA_STRING, var, pVar))
    {
        return false;
    }

    //还要把以前的key的内容删除
    if (IsKey(nCol))
    {
        std::string& strOldValue = boost::get<std::string>(pVar->variantData);
        mxStringKeyMap.erase(strOldValue);

        if (mxStringKeyMap.find(value) != mxStringKeyMap.end())
        {
            return false;
        }

        mxStringKeyMap.insert(std::map<std::string, int>::value_type(value, nRow));
    }


    NFCDataList oldValue;
    NFCDataList newValue;

    oldValue.Append(*pVar);
    newValue.Add(value);

    pVar->variantData = (std::string)value;

    OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

    return true;
}

bool NFCRecord::SetString(const int nRow, const std::string& strColTag, const char* value)
{
    int nCol = GetCol(strColTag);
    return SetString(nRow, nCol, value);
}

bool NFCRecord::SetObject(const int nRow, const int nCol, const NFIDENTID& value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

	if (TDATA_TYPE::TDATA_OBJECT != GetColType(nCol))
	{
		return false;
	}

	if (!IsUsed(nRow))
	{
		return false;
	}

    NFIDataList::TData var;
    var.nType = TDATA_OBJECT;
    var.variantData = value;
	
    NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

    if (!ValidAdd(TDATA_OBJECT, var, pVar))
    {
        return false;
    }
    //还要把以前的key的内容删除
    if (IsKey(nCol))
    {
        NFIDENTID xOldValue = boost::get<NFIDENTID>(pVar->variantData);
        mxObjectKeyMap.erase(xOldValue);

        if (mxObjectKeyMap.find(value) != mxObjectKeyMap.end())
        {
            return false;
        }

        mxObjectKeyMap.insert(std::map<NFIDENTID, int>::value_type(value, nRow));
    }

    NFCDataList oldValue;
    NFCDataList newValue;

    oldValue.Append(*pVar);
    newValue.Add(value);

    pVar->variantData = value;

    OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

    return true;
}

bool NFCRecord::SetObject(const int nRow, const std::string& strColTag, const NFIDENTID& value)
{
    int nCol = GetCol(strColTag);
    return SetObject(nRow, nCol, value);
}

bool NFCRecord::SetPointer(const int nRow, const int nCol, const void* value)
{

    return false;
}

bool NFCRecord::SetPointer(const int nRow, const std::string& strColTag, const void* value)
{
    int nCol = GetCol(strColTag);
    return SetPointer(nRow, nCol, value);
}

// 获得数据
bool NFCRecord::QueryRow(const int nRow, NFIDataList& varList)
{
    if (!ValidRow(nRow))
    {
        return false;
    }

	if (!IsUsed(nRow))
	{
		return false;
	}

    varList.Clear();
    for (int i = 0; i < GetCols(); ++i)
    {
        NF_SHARE_PTR<NFIDataList::TData> pVar = mtRecordVec.at(GetPos(nRow, i));
        if (pVar.get())
        {
            varList.Append(*pVar);
        }
        else
        {
            switch (GetColType(i))
            {
                case TDATA_INT:
                    varList.Add(NFINT64(0));
                    break;

                case TDATA_FLOAT:
                    varList.Add(0.0f);
                    break;

                case TDATA_DOUBLE:
                    varList.Add(0.0f);
                    break;

                case TDATA_STRING:
                    varList.Add(NULL_STR.c_str());
                    break;

                case TDATA_OBJECT:
                    varList.Add(NFIDENTID());
                    break;
                default:
                    return false;
                    break;
            }
        }
    }

    if (varList.GetCount() != GetCols())
    {
        return false;
    }

    return true;
}

NFINT64 NFCRecord::GetInt(const int nRow, const int nCol) const
{
    if (!ValidPos(nRow, nCol))
    {
        return 0;
    }

	if (!IsUsed(nRow))
	{
		return 0;
	}

    const NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar.get())
    {
        return 0;
    }

    if (TDATA_INT == pVar->nType)
    {
        return boost::get<NFINT64>(pVar->variantData);
    }

    return 0;
}

NFINT64 NFCRecord::GetInt(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetInt(nRow, nCol);
}

float NFCRecord::GetFloat(const int nRow, const int nCol) const
{
    if (!ValidPos(nRow, nCol))
    {
        return 0.0f;
    }

	if (!IsUsed(nRow))
	{
		return 0.0f;
	}

    const NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar.get())
    {
        return 0.0f;
    }

    if (TDATA_FLOAT == pVar->nType)
    {
        return boost::get<float>(pVar->variantData);
    }

    return 0.0f;
}

float NFCRecord::GetFloat(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetFloat(nRow, nCol);
}

double NFCRecord::GetDouble(const int nRow, const int nCol) const
{
    if (!ValidPos(nRow, nCol))
    {
        return 0.0f;
    }

	if (!IsUsed(nRow))
	{
		return 0.0f;
	}

    const NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar.get())
    {
        return 0.0f;
    }

    if (TDATA_DOUBLE == pVar->nType)
    {
        return boost::get<double>(pVar->variantData);
    }

    return 0.0f;
}

double NFCRecord::GetDouble(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetDouble(nRow, nCol);
}

const std::string& NFCRecord::GetString(const int nRow, const int nCol) const
{
    if (!ValidPos(nRow, nCol))
    {
        return NULL_STR;
    }

	if (!IsUsed(nRow))
	{
		return NULL_STR;
	}

    const NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar.get())
    {
        return NULL_STR;
    }

    if (TDATA_STRING == pVar->nType)
    {
        return boost::get<const std::string&>(pVar->variantData);
    }

    return NULL_STR;
}

const std::string& NFCRecord::GetString(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetString(nRow, nCol);
}

NFIDENTID NFCRecord::GetObject(const int nRow, const int nCol) const
{
    if (!ValidPos(nRow, nCol))
    {
        return NFIDENTID();
    }

	if (!IsUsed(nRow))
	{
		return NFIDENTID();
	}

    const  NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar.get())
    {
        return NFIDENTID();
    }

    if (TDATA_OBJECT == pVar->nType)
    {
        return boost::get<NFIDENTID>(pVar->variantData);
    }

    return NFIDENTID();
}

NFIDENTID NFCRecord::GetObject(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetObject(nRow, nCol);
}

void* NFCRecord::GetPointer(const int nRow, const int nCol) const
{
    //     NFIDataList::TData* pVar = GetTData(nRow, nCol);
    //     if (pVar && TDATA_POINTER == pVar->nType)
    //     {
    //         return boost::get<void*>(pVar->variantData);
    //     }

    return NULL;
}

void* NFCRecord::GetPointer(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetPointer(nRow, nCol);
}

int NFCRecord::FindRowByColValue(const int nCol, const NFIDataList& var, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    TDATA_TYPE eType = var.Type(0);
    if (eType != mVarRecordType.Type(nCol))
    {
        return -1;
    }

    switch (eType)
    {
        case TDATA_INT:
            return FindInt(nCol, var.Int(nCol), varResult);
            break;

        case TDATA_FLOAT:
            return FindFloat(nCol, var.Float(nCol), varResult);
            break;

        case TDATA_DOUBLE:
            return FindDouble(nCol, var.Double(nCol), varResult);
            break;

        case TDATA_STRING:
            return FindString(nCol, var.String(nCol).c_str(), varResult);
            break;

        case TDATA_OBJECT:
            return FindObject(nCol, var.Object(nCol), varResult);
            break;

        default:
            break;
    }

    return -1;
}

int NFCRecord::FindRowByColValue(const std::string& strColTag, const NFIDataList& var, NFIDataList& varResult)
{
    int nCol = GetCol(strColTag);
    return FindRowByColValue(nCol, var, varResult);
}

int NFCRecord::FindInt(const int nCol, const NFINT64 value, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    if (TDATA_INT != mVarRecordType.Type(nCol))
    {
        return -1;
    }

	if (IsKey(nCol))
	{
		std::map<NFINT64, int>::iterator it = mxIntKeyMap.begin();
		for (; it != mxIntKeyMap.end(); ++it)
		{
			varResult.AddInt(it->second);
		}

		return varResult.GetCount();
	}
	else
	{
		for (int i = 0; i < mnMaxRow; ++i)
		{
			if (!IsUsed(i))
			{
				continue;
			}

			if (GetInt(i, nCol) == value)
			{
				varResult << NFINT32(i);
			}
		}

		return varResult.GetCount();
	}

	return -1;
}

int NFCRecord::FindInt(const std::string& strColTag, const NFINT64 value, NFIDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindInt(nCol, value, varResult);
}

int NFCRecord::FindFloat(const int nCol, const float value, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    if (TDATA_FLOAT != mVarRecordType.Type(nCol))
    {
        return -1;
    }

    for (int i = 0; i < mnMaxRow; ++i)
    {
        if (!IsUsed(i))
        {
            continue;
        }

        if (GetFloat(i, nCol) == value)
        {
            varResult << NFINT32(i);
        }
    }

    return varResult.GetCount();
}

int NFCRecord::FindFloat(const std::string& strColTag, const float value, NFIDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindFloat(nCol, value, varResult);
}

int NFCRecord::FindDouble(const int nCol, const double value, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    if (TDATA_DOUBLE != mVarRecordType.Type(nCol))
    {
        return -1;
    }

    for (int i = 0; i < mnMaxRow; ++i)
    {
        if (!IsUsed(i))
        {
            continue;
        }

        if (GetDouble(i, nCol) == value)
        {
            varResult << NFINT32(i);
        }
    }

    return varResult.GetCount();
}

int NFCRecord::FindDouble(const std::string& strColTag, const double value, NFIDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindDouble(nCol, value, varResult);
}

int NFCRecord::FindString(const int nCol, const char* value, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    if (TDATA_STRING != mVarRecordType.Type(nCol))
    {
        return -1;
    }

	if (IsKey(nCol))
	{
		std::map<std::string, int>::iterator it = mxStringKeyMap.begin();
		for (; it != mxStringKeyMap.end(); ++it)
		{
			varResult.AddInt(it->second);
		}

		return varResult.GetCount();
	}
	else
	{
		for (int i = 0; i < mnMaxRow; ++i)
		{
			if (!IsUsed(i))
			{
				continue;
			}

			const std::string& strData = GetString(i, nCol);
			if (0 == strcmp(strData.c_str(), value))
			{
				varResult << NFINT32(i);
			}
		}

		return varResult.GetCount();
	}

	return -1;
}

int NFCRecord::FindString(const std::string& strColTag, const char* value, NFIDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindString(nCol, value, varResult);
}

int NFCRecord::FindObject(const int nCol, const NFIDENTID& value, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    if (TDATA_OBJECT != mVarRecordType.Type(nCol))
    {
        return -1;
    }

	if (IsKey(nCol))
	{
		std::map<NFIDENTID, int>::iterator it = mxObjectKeyMap.begin();
		for (; it != mxObjectKeyMap.end(); ++it)
		{
			varResult.AddInt(it->second);
		}

		return varResult.GetCount();
	}
	else
	{
		for (int i = 0; i < mnMaxRow; ++i)
		{
			if (!IsUsed(i))
			{
				continue;
			}

			if (GetObject(i, nCol) == value)
			{
				varResult << NFINT32(i);
			}
		}

		return varResult.GetCount();
	}

	return -1;
}

int NFCRecord::FindObject(const std::string& strColTag, const NFIDENTID& value, NFIDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindObject(nCol, value, varResult);
}

int NFCRecord::FindPointer(const int nCol, const void* value, NFIDataList& varResult)
{
    return varResult.GetCount();
}

int NFCRecord::FindPointer(const std::string& strColTag, const void* value, NFIDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindPointer(nCol, value, varResult);
}

bool NFCRecord::Remove(const int nRow)
{
    if (ValidRow(nRow))
    {
        if (IsUsed(nRow))
        {
            OnEventHandler(mSelf, mstrRecordName.c_str(), Del, nRow, 0, NFCDataList(), NFCDataList());

			if (mnKeyCol >= 0)
			{
				TDATA_TYPE xType = GetColType(mnKeyCol);
				if (TDATA_TYPE::TDATA_INT == xType)
				{
					mxIntKeyMap.erase(GetInt(nRow, mnKeyCol));
				}
				else if (TDATA_TYPE::TDATA_STRING == xType)
				{
					mxStringKeyMap.erase(GetString(nRow, mnKeyCol));
				}
				else if (TDATA_TYPE::TDATA_OBJECT == xType)
				{
					mxObjectKeyMap.erase(GetObject(nRow, mnKeyCol));
				}
			}

            mVecUsedState[nRow] = 0;

            //             for (int i = 0; i < GetCols(); ++i)
            //             {
            //                 NF_SHARE_PTR<NFIDataList::TData>& var = mtRecordVec.at(GetPos(nRow, i));
            //                 var.reset();
            //            }
			

            return true;
        }
    }

    return false;
}

bool NFCRecord::Clear()
{
    for (int i = GetRows() - 1; i >= 0; i--)
    {
        Remove(i);
    }

    return true;
}

void NFCRecord::AddRecordHook(const RECORD_EVENT_FUNCTOR_PTR& cb)
{
    mtRecordCallback.push_back(cb);
}

const bool NFCRecord::GetSave()
{
    return mbSave;
}

const bool NFCRecord::GetView()
{
    return mbView;
}

const bool NFCRecord::GetPublic()
{
    return mbPublic;
}

const bool NFCRecord::GetPrivate()
{
    return mbPrivate;
}

const int NFCRecord::GetIndex()
{
    return mnIndex;
}

int NFCRecord::GetPos(int nRow, int nCol) const
{
    return nRow * mVarRecordType.GetCount() + nCol;
}

const std::string& NFCRecord::GetName() const
{
    return mstrRecordName;
}

void NFCRecord::SetSave(const bool bSave)
{
    mbSave = bSave;
}

void NFCRecord::SetView(const bool bView)
{
    mbView = bView;
}

void NFCRecord::SetPublic(const bool bPublic)
{
    mbPublic = bPublic;
}

void NFCRecord::SetPrivate(const bool bPrivate)
{
    mbPrivate = bPrivate;
}

void NFCRecord::SetName(const char* strName)
{
    mstrRecordName = strName;
}

const NFIDataList& NFCRecord::GetInitData() const
{
    return mVarRecordType;
}

void NFCRecord::OnEventHandler(const NFIDENTID& self, const std::string& strRecordName, const int nOpType, const int nRow, const int nCol, const NFIDataList& oldVar, const NFIDataList& newVar)
{
    TRECORDCALLBACKEX::iterator itr = mtRecordCallback.begin();
    TRECORDCALLBACKEX::iterator end = mtRecordCallback.end();
    for (; itr != end; ++itr)
    {
        //NFIDataList参数:所属对象名string，操作类型int，Row,Col, OLD属性值，NEW属性值
        RECORD_EVENT_FUNCTOR_PTR functorPtr = *itr;
        functorPtr.get()->operator()(self, strRecordName, nOpType, nRow, nCol, oldVar, newVar, NFCDataList());
    }
}

bool NFCRecord::IsUsed(const int nRow) const
{
    if (ValidRow(nRow))
    {
        return (mVecUsedState[nRow] > 0);
    }

    return false;
}

bool NFCRecord::SwapRowInfo(const int nOriginRow, const int nTargetRow)
{
	if (!IsUsed(nOriginRow))
	{
		return false;
	}

    if (ValidRow(nOriginRow)
        && ValidRow(nTargetRow))
    {
		if (mnKeyCol >= 0)
		{
			TDATA_TYPE xType = GetColType(mnKeyCol);
			if (TDATA_TYPE::TDATA_INT == xType)
			{
				if (IsUsed(nOriginRow))
				{
					if (IsUsed(nTargetRow))
					{
						int nOldValue = GetInt(nOriginRow, mnKeyCol);
						int nNewValue = GetInt(nTargetRow, mnKeyCol);

						std::map<NFINT64, int>::iterator itOldValue = mxIntKeyMap.find(nOldValue);
						if (itOldValue != mxIntKeyMap.end())
						{
							itOldValue->second = nTargetRow;
						}

						std::map<NFINT64, int>::iterator itNewValue = mxIntKeyMap.find(nNewValue);
						if (itNewValue != mxIntKeyMap.end())
						{
							itNewValue->second = nOriginRow;
						}
					}
					else
					{
						int nOldValue = GetInt(nOriginRow, mnKeyCol);
						std::map<NFINT64, int>::iterator itOldValue = mxIntKeyMap.find(nOldValue);
						if (itOldValue != mxIntKeyMap.end())
						{
							itOldValue->second = nTargetRow;
						}
					}
				}
			}
			else if (TDATA_TYPE::TDATA_STRING == xType)
			{
				if (IsUsed(nOriginRow))
				{
					if (IsUsed(nTargetRow))
					{
						const std::string& strOldValue = GetString(nOriginRow, mnKeyCol);
						const std::string& strNewValue = GetString(nTargetRow, mnKeyCol);

						std::map<std::string, int>::iterator itOldValue = mxStringKeyMap.find(strOldValue);
						if (itOldValue != mxStringKeyMap.end())
						{
							itOldValue->second = nTargetRow;
						}

						std::map<std::string, int>::iterator itNewValue = mxStringKeyMap.find(strNewValue);
						if (itNewValue != mxStringKeyMap.end())
						{
							itNewValue->second = nOriginRow;
						}
					}
					else
					{
						const std::string& strOldValue = GetString(nOriginRow, mnKeyCol);
						std::map<std::string, int>::iterator itOldValue = mxStringKeyMap.find(strOldValue);
						if (itOldValue != mxStringKeyMap.end())
						{
							itOldValue->second = nTargetRow;
						}
					}
				}
			}
			else if (TDATA_TYPE::TDATA_OBJECT == xType)
			{
				if (IsUsed(nOriginRow))
				{
					if (IsUsed(nTargetRow))
					{
						const NFIDENTID xOldValue = GetObject(nOriginRow, mnKeyCol);
						const NFIDENTID xNewValue = GetObject(nTargetRow, mnKeyCol);

						std::map<NFIDENTID, int>::iterator itOldValue = mxObjectKeyMap.find(xOldValue);
						if (itOldValue != mxObjectKeyMap.end())
						{
							itOldValue->second = nTargetRow;
						}

						std::map<NFIDENTID, int>::iterator itNewValue = mxObjectKeyMap.find(xNewValue);
						if (itNewValue != mxObjectKeyMap.end())
						{
							itNewValue->second = nOriginRow;
						}
					}
					else
					{
						const NFIDENTID xOldValue = GetObject(nOriginRow, mnKeyCol);
						std::map<NFIDENTID, int>::iterator itOldValue = mxObjectKeyMap.find(xOldValue);
						if (itOldValue != mxObjectKeyMap.end())
						{
							itOldValue->second = nTargetRow;
						}
					}
				}
			}
		}

        for (int i = 0; i < GetCols(); ++i)
        {
            NF_SHARE_PTR<NFIDataList::TData> pOrigin = mtRecordVec.at(GetPos(nOriginRow, i));
            mtRecordVec[GetPos(nOriginRow, i)] = mtRecordVec.at(GetPos(nTargetRow, i));
            mtRecordVec[GetPos(nTargetRow, i)] = pOrigin;
        }

        int nOriginUse = mVecUsedState[nOriginRow];
        mVecUsedState[nOriginRow] = mVecUsedState[nTargetRow];
        mVecUsedState[nTargetRow] = nOriginUse;

        OnEventHandler(mSelf, mstrRecordName, Swap, nOriginRow, nTargetRow, NFCDataList(), NFCDataList());

        return true;
    }

    return false;
}

const NFIDataList& NFCRecord::GetInitDesc() const
{
    return mVarRecordDesc;
}

const NFIDataList& NFCRecord::GetTag() const
{
    return mVarRecordTag;
}

const NFIDataList& NFCRecord::GetRelatedRecord() const
{
    return mVarRecordRelation;
}

const NFIRecord::TRECORDVEC& NFCRecord::GetRecordVec() const
{
    return mtRecordVec;
}

bool NFCRecord::SetUsed(const int nRow, const int bUse)
{
    if (ValidRow(nRow))
    {
        mVecUsedState[nRow] = bUse;
        return true;
    }

    return false;
}

bool NFCRecord::SetKey(const int nCol, const int bKey)
{
	mVarRecordKey.SetInt(nCol, bKey);
    return true;
}

bool NFCRecord::IsKey(const int nCol) const
{

    if (ValidRow(nCol))
    {
		if (nCol == mnKeyCol)
		{
			return true;
		}
        //return (mVarRecordKey.Int(nCol) > 0);
        //return (m_pRecordKeyState[nRow] > 0);
    }

    return false;
}

const NFIDataList& NFCRecord::GetKeyState() const
{
    return mVarRecordKey;
}

bool NFCRecord::ValidPos(int nRow, int nCol) const
{
    if (ValidCol(nCol)
        && ValidRow(nRow))
    {
        return true;
    }

    return false;
}

bool NFCRecord::ValidRow(int nRow) const
{
    if (nRow >= GetRows() || nRow < 0)
    {
        return false;
    }

    return true;
}

bool NFCRecord::ValidCol(int nCol) const
{
    if (nCol >= GetCols() || nCol < 0)
    {
        return false;
    }

    return true;
}

int NFCRecord::GetCol(const std::string& strTag) const
{
    std::map<std::string, int>::const_iterator it = mmTag.find(strTag);
    if (it != mmTag.end())
    {
        return it->second;
    }

    return -1;
}

bool NFCRecord::GetRelatedTag(const std::string& strSrcTag, const std::string& strRelatedRecord, std::string& strRelatedTag)
{
    int nCol = GetCol(strSrcTag);
    if (nCol == -1)
    {
        return false;
    }

    RelationRecordMap::iterator iter = mmRelationRecord.find(RelationRecordColType(nCol, strRelatedTag));
    if (iter != mmRelationRecord.end())
    {
        strRelatedTag = iter->second;
        return true;
    }

    return false;
}
