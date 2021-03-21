/*
            This file is part of: 
                NoahFrame
            https://github.com/ketoo/NoahGameFrame

   Copyright 2009 - 2021 NoahFrame(NoahGameFrame)

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

	NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pCommonRedisModule->GetPropertyInfo(self.ToString(), NFrame::Player::ThisName(), false, true);
	if (xPropertyManager)
	{
		*(roleData.mutable_property()->mutable_player_id()) = NFINetModule::NFToPB(self);

		pCommonRedisModule->ConvertPropertyManagerToPB(xPropertyManager, roleData.mutable_property(), false, true);

		pCommonRedisModule->GetRecordInfo(self.ToString(), NFrame::Player::ThisName(), roleData.mutable_record(), false, true);
		
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
		m_pCommonRedisModule->SavePropertyInfo(self.ToString(), xPropManager, false, true);
	}

	pCommonRedisModule->SaveRecordInfo(self.ToString(), roleData.record(), -1);

	return true;
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

bool NFPlayerRedisModule::CreateRole(const std::string & account, const std::string & strRoleName, const NFGUID & id, const int nHomeSceneID)
{
	const std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(account);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(account);
	if (xNoSqlDriver)
	{
		if (!xNoSqlDriver->EXISTS(strAccountKey))
		{
			m_pAccountRedisModule->AddAccount(account, account);
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
					xProperty->SetString(account);
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

				m_pCommonRedisModule->SavePropertyInfo(id.ToString(), xPropertyManager, false, true);
			}

			return true;
		}
	}

	return false;
}

bool NFPlayerRedisModule::GetRoleInfo(const std::string & account, std::string & strRoleName, NFGUID & id)
{
	std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(account);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(account);
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
