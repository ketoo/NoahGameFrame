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
    mSelf = 0;
    mbSave = false;
    mbPublic = false;
    mbPrivate = false;
    mbHasKey = false;
    mstrRecordName = "";
    mnMaxRow = 0;

}

NFCRecord::NFCRecord(const NFIDENTID& self, const std::string& strRecordName, const NFIDataList& valueList, const NFIDataList& keyList, const NFIDataList& descList, const NFIDataList& tagList, const NFIDataList& relateRecordList, int nMaxRow, bool bPublic,  bool bPrivate,  bool bSave, int nIndex)
{
    mVarRecordType = valueList;
    mVarRecordDesc = descList;
    mVarRecordKey = keyList;
    mVarRecordTag = tagList;
    mVarRecordRelation = relateRecordList;

    mSelf = self;
    mbSave = bSave;
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
            mbHasKey = true;
            break;
        }
    }

    // TODO:可以考虑在此处直接加入默认值
    for (int i = 0; i < GetRows() * GetCols(); i++)
    {
        mtRecordVec.push_back(std::shared_ptr<NFIDataList::TData>());
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
            const std::string& strRecordTag =singleRelationList.String(1);
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

const std::string& NFCRecord::GetColTag( const int nCol ) const
{
    return mVarRecordTag.String(nCol);
}

// 添加数据
int NFCRecord::AddRow(const int nRow)
{
    return AddRow(nRow, mVarRecordType);
}

//类型，新值，原值==给原值赋新值，没有就new
bool ValidAdd(TDATA_TYPE eType, const NFIDataList::TData& var, std::shared_ptr<NFIDataList::TData>& pVar)
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

        pVar = std::shared_ptr<NFIDataList::TData>(NF_NEW NFIDataList::TData());
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
            int nUse = mVecUsedState[i];
            if (nUse <= 0)
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
    }

    if (bType)
    {
        mVecUsedState[nFindRow] = 1;
        for (int i = 0; i < GetCols(); ++i)
        {
            std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nFindRow, i));//GetTData(nFindRow, i);
            if(!ValidAdd(GetColType(i), *var.GetStackConst(i), pVar))
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

bool NFCRecord::SetInt(const int nRow, const int nCol, const int value)
{
    if (!ValidPos(nRow, nCol))
    {
        return false;
    }

    NFIDataList::TData var;
    var.nType = TDATA_INT;
    var.variantData = value;

    std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if(!ValidAdd(GetColType(nCol), var, pVar))
    {
        return false;
    }

    if (TDATA_INT == pVar->nType)
    {
        NFCDataList oldValue;
        NFCDataList newValue;

        oldValue.Append(*pVar);
        newValue.Add(value);

        pVar->variantData = value;

        OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

        return true;
    }

    return false;
}

bool NFCRecord::SetInt( const int nRow, const std::string& strColTag, const int value )
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

    NFIDataList::TData var;
    var.nType = TDATA_FLOAT;
    var.variantData = value;

    std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if(!ValidAdd(GetColType(nCol), var, pVar))
    {
        return false;
    }

    if (TDATA_FLOAT == pVar->nType)
    {
        NFCDataList oldValue;
        NFCDataList newValue;

        oldValue.Append(*pVar);
        newValue.Add(value);

        pVar->variantData = value;

        OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

        return true;
    }

    return false;
}

bool NFCRecord::SetFloat( const int nRow, const std::string& strColTag, const float value )
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

    NFIDataList::TData var;
    var.nType = TDATA_DOUBLE;
    var.variantData = value;

    std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if(!ValidAdd(GetColType(nCol), var, pVar))
    {
        return false;
    }

    if (TDATA_DOUBLE == pVar->nType)
    {
        NFCDataList oldValue;
        NFCDataList newValue;

        oldValue.Append(*pVar);
        newValue.Add(value);

        pVar->variantData = value;

        OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

        return true;
    }

    return false;
}

bool NFCRecord::SetDouble( const int nRow, const std::string& strColTag, const double value )
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

    NFIDataList::TData var;
    var.nType = TDATA_STRING;
    var.variantData = std::string(value);

    std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if(!ValidAdd(GetColType(nCol), var, pVar))
    {
        return false;
    }

    if (TDATA_STRING == pVar->nType)
    {
        NFCDataList oldValue;
        NFCDataList newValue;

        oldValue.Append(*pVar);
        newValue.Add(value);

        pVar->variantData = (std::string)value;

        OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

        return true;
    }

    return false;
}

bool NFCRecord::SetString( const int nRow, const std::string& strColTag, const char* value )
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

    NFIDataList::TData var;
    var.nType = TDATA_OBJECT;
    var.variantData = (NFINT64)value.nData64;

    std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if(!ValidAdd(GetColType(nCol), var, pVar))
    {
        return false;
    }

    if (TDATA_OBJECT == pVar->nType)
    {
        NFCDataList oldValue;
        NFCDataList newValue;

        oldValue.Append(*pVar);
        newValue.Add(value);

        pVar->variantData = value.nData64;

        OnEventHandler(mSelf, mstrRecordName, UpData, nRow, nCol, oldValue, newValue);

        return true;
    }

    return false;
}

bool NFCRecord::SetObject( const int nRow, const std::string& strColTag, const NFIDENTID& value )
{
    int nCol = GetCol(strColTag);
    return SetObject(nRow, nCol, value);
}

bool NFCRecord::SetPointer(const int nRow, const int nCol, const void* value)
{

    return false;
}

bool NFCRecord::SetPointer( const int nRow, const std::string& strColTag, const void* value )
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

    varList.Clear();
    for (int i = 0; i < GetCols(); ++i)
    {
        std::shared_ptr<NFIDataList::TData> pVar = mtRecordVec.at(GetPos(nRow, i));
        if (pVar.get())
        {
            varList.Append(*pVar);
        }
        else
        {
            switch (GetColType(i))
            {
            case TDATA_INT:
                varList.Add(0);
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
                varList.Add(0);
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

int NFCRecord::GetInt(const int nRow, const int nCol) const
{
    if (!ValidPos(nRow, nCol))
    {
        return 0;
    }

    const std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar.get())
    {
        return 0;
    }

    if (TDATA_INT == pVar->nType)
    {
        return boost::get<int>(pVar->variantData);
    }

    return 0;
}

int NFCRecord::GetInt( const int nRow, const std::string& strColTag ) const
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

    const std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
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

float NFCRecord::GetFloat( const int nRow, const std::string& strColTag ) const
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

    const std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
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

double NFCRecord::GetDouble( const int nRow, const std::string& strColTag ) const
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

    const std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
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

const std::string& NFCRecord::GetString( const int nRow, const std::string& strColTag ) const
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

   const  std::shared_ptr<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar.get())
    {
        return NFIDENTID();
    }

    if (TDATA_OBJECT == pVar->nType)
    {
        return boost::get<NFINT64>(pVar->variantData);
    }

    return NFIDENTID();
}

NFIDENTID NFCRecord::GetObject( const int nRow, const std::string& strColTag ) const
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

void* NFCRecord::GetPointer( const int nRow, const std::string& strColTag ) const
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

int NFCRecord::FindRowByColValue( const std::string& strColTag, const NFIDataList& var, NFIDataList& varResult )
{
    int nCol = GetCol(strColTag);
    return FindRowByColValue(nCol, var, varResult);
}

int NFCRecord::FindInt(const int nCol, const int value, NFIDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    if (TDATA_INT != mVarRecordType.Type(nCol))
    {
        return -1;
    }

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

int NFCRecord::FindInt( const std::string& strColTag, const int value, NFIDataList& varResult )
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

int NFCRecord::FindFloat( const std::string& strColTag, const float value, NFIDataList& varResult )
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

int NFCRecord::FindDouble( const std::string& strColTag, const double value, NFIDataList& varResult )
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

int NFCRecord::FindString( const std::string& strColTag, const char* value, NFIDataList& varResult )
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

    for (int i = 0; i < mnMaxRow; ++i)
    {
        if (!IsUsed(i))
        {
            continue;
        }

        if (GetObject(i, nCol) == value.nData64)
        {
            varResult << NFINT32(i);
        }
    }

    return varResult.GetCount();
}

int NFCRecord::FindObject( const std::string& strColTag, const NFIDENTID& value, NFIDataList& varResult )
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

int NFCRecord::FindPointer( const std::string& strColTag, const void* value, NFIDataList& varResult )
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

            mVecUsedState[nRow] = 0;

//             for (int i = 0; i < GetCols(); ++i)
//             {
//                 std::shared_ptr<NFIDataList::TData>& var = mtRecordVec.at(GetPos(nRow, i));
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

int NFCRecord::GetPos( int nRow, int nCol ) const
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
    if (ValidRow(nOriginRow)
        && ValidRow(nTargetRow))
    {
        for (int i = 0; i < GetCols(); ++i)
        {
            std::shared_ptr<NFIDataList::TData> pOrigin = mtRecordVec.at(GetPos(nOriginRow, i));
            mtRecordVec[GetPos(nOriginRow, i)] = mtRecordVec.at(GetPos(nTargetRow, i));
            mtRecordVec[GetPos(nTargetRow, i)] = pOrigin;
        }

        int nOriginUse = mVecUsedState[nOriginRow];
        mVecUsedState[nOriginRow] = mVecUsedState[nTargetRow];
        mVecUsedState[nTargetRow] = nOriginUse;

        OnEventHandler(mSelf, mstrRecordName, Changed, nOriginRow, nTargetRow, NFCDataList(), NFCDataList());

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

bool NFCRecord::SetKey( const int nRow, const int bKey )
{


    return false;
}

bool NFCRecord::IsKey( const int nRow ) const
{

    if (ValidRow(nRow))
    {
        return (mVarRecordKey.Int(nRow) > 0);
        //return (m_pRecordKeyState[nRow] > 0);
    }

    return false;
}

const NFIDataList& NFCRecord::GetKeyState() const
{
    return mVarRecordKey;
}

bool NFCRecord::ValidPos( int nRow, int nCol ) const
{
    if (ValidCol(nCol)
        && ValidRow(nRow))
    {
        return true;
    }

    return false;
}

bool NFCRecord::ValidRow( int nRow ) const
{
    if (nRow >= GetRows() || nRow < 0)
    {
        return false;
    }

    return true;
}

bool NFCRecord::ValidCol( int nCol ) const
{
    if (nCol >= GetCols() || nCol < 0)
    {
        return false;
    }

    return true;
}

int NFCRecord::GetCol( const std::string& strTag ) const
{
    std::map<std::string, int>::const_iterator it = mmTag.find(strTag);
    if (it != mmTag.end())
    {
        return it->second;
    }

    return -1;
}

bool NFCRecord::GetRelatedTag(const std::string& strSrcTag, const std::string& strRelatedRecord, OUT std::string& strRelatedTag)
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