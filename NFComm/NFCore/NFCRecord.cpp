// -------------------------------------------------------------------------
//    @FileName         :    NFCRecord.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2012-03-01
//    @Module           :    NFCRecord
//
// -------------------------------------------------------------------------

#include <exception>
#include "NFDataList.hpp"
#include "NFCRecord.h"

NFCRecord::NFCRecord()
{
    mSelf = NFGUID();

    mbSave = false;
    mbPublic = false;
    mbPrivate = false;
    mbCache = false;
	mbUpload = false;

    mstrRecordName = "";
    mnMaxRow = 0;

}

NFCRecord::NFCRecord(const NFGUID& self, const std::string& strRecordName, const NF_SHARE_PTR<NFDataList>& valueList, const NF_SHARE_PTR<NFDataList>& tagList, const int nMaxRow)
{
	mVarRecordType = valueList;
    mVarRecordTag = tagList;

    mSelf = self;

    mbSave = false;
    mbPublic = false;
    mbPrivate = false;
    mbCache = false;
	mbUpload = false;

    mstrRecordName = strRecordName;

    mnMaxRow = nMaxRow;
    
    mVecUsedState.resize(mnMaxRow);

    for (int i = 0; i < mnMaxRow; i++)
    {
        mVecUsedState[i] = 0;
    }

	//init share_pointer for all data
    for (int i = 0; i < GetRows() * GetCols(); i++)
    {
        mtRecordVec.push_back(NF_SHARE_PTR<NFData>());
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

NFDATA_TYPE NFCRecord::GetColType(const int nCol) const
{
    return mVarRecordType->Type(nCol);
}

const std::string& NFCRecord::GetColTag(const int nCol) const
{
    return mVarRecordTag->String(nCol);
}


int NFCRecord::AddRow(const int nRow)
{
    return AddRow(nRow, *mVarRecordType);
}

int NFCRecord::AddRow(const int nRow, const NFDataList& var)
{
	bool bCover = false;
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
	else
	{
		if (IsUsed(nFindRow))
	    {
	        bCover = true;
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

    SetUsed(nFindRow, 1);

    for (int i = 0; i < GetCols(); ++i)
    {
        NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nFindRow, i));
		if (nullptr == pVar)
		{
			pVar = NF_SHARE_PTR<NFData>(NF_NEW NFData(var.Type(i)));
		}

		pVar->variantData = var.GetStack(i)->variantData;
    }

	RECORD_EVENT_DATA xEventData;
	xEventData.nOpType = bCover? RECORD_EVENT_DATA::Cover : RECORD_EVENT_DATA::Add;
	xEventData.nRow = nFindRow;
	xEventData.nCol = 0;
	xEventData.strRecordName = mstrRecordName;

	NFData tData;
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

	NFData var;
    var.SetInt(value);

    NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

	if (mtRecordCallback.size() == 0)
	{
		pVar->SetInt(value);
	}
	else
	{
		NFData oldValue;
		oldValue.SetInt(pVar->GetInt());

		pVar->SetInt(value);

		RECORD_EVENT_DATA xEventData;
		xEventData.nOpType = RECORD_EVENT_DATA::Update;
		xEventData.nRow = nRow;
		xEventData.nCol = nCol;
		xEventData.strRecordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

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

    NFData var;
    var.SetFloat(value);

    NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

	if (mtRecordCallback.size() == 0)
	{
		pVar->SetFloat(value);
	}
	else
	{
		NFData oldValue;
		oldValue.SetFloat(pVar->GetFloat());
		pVar->SetFloat(value);

		RECORD_EVENT_DATA xEventData;
		xEventData.nOpType = RECORD_EVENT_DATA::Update;
		xEventData.nRow = nRow;
		xEventData.nCol = nCol;
		xEventData.strRecordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

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

    NFData var;
    var.SetString(value);

    NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

	if (mtRecordCallback.size() == 0)
	{
		pVar->SetString(value);
	}
	else
	{
		NFData oldValue;
		oldValue.SetString(pVar->GetString());

		pVar->SetString(value);

		RECORD_EVENT_DATA xEventData;
		xEventData.nOpType = RECORD_EVENT_DATA::Update;
		xEventData.nRow = nRow;
		xEventData.nCol = nCol;
		xEventData.strRecordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

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

    NFData var;
    var.SetObject(value);

    NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

	if (mtRecordCallback.size() == 0)
	{
		pVar->SetObject(value);
	}
	else
	{
		NFData oldValue;
		oldValue.SetObject(pVar->GetObject());

		pVar->SetObject(value);

		RECORD_EVENT_DATA xEventData;
		xEventData.nOpType = RECORD_EVENT_DATA::Update;
		xEventData.nRow = nRow;
		xEventData.nCol = nCol;
		xEventData.strRecordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

    return true;
}

bool NFCRecord::SetObject(const int nRow, const std::string& strColTag, const NFGUID& value)
{
	int nCol = GetCol(strColTag);
	return SetObject(nRow, nCol, value);
}

bool NFCRecord::SetVector2(const int nRow, const int nCol, const NFVector2& value)
{
	if (!ValidPos(nRow, nCol))
	{
		return false;
	}

	if (TDATA_VECTOR2 != GetColType(nCol))
	{
		return false;
	}

	if (!IsUsed(nRow))
	{
		return false;
	}

	NFData var;
	var.SetVector2(value);

	NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

	if (mtRecordCallback.size() == 0)
	{
		pVar->SetVector2(value);
	}
	else
	{
		NFData oldValue;
		oldValue.SetVector2(pVar->GetVector2());

		pVar->SetVector2(value);

		RECORD_EVENT_DATA xEventData;
		xEventData.nOpType = RECORD_EVENT_DATA::Update;
		xEventData.nRow = nRow;
		xEventData.nCol = nCol;
		xEventData.strRecordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

	return true;
}

bool NFCRecord::SetVector3(const int nRow, const int nCol, const NFVector3& value)
{
	if (!ValidPos(nRow, nCol))
	{
		return false;
	}

	if (TDATA_VECTOR3 != GetColType(nCol))
	{
		return false;
	}

	if (!IsUsed(nRow))
	{
		return false;
	}

	NFData var;
	var.SetVector3(value);

	NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));

	//must have memory
	if (nullptr == pVar)
	{
		return false;
	}

	if (var == *pVar)
	{
		return false;
	}

	if (mtRecordCallback.size() == 0)
	{
		pVar->SetVector3(value);
	}
	else
	{
		NFData oldValue;
		oldValue.SetVector3(pVar->GetVector3());

		pVar->SetVector3(value);

		RECORD_EVENT_DATA xEventData;
		xEventData.nOpType = RECORD_EVENT_DATA::Update;
		xEventData.nRow = nRow;
		xEventData.nCol = nCol;
		xEventData.strRecordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

	return true;
}

bool NFCRecord::SetVector2(const int nRow, const std::string& strColTag, const NFVector2& value)
{
	int nCol = GetCol(strColTag);
	return SetVector2(nRow, nCol, value);
}

bool NFCRecord::SetVector3(const int nRow, const std::string& strColTag, const NFVector3& value)
{
	int nCol = GetCol(strColTag);
	return SetVector3(nRow, nCol, value);
}


bool NFCRecord::QueryRow(const int nRow, NFDataList& varList)
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
        NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, i));
        if (pVar)
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

				case TDATA_VECTOR2:
					varList.Add(NFVector2());
					break;

				case TDATA_VECTOR3:
					varList.Add(NFVector3());
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

    const NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar)
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

    const NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar)
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

    const NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar)
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

    const  NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
    if (!pVar)
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

const NFVector2& NFCRecord::GetVector2(const int nRow, const int nCol) const
{
	if (!ValidPos(nRow, nCol))
	{
		return NULL_VECTOR2;
	}

	if (!IsUsed(nRow))
	{
		return NULL_VECTOR2;
	}

	const  NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
	if (!pVar)
	{
		return NULL_VECTOR2;
	}

	return pVar->GetVector2();
}

const NFVector2& NFCRecord::GetVector2(const int nRow, const std::string& strColTag) const
{
	int nCol = GetCol(strColTag);
	return GetVector2(nRow, nCol);
}

const NFVector3& NFCRecord::GetVector3(const int nRow, const int nCol) const
{
	if (!ValidPos(nRow, nCol))
	{
		return NULL_VECTOR3;
	}

	if (!IsUsed(nRow))
	{
		return NULL_VECTOR3;
	}

	const  NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, nCol));
	if (!pVar)
	{
		return NULL_VECTOR3;
	}

	return pVar->GetVector3();
}

const NFVector3& NFCRecord::GetVector3(const int nRow, const std::string& strColTag) const
{
	int nCol = GetCol(strColTag);
	return GetVector3(nRow, nCol);
}

int NFCRecord::FindRowByColValue(const int nCol, const NFData& var, NFDataList& varResult)
{
    if (!ValidCol(nCol))
    {
        return -1;
    }

    NFDATA_TYPE eType = var.GetType();
    if (eType != mVarRecordType->Type(nCol))
    {
        return -1;
    }

    switch (eType)
    {
        case TDATA_INT:
            return FindInt(nCol, var.GetInt(), varResult);
            break;

        case TDATA_FLOAT:
            return FindFloat(nCol, var.GetFloat(), varResult);
            break;

        case TDATA_STRING:
            return FindString(nCol, var.GetString(), varResult);
            break;

        case TDATA_OBJECT:
            return FindObject(nCol, var.GetObject(), varResult);
            break;

		case TDATA_VECTOR2:
			return FindVector2(nCol, var.GetVector2(), varResult);
			break;

        case TDATA_VECTOR3:
			return FindVector3(nCol, var.GetVector3(), varResult);
			break;

        default:
            break;
    }

    return -1;
}

int NFCRecord::FindRowByColValue(const std::string& strColTag, const NFData& var, NFDataList& varResult)
{
    int nCol = GetCol(strColTag);
    return FindRowByColValue(nCol, var, varResult);
}

int NFCRecord::FindInt(const int nCol, const NFINT64 value, NFDataList& varResult)
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

int NFCRecord::FindInt(const std::string& strColTag, const NFINT64 value, NFDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindInt(nCol, value, varResult);
}

int NFCRecord::FindFloat(const int nCol, const double value, NFDataList& varResult)
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

int NFCRecord::FindFloat(const std::string& strColTag, const double value, NFDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindFloat(nCol, value, varResult);
}

int NFCRecord::FindString(const int nCol, const std::string& value, NFDataList& varResult)
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
        for (int i = 0; i < mnMaxRow; ++i)
        {
            if (!IsUsed(i))
            {
                continue;
            }

            const std::string& strData = GetString(i, nCol);
            if (0 == strcmp(strData.c_str(), value.c_str()))
            {
                varResult << (int64_t)i;
            }
        }

        return varResult.GetCount();
    }

    return -1;
}

int NFCRecord::FindString(const std::string& strColTag, const std::string& value, NFDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindString(nCol, value, varResult);
}

int NFCRecord::FindObject(const int nCol, const NFGUID& value, NFDataList& varResult)
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
        for (int i = 0; i < mnMaxRow; ++i)
        {
            if (!IsUsed(i))
            {
                continue;
            }

            if (GetObject(i, nCol) == value)
            {
                varResult << (int64_t)i;
            }
        }

        return varResult.GetCount();
    }

    return -1;
}

int NFCRecord::FindObject(const std::string& strColTag, const NFGUID& value, NFDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindObject(nCol, value, varResult);
}

int NFCRecord::FindVector2(const int nCol, const NFVector2& value, NFDataList& varResult)
{
	if (!ValidCol(nCol))
	{
		return -1;
	}

	if (TDATA_VECTOR2 != mVarRecordType->Type(nCol))
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

			if (GetVector2(i, nCol) == value)
			{
				varResult << (int64_t)i;
			}
		}

		return varResult.GetCount();
	}

	return -1;
}

int NFCRecord::FindVector2(const std::string& strColTag, const NFVector2& value, NFDataList& varResult)
{
	if (strColTag.empty())
	{
		return -1;
	}

	int nCol = GetCol(strColTag);
	return FindVector2(nCol, value, varResult);
}

int NFCRecord::FindVector3(const int nCol, const NFVector3& value, NFDataList& varResult)
{
	if (!ValidCol(nCol))
	{
		return -1;
	}

	if (TDATA_VECTOR3 != mVarRecordType->Type(nCol))
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

			if (GetVector3(i, nCol) == value)
			{
				varResult << (int64_t)i;
			}
		}

		return varResult.GetCount();
	}

	return -1;
}

int NFCRecord::FindRowByColValue(const int nCol, const NFData & var)
{
	NFDataList xDataList;
	int nRowCount = FindRowByColValue(nCol, var, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindInt(const int nCol, const NFINT64 value)
{
	NFDataList xDataList;
	int nRowCount = FindInt(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindFloat(const int nCol, const double value)
{
	NFDataList xDataList;
	int nRowCount = FindFloat(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindString(const int nCol, const std::string & value)
{
	NFDataList xDataList;
	int nRowCount = FindString(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindObject(const int nCol, const NFGUID & value)
{
	NFDataList xDataList;
	int nRowCount = FindObject(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindVector2(const int nCol, const NFVector2 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector2(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindVector3(const int nCol, const NFVector3 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector3(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindVector3(const std::string& strColTag, const NFVector3& value, NFDataList& varResult)
{
	if (strColTag.empty())
	{
		return -1;
	}

	int nCol = GetCol(strColTag);
	return FindVector3(nCol, value, varResult);
}

int NFCRecord::FindRowByColValue(const std::string & strColTag, const NFData & var)
{
	NFDataList xDataList;
	int nRowCount = FindRowByColValue(strColTag, var, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindInt(const std::string & strColTag, const NFINT64 value)
{
	NFDataList xDataList;
	int nRowCount = FindInt(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindFloat(const std::string & strColTag, const double value)
{
	NFDataList xDataList;
	int nRowCount = FindFloat(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindString(const std::string & strColTag, const std::string & value)
{
	NFDataList xDataList;
	int nRowCount = FindString(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindObject(const std::string & strColTag, const NFGUID & value)
{
	NFDataList xDataList;
	int nRowCount = FindObject(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindVector2(const std::string & strColTag, const NFVector2 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector2(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFCRecord::FindVector3(const std::string & strColTag, const NFVector3 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector3(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

bool NFCRecord::Remove(const int nRow)
{
    if (ValidRow(nRow))
    {
        if (IsUsed(nRow))
        {
			RECORD_EVENT_DATA xEventData;
			xEventData.nOpType = RECORD_EVENT_DATA::Del;
			xEventData.nRow = nRow;
			xEventData.nCol = 0;
			xEventData.strRecordName = mstrRecordName;

			OnEventHandler(mSelf, xEventData, NFData(), NFData());

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

const bool NFCRecord::GetUpload()
{
	return mbUpload;
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

void NFCRecord::SetUpload(const bool bUpload)
{
	mbUpload = bUpload;
}

void NFCRecord::SetPublic(const bool bPublic)
{
    mbPublic = bPublic;
}

void NFCRecord::SetPrivate(const bool bPrivate)
{
    mbPrivate = bPrivate;
}

void NFCRecord::SetName(const std::string& strName)
{
    mstrRecordName = strName;
}

const NF_SHARE_PTR<NFDataList> NFCRecord::GetInitData() const
{
    NF_SHARE_PTR<NFDataList> pIniData = NF_SHARE_PTR<NFDataList>( NF_NEW NFDataList());
    pIniData->Append(*mVarRecordType);

    return pIniData;
}

void NFCRecord::OnEventHandler(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    TRECORDCALLBACKEX::iterator itr = mtRecordCallback.begin();
    TRECORDCALLBACKEX::iterator end = mtRecordCallback.end();
    for (; itr != end; ++itr)
    {
        
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
            NF_SHARE_PTR<NFData> pOrigin = mtRecordVec.at(GetPos(nOriginRow, i));
            mtRecordVec[GetPos(nOriginRow, i)] = mtRecordVec.at(GetPos(nTargetRow, i));
            mtRecordVec[GetPos(nTargetRow, i)] = pOrigin;
        }

        int nOriginUse = mVecUsedState[nOriginRow];
        mVecUsedState[nOriginRow] = mVecUsedState[nTargetRow];
        mVecUsedState[nTargetRow] = nOriginUse;

        RECORD_EVENT_DATA xEventData;
        xEventData.nOpType = RECORD_EVENT_DATA::Swap;
        xEventData.nRow = nOriginRow;
        xEventData.nCol = nTargetRow;
        xEventData.strRecordName = mstrRecordName;

        NFData xData;
        OnEventHandler(mSelf, xEventData, xData, xData);

        return true;
    }

    return false;
}

const NF_SHARE_PTR<NFDataList> NFCRecord::GetTag() const
{
    NF_SHARE_PTR<NFDataList> pIniData = NF_SHARE_PTR<NFDataList>(NF_NEW NFDataList());
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

bool NFCRecord::PreAllocMemoryForRow(const int nRow)
{
	if (!IsUsed(nRow))
	{
		return false;
	}

	for (int i = 0; i < GetCols(); ++i)
	{
		NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, i));
		if (nullptr == pVar)
		{
			pVar = NF_SHARE_PTR<NFData>(NF_NEW NFData(mVarRecordType->Type(i)));
		}

		pVar->variantData = mVarRecordType->GetStack(i)->variantData;
	}
	return true;
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
