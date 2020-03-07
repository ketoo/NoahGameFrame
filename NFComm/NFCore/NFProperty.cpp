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



#include "NFProperty.h"
#include <complex>

NFProperty::NFProperty()
{
	mbPublic = false;
	mbPrivate = false;
	mbSave = false;
	mbCache = false;
	mbRef = false;
	mbForce = false;
	mbUpload = false;

	mSelf = NFGUID();
	eType = TDATA_UNKNOWN;

	msPropertyName = "";
}

NFProperty::NFProperty(const NFGUID& self, const std::string& strPropertyName, const NFDATA_TYPE varType)
{
	mbPublic = false;
	mbPrivate = false;
	mbSave = false;
	mbCache = false;
	mbRef = false;
	mbUpload = false;

	mSelf = self;

	msPropertyName = strPropertyName;
	eType = varType;
}

NFProperty::~NFProperty()
{
	for (TPROPERTYCALLBACKEX::iterator iter = mtPropertyCallback.begin(); iter != mtPropertyCallback.end(); ++iter)
	{
		iter->reset();
	}

	mtPropertyCallback.clear();
	mxData.reset();
}

void NFProperty::SetValue(const NFData& xData)
{
	if (eType != xData.GetType()
		|| xData.GetType() == NFDATA_TYPE::TDATA_UNKNOWN)
	{
		return;
	}

	if (xData.IsNullValue())
	{
		return;
	}

	if (nullptr == mxData)
	{
		mxData = NF_SHARE_PTR<NFData>(NF_NEW NFData(xData));
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->variantData = xData.variantData;
	}
	else
	{
		NFData oldValue;
		oldValue = *mxData;

		mxData->variantData = xData.variantData;

		NFData newValue;
		newValue = *mxData;

		OnEventHandler(oldValue, newValue);
	}

}

void NFProperty::SetValue(const NFIProperty* pProperty)
{
	SetValue(pProperty->GetValue());
}

const NFData& NFProperty::GetValue() const
{
	if (mxData)
	{
		return *mxData;
	}

	return NULL_TDATA;
}

const std::string&  NFProperty::GetKey() const
{
	return msPropertyName;
}

const bool NFProperty::GetSave() const
{
	return mbSave;
}

const bool NFProperty::GetPublic() const
{
	return mbPublic;
}

const bool NFProperty::GetPrivate() const
{
	return mbPrivate;
}

const bool NFProperty::GetCache() const
{
	return mbCache;
}

const bool NFProperty::GetRef() const
{
	return mbRef;
}

const bool NFProperty::GetForce() const
{
	return mbForce;
}

const bool NFProperty::GetUpload() const
{
	return mbUpload;
}

void NFProperty::SetSave(bool bSave)
{
	mbSave = bSave;
}

void NFProperty::SetPublic(bool bPublic)
{
	mbPublic = bPublic;
}

void NFProperty::SetPrivate(bool bPrivate)
{
	mbPrivate = bPrivate;
}

void NFProperty::SetCache(bool bCache)
{
    mbCache = bCache;
}

void NFProperty::SetRef(bool bRef)
{
	mbRef = bRef;
}

void NFProperty::SetForce(bool bRef)
{
	mbForce = bRef;
}

void NFProperty::SetUpload(bool bUpload)
{
	mbUpload = bUpload;
}

NFINT64 NFProperty::GetInt() const
{
	if (!mxData)
	{
		return 0;
	}

	return mxData->GetInt();
}

int NFProperty::GetInt32() const
{
	if (!mxData)
	{
		return 0;
	}

	return (int)mxData->GetInt();
}


double NFProperty::GetFloat() const
{
	if (!mxData)
	{
		return 0.0;
	}

	return mxData->GetFloat();
}

const std::string& NFProperty::GetString() const
{
	if (!mxData)
	{
		return NULL_STR;
	}

	return mxData->GetString();
}

const NFGUID& NFProperty::GetObject() const
{
	if (!mxData)
	{
		return NULL_OBJECT;
	}

	return mxData->GetObject();
}

const NFVector2& NFProperty::GetVector2() const
{
	if (!mxData)
	{
		return NULL_VECTOR2;
	}

	return mxData->GetVector2();
}

const NFVector3& NFProperty::GetVector3() const
{
	if (!mxData)
	{
		return NULL_VECTOR3;
	}

	return mxData->GetVector3();
}

void NFProperty::RegisterCallback(const PROPERTY_EVENT_FUNCTOR_PTR& cb)
{
	mtPropertyCallback.push_back(cb);
}

int NFProperty::OnEventHandler(const NFData& oldVar, const NFData& newVar)
{
	if (mtPropertyCallback.size() <= 0)
	{
		return 0;
	}

	TPROPERTYCALLBACKEX::iterator it = mtPropertyCallback.begin();
	TPROPERTYCALLBACKEX::iterator end = mtPropertyCallback.end();
	for (; it != end; ++it)
	{
		
		PROPERTY_EVENT_FUNCTOR_PTR& pFunPtr = *it;
		PROPERTY_EVENT_FUNCTOR* pFunc = pFunPtr.get();
		pFunc->operator()(mSelf, msPropertyName, oldVar, newVar);
	}

	return 0;
}

bool NFProperty::SetInt(const NFINT64 value)
{
	if (eType != TDATA_INT)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (0 == value)
		{
			return false;
		}

		mxData = NF_SHARE_PTR<NFData>(NF_NEW NFData(TDATA_INT));
		mxData->SetInt(0);
	}

	if (value == mxData->GetInt())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetInt(value);
	}
	else
	{
		NFData oldValue;
		oldValue = *mxData;

		mxData->SetInt(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool NFProperty::SetFloat(const double value)
{
	if (eType != TDATA_FLOAT)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (IsZeroDouble(value))
		{
			return false;
		}

		mxData = NF_SHARE_PTR<NFData>(NF_NEW NFData(TDATA_FLOAT));
		mxData->SetFloat(0.0);
	}

	if (IsZeroDouble(value - mxData->GetFloat()))
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetFloat(value);
	}
	else
	{
		NFData oldValue;
		oldValue = *mxData;

		mxData->SetFloat(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool NFProperty::SetString(const std::string& value)
{
	if (eType != TDATA_STRING)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (value.empty())
		{
			return false;
		}

		mxData = NF_SHARE_PTR<NFData>(NF_NEW NFData(TDATA_STRING));
		mxData->SetString(NULL_STR);
	}

	if (value == mxData->GetString())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetString(value);
	}
	else
	{
		NFData oldValue;
		oldValue = *mxData;

		mxData->SetString(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool NFProperty::SetObject(const NFGUID& value)
{
	if (eType != TDATA_OBJECT)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (value.IsNull())
		{
			return false;
		}

		mxData = NF_SHARE_PTR<NFData>(NF_NEW NFData(TDATA_OBJECT));
		mxData->SetObject(NFGUID());
	}

	if (value == mxData->GetObject())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetObject(value);
	}
	else
	{
		NFData oldValue;
		oldValue = *mxData;

		mxData->SetObject(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool NFProperty::SetVector2(const NFVector2& value)
{
	if (eType != TDATA_VECTOR2)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (value.IsZero())
		{
			return false;
		}

		mxData = NF_SHARE_PTR<NFData>(NF_NEW NFData(TDATA_VECTOR2));
		mxData->SetVector2(NFVector2());
	}

	if (value == mxData->GetVector2())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetVector2(value);
	}
	else
	{
		NFData oldValue;
		oldValue = *mxData;

		mxData->SetVector2(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool NFProperty::SetVector3(const NFVector3& value)
{
	if (eType != TDATA_VECTOR3)
	{
		return false;
	}

	if (!mxData)
	{
		
		if (value.IsZero())
		{
			return false;
		}

		mxData = NF_SHARE_PTR<NFData>(NF_NEW NFData(TDATA_VECTOR3));
		mxData->SetVector3(NFVector3());
	}

	if (value == mxData->GetVector3())
	{
		return false;
	}

	if (mtPropertyCallback.size() == 0)
	{
		mxData->SetVector3(value);
	}
	else
	{
		NFData oldValue;
		oldValue = *mxData;

		mxData->SetVector3(value);

		OnEventHandler(oldValue, *mxData);
	}

	return true;
}

bool NFProperty::Changed() const
{
	return !(GetValue().IsNullValue());
}

const NFDATA_TYPE NFProperty::GetType() const
{
	return eType;
}

const bool NFProperty::GeUsed() const
{
	if (mxData)
	{
		return true;
	}

	return false;
}

std::string NFProperty::ToString()
{
	std::string strData;
	const NFDATA_TYPE eType = GetType();
	switch (eType)
	{
	case TDATA_INT:
		strData = lexical_cast<std::string> (GetInt());
		break;
	case TDATA_FLOAT:
		strData = lexical_cast<std::string> (GetFloat());
		break;
	case TDATA_STRING:
		strData = GetString();
		break;
	case TDATA_OBJECT:
		strData = GetObject().ToString();
		break;
	case TDATA_VECTOR2:
		strData = GetVector2().ToString();
		break;
	case TDATA_VECTOR3:
		strData = GetVector3().ToString();
		break;
	default:
		strData = NULL_STR;
		break;
	}

	return strData;
}

void NFProperty::ToMemoryCounterString(std::string& data)
{
	data.append(this->mSelf.ToString());
	data.append(":");
	data.append(this->msPropertyName);
}

bool NFProperty::FromString(const std::string& strData)
{
	try
	{
		switch (GetType())
		{
		case TDATA_INT:
			SetInt(lexical_cast<int64_t> (strData));
			break;

		case TDATA_FLOAT:
			SetFloat(lexical_cast<float> (strData));
			break;

		case TDATA_STRING:
			SetString(strData);
			break;

		case TDATA_OBJECT:
		{
			NFGUID xID;
			xID.FromString(strData);
			SetObject(xID);
		}
		break;

		case TDATA_VECTOR2:
		{
			NFVector2 v;
			v.FromString(strData);
			SetVector2(v);
		}
		break;

		case TDATA_VECTOR3:
		{
			NFVector3 v;
			v.FromString(strData);
			SetVector3(v);
		}
		break;

		default:
			break;
		}

		return true;
	}
	catch (...)
	{
		return false;
	}

	return false;
}

bool NFProperty::DeSerialization()
{
	bool bRet = false;

	const NFDATA_TYPE eType = GetType();
	if (eType == TDATA_STRING && nullptr != mxData && !mxData->IsNullValue())
	{
		NFDataList xDataList;
		const std::string& strData = mxData->GetString();

		xDataList.Split(strData.c_str(), ";");
		if (xDataList.GetCount() <= 0)
		{
			return bRet;
		}

		NFDataList xTemDataList;
		xTemDataList.Split(xDataList.String(0).c_str(), ",");
		const int nSubDataLength = xTemDataList.GetCount();

		if (xDataList.GetCount() == 1 && nSubDataLength == 1)
		{
			//most of property value only one value
			return bRet;
		}

		if (nullptr == mxEmbeddedList)
		{
			mxEmbeddedList = NF_SHARE_PTR<NFList<std::string>>(NF_NEW NFList<std::string>());
		}
		else
		{
			mxEmbeddedList->ClearAll();
		}

		for (int i = 0; i < xDataList.GetCount(); ++i)
		{
			if (xDataList.String(i).empty())
			{
				NFASSERT(0, strData, __FILE__, __FUNCTION__);
			}

			mxEmbeddedList->Add(xDataList.String(i));
		}



		////////////////////////////////////////

		if (nSubDataLength < 2 || nSubDataLength > 2)
		{
			return bRet;
		}

		if (nullptr == mxEmbeddedMap)
		{
			mxEmbeddedMap = NF_SHARE_PTR<NFMapEx<std::string, std::string>>(NF_NEW NFMapEx<std::string, std::string>());
		}
		else
		{
			mxEmbeddedMap->ClearAll();
		}

		for (int i = 0; i < xDataList.GetCount(); ++i)
		{
			NFDataList xTemDataList;
			const std::string& strTemData = xDataList.String(i);
			xTemDataList.Split(strTemData.c_str(), ",");
			{
				if (xTemDataList.GetCount() != nSubDataLength)
				{
					NFASSERT(0, strTemData, __FILE__, __FUNCTION__);
				}

				const std::string& strKey = xTemDataList.String(0);
				const std::string& strValue = xTemDataList.String(1);

				if (strKey.empty() || strValue.empty())
				{
					NFASSERT(0, strTemData, __FILE__, __FUNCTION__);
				}

				mxEmbeddedMap->AddElement(strKey, NF_SHARE_PTR<std::string>(NF_NEW std::string(strValue)));
			}
		}

		bRet = true;
	}

	return bRet;
}

const NF_SHARE_PTR<NFList<std::string>> NFProperty::GetEmbeddedList() const
{
	return this->mxEmbeddedList;
}

const NF_SHARE_PTR<NFMapEx<std::string, std::string>> NFProperty::GetEmbeddedMap() const
{
	return this->mxEmbeddedMap;
}
