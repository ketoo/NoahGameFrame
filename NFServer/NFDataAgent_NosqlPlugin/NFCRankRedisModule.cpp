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
	m_pNoSqlModule = pPluginManager->FindModule<NFINoSqlModule>();

	return true;
}

bool NFCRankRedisModule::SetRankValue(const NFMsg::ERankType eRankType, const NFGUID& self, const double value)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZADD(strRankKey, self.ToString(), value);
}

bool NFCRankRedisModule::IncrRankValue(const NFMsg::ERankType eRankType, const NFGUID & self, const double value)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	double newValue;
	return pNoSqlDriver->ZINCRBY(strRankKey, self.ToString(), value, newValue);
}

bool NFCRankRedisModule::RemoveRankValue(const NFMsg::ERankType eRankType, const NFGUID & self)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZREM(strRankKey, self.ToString());
}

bool NFCRankRedisModule::RemoveRangeByRank(const NFMsg::ERankType eRankType, const int nStart, const int nStop)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZREMRANGEBYRANK(strRankKey, nStart, nStop);
}

bool NFCRankRedisModule::RemoveRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZREMRANGEBYSCORE(strRankKey, nMin, nMax);
}

bool NFCRankRedisModule::GetRankValue(const NFMsg::ERankType eRankType, const NFGUID & self, double& dwValue)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZSCORE(strRankKey, self.ToString(), dwValue);
}

bool NFCRankRedisModule::GetRange(const NFMsg::ERankType eRankType, const int nStart, const int nStop, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZREVRANGE(strRankKey, nStart, nStop, memberScoreVec);
}

bool NFCRankRedisModule::GetRangeByScore(const NFMsg::ERankType eRankType, const int nMin, const int nMax, std::vector<std::pair<std::string, double>>& memberScoreVec)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	return pNoSqlDriver->ZRANGEBYSCORE(strRankKey, nMin, nMax, memberScoreVec);
}

int NFCRankRedisModule::GetRankMemberCount(const NFMsg::ERankType eRankType)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	int nCount = 0;
	pNoSqlDriver->ZCARD(strRankKey, nCount);

	return nCount;
}

int NFCRankRedisModule::GetRankRangeMemberCount(const NFMsg::ERankType eRankType, const int nMin, const int nMax)
{
	std::string strRankKey = MakeRedisKey(eRankType);
	NF_SHARE_PTR<NFIRedisClient> pNoSqlDriver = m_pNoSqlModule->GetDriverBySuit(strRankKey);
	if (!pNoSqlDriver)
	{
		return 0;
	}

	int nCount = 0;
	pNoSqlDriver->ZCOUNT(strRankKey, nMin, nMax, nCount);

	return nCount;
}

std::string NFCRankRedisModule::MakeRedisKey(const NFMsg::ERankType eRankType)
{
	std::string szKey = "Rank_" + ERankType_Name(eRankType);
	
	return szKey;
}
