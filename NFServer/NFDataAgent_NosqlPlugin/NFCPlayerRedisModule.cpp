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
	m_pLogicClassModule = pPluginManager->FindModule<NFILogicClassModule>("NFCLogicClassModule");
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>("NFCNoSqlModule");
	m_pCommonRedisModule = pPluginManager->FindModule<NFICommonRedisModule>("NFCCommonRedisModule");

	assert(NULL != m_pLogicClassModule);
	assert(NULL != m_pNoSqlModule);
	assert(NULL != m_pCommonRedisModule);

	return true;
}

int64_t NFCPlayerRedisModule::GetPlayerCacheGameID(const NFGUID & self)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
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
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
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
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
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
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
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
