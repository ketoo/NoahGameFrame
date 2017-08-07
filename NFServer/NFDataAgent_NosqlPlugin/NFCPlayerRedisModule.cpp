// -------------------------------------------------------------------------
//    @FileName			:    NFCPlayerRedisModule.cpp
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCPlayerRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCPlayerRedisModule.h"

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
	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCPlayerRedisModule::OnObjectPlayerEvent);
	m_pKernelModule->AddClassCallBack(NFrame::Guild::ThisName(), this, &NFCPlayerRedisModule::OnObjectGuildEvent);

	return true;
}
/*
int64_t NFCPlayerRedisModule::GetPlayerCacheGameID(const NFGUID & self)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (!pNoSqlDriver)
	{
		return 0;
	}

	std::string strValue;
	int64_t nGameID = 0;
	std::string strKey = GetOnlineGameServerKey();

	if (pNoSqlDriver->HGet(strKey, self.ToString(), strValue))
	{
		try
		{
			nGameID = lexical_cast<int64_t>(strValue);
		}
		catch (...)
		{
		}
	}

	return nGameID;
}

int64_t NFCPlayerRedisModule::GetPlayerCacheProxyID(const NFGUID & self)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (!pNoSqlDriver)
	{
		return 0;
	}

	std::string strValue;
	int64_t nProxyID = 0;
	std::string strKey = GetOnlineProxyServerKey();

	if (pNoSqlDriver->HGet(strKey, self.ToString(), strValue))
	{
		try
		{
			nProxyID = lexical_cast<int64_t>(strValue);
		}
		catch (...)
		{
		}
	}

	return nProxyID;
}

bool NFCPlayerRedisModule::GetPlayerCacheGameID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (!pNoSqlDriver)
	{
		return 0;
	}

	std::string strKey = GetOnlineGameServerKey();
	std::vector<std::string> strValueList;

	if (pNoSqlDriver->HMGet(strKey, xList, strValueList))
	{
		for (int i = 0; i < strValueList.size(); ++i)
		{
			int64_t nGameID = 0;

			try
			{
				nGameID = lexical_cast<int64_t>(strValueList[i]);
			}
			catch (...)
			{
			}

			xResultList.push_back(nGameID);
		}

		return true;
	}

	return false;
}

bool NFCPlayerRedisModule::GetPlayerCacheProxyID(const std::vector<std::string>& xList, std::vector<int64_t>& xResultList)
{
	NF_SHARE_PTR<NFINoSqlDriver> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuitConsistent();
	if (!pNoSqlDriver)
	{
		return 0;
	}

	std::string strKey = GetOnlineProxyServerKey();
	std::vector<std::string> strValueList;

	if (pNoSqlDriver->HMGet(strKey, xList, strValueList))
	{
		for (int i = 0; i < strValueList.size(); ++i)
		{
			int64_t nProxyID = 0;

			try
			{
				nProxyID = lexical_cast<int64_t>(strValueList[i]);
			}
			catch (...)
			{
			}

			xResultList.push_back(nProxyID);
		}

		return true;
	}

	return false;
}
*/
bool NFCPlayerRedisModule::LoadPlayerData(const NFGUID & self)
{
	mxObjectDataCache.RemoveElement(self);

	NF_SHARE_PTR<PlayerDataCache> xPlayerDataCache(NF_NEW PlayerDataCache());
	mxObjectDataCache.AddElement(self, xPlayerDataCache);

	m_pCommonRedisModule->LoadCachePropertyListPB(self, xPlayerDataCache->xPbPropertyCacheList);
	m_pCommonRedisModule->LoadStoragePropertyListPB(self, xPlayerDataCache->xPbPropertyStorageList);
	m_pCommonRedisModule->LoadCacheRecordListPB(self, xPlayerDataCache->xPbRecordCacheList);
	m_pCommonRedisModule->LoadStorageRecordListPB(self, xPlayerDataCache->xPbRecordStorageList);

#ifdef NF_DEBUG_MODE
	for (int i = 0; i < xPlayerDataCache->xPbPropertyCacheList.property_int_list_size(); ++i)
	{
		const NFMsg::PropertyInt& xPropertyData = xPlayerDataCache->xPbPropertyCacheList.property_int_list(i);
		const std::string& strPropertyName = xPropertyData.property_name();
		const NFINT64 xPropertyValue = xPropertyData.data();
		
		m_pLogModule->LogNormal(NFILogModule::NF_LOG_LEVEL::NLL_DEBUG_NORMAL, self, strPropertyName, xPropertyValue, "PbPropertyCacheList", i);
	}

	for (int iRecord = 0; iRecord < xPlayerDataCache->xPbRecordCacheList.record_list_size(); ++iRecord)
	{
		const NFMsg::ObjectRecordBase& xRecordBase = xPlayerDataCache->xPbRecordCacheList.record_list(iRecord);
		const std::string& strRecordName = xRecordBase.record_name();

		
		for (int iRow = 0; iRow < xRecordBase.row_struct_size(); ++iRow)
		{

			const NFMsg::RecordAddRowStruct& xAddRowStruct = xRecordBase.row_struct(iRow);

			for (int iStr = 0; iStr < xAddRowStruct.record_string_list_size(); iStr++)
			{
				const NFMsg::RecordString& xPropertyData = xAddRowStruct.record_string_list(iStr);
				const int nRow = xPropertyData.row();
				const int nCol = xPropertyData.col();
				const std::string& xPropertyValue = xPropertyData.data();

				std::ostringstream strData;
				strData  << strRecordName << " Row:" << nRow << " Col:" << nCol << " " << xPropertyValue;

				m_pLogModule->LogNormal(NFILogModule::NF_LOG_LEVEL::NLL_DEBUG_NORMAL, self, strData, "xPbRecordCacheList", iRow);
			}
		}
	}
#endif // NF_DEBUG_MODE

	for (int i = 0; i < xPlayerDataCache->xPbPropertyCacheList.property_int_list_size(); ++i)
	{
		const NFMsg::PropertyInt& xPropertyData = xPlayerDataCache->xPbPropertyCacheList.property_int_list(i);
		const std::string& strPropertyName = xPropertyData.property_name();
		if (strPropertyName == NFrame::Player::HomeSceneID())
		{
			const NFINT64 xPropertyValue = xPropertyData.data();
			xPlayerDataCache->nHomeSceneID = (int)xPropertyValue;
			break;
		}
	}

	return true;
}

int NFCPlayerRedisModule::GetPlayerHomeSceneID(const NFGUID & self)
{
	NF_SHARE_PTR<PlayerDataCache> xPlayerDataCache = mxObjectDataCache.GetElement(self);
	if (xPlayerDataCache)
	{
		return xPlayerDataCache->nHomeSceneID;
	}

	return 0;
}

bool NFCPlayerRedisModule::SavePlayerTile(const int nSceneID, const NFGUID & self, const std::string & strTileData)
{
	std::string strTileKey = m_pCommonRedisModule->GetTileCacheKey(nSceneID);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitRandom();
	if (xNoSqlDriver)
	{
		return xNoSqlDriver->HSet(strTileKey, self.ToString(), strTileData);
	}

	return false;
}

bool NFCPlayerRedisModule::LoadPlayerTile(const int nSceneID, const NFGUID & self, std::string & strTileData)
{
	std::string strTileKey = m_pCommonRedisModule->GetTileCacheKey(nSceneID);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitRandom();
	if (xNoSqlDriver && xNoSqlDriver->Exists(strTileKey))
	{
		return xNoSqlDriver->HGet(strTileKey, self.ToString(), strTileData);
	}

	return false;
}

bool NFCPlayerRedisModule::LoadPlayerTileRandom(const int nSceneID, NFGUID& xPlayer, std::string & strTileData)
{
	std::string strTileKey = m_pCommonRedisModule->GetTileCacheKey(nSceneID);
	NF_SHARE_PTR<NFINoSqlDriver> xNoSqlDriver = m_pNoSqlModule->GetDriverBySuitRandom();
	if (xNoSqlDriver && xNoSqlDriver->Exists(strTileKey))
	{
		//need to cache this keys
		std::vector<std::string> vKeys;
		if (xNoSqlDriver->HKeys(strTileKey, vKeys))
		{
			int nKeyIndex = m_pKernelModule->Random(0, (int)vKeys.size());
			std::string strKey = vKeys[nKeyIndex];
			if (xPlayer.FromString(strKey) && xNoSqlDriver->HGet(strTileKey, strKey, strTileData))
			{
				if (mxObjectTileCache.ExistElement(xPlayer))
				{
					mxObjectTileCache.RemoveElement(xPlayer);
				}

				mxObjectTileCache.AddElement(xPlayer, NF_SHARE_PTR<std::string>(NF_NEW std::string(strTileData)));

				return true;
			}
		}
	}

	return false;
}

bool NFCPlayerRedisModule::LoadPlayerTileRandomCache(const NFGUID & xPlayer, std::string & strTileData)
{
	if (mxObjectTileCache.ExistElement(xPlayer))
	{
		NF_SHARE_PTR<std::string> xData = mxObjectTileCache.GetElement(xPlayer);
		if (xData)
		{
			strTileData = *xData;
		}

		return true;
	}

	return false;
}

bool NFCPlayerRedisModule::SavePlayerData(const NFGUID & self)
{
	m_pCommonRedisModule->SaveCachePropertyInfo(self, m_pKernelModule->GetObject(self)->GetPropertyManager());
	m_pCommonRedisModule->SaveCacheRecordInfo(self, m_pKernelModule->GetObject(self)->GetRecordManager());

	m_pCommonRedisModule->SaveStroragePropertyInfo(self, m_pKernelModule->GetObject(self)->GetPropertyManager());
	m_pCommonRedisModule->SaveStrorageRecordInfo(self, m_pKernelModule->GetObject(self)->GetRecordManager());

	return true;
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

const bool NFCPlayerRedisModule::AttachData(const NFGUID & self)
{
	NF_SHARE_PTR<PlayerDataCache> xPlayerDataCache = mxObjectDataCache.GetElement(self);
	if (xPlayerDataCache)
	{
		//way 1:load first then create object, especially we have loaded nosql plugin
		NF_SHARE_PTR<NFIObject> pObject = m_pKernelModule->GetObject(self);
		if (pObject)
		{
			NF_SHARE_PTR<NFIPropertyManager> pPropertyManager = pObject->GetPropertyManager();
			NF_SHARE_PTR<NFIRecordManager> pRecordManager = pObject->GetRecordManager();

			m_pCommonRedisModule->ConvertPBToPropertyManager(xPlayerDataCache->xPbPropertyCacheList, pPropertyManager, true);
			m_pCommonRedisModule->ConvertPBToPropertyManager(xPlayerDataCache->xPbPropertyStorageList, pPropertyManager, false);
			m_pCommonRedisModule->ConvertPBToRecordManager(xPlayerDataCache->xPbRecordCacheList, pRecordManager, true);
			m_pCommonRedisModule->ConvertPBToRecordManager(xPlayerDataCache->xPbRecordStorageList, pRecordManager, false);

			return true;
		}
	}
	else
	{
		//way 2:load data when creating a object, especially we donot loaded any sql or nosql plugin
	}

	return false;
}

int NFCPlayerRedisModule::OnObjectPlayerEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		OnOffline(self);

		SavePlayerData(self);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
	{
		OnOnline(self);
		AttachData(self);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		mxObjectDataCache.RemoveElement(self);
	}

	return 0;
}

int NFCPlayerRedisModule::OnObjectGuildEvent(const NFGUID & self, const std::string & strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFDataList & var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		SavePlayerData(self);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
	{
		AttachData(self);
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_FINISH == eClassEvent)
	{
		mxObjectDataCache.RemoveElement(self);
	}

	return 0;
}

void NFCPlayerRedisModule::OnOnline(const NFGUID & self)
{
	const int nGateID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GateID());
	const int nServerID = m_pKernelModule->GetPropertyInt32(self, NFrame::Player::GameID());
}

void NFCPlayerRedisModule::OnOffline(const NFGUID & self)
{
	const int nGateID = 0;
	const int nServerID = 0;
}
