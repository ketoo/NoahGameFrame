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

#include "NFPlayerRedisModule.h"
#include "NFCommonRedisModule.h"

NFPlayerRedisModule::NFPlayerRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFPlayerRedisModule::Init()
{
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	m_pAccountRedisModule = pPluginManager->FindModule<NFIAccountRedisModule>();
	
	return true;
}

bool NFPlayerRedisModule::Shut()
{
	return true;
}

bool NFPlayerRedisModule::Execute()
{
	return true;
}

bool NFPlayerRedisModule::AfterInit()
{

	return true;
}

bool NFPlayerRedisModule::LoadPlayerData(const NFGUID & self, NFMsg::RoleDataPack& roleData)
{
	NFCommonRedisModule* pCommonRedisModule = (NFCommonRedisModule*)(m_pCommonRedisModule);

	NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pCommonRedisModule->GetPropertyInfo(self, NFrame::Player::ThisName());
	if (xPropertyManager)
	{
		*(roleData.mutable_property()->mutable_player_id()) = NFINetModule::NFToPB(self);

		pCommonRedisModule->ConvertPropertyManagerToPB(xPropertyManager, roleData.mutable_property());

		pCommonRedisModule->GetRecordInfo(self, NFrame::Player::ThisName(), roleData.mutable_record());
		
		return true;
	}

	
	m_pLogModule->LogError(self, "loaded data false", __FUNCTION__, __LINE__);

	return false;
}

bool NFPlayerRedisModule::SavePlayerData(const NFGUID & self, const NFMsg::RoleDataPack& roleData)
{
	NFCommonRedisModule* pCommonRedisModule = (NFCommonRedisModule*)m_pCommonRedisModule;

	NF_SHARE_PTR<NFIPropertyManager> xPropManager = pCommonRedisModule->NewPropertyManager(NFrame::Player::ThisName());
	if (pCommonRedisModule->ConvertPBToPropertyManager(roleData.property(), xPropManager))
	{
		m_pCommonRedisModule->SavePropertyInfo(self, xPropManager);
	}

	pCommonRedisModule->SaveRecordInfo(self, roleData.record(), -1);

	return true;
}

bool NFPlayerRedisModule::SavePlayerTile(const int nSceneID, const NFGUID & self, const std::string & strTileData)
{
	std::string strTileKey = m_pCommonRedisModule->GetTileCacheKey(nSceneID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HSET(strTileKey, self.ToString(), strTileData);
	}

	return false;
}

bool NFPlayerRedisModule::LoadPlayerTile(const int nSceneID, const NFGUID & self, std::string & strTileData)
{
	std::string strTileKey = m_pCommonRedisModule->GetTileCacheKey(nSceneID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (xNoSqlDriver && xNoSqlDriver->EXISTS(strTileKey))
	{
		return xNoSqlDriver->HGET(strTileKey, self.ToString(), strTileData);
	}

	return false;
}

bool NFPlayerRedisModule::LoadPlayerTileRandom(const int nSceneID, NFGUID& xPlayer, std::string & strTileData)
{
	std::string strTileKey = m_pCommonRedisModule->GetTileCacheKey(nSceneID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(xPlayer.ToString());
	if (xNoSqlDriver && xNoSqlDriver->EXISTS(strTileKey))
	{
		//its a good way to cache these keys to boost the performance
		std::vector<std::string> vKeys;
		if (xNoSqlDriver->HKEYS(strTileKey, vKeys))
		{
			int nKeyIndex = m_pKernelModule->Random(0, (int)vKeys.size());
			std::string strKey = vKeys[nKeyIndex];
			if (xPlayer.FromString(strKey) && xNoSqlDriver->HGET(strTileKey, strKey, strTileData))
			{

				return true;
			}
		}
	}

	return false;
}

bool NFPlayerRedisModule::GetPropertyList(const NFGUID& self, const std::vector<std::string>& fields, std::vector<std::string>& values)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (!pDriver)
	{
		return false;
	}

	std::string strCacheKey = m_pCommonRedisModule->GetPropertyCacheKey(self);
	if (pDriver->HMGET(strCacheKey, fields, values))
	{
		return true;
	}


	return false;
}

NFINT64 NFPlayerRedisModule::GetPropertyInt(const NFGUID & self, const std::string & strPropertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (!pDriver)
	{
		return 0;
	}

	std::string strValue;
	std::string strCacheKey = m_pCommonRedisModule->GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, strPropertyName, strValue))
	{
		return 0;
	}

	return lexical_cast<NFINT64>(strValue);
}

int NFPlayerRedisModule::GetPropertyInt32(const NFGUID & self, const std::string & strPropertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (!pDriver)
	{
		return 0;
	}

	std::string strValue;
	std::string strCacheKey = m_pCommonRedisModule->GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, strPropertyName, strValue))
	{
		return 0;
	}

	return lexical_cast<NFINT64>(strValue);
}

double NFPlayerRedisModule::GetPropertyFloat(const NFGUID & self, const std::string & strPropertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (!pDriver)
	{
		return 0;
	}

	std::string strValue;
	std::string strCacheKey = m_pCommonRedisModule->GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, strPropertyName, strValue))
	{
		return 0;
	}

	return lexical_cast<double>(strValue);
}

std::string NFPlayerRedisModule::GetPropertyString(const NFGUID & self, const std::string & strPropertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (!pDriver)
	{
		return "";
	}

	std::string strValue;
	std::string strCacheKey = m_pCommonRedisModule->GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, strPropertyName, strValue))
	{
		return "";
	}

	return strValue;
}

NFGUID NFPlayerRedisModule::GetPropertyObject(const NFGUID & self, const std::string & strPropertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (!pDriver)
	{
		return NFGUID();
	}

	std::string strValue;
	std::string strCacheKey = m_pCommonRedisModule->GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, strPropertyName, strValue))
	{
		return NFGUID();
	}
	NFGUID xID;
	xID.FromString(strValue);
	return xID;
}

NFVector2 NFPlayerRedisModule::GetPropertyVector2(const NFGUID & self, const std::string & strPropertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (!pDriver)
	{
		return NFVector2();
	}

	std::string strValue;
	std::string strCacheKey = m_pCommonRedisModule->GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, strPropertyName, strValue))
	{
		return NFVector2();
	}

	NFVector2 xValue;
	xValue.FromString(strValue);
	return xValue;
}

NFVector3 NFPlayerRedisModule::GetPropertyVector3(const NFGUID & self, const std::string & strPropertyName)
{
	NF_SHARE_PTR<NFIRedisClient> pDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (!pDriver)
	{
		return NFVector3();
	}

	std::string strValue;
	std::string strCacheKey = m_pCommonRedisModule->GetPropertyCacheKey(self);
	if (!pDriver->HGET(strCacheKey, strPropertyName, strValue))
	{
		return NFVector3();
	}

	NFVector3 xValue;
	xValue.FromString(strValue);
	return xValue;
}

std::string NFPlayerRedisModule::GetOnlineGameServerKey()
{
	//if (strValue == "**nonexistent-key**")
	return "OnlineGameKey";
}

std::string NFPlayerRedisModule::GetOnlineProxyServerKey()
{
	return "OnlineProxyKey";
}


bool NFPlayerRedisModule::ExistRoleName(const std::string & strRoleName)
{
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->EXISTS(strRoleName);
	}

	return false;
}

bool NFPlayerRedisModule::CreateRole(const std::string & strAccount, const std::string & strRoleName, const NFGUID & id, const int nHomeSceneID)
{
	const std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		if (!xNoSqlDriver->EXISTS(strAccountKey))
		{
			m_pAccountRedisModule->AddAccount(strAccount, strAccount);
		}

		if (xNoSqlDriver->EXISTS(strAccountKey) && !xNoSqlDriver->EXISTS(strRoleName))
		{
			std::vector<std::string> vecFields;
			std::vector<std::string> vecValues;

			vecFields.push_back(NFrame::Player::Name());
			vecFields.push_back(NFrame::Player::ID());

			vecValues.push_back(strRoleName);
			vecValues.push_back(id.ToString());

			xNoSqlDriver->HMSET(strAccountKey, vecFields, vecValues);

			NF_SHARE_PTR<NFIRedisClient> xRoleNameNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
			if (xRoleNameNoSqlDriver)
			{
				//the name ref to the guid
				xRoleNameNoSqlDriver->HSET(strRoleName, NFrame::Player::ID(), id.ToString());
			}

			NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pCommonRedisModule->NewPropertyManager(NFrame::Player::ThisName());
			if (xPropertyManager)
			{
				NF_SHARE_PTR<NFIProperty> xProperty = xPropertyManager->GetElement(NFrame::Player::Account());
				if (xProperty)
				{
					xProperty->SetString(strAccount);
				}

				xProperty = xPropertyManager->GetElement(NFrame::Player::Level());
				if (xProperty)
				{
					xProperty->SetInt(0);
				}

				xProperty = xPropertyManager->GetElement(NFrame::Player::Name());
				if (xProperty)
				{
					xProperty->SetString(strRoleName);
				}

				m_pCommonRedisModule->SavePropertyInfo(id, xPropertyManager);
			}

			return true;
		}
	}

	return false;
}

bool NFPlayerRedisModule::GetRoleInfo(const std::string & strAccount, std::string & strRoleName, NFGUID & id)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
		if (xNoSqlDriver->EXISTS(strAccountKey))
		{
			std::string strID;
			bool bRoleNameRet = xNoSqlDriver->HGET(strAccountKey, NFrame::Player::Name(), strRoleName);
			bool bRoleIDRet = xNoSqlDriver->HGET(strAccountKey, NFrame::Player::ID(), strID);
			if (bRoleNameRet && bRoleIDRet
				&& !strRoleName.empty() && !strID.empty())
			{
				return id.FromString(strID);
			}

			return false;
		}
	}

	return false;
}
