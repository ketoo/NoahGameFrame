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

NFRecord::NFRecord(const NFGUID& self, const std::string& strRecordName, const NF_SHARE_PTR<NFDataList>& valueList, const NF_SHARE_PTR<NFDataList>& tagList, const int nMaxRow)
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

NFDATA_TYPE NFRecord::GetColType(const int nCol) const
{
    return mVarRecordType->Type(nCol);
}

const std::string& NFRecord::GetColTag(const int nCol) const
{
    return mVarRecordTag->String(nCol);
}


int NFRecord::AddRow(const int nRow)
{
    return AddRow(nRow, *mVarRecordType);
}

int NFRecord::AddRow(const int nRow, const NFDataList& var)
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
    OnEventHandler(mSelf, xEventData, tData, tData);

    return nFindRow;
}

bool NFRecord::SetRow(const int nRow, const NFDataList & var)
{
	if (var.GetCount() != GetCols())
	{
		return false;
	}

	if (!IsUsed(nRow))
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
		NF_SHARE_PTR<NFData>& pVar = mtRecordVec.at(GetPos(nRow, i));
		if (nullptr == pVar)
		{
			pVar = NF_SHARE_PTR<NFData>(NF_NEW NFData(var.Type(i)));
		}

		NFData oldValue = *pVar;

		pVar->variantData = var.GetStack(i)->variantData;

		RECORD_EVENT_DATA xEventData;
		xEventData.nOpType = RECORD_EVENT_DATA::Update;
		xEventData.nRow = nRow;
		xEventData.nCol = i;
		xEventData.strRecordName = mstrRecordName;

		OnEventHandler(mSelf, xEventData, oldValue, *pVar);
	}

	return false;
}

bool NFRecord::SetInt(const int nRow, const int nCol, const NFINT64 value)
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

bool NFRecord::SetInt(const int nRow, const std::string& strColTag, const NFINT64 value)
{
    int nCol = GetCol(strColTag);
    return SetInt(nRow, nCol, value);
}

bool NFRecord::SetFloat(const int nRow, const int nCol, const double value)
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

bool NFRecord::SetFloat(const int nRow, const std::string& strColTag, const double value)
{
    int nCol = GetCol(strColTag);
    return SetFloat(nRow, nCol, value);
}

bool NFRecord::SetString(const int nRow, const int nCol, const std::string& value)
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

bool NFRecord::SetString(const int nRow, const std::string& strColTag, const std::string& value)
{
    int nCol = GetCol(strColTag);
    return SetString(nRow, nCol, value);
}

bool NFRecord::SetObject(const int nRow, const int nCol, const NFGUID& value)
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

bool NFRecord::SetObject(const int nRow, const std::string& strColTag, const NFGUID& value)
{
	int nCol = GetCol(strColTag);
	return SetObject(nRow, nCol, value);
}

bool NFRecord::SetVector2(const int nRow, const int nCol, const NFVector2& value)
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

bool NFRecord::SetVector3(const int nRow, const int nCol, const NFVector3& value)
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

bool NFRecord::SetVector2(const int nRow, const std::string& strColTag, const NFVector2& value)
{
	int nCol = GetCol(strColTag);
	return SetVector2(nRow, nCol, value);
}

bool NFRecord::SetVector3(const int nRow, const std::string& strColTag, const NFVector3& value)
{
	int nCol = GetCol(strColTag);
	return SetVector3(nRow, nCol, value);
}


bool NFRecord::QueryRow(const int nRow, NFDataList& varList)
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

NFINT64 NFRecord::GetInt(const int nRow, const int nCol) const
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

NFINT64 NFRecord::GetInt(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetInt(nRow, nCol);
}

double NFRecord::GetFloat(const int nRow, const int nCol) const
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

double NFRecord::GetFloat(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetFloat(nRow, nCol);
}

const std::string& NFRecord::GetString(const int nRow, const int nCol) const
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

const std::string& NFRecord::GetString(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetString(nRow, nCol);
}

const NFGUID& NFRecord::GetObject(const int nRow, const int nCol) const
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

const NFGUID& NFRecord::GetObject(const int nRow, const std::string& strColTag) const
{
    int nCol = GetCol(strColTag);
    return GetObject(nRow, nCol);
}

const NFVector2& NFRecord::GetVector2(const int nRow, const int nCol) const
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

const NFVector2& NFRecord::GetVector2(const int nRow, const std::string& strColTag) const
{
	int nCol = GetCol(strColTag);
	return GetVector2(nRow, nCol);
}

const NFVector3& NFRecord::GetVector3(const int nRow, const int nCol) const
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

const NFVector3& NFRecord::GetVector3(const int nRow, const std::string& strColTag) const
{
	int nCol = GetCol(strColTag);
	return GetVector3(nRow, nCol);
}

int NFRecord::FindRowByColValue(const int nCol, const NFData& var, NFDataList& varResult)
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

int NFRecord::FindRowByColValue(const std::string& strColTag, const NFData& var, NFDataList& varResult)
{
    int nCol = GetCol(strColTag);
    return FindRowByColValue(nCol, var, varResult);
}

int NFRecord::FindInt(const int nCol, const NFINT64 value, NFDataList& varResult)
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

int NFRecord::FindInt(const std::string& strColTag, const NFINT64 value, NFDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindInt(nCol, value, varResult);
}

int NFRecord::FindFloat(const int nCol, const double value, NFDataList& varResult)
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

int NFRecord::FindFloat(const std::string& strColTag, const double value, NFDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindFloat(nCol, value, varResult);
}

int NFRecord::FindString(const int nCol, const std::string& value, NFDataList& varResult)
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

int NFRecord::FindString(const std::string& strColTag, const std::string& value, NFDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindString(nCol, value, varResult);
}

int NFRecord::FindObject(const int nCol, const NFGUID& value, NFDataList& varResult)
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

int NFRecord::FindObject(const std::string& strColTag, const NFGUID& value, NFDataList& varResult)
{
    if (strColTag.empty())
    {
        return -1;
    }

    int nCol = GetCol(strColTag);
    return FindObject(nCol, value, varResult);
}

int NFRecord::FindVector2(const int nCol, const NFVector2& value, NFDataList& varResult)
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

int NFRecord::FindVector2(const std::string& strColTag, const NFVector2& value, NFDataList& varResult)
{
	if (strColTag.empty())
	{
		return -1;
	}

	int nCol = GetCol(strColTag);
	return FindVector2(nCol, value, varResult);
}

int NFRecord::FindVector3(const int nCol, const NFVector3& value, NFDataList& varResult)
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

int NFRecord::FindRowByColValue(const int nCol, const NFData & var)
{
	NFDataList xDataList;
	int nRowCount = FindRowByColValue(nCol, var, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindInt(const int nCol, const NFINT64 value)
{
	NFDataList xDataList;
	int nRowCount = FindInt(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindFloat(const int nCol, const double value)
{
	NFDataList xDataList;
	int nRowCount = FindFloat(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindString(const int nCol, const std::string & value)
{
	NFDataList xDataList;
	int nRowCount = FindString(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindObject(const int nCol, const NFGUID & value)
{
	NFDataList xDataList;
	int nRowCount = FindObject(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector2(const int nCol, const NFVector2 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector2(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector3(const int nCol, const NFVector3 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector3(nCol, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector3(const std::string& strColTag, const NFVector3& value, NFDataList& varResult)
{
	if (strColTag.empty())
	{
		return -1;
	}

	int nCol = GetCol(strColTag);
	return FindVector3(nCol, value, varResult);
}

int NFRecord::FindRowByColValue(const std::string & strColTag, const NFData & var)
{
	NFDataList xDataList;
	int nRowCount = FindRowByColValue(strColTag, var, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindInt(const std::string & strColTag, const NFINT64 value)
{
	NFDataList xDataList;
	int nRowCount = FindInt(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindFloat(const std::string & strColTag, const double value)
{
	NFDataList xDataList;
	int nRowCount = FindFloat(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindString(const std::string & strColTag, const std::string & value)
{
	NFDataList xDataList;
	int nRowCount = FindString(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindObject(const std::string & strColTag, const NFGUID & value)
{
	NFDataList xDataList;
	int nRowCount = FindObject(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector2(const std::string & strColTag, const NFVector2 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector2(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

int NFRecord::FindVector3(const std::string & strColTag, const NFVector3 & value)
{
	NFDataList xDataList;
	int nRowCount = FindVector3(strColTag, value, xDataList);
	if (nRowCount > 0 && xDataList.GetCount() > 0)
	{
		return (int) xDataList.Int(0);
	}

	return -1;
}

bool NFRecord::Remove(const int nRow)
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

int NFRecord::GetPos(int nRow, int nCol) const
{
    return nRow * mVarRecordType->GetCount() + nCol;
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

void NFRecord::SetName(const std::string& strName)
{
    mstrRecordName = strName;
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

bool NFRecord::IsUsed(const int nRow) const
{
    if (ValidRow(nRow))
    {
        return (mVecUsedState[nRow] > 0);
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
        xEventData.nRow = nOriginRow;
        xEventData.nCol = nTargetRow;
        xEventData.strRecordName = mstrRecordName;

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

bool NFRecord::SetUsed(const int nRow, const int bUse)
{
    if (ValidRow(nRow))
    {
        mVecUsedState[nRow] = bUse;
        return true;
    }

    return false;
}

bool NFRecord::PreAllocMemoryForRow(const int nRow)
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

bool NFRecord::ValidPos(int nRow, int nCol) const
{
    if (ValidCol(nCol)
        && ValidRow(nRow))
    {
        return true;
    }

    return false;
}

bool NFRecord::ValidRow(int nRow) const
{
    if (nRow >= GetRows() || nRow < 0)
    {
        return false;
    }

    return true;
}

bool NFRecord::ValidCol(int nCol) const
{
    if (nCol >= GetCols() || nCol < 0)
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
