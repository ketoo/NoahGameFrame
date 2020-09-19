/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: lvsheng.huang
   
   NoahFrame is open-source software and you can redistribute it and/or modify
   it under the terms of the License; besides, anyone who use this file/software must include this copyright announcement.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/



#include <exception>
#include "NFDataList.hpp"
#include "NFRecord.h"

NFRecord::NFRecord()
{
    mSelf = NFGUID();

    mbSave = false;
    mbPublic = false;
    mbPrivate = false;
    mbCache = false;
	mbRef = false;
	mbForce = false;
	mbUpload = false;

    mstrRecordName = "";
    mnMaxRow = 0;

}

NFRecord::NFRecord(const NFGUID& self, const std::string& recordName, const NF_SHARE_PTR<NFDataList>& valueList, const NF_SHARE_PTR<NFDataList>& tagList, const int nMaxRow)
{
	mVarRecordType = valueList;
    mVarRecordTag = tagList;

    mSelf = self;

    mbSave = false;
    mbPublic = false;
    mbPrivate = false;
    mbCache = false;
	mbUpload = false;

    mstrRecordName = recordName;

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

	//it would be optimized in future as it should apply the memory by onetime
    for (int i = 0; i < mVarRecordTag->GetCount(); ++i)
    {
        if (!mVarRecordTag->String(i).empty())
        {
            mmTag[mVarRecordTag->String(i)] = i;
        }
    }
}

NFRecord::~NFRecord()
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

std::string NFRecord::ToString()
{
    std::stringstream ss;
    ss << this->GetName() << std::endl;

    for (int i = 0; i < this->GetRows(); ++i)
    {
        if (IsUsed(i))
        {
            NFDataList rowDataList;
            if (this->QueryRow(i, rowDataList))
            {
                ss << "ROW:" << i << "==>" << rowDataList.ToString() << std::endl;
            }
        }
    }

    return ss.str();
}
void NFRecord::ToMemoryCounterString(std::string& data)
{
    data.append(this->mSelf.ToString());
    data.append(":");
    data.append(this->GetName());
    data.append(" ");
}

int NFRecord::GetCols() const
{
    return mVarRecordType->GetCount();
}

int NFRecord::GetRows() const
{
    return mnMaxRow;
}

int NFRecord::GetUsedRows() const
{
	int usedCount = 0;
	for (int i = 0; i < mVecUsedState.size(); ++i)
	{
		if (mVecUsedState[i] > 0)
		{
			usedCount++;
		}
	}

	return usedCount;
}

NFDATA_TYPE NFRecord::GetColType(const int col) const
{
    return mVarRecordType->Type(col);
}

const std::string& NFRecord::GetColTag(const int col) const
{
    return mVarRecordTag->String(col);
}


int NFRecord::AddRow(const int row)
{
    return AddRow(row, *mVarRecordType);
}

int NFRecord::AddRow(const int row, const NFDataList& var)
{
	bool bCover = false;
    int nFindRow = row;
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
	xEventData.row = nFindRow;
	xEventData.col = 0;
	xEventData.recordName = mstrRecordName;

	NFData tData;
    OnEventHandler(mSelf, xEventData, tData, tData);

    return nFindRow;
}

bool NFRecord::SetRow(const int row, const NFDataList & var)
{
	if (var.GetCount() != GetCols())
	{
		return false;
	}

	if (!IsUsed(row))
	{
		return false;
	}

	for (int i = 0; i < GetCols(); ++i)
	{
		if (var.Type(i) != GetColType(i))
		{
			return false;
		}
	}

	for (int i = 0; i < GetCols(); ++i)
	{
		NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, i));
		if (nullptr == pVar)
		{
			pVar = NF_SHARE_PTR<NFData>(NF_NEW NFData(var.Type(i)));
		}

		NFData oldValue = *pVar;

		pVar->variantData = var.GetStack(i)->variantData;

		RECORD_EVENT_DATA xEventData;
		xEventData.nOpType = RECORD_EVENT_DATA::Update;
		xEventData.row = row;
		xEventData.col = i;
		xEventData.recordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

	return false;
}

bool NFRecord::SetInt(const int row, const int col, const NFINT64 value)
{
    if (!ValidPos(row, col))
    {
        return false;
    }

    if (TDATA_INT != GetColType(col))
    {
        return false;
    }

    if (!IsUsed(row))
    {
        return false;
    }

	NFData var;
    var.SetInt(value);

    NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));
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
		xEventData.row = row;
		xEventData.col = col;
		xEventData.recordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

    return true;
}

bool NFRecord::SetInt(const int row, const std::string& colTag, const NFINT64 value)
{
    int col = GetCol(colTag);
    return SetInt(row, col, value);
}

bool NFRecord::SetFloat(const int row, const int col, const double value)
{
    if (!ValidPos(row, col))
    {
        return false;
    }

    if (TDATA_FLOAT != GetColType(col))
    {
        return false;
    }

    if (!IsUsed(row))
    {
        return false;
    }

    NFData var;
    var.SetFloat(value);

    NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));

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
		xEventData.row = row;
		xEventData.col = col;
		xEventData.recordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

    return true;
}

bool NFRecord::SetFloat(const int row, const std::string& colTag, const double value)
{
    int col = GetCol(colTag);
    return SetFloat(row, col, value);
}

bool NFRecord::SetString(const int row, const int col, const std::string& value)
{
    if (!ValidPos(row, col))
    {
        return false;
    }

    if (TDATA_STRING != GetColType(col))
    {
        return false;
    }

    if (!IsUsed(row))
    {
        return false;
    }

    NFData var;
    var.SetString(value);

    NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));

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
		xEventData.row = row;
		xEventData.col = col;
		xEventData.recordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

    return true;
}

bool NFRecord::SetString(const int row, const std::string& colTag, const std::string& value)
{
    int col = GetCol(colTag);
    return SetString(row, col, value);
}

bool NFRecord::SetObject(const int row, const int col, const NFGUID& value)
{
    if (!ValidPos(row, col))
    {
        return false;
    }

    if (TDATA_OBJECT != GetColType(col))
    {
        return false;
    }

    if (!IsUsed(row))
    {
        return false;
    }

    NFData var;
    var.SetObject(value);

    NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));

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
		xEventData.row = row;
		xEventData.col = col;
		xEventData.recordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

    return true;
}

bool NFRecord::SetObject(const int row, const std::string& colTag, const NFGUID& value)
{
	int col = GetCol(colTag);
	return SetObject(row, col, value);
}

bool NFRecord::SetVector2(const int row, const int col, const NFVector2& value)
{
	if (!ValidPos(row, col))
	{
		return false;
	}

	if (TDATA_VECTOR2 != GetColType(col))
	{
		return false;
	}

	if (!IsUsed(row))
	{
		return false;
	}

	NFData var;
	var.SetVector2(value);

	NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));

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
		xEventData.row = row;
		xEventData.col = col;
		xEventData.recordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

	return true;
}

bool NFRecord::SetVector3(const int row, const int col, const NFVector3& value)
{
	if (!ValidPos(row, col))
	{
		return false;
	}

	if (TDATA_VECTOR3 != GetColType(col))
	{
		return false;
	}

	if (!IsUsed(row))
	{
		return false;
	}

	NFData var;
	var.SetVector3(value);

	NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));

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
		xEventData.row = row;
		xEventData.col = col;
		xEventData.recordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

	return true;
}

bool NFRecord::SetVector2(const int row, const std::string& colTag, const NFVector2& value)
{
	int col = GetCol(colTag);
	return SetVector2(row, col, value);
}

bool NFRecord::SetVector3(const int row, const std::string& colTag, const NFVector3& value)
{
	int col = GetCol(colTag);
	return SetVector3(row, col, value);
}


bool NFRecord::QueryRow(const int row, NFDataList& varList)
{
    if (!ValidRow(row))
    {
        return false;
    }

    if (!IsUsed(row))
    {
        return false;
    }

    varList.Clear();
    for (int i = 0; i < GetCols(); ++i)
    {
        NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, i));
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

NFINT64 NFRecord::GetInt(const int row, const int col) const
{
    if (!ValidPos(row, col))
    {
        return 0;
    }

    if (!IsUsed(row))
    {
        return 0;
    }

    const NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));
    if (!pVar)
    {
        return 0;
    }

    return pVar->GetInt();
}

NFINT64 NFRecord::GetInt(const int row, const std::string& colTag) const
{
    int col = GetCol(colTag);
    return GetInt(row, col);
}

double NFRecord::GetFloat(const int row, const int col) const
{
    if (!ValidPos(row, col))
    {
        return 0.0f;
    }

    if (!IsUsed(row))
    {
        return 0.0f;
    }

    const NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));
    if (!pVar)
    {
        return 0.0f;
    }

    return pVar->GetFloat();
}

double NFRecord::GetFloat(const int row, const std::string& colTag) const
{
    int col = GetCol(colTag);
    return GetFloat(row, col);
}

const std::string& NFRecord::GetString(const int row, const int col) const
{
    if (!ValidPos(row, col))
    {
        return NULL_STR;
    }

    if (!IsUsed(row))
    {
        return NULL_STR;
    }

    const NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));
    if (!pVar)
    {
        return NULL_STR;
    }

    return pVar->GetString();
}

const std::string& NFRecord::GetString(const int row, const std::string& colTag) const
{
    int col = GetCol(colTag);
    return GetString(row, col);
}

const NFGUID& NFRecord::GetObject(const int row, const int col) const
{
    if (!ValidPos(row, col))
    {
        return NULL_OBJECT;
    }

    if (!IsUsed(row))
    {
        return NULL_OBJECT;
    }

    const  NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));
    if (!pVar)
    {
        return NULL_OBJECT;
    }

    return pVar->GetObject();
}

const NFGUID& NFRecord::GetObject(const int row, const std::string& colTag) const
{
    int col = GetCol(colTag);
    return GetObject(row, col);
}

const NFVector2& NFRecord::GetVector2(const int row, const int col) const
{
	if (!ValidPos(row, col))
	{
		return NULL_VECTOR2;
	}

	if (!IsUsed(row))
	{
		return NULL_VECTOR2;
	}

	const  NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));
	if (!pVar)
	{
		return NULL_VECTOR2;
	}

	return pVar->GetVector2();
}

const NFVector2& NFRecord::GetVector2(const int row, const std::string& colTag) const
{
	int col = GetCol(colTag);
	return GetVector2(row, col);
}

const NFVector3& NFRecord::GetVector3(const int row, const int col) const
{
	if (!ValidPos(row, col))
	{
		return NULL_VECTOR3;
	}

	if (!IsUsed(row))
	{
		return NULL_VECTOR3;
	}

	const  NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, col));
	if (!pVar)
	{
		return NULL_VECTOR3;
	}

	return pVar->GetVector3();
}

const NFVector3& NFRecord::GetVector3(const int row, const std::string& colTag) const
{
	int col = GetCol(colTag);
	return GetVector3(row, col);
}

int NFRecord::FindRowByColValue(const int col, const NFData& var, NFDataList& varResult)
{
    if (!ValidCol(col))
    {
        return -1;
    }

    NFDATA_TYPE eType = var.GetType();
    if (eType != mVarRecordType->Type(col))
    {
        return -1;
    }

    switch (eType)
    {
        case TDATA_INT:
            return FindInt(col, var.GetInt(), varResult);
            break;

        case TDATA_FLOAT:
            return FindFloat(col, var.GetFloat(), varResult);
            break;

        case TDATA_STRING:
            return FindString(col, var.GetString(), varResult);
            break;

        case TDATA_OBJECT:
            return FindObject(col, var.GetObject(), varResult);
            break;

		case TDATA_VECTOR2:
			return FindVector2(col, var.GetVector2(), varResult);
			break;

        case TDATA_VECTOR3:
			return FindVector3(col, var.GetVector3(), varResult);
			break;

        default:
            break;
    }

    return -1;
}

int NFRecord::FindRowByColValue(const std::string& colTag, const NFData& var, NFDataList& varResult)
{
    int col = GetCol(colTag);
    return FindRowByColValue(col, var, varResult);
}

int NFRecord::FindInt(const int col, const NFINT64 value, NFDataList& varResult)
{
    if (!ValidCol(col))
    {
        return -1;
    }

    if (TDATA_INT != mVarRecordType->Type(col))
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

            if (GetInt(i, col) == value)
            {
                varResult << i;
            }
        }

        return varResult.GetCount();
    }

    return -1;
}

int NFRecord::FindInt(const std::string& colTag, const NFINT64 value, NFDataList& varResult)
{
    if (colTag.empty())
    {
        return -1;
    }

    int col = GetCol(colTag);
    return FindInt(col, value, varResult);
}

int NFRecord::FindFloat(const int col, const double value, NFDataList& varResult)
{
    if (!ValidCol(col))
    {
        return -1;
    }

    if (TDATA_FLOAT != mVarRecordType->Type(col))
    {
        return -1;
    }

    for (int i = 0; i < mnMaxRow; ++i)
    {
        if (!IsUsed(i))
        {
            continue;
        }

        if (GetFloat(i, col) == value)
        {
            varResult << i;
        }
    }

    return varResult.GetCount();
}

int NFRecord::FindFloat(const std::string& colTag, const double value, NFDataList& varResult)
{
    if (colTag.empty())
    {
        return -1;
    }

    int col = GetCol(colTag);
    return FindFloat(col, value, varResult);
}

int NFRecord::FindString(const int col, const std::string& value, NFDataList& varResult)
{
    if (!ValidCol(col))
    {
        return -1;
    }

    if (TDATA_STRING != mVarRecordType->Type(col))
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

            const std::string& strData = GetString(i, col);
            if (0 == strcmp(strData.c_str(), value.c_str()))
            {
                varResult << (int64_t)i;
            }
        }

        return varResult.GetCount();
    }

    return -1;
}

int NFRecord::FindString(const std::string& colTag, const std::string& value, NFDataList& varResult)
{
    if (colTag.empty())
    {
        return -1;
    }

    int col = GetCol(colTag);
    return FindString(col, value, varResult);
}

int NFRecord::FindObject(const int col, const NFGUID& value, NFDataList& varResult)
{
    if (!ValidCol(col))
    {
        return -1;
    }

    if (TDATA_OBJECT != mVarRecordType->Type(col))
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

            if (GetObject(i, col) == value)
            {
                varResult << (int64_t)i;
            }
        }

        return varResult.GetCount();
    }

    return -1;
}

int NFRecord::FindObject(const std::string& colTag, const NFGUID& value, NFDataList& varResult)
{
    if (colTag.empty())
    {
        return -1;
    }

    int col = GetCol(colTag);
    return FindObject(col, value, varResult);
}

int NFRecord::FindVector2(const int col, const NFVector2& value, NFDataList& varResult)
{
	if (!ValidCol(col))
	{
		return -1;
	}

	if (TDATA_VECTOR2 != mVarRecordType->Type(col))
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

			if (GetVector2(i, col) == value)
			{
				varResult << (int64_t)i;
			}
		}

		return varResult.GetCount();
	}

	return -1;
}

int NFRecord::FindVector2(const std::string& colTag, const NFVector2& value, NFDataList& varResult)
{
	if (colTag.empty())
	{
		return -1;
	}

	int col = GetCol(colTag);
	return FindVector2(col, value, varResult);
}

int NFRecord::FindVector3(const int col, const NFVector3& value, NFDataList& varResult)
{
	if (!ValidCol(col))
	{
		return -1;
	}

	if (TDATA_VECTOR3 != mVarRecordType->Type(col))
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

			if (GetVector3(i, col) == value)
			{
				varResult << (int64_t)i;
			}
		}

		return varResult.GetCount();
	}

	return -1;
}

int NFRecord::FindRowByColValue(const int col, const NFData & var)
{
	NFDataList xDataList;
	int nRowCount = FindRowByColValue(col, var, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindInt(const int col, const NFINT64 value)
{
	NFDataList xDataList;
	int nRowCount = FindInt(col, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindFloat(const int col, const double value)
{
	NFDataList xDataList;
	int nRowCount = FindFloat(col, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindString(const int col, const std::string & value)
{
	NFDataList xDataList;
	int nRowCount = FindString(col, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindObject(const int col, const NFGUID & value)
{
	NFDataList xDataList;
	int nRowCount = FindObject(col, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector2(const int col, const NFVector2 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector2(col, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector3(const int col, const NFVector3 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector3(col, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector3(const std::string& colTag, const NFVector3& value, NFDataList& varResult)
{
	if (colTag.empty())
	{
		return -1;
	}

	int col = GetCol(colTag);
	return FindVector3(col, value, varResult);
}

int NFRecord::FindRowByColValue(const std::string & colTag, const NFData & var)
{
	NFDataList xDataList;
	int nRowCount = FindRowByColValue(colTag, var, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindInt(const std::string & colTag, const NFINT64 value)
{
	NFDataList xDataList;
	int nRowCount = FindInt(colTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindFloat(const std::string & colTag, const double value)
{
	NFDataList xDataList;
	int nRowCount = FindFloat(colTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindString(const std::string & colTag, const std::string & value)
{
	NFDataList xDataList;
	int nRowCount = FindString(colTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindObject(const std::string & colTag, const NFGUID & value)
{
	NFDataList xDataList;
	int nRowCount = FindObject(colTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector2(const std::string & colTag, const NFVector2 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector2(colTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector3(const std::string & colTag, const NFVector3 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector3(colTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

bool NFRecord::Remove(const int row)
{
    if (ValidRow(row))
    {
        if (IsUsed(row))
        {
			RECORD_EVENT_DATA xEventData;
			xEventData.nOpType = RECORD_EVENT_DATA::Del;
			xEventData.row = row;
			xEventData.col = 0;
			xEventData.recordName = mstrRecordName;

			OnEventHandler(mSelf, xEventData, NFData(), NFData());

			mVecUsedState[row] = 0;

			xEventData.nOpType = RECORD_EVENT_DATA::AfterDel;

			OnEventHandler(mSelf, xEventData, NFData(), NFData());

			return true;
        }
    }

    return false;
}

bool NFRecord::Clear()
{
    for (int i = GetRows() - 1; i >= 0; i--)
    {
        Remove(i);
    }

    return true;
}

void NFRecord::AddRecordHook(const RECORD_EVENT_FUNCTOR_PTR& cb)
{
    mtRecordCallback.push_back(cb);
}

const bool NFRecord::GetSave()
{
    return mbSave;
}

const bool NFRecord::GetCache()
{
    return mbCache;
}

const bool NFRecord::GetRef()
{
	return mbRef;
}

const bool NFRecord::GetForce()
{
	return mbForce;
}

const bool NFRecord::GetUpload()
{
	return mbUpload;
}

const bool NFRecord::GetPublic()
{
    return mbPublic;
}

const bool NFRecord::GetPrivate()
{
    return mbPrivate;
}

int NFRecord::GetPos(int row, int col) const
{
    return row * mVarRecordType->GetCount() + col;
}

const std::string& NFRecord::GetName() const
{
    return mstrRecordName;
}

void NFRecord::SetSave(const bool bSave)
{
    mbSave = bSave;
}

void NFRecord::SetCache(const bool bCache)
{
    mbCache = bCache;
}

void NFRecord::SetRef(const bool bRef)
{
	mbRef = bRef;
}

void NFRecord::SetForce(const bool bForce)
{
	mbForce = bForce;
}

void NFRecord::SetUpload(const bool bUpload)
{
	mbUpload = bUpload;
}

void NFRecord::SetPublic(const bool bPublic)
{
    mbPublic = bPublic;
}

void NFRecord::SetPrivate(const bool bPrivate)
{
    mbPrivate = bPrivate;
}

void NFRecord::SetName(const std::string& name)
{
    mstrRecordName = name;
}

NF_SHARE_PTR<NFDataList> NFRecord::GetInitData() const
{
    NF_SHARE_PTR<NFDataList> pIniData = NF_SHARE_PTR<NFDataList>( NF_NEW NFDataList());
    pIniData->Append(*mVarRecordType);

    return pIniData;
}

void NFRecord::OnEventHandler(const NFGUID& self, const RECORD_EVENT_DATA& xEventData, const NFData& oldVar, const NFData& newVar)
{
    TRECORDCALLBACKEX::iterator itr = mtRecordCallback.begin();
    TRECORDCALLBACKEX::iterator end = mtRecordCallback.end();
    for (; itr != end; ++itr)
    {
        
        RECORD_EVENT_FUNCTOR_PTR functorPtr = *itr;
        functorPtr->operator()(self, xEventData, oldVar, newVar);
    }
}

bool NFRecord::IsUsed(const int row) const
{
    if (ValidRow(row))
    {
        return (mVecUsedState[row] > 0);
    }

    return false;
}

bool NFRecord::SwapRowInfo(const int nOriginRow, const int nTargetRow)
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
        xEventData.row = nOriginRow;
        xEventData.col = nTargetRow;
        xEventData.recordName = mstrRecordName;

        NFData xData;
        OnEventHandler(mSelf, xEventData, xData, xData);

        return true;
    }

    return false;
}

const NF_SHARE_PTR<NFDataList> NFRecord::GetTag() const
{
	//TODO
    NF_SHARE_PTR<NFDataList> pIniData = NF_SHARE_PTR<NFDataList>(NF_NEW NFDataList());
    pIniData->Append(*mVarRecordTag);
    return pIniData;
}

const NFIRecord::TRECORDVEC& NFRecord::GetRecordVec() const
{
    return mtRecordVec;
}

bool NFRecord::SetUsed(const int row, const int bUse)
{
    if (ValidRow(row))
    {
        mVecUsedState[row] = bUse;
        return true;
    }

    return false;
}

bool NFRecord::PreAllocMemoryForRow(const int row)
{
	if (!IsUsed(row))
	{
		return false;
	}

	for (int i = 0; i < GetCols(); ++i)
	{
		NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(row, i));
		if (nullptr == pVar)
		{
			pVar = NF_SHARE_PTR<NFData>(NF_NEW NFData(mVarRecordType->Type(i)));
		}

		pVar->variantData = mVarRecordType->GetStack(i)->variantData;
	}
	return true;
}

bool NFRecord::ValidPos(int row, int col) const
{
    if (ValidCol(col)
        && ValidRow(row))
    {
        return true;
    }

    return false;
}

bool NFRecord::ValidRow(int row) const
{
    if (row >= GetRows() || row < 0)
    {
        return false;
    }

    return true;
}

bool NFRecord::ValidCol(int col) const
{
    if (col >= GetCols() || col < 0)
    {
        return false;
    }

    return true;
}

int NFRecord::GetCol(const std::string& strTag) const
{
    std::map<std::string, int>::const_iterator it = mmTag.find(strTag);
    if (it != mmTag.end())
    {
        return it->second;
    }

    return -1;
}
