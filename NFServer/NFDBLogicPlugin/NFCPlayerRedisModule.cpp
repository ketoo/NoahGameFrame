// -------------------------------------------------------------------------
//    @FileName			:    NFCPlayerRedisModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCPlayerRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCPlayerRedisModule.h"
#include "NFCCommonRedisModule.h"

NFCPlayerRedisModule::NFCPlayerRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFCPlayerRedisModule::Init()
{
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();
	m_pLogModule = pPluginManager->FindModule<NFILogModule>();
	
	return true;
}

bool NFCPlayerRedisModule::Shut()
{
	return true;
}

bool NFCPlayerRedisModule::Execute()
{
	return true;
}

bool NFCPlayerRedisModule::AfterInit()
{

	return true;
}

bool NFCPlayerRedisModule::LoadPlayerData(const NFGUID & self, NFMsg::RoleDataPack& roleData)
{
	NFCCommonRedisModule* pCommonRedisModule = (NFCCommonRedisModule*)(m_pCommonRedisModule);

	NF_SHARE_PTR<NFIPropertyManager> xPropertyManager = m_pCommonRedisModule->GetPropertyInfo(self, NFrame::Player::ThisName());
	if (xPropertyManager)
	{
		*(roleData.mutable_property()->mutable_player_id()) = NFINetModule::NFToPB(self);

		pCommonRedisModule->ConvertPropertyManagerToPB(xPropertyManager, roleData.mutable_property());

		pCommonRedisModule->GetRecordInfo(self, NFrame::Player::ThisName(), roleData.mutable_record());
		
		return true;
	}

	

	/*
	if (xRecordManager)
	{
		NF_SHARE_PTR<NFIRecord> xRecord = xRecordManager->First();
		while (xRecord)
		{
			NFMsg::ObjectRecordBase* pRecord = roleData.mutable_record()->add_record_list();

			pCommonRedisModule->ConvertRecordToPB(xRecord, pRecord);

			xRecord = xRecordManager->Next();
		}
	}
	*/
	m_pLogModule->LogNormal(NFILogModule::NF_LOG_LEVEL::NLL_ERROR_NORMAL, self, "loaded data false", NFGetTimeMS());

	return false;
}

bool NFCPlayerRedisModule::SavePlayerData(const NFGUID & self, const NFMsg::RoleDataPack& roleData)
{
	NFCCommonRedisModule* pCommonRedisModule = (NFCCommonRedisModule*)m_pCommonRedisModule;

	NF_SHARE_PTR<NFIPropertyManager> xPropManager = pCommonRedisModule->NewPropertyManager(NFrame::Player::ThisName());
	if (pCommonRedisModule->ConvertPBToPropertyManager(roleData.property(), xPropManager))
	{
		m_pCommonRedisModule->SavePropertyInfo(self, xPropManager);
	}

	pCommonRedisModule->SaveRecordInfo(self, roleData.record(), -1);

	return true;
}

bool NFCPlayerRedisModule::SavePlayerTile(const int nSceneID, const NFGUID & self, const std::string & strTileData)
{
	std::string strTileKey = m_pCommonRedisModule->GetTileCacheKey(nSceneID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HSET(strTileKey, self.ToString(), strTileData);
	}

	return false;
}

bool NFCPlayerRedisModule::LoadPlayerTile(const int nSceneID, const NFGUID & self, std::string & strTileData)
{
	std::string strTileKey = m_pCommonRedisModule->GetTileCacheKey(nSceneID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(self.ToString());
	if (xNoSqlDriver && xNoSqlDriver->EXISTS(strTileKey))
	{
		return xNoSqlDriver->HGET(strTileKey, self.ToString(), strTileData);
	}

	return false;
}

bool NFCPlayerRedisModule::LoadPlayerTileRandom(const int nSceneID, NFGUID& xPlayer, std::string & strTileData)
{
	std::string strTileKey = m_pCommonRedisModule->GetTileCacheKey(nSceneID);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(xPlayer.ToString());
	if (xNoSqlDriver && xNoSqlDriver->EXISTS(strTileKey))
	{
		//need to cache this keys
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

bool NFCPlayerRedisModule::LoadPlayerTileRandomCache(const NFGUID & xPlayer, std::string & strTileData)
{

	return false;
}

NFINT64 NFCPlayerRedisModule::GetPropertyInt(const NFGUID & self, const std::string & strPropertyName)
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

int NFCPlayerRedisModule::GetPropertyInt32(const NFGUID & self, const std::string & strPropertyName)
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

double NFCPlayerRedisModule::GetPropertyFloat(const NFGUID & self, const std::string & strPropertyName)
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

std::string NFCPlayerRedisModule::GetPropertyString(const NFGUID & self, const std::string & strPropertyName)
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

NFGUID NFCPlayerRedisModule::GetPropertyObject(const NFGUID & self, const std::string & strPropertyName)
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

NFVector2 NFCPlayerRedisModule::GetPropertyVector2(const NFGUID & self, const std::string & strPropertyName)
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

NFVector3 NFCPlayerRedisModule::GetPropertyVector3(const NFGUID & self, const std::string & strPropertyName)
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

std::string NFCPlayerRedisModule::GetOnlineGameServerKey()
{
	//if (strValue == "**nonexistent-key**")
	return "OnlineGameKey";
}

std::string NFCPlayerRedisModule::GetOnlineProxyServerKey()
{
	return "OnlineProxyKey";
}


bool NFCPlayerRedisModule::ExistRoleName(const std::string & strRoleName)
{
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->EXISTS(strRoleName);
	}

	return false;
}

bool NFCPlayerRedisModule::CreateRole(const std::string & strAccount, const std::string & strRoleName, const NFGUID & id)
{
	const std::string strAccountKey = m_pCommonRedisModule->GetAccountCacheKey(strAccount);
	NF_SHARE_PTR<NFIRedisClient> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strAccount);
	if (xNoSqlDriver)
	{
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
					xProperty->SetInt(1);
				}

				m_pCommonRedisModule->SavePropertyInfo(id, xPropertyManager);
			}

			return true;
		}
	}

	return false;
}

bool NFCPlayerRedisModule::GetRoleInfo(const std::string & strAccount, std::string & strRoleName, NFGUID & id)
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
