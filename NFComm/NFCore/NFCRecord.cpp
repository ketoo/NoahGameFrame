// -------------------------------------------------------------------------
//    @FileName         :    NFCRecord.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCRecord
//
// -------------------------------------------------------------------------

#include <exception>
#include "NFIDataList.h"
#include "NFCRecord.h"

NFCRecord::NFCRecord()
{
    mSelf = NFGUID();

    mbSave = false;
    mbPublic = false;
    mbPrivate = false;
    mbCache = false;

    mstrRecordName = "";
    mnMaxRow = 0;

}

NFCRecord::NFCRecord(const NFGUID& self, const std::string& strRecordName, const NF_SHARE_PTR<NFIDataList>& valueList, const NF_SHARE_PTR<NFIDataList>& tagList, const int nMaxRow)
{
	mVarRecordType = valueList;
    mVarRecordTag = tagList;

    mSelf = self;

    mbSave = false;
    mbPublic = false;
    mbPrivate = false;
    mbCache = false;

    mstrRecordName = strRecordName;

    mnMaxRow = nMaxRow;
    //确定大小
    mVecUsedState.resize(mnMaxRow);

    for (int i = 0; i < mnMaxRow; i++)
    {
        mVecUsedState[i] = 0;
    }

	//init share_pointer for all data
    for (int i = 0; i < GetRows() * GetCols(); i++)
    {
        mtRecordVec.push_back(NF_SHARE_PTR<NFIDataList::TData>());
    }

	//optimize would be better, it should be applied memory space only once
    for (int i = 0; i < mVarRecordTag->GetCount(); ++i)
    {
        if (!mVarRecordTag->String(i).empty())
        {
            mmTag[mVarRecordTag->String(i)] = i;
        }
    }
}

NFCRecord::~NFCRecord()
{
    for (TRECORDVEC::iterator iter = mtRecordVec.begin(); iter != mtRecordVec.end(); ++iter)
    {
        iter->reset();
    }

    for (TRECORDCALLBACKEX::iterator iter = mtRecordCallback.begin(); iter != mtRecordCallback.end(); ++iter)
    {
        iter->reset();
    }

    mtRecordVec.clear();
    mVecUsedState.clear();
    mtRecordCallback.clear();
}

int NFCRecord::GetCols() const
{
    return mVarRecordType->GetCount();
}

int NFCRecord::GetRows() const
{
    return mnMaxRow;
}

TDATA_TYPE NFCRecord::GetColType(const int nCol) const
{
    return mVarRecordType->Type(nCol);
}

const std::string& NFCRecord::GetColTag(const int nCol) const
{
    return mVarRecordTag->String(nCol);
}

// 添加数据
int NFCRecord::AddRow(const int nRow)
{
    return AddRow(nRow, *mVarRecordType);
}

bool ValidSet(TDATA_TYPE eType, const NFIDataList::TData& var, NF_SHARE_PTR<NFIDataList::TData>& pVar)
{
    if (var.GetType() != eType)
    {
        return false;
    }

    if (nullptr == pVar)
    {
        pVar = NF_SHARE_PTR<NFIDataList::TData>(NF_NEW NFIDataList::TData(eType));
    }

    if (pVar->GetType() != eType)
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
            if (!IsUsed(i))
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

    for (int i = 0; i < GetCols(); ++i)
    {
        if (var.Type(i) != GetColType(i))
        {
            return -1;
        }
    }

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = Add;
    xEventData.nRow = nFindRow;
    xEventData.nCol = 0;
    xEventData.strRecordName = mstrRecordName;

    if (IsUsed(nFindRow))
    {
        //delete
        xEventData.nOpType = Cover;
    }

    SetUsed(nFindRow, 1);

    for (int i = 0; i < GetCols(); ++i)
    {
		//pVar 找出来的不一定有效
        NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nFindRow, i));
        if (!ValidSet(GetColType(i), *(var.GetStack(i)), pVar))
        {
            //添加失败--不存在这样的情况，因为类型上面已经监测过，如果返回的话，那么添加的数据是0的话就会返回，导致结果错误
			return -1;
        }
    }

    NFIDataList::TData tData;
    OnEventHandler(mSelf, xEventData, tData, tData); //FIXME:RECORD

    return nFindRow;
}

bool NFCRecord::SetInt(const int nRow, const int nCol, const NFINT64 value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

    if (TDATA_INT != GetColType(nCol))
    {
        return false;
    }

    if (!IsUsed(nRow))
    {
        return false;
    }

    NFIDataList::TData var;
    var.SetInt(value);

    NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

    NFCDataList::TData oldValue;
    oldValue.SetInt(pVar->GetInt());

    pVar->variantData = value;

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = Update;
    xEventData.nRow = nRow;
    xEventData.nCol = nCol;
    xEventData.strRecordName = mstrRecordName;

    OnEventHandler(mSelf, xEventData, oldValue, *pVar);

    return true;
}

bool NFCRecord::SetInt(const int nRow, const std::string& strColTag, const NFINT64 value)
{
    int nCol = GetCol(strColTag);
    return SetInt(nRow, nCol, value);
}

bool NFCRecord::SetFloat(const int nRow, const int nCol, const double value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

    if (TDATA_FLOAT != GetColType(nCol))
    {
        return false;
    }

    if (!IsUsed(nRow))
    {
        return false;
    }

    NFIDataList::TData var;
    var.SetFloat(value);

    NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

    NFCDataList::TData oldValue;
    oldValue.SetFloat(pVar->GetFloat());

    pVar->variantData = value;

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = Update;
    xEventData.nRow = nRow;
    xEventData.nCol = nCol;
    xEventData.strRecordName = mstrRecordName;

    OnEventHandler(mSelf, xEventData, oldValue, *pVar);

    return true;
}

bool NFCRecord::SetFloat(const int nRow, const std::string& strColTag, const double value)
{
    int nCol = GetCol(strColTag);
    return SetFloat(nRow, nCol, value);
}

bool NFCRecord::SetString(const int nRow, const int nCol, const std::string& value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

    if (TDATA_STRING != GetColType(nCol))
    {
        return false;
    }

    if (!IsUsed(nRow))
    {
        return false;
    }

    NFIDataList::TData var;
    var.SetString(value);

    NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

    NFCDataList::TData oldValue;
    oldValue.SetString(pVar->GetString());

    pVar->variantData = (std::string)value;

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = Update;
    xEventData.nRow = nRow;
    xEventData.nCol = nCol;
    xEventData.strRecordName = mstrRecordName;

    OnEventHandler(mSelf, xEventData, oldValue, *pVar);

    return true;
}

bool NFCRecord::SetString(const int nRow, const std::string& strColTag, const std::string& value)
{
    int nCol = GetCol(strColTag);
    return SetString(nRow, nCol, value);
}

bool NFCRecord::SetObject(const int nRow, const int nCol, const NFGUID& value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

    if (TDATA_OBJECT != GetColType(nCol))
    {
        return false;
    }

    if (!IsUsed(nRow))
    {
        return false;
    }

    NFIDataList::TData var;
    var.SetObject(value);

    NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

    NFCDataList::TData oldValue;
    oldValue.SetObject(pVar->GetObject());

    pVar->variantData = value;

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = Update;
    xEventData.nRow = nRow;
    xEventData.nCol = nCol;
    xEventData.strRecordName = mstrRecordName;

    OnEventHandler(mSelf, xEventData, oldValue, *pVar);

    return true;
}

bool NFCRecord::SetObject(const int nRow, const std::string& strColTag, const NFGUID& value)
{
    int nCol = GetCol(strColTag);
    return SetObject(nRow, nCol, value);
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

                case TDATA_STRING:
                    varList.Add(NULL_STR.c_str());
                    break;

                case TDATA_OBJECT:
                    varList.Add(NFGUID());
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

    return pVar->GetInt();
}

NFINT64 NFCRecord::GetInt(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetInt(nRow, nCol);
}

double NFCRecord::GetFloat(const int nRow, const int nCol) const
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

    return pVar->GetFloat();
}

double NFCRecord::GetFloat(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetFloat(nRow, nCol);
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

    return pVar->GetString();
}

const std::string& NFCRecord::GetString(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetString(nRow, nCol);
}

const NFGUID& NFCRecord::GetObject(const int nRow, const int nCol) const
{
    if (!ValidPos(nRow, nCol))
    {
        return NULL_OBJECT;
    }

    if (!IsUsed(nRow))
    {
        return NULL_OBJECT;
    }

    const  NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar.get())
    {
        return NULL_OBJECT;
    }

    return pVar->GetObject();
}

const NFGUID& NFCRecord::GetObject(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetObject(nRow, nCol);
}

int NFCRecord::FindRowByColValue(const int nCol, const NFIDataList& var, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    TDATA_TYPE eType = var.Type(0);
    if (eType != mVarRecordType->Type(nCol))
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

    if (TDATA_INT != mVarRecordType->Type(nCol))
    {
        return -1;
    }

    {
        for (int i = 0; i < mnMaxRow; ++i)
        {
            if (!IsUsed(i))
            {
                continue;
            }

            if (GetInt(i, nCol) == value)
            {
                varResult << i;
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

int NFCRecord::FindFloat(const int nCol, const double value, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    if (TDATA_FLOAT != mVarRecordType->Type(nCol))
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
            varResult << i;
        }
    }

    return varResult.GetCount();
}

int NFCRecord::FindFloat(const std::string& strColTag, const double value, NFIDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindFloat(nCol, value, varResult);
}

int NFCRecord::FindString(const int nCol, const char* value, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    if (TDATA_STRING != mVarRecordType->Type(nCol))
    {
        return -1;
    }


    {
        for (int64_t i = 0; i < mnMaxRow; ++i)
        {
            if (!IsUsed(i))
            {
                continue;
            }

            const std::string& strData = GetString(i, nCol);
            if (0 == strcmp(strData.c_str(), value))
            {
                varResult << i;
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


int NFCRecord::FindString(const std::string& strColTag, const std::string& value, NFIDataList& varResult)
{
	return FindString(strColTag, value.c_str(), varResult);
}

int NFCRecord::FindString(const int nCol, const std::string& value, NFIDataList& varResult)
{
	return FindString(nCol, value.c_str(), varResult);
}

int NFCRecord::FindObject(const int nCol, const NFGUID& value, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    if (TDATA_OBJECT != mVarRecordType->Type(nCol))
    {
        return -1;
    }

    {
        for (int64_t i = 0; i < mnMaxRow; ++i)
        {
            if (!IsUsed(i))
            {
                continue;
            }

            if (GetObject(i, nCol) == value)
            {
                varResult << i;
            }
        }

        return varResult.GetCount();
    }

    return -1;
}

int NFCRecord::FindObject(const std::string& strColTag, const NFGUID& value, NFIDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindObject(nCol, value, varResult);
}

bool NFCRecord::Remove(const int nRow)
{
    if (ValidRow(nRow))
    {
        if (IsUsed(nRow))
        {
            RECORD_EVENT_DATA xEventData;
            xEventData.nOpType = Del;
            xEventData.nRow = nRow;
            xEventData.nCol = 0;
            xEventData.strRecordName = mstrRecordName;

            OnEventHandler(mSelf, xEventData, NFCDataList::TData(), NFCDataList::TData());

            mVecUsedState[nRow] = 0;
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

const bool NFCRecord::GetCache()
{
    return mbCache;
}

const bool NFCRecord::GetPublic()
{
    return mbPublic;
}

const bool NFCRecord::GetPrivate()
{
    return mbPrivate;
}

int NFCRecord::GetPos(int nRow, int nCol) const
{
    return nRow * mVarRecordType->GetCount() + nCol;
}

const std::string& NFCRecord::GetName() const
{
    return mstrRecordName;
}

void NFCRecord::SetSave(const bool bSave)
{
    mbSave = bSave;
}

void NFCRecord::SetCache(const bool bCache)
{
    mbCache = bCache;
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

const NF_SHARE_PTR<NFIDataList> NFCRecord::GetInitData() const
{
    NF_SHARE_PTR<NFIDataList> pIniData = NF_SHARE_PTR<NFIDataList>( NF_NEW NFCDataList());
    pIniData->Append(*mVarRecordType);

    return pIniData;
}

void NFCRecord::OnEventHandler(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    TRECORDCALLBACKEX::iterator itr = mtRecordCallback.begin();
    TRECORDCALLBACKEX::iterator end = mtRecordCallback.end();
    for (; itr != end; ++itr)
    {
        //NFIDataList参数:所属对象名string，操作类型int，Row,Col, OLD属性值，NEW属性值
        RECORD_EVENT_FUNCTOR_PTR functorPtr = *itr;
        functorPtr.get()->operator()(self, xEventData, oldVar, newVar);
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
        for (int i = 0; i < GetCols(); ++i)
        {
            NF_SHARE_PTR<NFIDataList::TData> pOrigin = mtRecordVec.at(GetPos(nOriginRow, i));
            mtRecordVec[GetPos(nOriginRow, i)] = mtRecordVec.at(GetPos(nTargetRow, i));
            mtRecordVec[GetPos(nTargetRow, i)] = pOrigin;
        }

        int nOriginUse = mVecUsedState[nOriginRow];
        mVecUsedState[nOriginRow] = mVecUsedState[nTargetRow];
        mVecUsedState[nTargetRow] = nOriginUse;

        RECORD_EVENT_DATA xEventData;
        xEventData.nOpType = Swap;
        xEventData.nRow = nOriginRow;
        xEventData.nCol = nTargetRow;
        xEventData.strRecordName = mstrRecordName;

        NFCDataList::TData xData;
        OnEventHandler(mSelf, xEventData, xData, xData);

        return true;
    }

    return false;
}

const NF_SHARE_PTR<NFIDataList> NFCRecord::GetTag() const
{
    NF_SHARE_PTR<NFIDataList> pIniData = NF_SHARE_PTR<NFIDataList>(NF_NEW NFCDataList());
    pIniData->Append(*mVarRecordTag);
    return pIniData;
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
