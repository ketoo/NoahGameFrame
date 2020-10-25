/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2020 NoahFrame(NoahGameFrame)

   File creator: chuanbo.guo
   
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

#include "NFCommonRedisModule.h"
#include "NFComm/NFCore/NFPropertyManager.h"
#include "NFComm/NFCore/NFRecordManager.h"
#include "NFComm/NFPluginModule/NFINetModule.h"
#include "NFComm/NFMessageDefine/NFProtocolDefine.hpp"


NFCommonRedisModule::NFCommonRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

std::string NFCommonRedisModule::GetPropertyCacheKey(const std::string& self)
{
    return self + "_ObjectProperty";
}

std::string NFCommonRedisModule::GetRecordCacheKey(const std::string& self)
{
    return self  + "_ObjectRecord";
}

std::string NFCommonRedisModule::GetFriendCacheKey(const std::string& self)
{
    return self + "_ObjectFriend";
}

std::string NFCommonRedisModule::GetFriendInviteCacheKey(const std::string& self)
{
    return self + "_ObjectFriendInvite";
}

std::string NFCommonRedisModule::GetBlockCacheKey(const std::string& self)
{
    return self + "_ObjectBlock";
}

std::string NFCommonRedisModule::GetTeamCacheKey(const std::string & self)
{
	return self + "_Team";
}

std::string NFCommonRedisModule::GetTeamInviteCacheKey(const std::string & self)
{
	return self + "_TeamInvite";
}

std::string NFCommonRedisModule::GetAccountCacheKey(const std::string & account)
{
	return account + "_AccountInfo";
}

std::string NFCommonRedisModule::GetTileCacheKey(const int & sceneID)
{
	return lexical_cast<std::string>(sceneID) + "_TileInfo";
}

std::string NFCommonRedisModule::GetSceneCacheKey(const int & sceneID)
{
	return lexical_cast<std::string>(sceneID) + "_SceneProps";
}

std::string NFCommonRedisModule::GetCellCacheKey(const std::string & strCellID)
{
	return strCellID + "_CellInfo";
}

bool NFCommonRedisModule::AfterInit()
{
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
    m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pElementModule = pPluginManager->FindModule<NFIElementModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();

    return true;
}

NF_SHARE_PTR<NFIPropertyManager> NFCommonRedisModule::NewPropertyManager(const std::string& className)
{
    NF_SHARE_PTR<NFIPropertyManager> pStaticClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager(className);
    if (pStaticClassPropertyManager)
    {
        NFGUID ident;
        NF_SHARE_PTR<NFIPropertyManager> pPropertyManager(NF_NEW NFPropertyManager(ident));

        NF_SHARE_PTR<NFIProperty> pStaticConfigPropertyInfo = pStaticClassPropertyManager->First();
        while (pStaticConfigPropertyInfo)
        {
			if (pStaticConfigPropertyInfo->GetSave() || pStaticConfigPropertyInfo->GetCache())
			{
				NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->AddProperty(ident, pStaticConfigPropertyInfo->GetKey(), pStaticConfigPropertyInfo->GetType());

				xProperty->SetPublic(pStaticConfigPropertyInfo->GetPublic());
				xProperty->SetPrivate(pStaticConfigPropertyInfo->GetPrivate());
				xProperty->SetSave(pStaticConfigPropertyInfo->GetSave());
				xProperty->SetCache(pStaticConfigPropertyInfo->GetCache());
				xProperty->SetRef(pStaticConfigPropertyInfo->GetRef());
			}

            pStaticConfigPropertyInfo = pStaticClassPropertyManager->Next();
        }

        return pPropertyManager;
    }

    return NF_SHARE_PTR<NFIPropertyManager>(NULL);
}

NF_SHARE_PTR<NFIRecordManager> NFCommonRedisModule::NewRecordManager(const std::string& className)
{
    NF_SHARE_PTR<NFIRecordManager> pStaticClassRecordManager = m_pLogicClassModule->GetClassRecordManager(className);
    if (pStaticClassRecordManager)
    {
        NFGUID ident;
        NF_SHARE_PTR<NFIRecordManager> pRecordManager(NF_NEW NFRecordManager(ident));

        NF_SHARE_PTR<NFIRecord> pConfigRecordInfo = pStaticClassRecordManager->First();
        while (pConfigRecordInfo)
        {
			if (pConfigRecordInfo->GetSave() || pConfigRecordInfo->GetCache())
			{
				NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->AddRecord(ident,
					pConfigRecordInfo->GetName(),
					pConfigRecordInfo->GetInitData(),
					pConfigRecordInfo->GetTag(),
					pConfigRecordInfo->GetRows());

				xRecord->SetPublic(pConfigRecordInfo->GetPublic());
				xRecord->SetPrivate(pConfigRecordInfo->GetPrivate());
				xRecord->SetSave(pConfigRecordInfo->GetSave());
				xRecord->SetCache(pConfigRecordInfo->GetCache());

			}

            pConfigRecordInfo = pStaticClassRecordManager->Next();
        }

        return pRecordManager;
    }

    return NF_SHARE_PTR<NFIRecordManager>(NULL);
}

NF_SHARE_PTR<NFIPropertyManager> NFCommonRedisModule::GetPropertyInfo(const std::string& self, const std::string& className, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList, const bool cache, const bool save, NF_SHARE_PTR<NFIPropertyManager> propertyManager)
{
	//TODO optimize
	NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = propertyManager;
	if (pPropertyManager == nullptr)
	{
		pPropertyManager = NewPropertyManager(className);
		if (!pPropertyManager)
		{
			return nullptr;
		}
	}

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
    if (!pDriver)
    {
        return nullptr;
    }

	//TODO
	//just run this function one time
	NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->First();
	while (xProperty)
	{
		if ((cache && xProperty->GetCache()) || (save && xProperty->GetSave()))
		{
			vKeyCacheList.push_back(xProperty->GetKey());
		}

		xProperty = pPropertyManager->Next();
	}

	//cache
	std::string strCacheKey = GetPropertyCacheKey(self);
    if (!pDriver->HMGET(strCacheKey, vKeyCacheList, vValueCacheList))
    {
        return nullptr;
    }

	if (vKeyCacheList.size() == vValueCacheList.size())
	{
		ConvertVectorToPropertyManager(vKeyCacheList, vValueCacheList, pPropertyManager, cache, save);

		return pPropertyManager;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIRecordManager> NFCommonRedisModule::GetRecordInfo(const std::string& self, const std::string& className, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList, const bool cache, const bool save, NF_SHARE_PTR<NFIRecordManager> recordManager)
{
	NF_SHARE_PTR<NFIRecordManager> pRecordManager = recordManager;
	if (pRecordManager == nullptr)
	{
		pRecordManager = NewRecordManager(className);
		if (!pRecordManager)
		{
			return nullptr;
		}
	}

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
    if (!pDriver)
    {
        return nullptr;
    }

	//TODO
	//just run this function one time
	NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First();
	while (xRecord)
	{
		if ((cache && xRecord->GetCache()) || (save && xRecord->GetSave()))
		{
			vKeyCacheList.push_back(xRecord->GetName());
		}

		xRecord = pRecordManager->Next();
	}

	//cache
	std::string strCacheKey = GetRecordCacheKey(self);
	if (!pDriver->HMGET(strCacheKey, vKeyCacheList, vValueCacheList))
	{
		return nullptr;
	}

	if (vKeyCacheList.size() == vValueCacheList.size())
	{
		ConvertVectorToRecordManager(vKeyCacheList, vValueCacheList, pRecordManager, cache, save);
		return pRecordManager;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIPropertyManager> NFCommonRedisModule::GetPropertyInfo(const std::string & self, const std::string & className, const bool cache, const bool save, NF_SHARE_PTR<NFIPropertyManager> propertyManager)
{
	std::vector<std::string> vKeyCacheList;
	std::vector<std::string> vValueCacheList;

	return GetPropertyInfo(self, className, vKeyCacheList, vValueCacheList, cache, save, propertyManager);
}

NF_SHARE_PTR<NFIRecordManager> NFCommonRedisModule::GetRecordInfo(const std::string & self, const std::string & className, const bool cache, const bool save, NF_SHARE_PTR<NFIRecordManager> recordManager)
{
	std::vector<std::string> vKeyCacheList;
	std::vector<std::string> vValueCacheList;

	return GetRecordInfo(self, className, vKeyCacheList, vValueCacheList, cache, save, recordManager);
}

bool NFCommonRedisModule::GetRecordInfo(const std::string & self, const std::string & className, NFMsg::ObjectRecordList * pRecordDataList, const bool cache, const bool save)
{
	*(pRecordDataList->mutable_player_id()) = NFINetModule::NFToPB(NFGUID(self));

	std::vector<std::string> vKeyCacheList;
	std::vector<std::string> vValueCacheList;
	NF_SHARE_PTR<NFIRecordManager> xRecordManager = GetRecordInfo(self, className, vKeyCacheList, vValueCacheList, cache, save);
	if (xRecordManager && vKeyCacheList.size() == vValueCacheList.size())
	{
		if (ConvertRecordManagerToPB(xRecordManager, pRecordDataList, cache, save))
		{
			return true;
		}
	}

	return false;
}

bool NFCommonRedisModule::SavePropertyInfo(const std::string& self, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const bool cache, const bool save, const int nExpireSecond)
{
    if (!pPropertyManager)
    {
        return false;
    }

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
    if (!pDriver)
    {
        return false;
    }

	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;
    if (!ConvertPropertyManagerToVector(pPropertyManager, vKeyList, vValueList, cache, save))
    {
        return false;
    }

	if (vKeyList.size() != vValueList.size())
	{
		return false;
	}

	std::string strKey= GetPropertyCacheKey(self);

    if (!pDriver->HMSET(strKey, vKeyList, vValueList))
    {
        return false;
    }

	if (nExpireSecond > 0)
	{
		pDriver->EXPIRE(strKey, nExpireSecond);
	}

    return true;
}

bool NFCommonRedisModule::SaveRecordInfo(const std::string& self, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const bool cache, const bool save, const int nExpireSecond)
{
    if (!pRecordManager)
    {
        return false;
    }

	NFMsg::ObjectRecordList xRecordList;
	if (ConvertRecordManagerToPB(pRecordManager, &xRecordList, cache, save))
	{
		return SaveRecordInfo(self, xRecordList, nExpireSecond);
	}

    return false;
}

bool NFCommonRedisModule::SaveRecordInfo(const std::string & self, const NFMsg::ObjectRecordList& xRecordData, const int nExpireSecond)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return false;
	}

	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;
	
	for (int i = 0; i < xRecordData.record_list_size(); ++i)
	{
		const NFMsg::ObjectRecordBase& xRecord = xRecordData.record_list(i);

		std::string strValue;
		if (!xRecord.SerializeToString(&strValue))
		{
			continue;
		}

		vKeyList.push_back(xRecord.record_name());
		vValueList.push_back(strValue);
	}


	if (vKeyList.size() != vValueList.size())
	{
		return false;
	}

	std::string strKey = GetRecordCacheKey(self);
	if (!pDriver->HMSET(strKey, vKeyList, vValueList))
	{
		return false;
	}

	if (nExpireSecond > 0)
	{
		pDriver->EXPIRE(strKey, nExpireSecond);
	}

	return true;
}

bool NFCommonRedisModule::GetPropertyList(const std::string& self, const std::vector<std::string>& fields, std::vector<std::string>& values)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return false;
	}

	std::string strCacheKey = GetPropertyCacheKey(self);
	if (pDriver->HMGET(strCacheKey, fields, values))
	{
		return true;
	}


	return false;
}

NFINT64 NFCommonRedisModule::GetPropertyInt(const std::string & self, const std::string & propertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return 0;
	}

	std::string strValue;
	std::string strCacheKey = GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, propertyName, strValue))
	{
		return 0;
	}

	return lexical_cast<NFINT64>(strValue);
}

int NFCommonRedisModule::GetPropertyInt32(const std::string & self, const std::string & propertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return 0;
	}

	std::string strValue;
	std::string strCacheKey = GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, propertyName, strValue))
	{
		return 0;
	}

	return lexical_cast<NFINT64>(strValue);
}

double NFCommonRedisModule::GetPropertyFloat(const std::string & self, const std::string & propertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return 0;
	}

	std::string strValue;
	std::string strCacheKey = GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, propertyName, strValue))
	{
		return 0;
	}

	return lexical_cast<double>(strValue);
}

std::string NFCommonRedisModule::GetPropertyString(const std::string & self, const std::string & propertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return "";
	}

	std::string strValue;
	std::string strCacheKey = GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, propertyName, strValue))
	{
		return "";
	}

	return strValue;
}

NFGUID NFCommonRedisModule::GetPropertyObject(const std::string & self, const std::string & propertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return NFGUID();
	}

	std::string strValue;
	std::string strCacheKey = GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, propertyName, strValue))
	{
		return NFGUID();
	}
	NFGUID xID;
	xID.FromString(strValue);
	return xID;
}

NFVector2 NFCommonRedisModule::GetPropertyVector2(const std::string & self, const std::string & propertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return NFVector2();
	}

	std::string strValue;
	std::string strCacheKey = GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, propertyName, strValue))
	{
		return NFVector2();
	}

	NFVector2 xValue;
	xValue.FromString(strValue);
	return xValue;
}

NFVector3 NFCommonRedisModule::GetPropertyVector3(const std::string & self, const std::string & propertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return NFVector3();
	}

	std::string strValue;
	std::string strCacheKey = GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, propertyName, strValue))
	{
		return NFVector3();
	}

	NFVector3 xValue;
	xValue.FromString(strValue);
	return xValue;
}

bool NFCommonRedisModule::ConvertVectorToPropertyManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const bool cache, const bool save)
{
	if (vKeyList.size() == vValueList.size())
	{
		for (int i = 0; i < vKeyList.size(); ++i)
		{
			const std::string& strKey = vKeyList[i];
			const std::string& value = vValueList[i];
			if (value.empty())
			{
				continue;
			}

			//std::cout << strKey << ":" << strValue << std::endl;

			NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->GetElement(strKey);
			if ((cache && pProperty->GetCache()) || (save && pProperty->GetSave()))
			{
				if(!pProperty->FromString(value))
				{
					//TODO
					//error
				}
			}
		}
	}

    return true;
}

bool NFCommonRedisModule::ConvertVectorToRecordManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const bool cache, const bool save)
{
	if (vKeyList.size() == vValueList.size())
	{
		for (int i = 0; i < vKeyList.size(); ++i)
		{
			const std::string& strKey = vKeyList[i];
			const std::string& value = vValueList[i];
			if (value.empty())
			{
				continue;
			}

			NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->GetElement(strKey);
			if ((cache && pRecord->GetCache()) || (save && pRecord->GetSave()))
			{
				NFMsg::ObjectRecordBase xRecordData;
				if (xRecordData.ParseFromString(value))
				{
					ConvertPBToRecord(xRecordData, pRecord);
				}
				else
				{
					//TODO
					//error
				}
			}
		}
	}

    return true;
}

bool NFCommonRedisModule::ConvertPropertyManagerToVector(NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, const bool cache, const bool save)
{
	for (NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->First(); pProperty != NULL; pProperty = pPropertyManager->Next())
	{
		//const int type = pProperty->GetType();
		if ((cache && pProperty->GetCache()) || (save && pProperty->GetSave()))
		{
			const std::string& propertyName = pProperty->GetKey();
			const std::string& strPropertyValue = pProperty->ToString();

			//std::cout << propertyName << ":" << strPropertyValue << std::endl;

			vKeyList.push_back(propertyName);
			vValueList.push_back(strPropertyValue);
		}
	}

	return true;
}

bool NFCommonRedisModule::ConvertRecordManagerToVector(NF_SHARE_PTR<NFIRecordManager> pRecordManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, const bool cache, const bool save)
{
	for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
	{
		if ((cache && pRecord->GetCache()) || (save && pRecord->GetSave()))
		{
			NFMsg::ObjectRecordBase xRecordData;

			ConvertRecordToPB(pRecord, &xRecordData);

			////
			std::string strValue;
			if (!xRecordData.SerializeToString(&strValue))
			{
				continue;
			}

			vKeyList.push_back(xRecordData.record_name());
			vValueList.push_back(strValue);
		}
	}

	return true;
}

bool NFCommonRedisModule::ConvertRecordToPB(const NF_SHARE_PTR<NFIRecord> pRecord, NFMsg::ObjectRecordBase * pRecordData)
{
	pRecordData->set_record_name(pRecord->GetName());

	for (int iRow = 0; iRow < pRecord->GetRows(); iRow++)
	{
		if (!pRecord->IsUsed(iRow))
		{
			continue;
		}

		NFMsg::RecordAddRowStruct* pRowData = pRecordData->add_row_struct();
		if (!pRowData)
		{
			continue;
		}

		pRowData->set_row(iRow);

		for (int iCol = 0; iCol < pRecord->GetCols(); iCol++)
		{
			const int type = pRecord->GetColType(iCol);
			switch (type)
			{
			case TDATA_INT:
			{
				NFMsg::RecordInt* pPropertyData = pRowData->add_record_int_list();
				const NFINT64 xPropertyValue = pRecord->GetInt(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					pPropertyData->set_data(xPropertyValue);
				}
			}
			break;
			case TDATA_FLOAT:
			{
				NFMsg::RecordFloat* pPropertyData = pRowData->add_record_float_list();
				const double xPropertyValue = pRecord->GetFloat(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					pPropertyData->set_data(xPropertyValue);
				}
			}
			break;
			case TDATA_STRING:
			{
				NFMsg::RecordString* pPropertyData = pRowData->add_record_string_list();
				const std::string& xPropertyValue = pRecord->GetString(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					pPropertyData->set_data(xPropertyValue);

				}
			}
			break;
			case TDATA_OBJECT:
			{
				NFMsg::RecordObject* pPropertyData = pRowData->add_record_object_list();
				const NFGUID xPropertyValue = pRecord->GetObject(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);
					*pPropertyData->mutable_data() = NFINetModule::NFToPB(xPropertyValue);
				}
			}
			break;
			case TDATA_VECTOR2:
			{
				NFMsg::RecordVector2* pPropertyData = pRowData->add_record_vector2_list();
				const NFVector2 xPropertyValue = pRecord->GetVector2(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);

					NFMsg::Vector2* pVec = pPropertyData->mutable_data();
					pVec->set_x(xPropertyValue.X());
					pVec->set_y(xPropertyValue.Y());
				}
			}
			break;
			case TDATA_VECTOR3:
			{
				NFMsg::RecordVector3* pPropertyData = pRowData->add_record_vector3_list();
				const NFVector3 xPropertyValue = pRecord->GetVector3(iRow, iCol);

				if (pPropertyData)
				{
					pPropertyData->set_col(iCol);
					pPropertyData->set_row(iRow);

					NFMsg::Vector3* pVec = pPropertyData->mutable_data();
					pVec->set_x(xPropertyValue.X());
					pVec->set_y(xPropertyValue.Y());
					pVec->set_z(xPropertyValue.Z());
				}
			}
			break;
			default:
				break;
			}
		}
	}

	return true;
}

bool NFCommonRedisModule::ConvertPBToRecord(const NFMsg::ObjectRecordBase& pRecordData, NF_SHARE_PTR<NFIRecord> pRecord)
{
	pRecord->Clear();

	for (int row = 0; row < pRecordData.row_struct_size(); row++)
	{
		const NFMsg::RecordAddRowStruct& xAddRowStruct = pRecordData.row_struct(row);

		auto initData = pRecord->GetInitData();
		if (initData)
		{
			for (int i = 0; i < xAddRowStruct.record_int_list_size(); i++)
			{
				const NFMsg::RecordInt& xPropertyData = xAddRowStruct.record_int_list(i);
				const int col = xPropertyData.col();
				const NFINT64 xPropertyValue = xPropertyData.data();

				initData->SetInt(col, xPropertyValue);
			}

			for (int i = 0; i < xAddRowStruct.record_float_list_size(); i++)
			{
				const NFMsg::RecordFloat& xPropertyData = xAddRowStruct.record_float_list(i);
				const int col = xPropertyData.col();
				const float xPropertyValue = xPropertyData.data();

				initData->SetFloat(col, xPropertyValue);
			}

			for (int i = 0; i < xAddRowStruct.record_string_list_size(); i++)
			{
				const NFMsg::RecordString& xPropertyData = xAddRowStruct.record_string_list(i);
				const int col = xPropertyData.col();
				const std::string& xPropertyValue = xPropertyData.data();

				initData->SetString(col, xPropertyValue);
			}

			for (int i = 0; i < xAddRowStruct.record_object_list_size(); i++)
			{
				const NFMsg::RecordObject& xPropertyData = xAddRowStruct.record_object_list(i);
				const int col = xPropertyData.col();
				const NFGUID xPropertyValue = NFINetModule::PBToNF(xPropertyData.data());

				initData->SetObject(col, xPropertyValue);
			}

			for (int i = 0; i < xAddRowStruct.record_vector2_list_size(); i++)
			{
				const NFMsg::RecordVector2& xPropertyData = xAddRowStruct.record_vector2_list(i);
				const int col = xPropertyData.col();
				const NFVector2 v = NFINetModule::PBToNF(xPropertyData.data());

				initData->SetVector2(col, v);
			}

			for (int i = 0; i < xAddRowStruct.record_vector3_list_size(); i++)
			{
				const NFMsg::RecordVector3& xPropertyData = xAddRowStruct.record_vector3_list(i);
				const int col = xPropertyData.col();
				const NFVector3 v = NFINetModule::PBToNF(xPropertyData.data());

				initData->SetVector3(col, v);
			}

			pRecord->AddRow(row, *initData);
		}
	}

	return false;
}

bool NFCommonRedisModule::ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager> pRecordManager, NFMsg::ObjectRecordList * pRecordDataList, const bool cache, const bool save)
{
	if (pRecordDataList == nullptr )
	{
		return false;
	}

	for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
	{
		if ((cache && pRecord->GetCache()) || (save && pRecord->GetSave()))
		{
			NFMsg::ObjectRecordBase *pRecordData = pRecordDataList->add_record_list();
			if (!pRecordData)
			{
				continue;
			}

			ConvertRecordToPB(pRecord, pRecordData);
		}
	}

	return true;
}

bool NFCommonRedisModule::ConvertPBToRecordManager(const NFMsg::ObjectRecordList & pRecordData, NF_SHARE_PTR<NFIRecordManager> pRecord)
{
	if (pRecord == nullptr)
	{
		return false;
	}

	for (int i = 0; i < pRecordData.record_list_size(); ++i)
	{
		const NFMsg::ObjectRecordBase& xRecordBase = pRecordData.record_list(i);
		NF_SHARE_PTR<NFIRecord> xRecord = pRecord->GetElement(xRecordBase.record_name());
		if (xRecord)
		{
			ConvertPBToRecord(xRecordBase, xRecord);
		}
	}

	return true;
}

bool NFCommonRedisModule::ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager> pProps, NFMsg::ObjectPropertyList * pPropertyData, const bool cache, const bool save)
{
	if (pProps)
	{
		NF_SHARE_PTR<NFIProperty> xPropert = pProps->First();
		while (xPropert)
		{
			if ((cache && xPropert->GetCache()) || (save && xPropert->GetSave()))
			{
				switch (xPropert->GetType())
				{
				case NFDATA_TYPE::TDATA_INT:
				{
					NFMsg::PropertyInt* pData = pPropertyData->add_property_int_list();
					pData->set_property_name(xPropert->GetKey());
					pData->set_data(xPropert->GetInt());
				}
				break;

				case NFDATA_TYPE::TDATA_FLOAT:
				{
					NFMsg::PropertyFloat* pData = pPropertyData->add_property_float_list();
					pData->set_property_name(xPropert->GetKey());
					pData->set_data(xPropert->GetFloat());
				}
				break;

				case NFDATA_TYPE::TDATA_OBJECT:
				{
					NFMsg::PropertyObject* pData = pPropertyData->add_property_object_list();
					pData->set_property_name(xPropert->GetKey());
					*(pData->mutable_data()) = NFINetModule::NFToPB(xPropert->GetObject());
				}
				break;

				case NFDATA_TYPE::TDATA_STRING:
				{
					NFMsg::PropertyString* pData = pPropertyData->add_property_string_list();
					pData->set_property_name(xPropert->GetKey());
					pData->set_data(xPropert->GetString());
				}
				break;

				case NFDATA_TYPE::TDATA_VECTOR2:
				{
					NFMsg::PropertyVector2* pData = pPropertyData->add_property_vector2_list();
					pData->set_property_name(xPropert->GetKey());
					*(pData->mutable_data()) = NFINetModule::NFToPB(xPropert->GetVector2());
				}
				break;

				case NFDATA_TYPE::TDATA_VECTOR3:
				{
					NFMsg::PropertyVector3* pData = pPropertyData->add_property_vector3_list();
					pData->set_property_name(xPropert->GetKey());
					*(pData->mutable_data()) = NFINetModule::NFToPB(xPropert->GetVector3());
				}
				break;
				default:
					break;
				}
			}
			xPropert = pProps->Next();
		}
	}

	return false;
}

bool NFCommonRedisModule::ConvertPBToPropertyManager(const NFMsg::ObjectPropertyList& pPropertyData, NF_SHARE_PTR<NFIPropertyManager> pProps)
{
	if (pProps)
	{
		for (int i = 0; i < pPropertyData.property_int_list_size(); ++i)
		{
			const NFMsg::PropertyInt& xData = pPropertyData.property_int_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_INT);
			}

			pProps->SetPropertyInt(xData.property_name(), xData.data());
		}

		for (int i = 0; i < pPropertyData.property_float_list_size(); ++i)
		{
			const NFMsg::PropertyFloat& xData = pPropertyData.property_float_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_FLOAT);
			}

			pProps->SetPropertyFloat(xData.property_name(), xData.data());
		}

		for (int i = 0; i < pPropertyData.property_string_list_size(); ++i)
		{
			const NFMsg::PropertyString& xData = pPropertyData.property_string_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_STRING);
			}

			pProps->SetPropertyString(xData.property_name(), xData.data());
		}

		for (int i = 0; i < pPropertyData.property_object_list_size(); ++i)
		{
			const NFMsg::PropertyObject& xData = pPropertyData.property_object_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_OBJECT);
			}

			pProps->SetPropertyObject(xData.property_name(), NFGUID(xData.data().svrid(), xData.data().index()));
		}

		for (int i = 0; i < pPropertyData.property_vector2_list_size(); ++i)
		{
			const NFMsg::PropertyVector2& xData = pPropertyData.property_vector2_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_VECTOR2);
			}

			pProps->SetPropertyVector2(xData.property_name(), NFVector2(xData.data().x(), xData.data().y()));
		}

		for (int i = 0; i < pPropertyData.property_vector3_list_size(); ++i)
		{
			const NFMsg::PropertyVector3& xData = pPropertyData.property_vector3_list(i);

			if (pProps->ExistElement(xData.property_name()))
			{
				pProps->AddProperty(pProps->Self(), xData.property_name(), NFDATA_TYPE::TDATA_VECTOR3);
			}

			pProps->SetPropertyVector3(xData.property_name(), NFVector3(xData.data().x(), xData.data().y(), xData.data().z()));
		}
	}

	return true;
}

bool NFCommonRedisModule::SavePropertyInfo(const std::string &self, const std::string &propertyName, const std::string &propertyValue)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return false;
	}

	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;

	vKeyList.push_back(propertyName);
	vValueList.push_back(propertyValue);

	if (vKeyList.size() != vValueList.size())
	{
		return false;
	}

	std::string strKey= GetPropertyCacheKey(self);

	if (!pDriver->HMSET(strKey, vKeyList, vValueList))
	{
		return false;
	}

	return false;
}

bool NFCommonRedisModule::GetPropertyList(const std::string &self, std::vector<std::pair<std::string, std::string>> &values)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self);
	if (!pDriver)
	{
		return false;
	}

	std::string strCacheKey = GetPropertyCacheKey(self);
	if (pDriver->HGETALL(strCacheKey, values))
	{
		return true;
	}

	return false;
}
