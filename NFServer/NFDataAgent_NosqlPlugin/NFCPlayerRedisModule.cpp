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
	m_pLogicClassModule = pPluginManager->FindModule<NFIClassModule>();
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>();
	m_pKernelModule = pPluginManager->FindModule<NFIKernelModule>();

	m_pKernelModule->AddClassCallBack(NFrame::Player::ThisName(), this, &NFCPlayerRedisModule::OnObjectClassEvent);

	return true;
}

int NFCPlayerRedisModule::OnObjectClassEvent(const NFGUID& self, const std::string& strClassName, const CLASS_OBJECT_EVENT eClassEvent, const NFIDataList& var)
{
	if (CLASS_OBJECT_EVENT::COE_DESTROY == eClassEvent)
	{
		//SaveDataToNoSql( self, true );
		//m_pLogModule->LogNormal(NFILogModule::NLL_INFO_NORMAL, self, "Player Offline", "");
	}
	else if (CLASS_OBJECT_EVENT::COE_CREATE_LOADDATA == eClassEvent)
	{
		//LoadDataFormNoSql( self );
	}

	return 0;
}

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

std::string NFCPlayerRedisModule::GetOnlineGameServerKey()
{
	return "OnlineGameKey";
}

std::string NFCPlayerRedisModule::GetOnlineProxyServerKey()
{
	return "OnlineProxyKey";
}
