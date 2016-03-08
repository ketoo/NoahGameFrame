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
    mnKeyCol = -1;
    mstrRecordName = "";
    mnMaxRow = 0;

}

NFCRecord::NFCRecord(const NFGUID& self, const std::string& strRecordName, const NFIDataList& valueList, const NFIDataList& keyList, const NFIDataList& descList, const NFIDataList& tagList, const NFIDataList& relateRecordList, int nMaxRow, bool bPublic,  bool bPrivate,  bool bSave, bool bView, int nIndex)
{
    mVarRecordType.Append(valueList);
    mVarRecordDesc.Append(descList);
    mVarRecordKey.Append(keyList);
    mVarRecordTag.Append(tagList);
    mVarRecordRelation.Append(relateRecordList);

    mSelf = self;
    mbSave = bSave;
    mbView = bView;
    mbPublic = bPublic;
    mbPrivate = bPrivate;
    mnIndex = nIndex;

    mstrRecordName = strRecordName;

    mnMaxRow = nMaxRow;
    //ȷ����С
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

    // TODO:���Կ����ڴ˴�ֱ�Ӽ���Ĭ��ֵ
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

// �������
int NFCRecord::AddRow(const int nRow)
{
    return AddRow(nRow, mVarRecordType);
}

//���ͣ���ֵ��ԭֵ==��ԭֵ����ֵ��û�о�new
bool ValidAdd(TDATA_TYPE eType, const NFIDataList::TData& var, NF_SHARE_PTR<NFIDataList::TData>& pVar)
{
    if (var.GetType() != eType)
    {
        return false;
    }

    if (!pVar.get())
    {
        if (!var.IsNullValue())
        {
            return false;
        }

        pVar = NF_SHARE_PTR<NFIDataList::TData>(NF_NEW NFIDataList::TData());
        switch (eType)
        {
            case TDATA_UNKNOWN:
                break;
            case TDATA_INT:
            {
                pVar->SetInt(0);
            }
            break;
            case TDATA_FLOAT:
            {
                pVar->SetFloat(0);
            }
            break;
            case TDATA_STRING:
            {
                pVar->SetString("");
            }
            break;
            case TDATA_OBJECT:
            {
                pVar->SetObject(NFGUID());
            }
            break;
            case TDATA_MAX:
                break;
            default:
                break;
        }
    }
    else
    {
        switch (eType)
        {
            case TDATA_UNKNOWN:
                break;
            case TDATA_INT:
            {
                if (pVar->GetInt() == var.GetInt())
                {
                    return false;
                }
            }
            break;
            case TDATA_FLOAT:
            {
                double fValue = pVar->GetFloat() - var.GetFloat();
                if (fValue < 0.001  && fValue > -0.001)
                {
                    return false;
                }
            }
            break;
            case TDATA_STRING:
            {
                if (pVar->GetString() == var.GetString())
                {
                    return false;
                }
            }
            break;
            case TDATA_OBJECT:
            {
                if (pVar->GetObject() == var.GetObject())
                {
                    return false;
                }
            }
            break;
            case TDATA_MAX:
                break;
            default:
                break;
        }
    }

    return true;
}

bool ValidSet(TDATA_TYPE eType, const NFIDataList::TData& var, NF_SHARE_PTR<NFIDataList::TData>& pVar)
{
    if (var.GetType() != eType)
    {
        return false;
    }

    if (!pVar.get())
    {
        if (!var.IsNullValue())
        {
            return false;
        }

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

        if (IsKey(i))
        {
            //Ϊkey,value->Row����key�����ظ�
            if (TDATA_INT == var.Type(i))
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
            else if (TDATA_STRING == var.Type(i))
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
            else if (TDATA_OBJECT == var.Type(i))
            {
                if (mxObjectKeyMap.find(var.Object(i)) == mxObjectKeyMap.end())
                {
                    mxObjectKeyMap.insert(std::map<NFGUID, int>::value_type(var.Object(i), nFindRow));
                }
                else
                {
                    return -1;
                }
            }
        }
    }


    SetUsed(nFindRow, 1);
    for (int i = 0; i < GetCols(); ++i)
    {
        NF_SHARE_PTR<NFIDataList::TData>& pVar = mtRecordVec.at(GetPos(nFindRow, i));//GetTData(nFindRow, i);
        if (!ValidSet(GetColType(i), *var.GetStack(i), pVar))
        {
            //���ʧ��--�������������������Ϊ���������Ѿ�������������صĻ�����ô��ӵ�������0�Ļ��ͻ᷵�أ����½������
            //                 mVecUsedState[nFindRow] = 0;
            //                 pVar.reset();
            //                 return -1;
        }
    }

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = Add;
    xEventData.nRow = nFindRow;
    xEventData.nCol = 0;
    xEventData.strRecordName = mstrRecordName;

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

    if (!ValidAdd(TDATA_INT, var, pVar))
    {
        return false;
    }

    //��Ҫ����ǰ��key������ɾ��
    if (IsKey(nCol))
    {
        //NFINT64 nOldValue = boost::get<NFINT64>(pVar->variantData);
        NFINT64 nOldValue = pVar->variantData.get<NFINT64>();
        mxIntKeyMap.erase(nOldValue);

        if (mxIntKeyMap.find(value) != mxIntKeyMap.end())
        {
            return false;
        }

        mxIntKeyMap.insert(std::map<NFINT64, int>::value_type(value, nRow));
    }

    NFCDataList::TData oldValue;
    NFCDataList::TData newValue;

    oldValue.SetInt(pVar->GetInt());
    newValue.SetInt(value);

    pVar->variantData = value;

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = UpData;
    xEventData.nRow = nRow;
    xEventData.nCol = nCol;
    xEventData.strRecordName = mstrRecordName;

    OnEventHandler(mSelf, xEventData, oldValue, newValue);

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

    if (!ValidAdd(TDATA_FLOAT, var, pVar))
    {
        return false;
    }

    NFCDataList::TData oldValue;
    NFCDataList::TData newValue;

    oldValue.SetFloat(pVar->GetFloat());
    newValue.SetFloat(value);

    pVar->variantData = value;

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = UpData;
    xEventData.nRow = nRow;
    xEventData.nCol = nCol;
    xEventData.strRecordName = mstrRecordName;

    OnEventHandler(mSelf, xEventData, oldValue, newValue);

    return true;
}

bool NFCRecord::SetFloat(const int nRow, const std::string& strColTag, const double value)
{
    int nCol = GetCol(strColTag);
    return SetFloat(nRow, nCol, value);
}

bool NFCRecord::SetString(const int nRow, const int nCol, const char* value)
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

    if (!ValidAdd(TDATA_STRING, var, pVar))
    {
        return false;
    }

    //��Ҫ����ǰ��key������ɾ��
    if (IsKey(nCol))
    {
        //std::string& strOldValue = boost::get<std::string>(pVar->variantData);
        std::string& strOldValue = pVar->variantData.get<std::string>();
        mxStringKeyMap.erase(strOldValue);

        if (mxStringKeyMap.find(value) != mxStringKeyMap.end())
        {
            return false;
        }

        mxStringKeyMap.insert(std::map<std::string, int>::value_type(value, nRow));
    }


    NFCDataList::TData oldValue;
    NFCDataList::TData newValue;

    oldValue.SetString(pVar->GetString());
    newValue.SetString(value);

    pVar->variantData = (std::string)value;

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = UpData;
    xEventData.nRow = nRow;
    xEventData.nCol = nCol;
    xEventData.strRecordName = mstrRecordName;

    OnEventHandler(mSelf, xEventData, oldValue, newValue);

    return true;
}

bool NFCRecord::SetString(const int nRow, const std::string& strColTag, const char* value)
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

    if (!ValidAdd(TDATA_OBJECT, var, pVar))
    {
        return false;
    }
    //��Ҫ����ǰ��key������ɾ��
    if (IsKey(nCol))
    {
        //NFGUID xOldValue = boost::get<NFGUID>(pVar->variantData);
        NFGUID xOldValue = pVar->variantData.get<NFGUID>();
        mxObjectKeyMap.erase(xOldValue);

        if (mxObjectKeyMap.find(value) != mxObjectKeyMap.end())
        {
            return false;
        }

        mxObjectKeyMap.insert(std::map<NFGUID, int>::value_type(value, nRow));
    }

    NFCDataList::TData oldValue;
    NFCDataList::TData newValue;

    oldValue.SetObject(pVar->GetObject());
    newValue.SetObject(value);

    pVar->variantData = value;

    RECORD_EVENT_DATA xEventData;
    xEventData.nOpType = UpData;
    xEventData.nRow = nRow;
    xEventData.nCol = nCol;
    xEventData.strRecordName = mstrRecordName;

    OnEventHandler(mSelf, xEventData, oldValue, newValue);

    return true;
}

bool NFCRecord::SetObject(const int nRow, const std::string& strColTag, const NFGUID& value)
{
    int nCol = GetCol(strColTag);
    return SetObject(nRow, nCol, value);
}

// �������
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

int NFCRecord::FindObject(const int nCol, const NFGUID& value, NFIDataList& varResult)
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
        std::map<NFGUID, int>::iterator it = mxObjectKeyMap.begin();
        for (; it != mxObjectKeyMap.end(); ++it)
        {
            varResult.AddInt(it->second);
        }

        return varResult.GetCount();
    }
    else
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

            if (mnKeyCol >= 0)
            {
                TDATA_TYPE xType = GetColType(mnKeyCol);
                if (TDATA_INT == xType)
                {
                    mxIntKeyMap.erase(GetInt(nRow, mnKeyCol));
                }
                else if (TDATA_STRING == xType)
                {
                    mxStringKeyMap.erase(GetString(nRow, mnKeyCol));
                }
                else if (TDATA_OBJECT == xType)
                {
                    mxObjectKeyMap.erase(GetObject(nRow, mnKeyCol));
                }
            }

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

void NFCRecord::OnEventHandler(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFIDataList::TData& oldVar, const NFIDataList::TData& newVar)
{
    TRECORDCALLBACKEX::iterator itr = mtRecordCallback.begin();
    TRECORDCALLBACKEX::iterator end = mtRecordCallback.end();
    for (; itr != end; ++itr)
    {
        //NFIDataList����:����������string����������int��Row,Col, OLD����ֵ��NEW����ֵ
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
        if (mnKeyCol >= 0)
        {
            TDATA_TYPE xType = GetColType(mnKeyCol);
            if (TDATA_INT == xType)
            {
                if (IsUsed(nOriginRow))
                {
                    if (IsUsed(nTargetRow))
                    {
                        NFINT64 nOldValue = GetInt(nOriginRow, mnKeyCol);
                        NFINT64 nNewValue = GetInt(nTargetRow, mnKeyCol);

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
                        NFINT64 nOldValue = GetInt(nOriginRow, mnKeyCol);
                        std::map<NFINT64, int>::iterator itOldValue = mxIntKeyMap.find(nOldValue);
                        if (itOldValue != mxIntKeyMap.end())
                        {
                            itOldValue->second = nTargetRow;
                        }
                    }
                }
            }
            else if (TDATA_STRING == xType)
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
            else if (TDATA_OBJECT == xType)
            {
                if (IsUsed(nOriginRow))
                {
                    if (IsUsed(nTargetRow))
                    {
                        const NFGUID xOldValue = GetObject(nOriginRow, mnKeyCol);
                        const NFGUID xNewValue = GetObject(nTargetRow, mnKeyCol);

                        std::map<NFGUID, int>::iterator itOldValue = mxObjectKeyMap.find(xOldValue);
                        if (itOldValue != mxObjectKeyMap.end())
                        {
                            itOldValue->second = nTargetRow;
                        }

                        std::map<NFGUID, int>::iterator itNewValue = mxObjectKeyMap.find(xNewValue);
                        if (itNewValue != mxObjectKeyMap.end())
                        {
                            itNewValue->second = nOriginRow;
                        }
                    }
                    else
                    {
                        const NFGUID xOldValue = GetObject(nOriginRow, mnKeyCol);
                        std::map<NFGUID, int>::iterator itOldValue = mxObjectKeyMap.find(xOldValue);
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
