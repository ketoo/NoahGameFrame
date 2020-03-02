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

std::string NFCommonRedisModule::GetPropertyCacheKey(const NFGUID& self)
{
    return self.ToString() + "_ObjectProperty";
}

std::string NFCommonRedisModule::GetRecordCacheKey(const NFGUID& self)
{
    return self.ToString() + "_ObjectRecord";
}

std::string NFCommonRedisModule::GetFriendCacheKey(const NFGUID& self)
{
    return self.ToString() + "_ObjectFriend";
}

std::string NFCommonRedisModule::GetFriendInviteCacheKey(const NFGUID& self)
{
    return self.ToString() + "_ObjectFriendInvite";
}

std::string NFCommonRedisModule::GetBlockCacheKey(const NFGUID& self)
{
    return self.ToString() + "_ObjectBlock";
}

std::string NFCommonRedisModule::GetTeamCacheKey(const NFGUID & self)
{
	return self.ToString() + "_Team";
}

std::string NFCommonRedisModule::GetTeamInviteCacheKey(const NFGUID & self)
{
	return self.ToString() + "_TeamInvite";
}

std::string NFCommonRedisModule::GetAccountCacheKey(const std::string & strAccount)
{
	return strAccount + "_AccountInfo";
}

std::string NFCommonRedisModule::GetTileCacheKey(const int & nSceneID)
{
	return lexical_cast<std::string>(nSceneID) + "_TileInfo";
}

std::string NFCommonRedisModule::GetSceneCacheKey(const int & nSceneID)
{
	return lexical_cast<std::string>(nSceneID) + "_SceneProps";
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

NF_SHARE_PTR<NFIPropertyManager> NFCommonRedisModule::NewPropertyManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFIPropertyManager> pStaticClassPropertyManager = m_pLogicClassModule->GetClassPropertyManager(strClassName);
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

NF_SHARE_PTR<NFIRecordManager> NFCommonRedisModule::NewRecordManager(const std::string& strClassName)
{
    NF_SHARE_PTR<NFIRecordManager> pStaticClassRecordManager = m_pLogicClassModule->GetClassRecordManager(strClassName);
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

NF_SHARE_PTR<NFIPropertyManager> NFCommonRedisModule::GetPropertyInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList)
{
	//TODO optimize
    NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = NewPropertyManager(strClassName);
    if (!pPropertyManager)
    {
        return nullptr;
    }

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return nullptr;
    }

	//TODO
	//just run this function one time
	NF_SHARE_PTR<NFIProperty> xProperty = pPropertyManager->First();
	while (xProperty)
	{
		if (xProperty->GetCache() || xProperty->GetSave())
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
		ConvertVectorToPropertyManager(vKeyCacheList, vValueCacheList, pPropertyManager);

		return pPropertyManager;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIRecordManager> NFCommonRedisModule::GetRecordInfo(const NFGUID& self, const std::string& strClassName, std::vector<std::string>& vKeyCacheList, std::vector<std::string>& vValueCacheList)
{
    NF_SHARE_PTR<NFIRecordManager> pRecordManager = NewRecordManager(strClassName);
    if (!pRecordManager.get())
    {
        return nullptr;
    }

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return nullptr;
    }


	//TODO
	//just run this function one time
	NF_SHARE_PTR<NFIRecord> xRecord = pRecordManager->First();
	while (xRecord)
	{
		if (xRecord->GetCache() || xRecord->GetSave())
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
		ConvertVectorToRecordManager(vKeyCacheList, vValueCacheList, pRecordManager);
		return pRecordManager;
	}

	return nullptr;
}

NF_SHARE_PTR<NFIPropertyManager> NFCommonRedisModule::GetPropertyInfo(const NFGUID & self, const std::string & strClassName)
{
	std::vector<std::string> vKeyCacheList;
	std::vector<std::string> vValueCacheList;

	return GetPropertyInfo(self, strClassName, vKeyCacheList, vValueCacheList);
}

NF_SHARE_PTR<NFIRecordManager> NFCommonRedisModule::GetRecordInfo(const NFGUID & self, const std::string & strClassName)
{
	std::vector<std::string> vKeyCacheList;
	std::vector<std::string> vValueCacheList;

	return GetRecordInfo(self, strClassName, vKeyCacheList, vValueCacheList);
}

bool NFCommonRedisModule::GetRecordInfo(const NFGUID & self, const std::string & strClassName, NFMsg::ObjectRecordList * pRecordDataList)
{
	*(pRecordDataList->mutable_player_id()) = NFINetModule::NFToPB(self);

	std::vector<std::string> vKeyCacheList;
	std::vector<std::string> vValueCacheList;
	NF_SHARE_PTR<NFIRecordManager> xRecordManager = GetRecordInfo(self, strClassName, vKeyCacheList, vValueCacheList);
	if (xRecordManager && vKeyCacheList.size() == vValueCacheList.size())
	{
		if (ConvertRecordManagerToPB(xRecordManager, pRecordDataList))
		{
			return true;
		}
	}

	return false;
}

bool NFCommonRedisModule::SavePropertyInfo(const NFGUID& self, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, const int nExpireSecond)
{
    if (self.IsNull())
    {
        return false;
    }

    if (!pPropertyManager)
    {
        return false;
    }

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
    if (!pDriver)
    {
        return false;
    }

	std::vector<std::string> vKeyList;
	std::vector<std::string> vValueList;
    if (!ConvertPropertyManagerToVector(pPropertyManager, vKeyList, vValueList))
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

bool NFCommonRedisModule::SaveRecordInfo(const NFGUID& self, NF_SHARE_PTR<NFIRecordManager> pRecordManager, const int nExpireSecond)
{
    if (self.IsNull())
    {
        return false;
    }

    if (!pRecordManager)
    {
        return false;
    }

	NFMsg::ObjectRecordList xRecordList;
	if (ConvertRecordManagerToPB(pRecordManager, &xRecordList))
	{
		return SaveRecordInfo(self, xRecordList, nExpireSecond);
	}

    return false;
}

bool NFCommonRedisModule::SaveRecordInfo(const NFGUID & self, const NFMsg::ObjectRecordList& xRecordData, const int nExpireSecond)
{
	if (self.IsNull())
	{
		return false;
	}

	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
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

bool NFCommonRedisModule::ConvertVectorToPropertyManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIPropertyManager> pPropertyManager)
{
	if (vKeyList.size() == vValueList.size())
	{
		for (int i = 0; i < vKeyList.size(); ++i)
		{
			const std::string& strKey = vKeyList[i];
			const std::string& strValue = vValueList[i];
			if (strValue.empty())
			{
				continue;
			}

			//std::cout << strKey << ":" << strValue << std::endl;

			NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->GetElement(strKey);
			if (!pProperty->GetCache() && !pProperty->GetSave())
			{
				continue;
			}

			if(!pProperty->FromString(strValue))
			{
				//TODO
				//error
			}
		}
	}

    return true;
}

bool NFCommonRedisModule::ConvertVectorToRecordManager(std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList, NF_SHARE_PTR<NFIRecordManager> pRecordManager)
{
	if (vKeyList.size() == vValueList.size())
	{
		for (int i = 0; i < vKeyList.size(); ++i)
		{
			const std::string& strKey = vKeyList[i];
			const std::string& strValue = vValueList[i];
			if (strValue.empty())
			{
				continue;
			}

			NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->GetElement(strKey);
			if (!pRecord->GetCache() && !pRecord->GetSave())
			{
				continue;
			}

			NFMsg::ObjectRecordBase xRecordData;
			if (xRecordData.ParseFromString(strValue))
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

    return true;
}

bool NFCommonRedisModule::ConvertPropertyManagerToVector(NF_SHARE_PTR<NFIPropertyManager> pPropertyManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList)
{
	for (NF_SHARE_PTR<NFIProperty> pProperty = pPropertyManager->First(); pProperty != NULL; pProperty = pPropertyManager->Next())
	{
		//const int nType = pProperty->GetType();
		if (!pProperty->GetCache() && !pProperty->GetSave())
		{
			continue;
		}

		const std::string& strPropertyName = pProperty->GetKey();
		const std::string& strPropertyValue = pProperty->ToString();

		//std::cout << strPropertyName << ":" << strPropertyValue << std::endl;

		vKeyList.push_back(strPropertyName);
		vValueList.push_back(strPropertyValue);
	}

	return true;
}

bool NFCommonRedisModule::ConvertRecordManagerToVector(NF_SHARE_PTR<NFIRecordManager> pRecordManager, std::vector<std::string>& vKeyList, std::vector<std::string>& vValueList)
{
	for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
	{
		if (!pRecord->GetCache() && !pRecord->GetSave())
		{
			continue;
		}

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

	return true;
}

bool NFCommonRedisModule::ConvertRecordToPB(const NF_SHARE_PTR<NFIRecord>& pRecord, NFMsg::ObjectRecordBase * pRecordData)
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
			const int nType = pRecord->GetColType(iCol);
			switch (nType)
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
	for (int iStuct = 0; iStuct < pRecordData.row_struct_size(); iStuct++)
	{
		const NFMsg::RecordAddRowStruct& xAddRowStruct = pRecordData.row_struct(iStuct);

		const int nCommonRow = xAddRowStruct.row();
		pRecord->SetUsed(nCommonRow, true);
		pRecord->PreAllocMemoryForRow(nCommonRow);

		for (int i = 0; i < xAddRowStruct.record_int_list_size(); i++)
		{
			const NFMsg::RecordInt& xPropertyData = xAddRowStruct.record_int_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFINT64 xPropertyValue = xPropertyData.data();

			pRecord->SetInt(nRow, nCol, xPropertyValue);
		}

		for (int i = 0; i < xAddRowStruct.record_float_list_size(); i++)
		{
			const NFMsg::RecordFloat& xPropertyData = xAddRowStruct.record_float_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const float xPropertyValue = xPropertyData.data();

			pRecord->SetFloat(nRow, nCol, xPropertyValue);
		}

		for (int i = 0; i < xAddRowStruct.record_string_list_size(); i++)
		{
			const NFMsg::RecordString& xPropertyData = xAddRowStruct.record_string_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const std::string& xPropertyValue = xPropertyData.data();

			pRecord->SetString(nRow, nCol, xPropertyValue.c_str());
		}

		for (int i = 0; i < xAddRowStruct.record_object_list_size(); i++)
		{
			const NFMsg::RecordObject& xPropertyData = xAddRowStruct.record_object_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFGUID xPropertyValue = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetObject(nRow, nCol, xPropertyValue);
		}

		for (int i = 0; i < xAddRowStruct.record_vector2_list_size(); i++)
		{
			const NFMsg::RecordVector2& xPropertyData = xAddRowStruct.record_vector2_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFVector2 v = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetVector2(nRow, nCol, v);
		}

		for (int i = 0; i < xAddRowStruct.record_vector3_list_size(); i++)
		{
			const NFMsg::RecordVector3& xPropertyData = xAddRowStruct.record_vector3_list(i);
			const int nRow = xPropertyData.row();
			const int nCol = xPropertyData.col();
			const NFVector3 v = NFINetModule::PBToNF(xPropertyData.data());

			pRecord->SetVector3(nRow, nCol, v);
		}
	}

	return false;
}

bool NFCommonRedisModule::ConvertRecordManagerToPB(const NF_SHARE_PTR<NFIRecordManager>& pRecordManager, NFMsg::ObjectRecordList * pRecordDataList)
{
	if (pRecordDataList == nullptr )
	{
		return false;
	}

	for (NF_SHARE_PTR<NFIRecord> pRecord = pRecordManager->First(); pRecord != NULL; pRecord = pRecordManager->Next())
	{
		if (!pRecord->GetCache() && !pRecord->GetSave())
		{
			continue;
		}

		NFMsg::ObjectRecordBase* pRecordData = pRecordDataList->add_record_list();
		if (!pRecordData)
		{
			continue;
		}

		ConvertRecordToPB(pRecord, pRecordData);
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

bool NFCommonRedisModule::ConvertPropertyManagerToPB(const NF_SHARE_PTR<NFIPropertyManager>& pProps, NFMsg::ObjectPropertyList * pPropertyData)
{
	if (pProps)
	{
		NF_SHARE_PTR<NFIProperty> xPropert = pProps->First();
		while (xPropert)
		{
			if (xPropert->GetCache() || xPropert->GetSave())
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
