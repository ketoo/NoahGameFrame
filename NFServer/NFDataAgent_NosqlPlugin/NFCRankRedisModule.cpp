// -------------------------------------------------------------------------
//    @FileName			:    NFCRankRedisModule.h
//    @Author           :    LvSheng.Huang
//    @Date             :    2013-10-03
//    @Module           :    NFCRankRedisModule
//    @Desc             :
// -------------------------------------------------------------------------
#include "NFCRankRedisModule.h"

NFCRankRedisModule::NFCRankRedisModule(NFIPluginManager * p)
{
	pPluginManager = p;
}

bool NFCRankRedisModule::Init()
{
	return true;
}

bool NFCRankRedisModule::Shut()
{
	return true;
}

bool NFCRankRedisModule::Execute()
{
	return true;
}

bool NFCRankRedisModule::AfterInit()
{
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>("NFCNoSqlModule");

	assert(NULL != m_pNoSqlModule);

	return true;
}

bool NFCRankRedisModule::SetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return false;
	}

	std::string strRankKey = MakeRedisKey(eRankType);

	return pNoSqlDriver->ZAdd(strRankKey, value, self.ToString());
}

bool NFCRankRedisModule::IncrRankValue(const NFMsg::ERankType eRankType, const NFGUID & self, const double value)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return false;
	}

	std::string strRankKey = MakeRedisKey(eRankType);

	return pNoSqlDriver->ZIncrBy(strRankKey, self.ToString(), value);
}

bool NFCRankRedisModule::RemoveRankValue(const NFMsg::ERankType eRankType, const NFGUID & self)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return false;
	}

	std::string strRankKey = MakeRedisKey(eRankType);

	return pNoSqlDriver->ZRem(strRankKey, self.ToString());
}

bool NFCRankRedisModule::RemoveRangeByRank(const NFMsg::ERankType eRankType, const int nStart, const int nStop)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return false;
	}

	std::string strRankKey = MakeRedisKey(eRankType);

	return pNoSqlDriver->ZRemRangeByRank(strRankKey, nStart, nStop);
}

bool NFCRankRedisModule::RemoveRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return false;
	}

	std::string strRankKey = MakeRedisKey(eRankType);

	return pNoSqlDriver->ZRemRangeByScore(strRankKey, nMin, nMax);
}

bool NFCRankRedisModule::GetRankValue(const NFMsg::ERankType eRankType, const NFGUID & self, double& dwValue)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return false;
	}

	std::string strRankKey = MakeRedisKey(eRankType);

	return pNoSqlDriver->ZScore(strRankKey, self.ToString(), dwValue);
}

bool NFCRankRedisModule::GetRange(const NFMsg::ERankType eRankType, const int nStart, const int nStop, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return false;
	}

	std::string strRankKey = MakeRedisKey(eRankType);

	return pNoSqlDriver->ZRevRange(strRankKey, nStart, nStop, memberScoreVec);
}

bool NFCRankRedisModule::GetRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return false;
	}

	std::string strRankKey = MakeRedisKey(eRankType);

	return pNoSqlDriver->ZRangeByScore(strRankKey, nMin, nMax, memberScoreVec);
}

int NFCRankRedisModule::GetRankMemberCount(const NFMsg::ERankType eRankType)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return 0;
	}

	std::string strRankKey = MakeRedisKey(eRankType);

	int nCount = 0;
	pNoSqlDriver->ZCard(strRankKey, nCount);

	return nCount;
}

int NFCRankRedisModule::GetRankRangeMemberCount(const NFMsg::ERankType eRankType, const int nMin, const int nMax)
{
	NFINoSqlDriver* pNoSqlDriver = m_pNoSqlModule->GetDriver();
	if (!pNoSqlDriver)
	{
		return 0;
	}

	std::string strRankKey = MakeRedisKey(eRankType);
	int nCount = 0;
	pNoSqlDriver->ZCount(strRankKey, nMin, nMax, nCount);

	return nCount;
}

std::string NFCRankRedisModule::MakeRedisKey(const NFMsg::ERankType eRankType)
{
	std::string szKey = "Rank_" + ERankType_Name(eRankType);
	
	return szKey;
}
